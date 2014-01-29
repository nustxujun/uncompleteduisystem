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
mName(name), mFactory(f), mManager(wm), mIsdirty(true), mMousePassThrough(false)
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
	helper.registerFunction(
		*WindowSystem::getSingleton().getScriptBind(), L"close", this, &Window::close);

	helper.registerFunction(
		*WindowSystem::getSingleton().getScriptBind(), L"setPosition", this, &Window::setPosition);

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
		{	
			renderer = ws.getRenderer(ret->second);
		}
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

void Window::draw()
{
	if (isDirty())
	{
		mRenderObject->notifyUpdateWindow();
		mIsdirty = false;
	}

	mRenderObject->render();

	std::for_each(mChildren.begin(), mChildren.end(),
		[](Children::value_type& c)
		{
			c.second->draw();
		});
}

const String& Window::getName()const
{
	return mName;
}

void Window::setProperty(const String& prop, const String& val)
{
	mPropertys[prop] = val;
	dirty();
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
	dirty();
}

void Window::setX(int x)
{
	mRect.moveTo(x, mRect.top);
}

void Window::setY(int y)
{
	mRect.moveTo(mRect.left, y);
}

void Window::setPosition(int x, int y)
{
	mRect.moveTo(x, y);
}

void Window::setWidth(int w)
{
	mRect.setWidth(w);
	dirty();
}

void Window::setHeight(int h)
{
	mRect.setHeight(h);
	dirty();
}

void Window::setSize(int w, int h)
{
	mRect.setWidth(w);
	mRect.setHeight(h);
	dirty();
}


void Window::dirty()
{
	mIsdirty = true;
	if (mParent)
		mParent->dirty();
}

bool Window::isDirty() const
{
	return mIsdirty;
}

bool Window::isMousePassThroughEnabled()const
{
	return mMousePassThrough;
}

//event
void Window::injectMouseMove(int posx, int posy, int deltax, int deltay)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_MOUSE_MOVE, mName.c_str(), posx, posy, deltax, deltay);
}

void Window::injectMouseButtonDown(Mouse::MouseButton btn)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_MOUSE_BUTTON_DOWN, mName.c_str(), btn);
}

void Window::injectMouseButtonUp(Mouse::MouseButton btn)
{
	WindowSystem::getSingleton().getScriptBind()->
		call<void>(ScriptObject::INJECT_MOUSE_BUTTON_UP, mName.c_str(), btn);
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
