#include "STWindow.h"
#include "STWindowManager.h"
#include "STRenderObject.h"
#include "STWindowProperty.h"
#include "Touten.h"
#include "TTBind.h"
#include "STWindowHelper.h"
#include "STWindowSystem.h"
#include "STBasicImage.h"
#include "STRenderObjectFactory.h"
#include "STException.h"

using namespace ST;

Window::Window(const String& name, const WindowFactory* f, WindowManager* wm) :
mName(name), mFactory(f), mManager(wm), mDirty(DT_ALL), mMousePassThrough(false),
mWorldAABBInvalid(true), mClippedByParent(true), mRect(0,0,0,0)
{
	mPropertys[WindowProperty::PROPERTY_NULL] = WindowProperty::PROPERTY_NULL;
}

Window::~Window()
{
	uninitialize();
}

void Window::unregisterFunction(const String& name)
{
	String realName = mName + L"::" + name;

	TT::Bind* bind = WindowSystem::getSingleton().getScriptBind();
	bind->remove(realName);
}

void Window::initializeScript()
{
	WindowHelper helper;
	TT::Bind& bind = *WindowSystem::getSingleton().getScriptBind();
	
	bind.call<void>(L"RegisterWindow", mName.c_str()); //֪ͨscirpt


	helper.registerBaseFunctions(this);

	helper.registerFunction(
		bind, L"close", this, &Window::close);

	helper.registerFunction(
		bind, L"setPosition", this, &Window::setPosition);

	helper.registerFunction(
		bind, L"getX", this, &Window::getX);

	helper.registerFunction(
		bind, L"getY", this, &Window::getY);

	helper.registerFunction(
		bind, L"setSize", this, &Window::setSize);

	helper.registerFunction(
		bind, L"setProperty", this, &Window::setProperty);

	helper.registerFunction(
		bind, L"isClippedByParent", this, &Window::isClippedByParent);

	helper.registerFunction(
		bind, L"setClippedByParent", this, &Window::setClippedByParent);

	helper.registerFunction(
		bind, L"isMousePassThroughEnabled", this, &Window::isMousePassThroughEnabled);

	helper.registerFunction(
		bind, L"setMousePassThrough", this, &Window::setMousePassThrough);


	const String& script = getProperty(WindowProperty::SCRIPT_INITIALIZER);
	const Char* initialiser = ScriptObject::INITIALIZE_WINDOW;
	if (script != WindowProperty::PROPERTY_NULL)
		initialiser = script.c_str();

	bind.call<void>(initialiser, mName.c_str());

}

void Window::uninitializeScript()
{
	TT::Bind* bind = WindowSystem::getSingleton().getScriptBind();
	std::for_each(mFuncNames.begin(), mFuncNames.end(), [&bind](const String& name)
	{
		bind->remove(name);
	});
	mFuncNames.clear();
}

const WindowFactory* Window::getFactory()const
{
	return mFactory;
}

Window* Window::createChild(const String& name, const CustomParameters* paras, const String& factory)
{
	Window* win = getChild(name);
	if (win) 
	{ 
		ST_EXCEPT(L"same child name", L"Window::createChild"); 
		return win; 
	}
	if (factory != L"")
		win = mManager->createWindow(name, paras, factory);
	else
		win = mManager->createWindow(name, paras);
	mChildren.insert(Children::value_type(name, win));
	win->mParent = this;
	return win;
}

Window* Window::getChild(const String& name)
{
	auto ret = mChildren.find(name);
	if (ret == mChildren.end())
		return nullptr;
	return ret->second;
}
void Window::destroyChild(const String& name)
{
	mChildren.erase(name);
	mManager->destroyWindow(name);
}

void Window::parseParameter(const CustomParameters& paras)
{
	auto end = paras.end();
	for (auto i = paras.begin(); i != end; ++i)
	{
		mPropertys[i->first] = i->second;
	}

	WindowSystem& ws = WindowSystem::getSingleton();

	//WindowRenderer
	WindowRenderer* renderer = nullptr;
	{
		auto ret = paras.find(WindowProperty::RENDERER);
		if (ret != end)
			renderer = ws.getRenderer(ret->second);
		else
			renderer = ws.getDefaultRenderer();
	}

	//RenderObject
	{
		auto ret = paras.find(WindowProperty::RENDER_OBJECT_FACTORY);
		String facname = RenderObjectFactory::NAME;
		if (ret != end)
			facname = ret->second;
	
		RenderObjectFactory* fac = ws.getRenderObjectFactory(facname);
		mRenderObject = fac->createImpl(this, renderer);
	}


}


void Window::initialize(const CustomParameters* paras)
{
	if (paras == nullptr)
	{
		CustomParameters temp;
		parseParameter(temp);
	}
	else
		parseParameter(*paras);

	initializeScript();
}

void Window::uninitialize()
{
	uninitializeScript();

	delete mRenderObject;
}

void Window::close()
{
	mManager->destroyWindow(mName);
}

void Window::update()
{
	if (!isDirty()) return;
	if (isDirty(DT_POSITION))
	{
		std::for_each(mChildren.begin(), mChildren.end(),
					  [](Children::value_type& c)
		{
			c.second->dirty(DT_PARENT_TRANSFORM);
		});

		mWorldAABBInvalid = true;
	}

	if (isDirty(DT_PARENT_TRANSFORM) && mParent)
		mWorldAABBInvalid = true;

	mRenderObject->notifyUpdateWindow(mDirty);

	mDirty = DT_CLEAR;
}


void Window::draw()
{
	update();

	mRenderObject->render();

	std::for_each(mChildren.begin(), mChildren.end(),
		[](Children::value_type& c)
		{
			c.second->draw();
		});
}

const RectF& Window::getWorldAABB()
{
	if (mWorldAABBInvalid)
	{
		mWorldAABB = mRenderObject->getWorldAABB();
		if (mClippedByParent && mParent)
		{
			const RectF& prect = mParent->getWorldAABB();
			mWorldAABB.left = std::max(prect.left, mWorldAABB.left);
			mWorldAABB.right = std::min(prect.right, mWorldAABB.right);
			mWorldAABB.top = std::max(prect.top, mWorldAABB.top);
			mWorldAABB.bottom = std::min(prect.bottom, mWorldAABB.bottom);
		}
		
		mWorldAABBInvalid = false;
	}

	return mWorldAABB;

}


const String& Window::getName()const
{
	return mName;
}

void Window::setProperty(const String& prop, const String& val)
{
	mPropertys[prop] = val;
	dirty(DT_PROPERTY);
}

const String& Window::getProperty(const String& key)const
{
	auto ret = mPropertys.find(key);
	if (ret == mPropertys.end())
		return mPropertys.find(WindowProperty::PROPERTY_NULL)->second;

	return ret->second;
}

Window* Window::getParent()
{
	return mParent;
}

RenderObject* Window::getRenderObject()
{
	return mRenderObject;
}


const RectI& Window::getRect()const
{
	return mRect;
}

RectI Window::getAbsRect()const
{
	RectI rect = mRect;
	if (mParent)
	{	
		const RectI& pr = mParent->getAbsRect();
		rect.move(pr.left, pr.top);
	}
	return rect;
}


int Window::getX() const
{
	return mRect.left;
}

int Window::getY() const
{
	return mRect.top;
}

int Window::getAbsX() const
{
	int x = mRect.left;
	if (mParent)
	{
		int px = mParent->getAbsX();
		x += px;
	}
	return x;
}

int Window::getAbsY() const
{
	int y = mRect.top;
	if (mParent)
	{
		int py = mParent->getAbsY();
		y += py;
	}
	return y;
}

int Window::getWidth() const
{
	return mRect.width();
}

int Window::getHeight() const
{
	return mRect.height();
}

void Window::setRect(const RectI& r)
{
	mRect = r;
	dirty(DT_SIZE | DT_POSITION);
}

void Window::setX(int x)
{
	mRect.moveTo(x, mRect.top);
	dirty(DT_POSITION);
}

void Window::setY(int y)
{
	mRect.moveTo(mRect.left, y);
	dirty(DT_POSITION);
}

void Window::setPosition(int x, int y)
{
	mRect.moveTo(x, y);
	dirty(DT_POSITION);
}

void Window::setWidth(int w)
{
	mRect.setWidth(w);
	dirty(DT_SIZE);
}

void Window::setHeight(int h)
{
	mRect.setHeight(h);
	dirty(DT_SIZE);
}

void Window::setSize(int w, int h)
{
	mRect.setWidth(w);
	mRect.setHeight(h);
	dirty(DT_SIZE);
}


void Window::dirty(unsigned int type)
{
	mDirty |= type;
	//if (mParent)
	//	mParent->dirty(type);
}

bool Window::isDirty(unsigned int type) const
{
	return (mDirty & type) != DT_CLEAR;
}

bool Window::isMousePassThroughEnabled()const
{
	return mMousePassThrough;
}

void Window::setMousePassThrough(bool val)
{
	mMousePassThrough = val;
}


bool Window::isClippedByParent()const
{
	return mClippedByParent;
}

void Window::setClippedByParent(bool val)
{
	mClippedByParent = val;
}

bool down = false;


//event

void Window::injectMouseMove(int posx, int posy, int deltax, int deltay)
{
	if (down)
	{
		mRect.move(deltax, deltay);
	}
	//WindowSystem::getSingleton().getScriptBind()->
	//	call<void>(ScriptObject::INJECT_MOUSE_MOVE, mName.c_str(), posx, posy, deltax, deltay);
}

void Window::injectMouseButtonDown(Mouse::MouseButton btn)
{
	down = true;
	//WindowSystem::getSingleton().getScriptBind()->
	//	call<void>(ScriptObject::INJECT_MOUSE_BUTTON_DOWN, mName.c_str(), btn);
}

void Window::injectMouseButtonUp(Mouse::MouseButton btn)
{
	down = false;
	//WindowSystem::getSingleton().getScriptBind()->
	//	call<void>(ScriptObject::INJECT_MOUSE_BUTTON_UP, mName.c_str(), btn);
}

void Window::injectMouseWheel(float delta)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_MOUSE_WHEEL, mName.c_str(), delta);
}

void Window::injectKeyDown(Key::KeyType key)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_KEY_DOWN, mName.c_str(), key);
}

void Window::injectKeyUp(Key::KeyType key)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_KEY_UP, mName.c_str(), key);
}
