#include "STWindowSystem.h"
#include "STException.h"
#include "STWindowRenderer.h"
#include "STWindow.h"
#include "Touten.h"
#include "TTBind.h"
#include "TTMemoryAllocator.h"

using namespace ST;

WindowSystem::WindowSystem()
{
	mTouten = new TT::Touten();
	mBind = new TT::Bind(mTouten);

	mDefaultRenderer = nullptr;
	mROFactorys[RenderObjectFactory::NAME] = &mDefaultROFactory;
}

WindowSystem::~WindowSystem()
{
	delete mBind;
	delete mTouten;
}

WindowRenderer* WindowSystem::getRenderer(const String& name)
{
	auto ret = mRenderers.find(name);
	if (mRenderers.end() != ret)
		return ret->second;

	ST_EXCEPT(L"windowrenderer not found", L"WindowSystem::getRenderer");
}

void WindowSystem::addRenderer(const String& name, WindowRenderer* wr)
{
	auto ret = mRenderers.insert(Renderers::value_type(name, wr));
	if (ret.second)
	{
		if (mDefaultRenderer == nullptr) mDefaultRenderer = wr;
		return;
	}

	ST_EXCEPT(L"windowrenderer is existed", L"WindowSystem::addRenderer");
}

WindowRenderer* WindowSystem::getDefaultRenderer()
{
	if (mDefaultRenderer != nullptr)
		return mDefaultRenderer;
	ST_EXCEPT(L"default windowrenderer is not existed", L"WindowSystem::getDefaultRenderer");
}

void WindowSystem::addRenderRoot(Window* root)
{
	mRoots.push_back(root);
}

void WindowSystem::removeRenderRoot(Window* root)
{
	auto ret = std::find(mRoots.begin(), mRoots.end(), root);
	if (ret != mRoots.end()) mRoots.erase(ret);
}

void WindowSystem::addRenderObjectFactory(const String& name, RenderObjectFactory* fac)
{
	auto ret = mROFactorys.insert(RenderObjectFactorys::value_type(name, fac));
	if (!ret.second) { ST_EXCEPT(L"Specify RenderObjectFactory is existed.", L"WindowSystem::addRenderObjectFactory")}

}

RenderObjectFactory* WindowSystem::getRenderObjectFactory(const String& name)
{
	auto ret = mROFactorys.find(name);
	if (ret != mROFactorys.end()) return ret->second;

	ST_EXCEPT(L"Specify RenderObjectFactory is not existed.", L"WindowSystem::getRenderObjectFactory");
}

RenderObjectFactory* WindowSystem::removeRenderObjectFactory(const String& name)
{
	auto ret = mROFactorys.find(name);
	if (ret == mROFactorys.end()) return nullptr;

	RenderObjectFactory* fac = ret->second;
	mROFactorys.erase(ret);
	return fac;
}

void WindowSystem::render()
{
	auto endi = mRenderers.end();
	for (auto i = mRenderers.begin(); i != endi; ++i)
		i->second->beginRendering();

	{
		auto endi = mRoots.end();
		for (auto i = mRoots.begin(); i != endi; ++i)
			(*i)->draw();
	}

	for (auto i = mRenderers.begin(); i != endi; ++i)
		i->second->endRendering();
}

TT::Bind* WindowSystem::getScriptBind()
{
	return mBind;		
}

bool WindowSystem::loadScript(const String& filename)
{
	return mTouten->loadFile(filename);
}
