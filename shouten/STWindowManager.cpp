#include "STWindowManager.h"
#include "STScriptObjectDefinition.h"
#include "STException.h"
#include "STWindow.h"
#include "STWindowSystem.h"
#include "STWindowManagerHelper.h"
#include "TTBind.h"

using namespace ST;

WindowManager::WindowManager()
{



}

WindowManager::~WindowManager()
{


}

void WindowManager::initializeImpl(const StringVec& files)
{
	registerFactory(DefaultWinFactory::NAME, &mDefaultFactory);

	initializeScript(files);
}

void WindowManager::uninitializeImpl()
{
	std::for_each(mWindows.begin(), mWindows.end(), [](Windows::value_type e){
		delete e.second;
	});

	uninitialzeScript();
}

void WindowManager::initializeScript(const StringVec& files)
{
	WindowManagerHelper helper;
	helper.registerFunction(this);

	WindowSystem& sys = WindowSystem::getSingleton();

	auto endi = files.end();
	for (auto i = files.begin(); i != endi; ++i)
		sys.loadScript(*i);

	sys.getScriptBind()->call<void>(ScriptObject::INITIALIZE_WINDOW_MANAGER);

}

void WindowManager::uninitialzeScript()
{
	//mTouten->call(ScriptObject::UNINITIALIZE_WINDOW_MANAGER);
}

Window* WindowManager::createWindow(const String& name, const CustomParameters* paras, const String& factory )
{
	auto fac = mFactorys.find(factory);
	if (fac == mFactorys.end())
	{
		ST_EXCEPT(L"Cant find specify factory", L"WindowManager");
		return nullptr;
	}

	if (isWindowExisted(name))
	{
		ST_EXCEPT(L"specify window is existed", L"WindowManager");
		return nullptr;
	}
	
	Window* win = fac->second->createWindowImpl(name, this);
	win->initialize(paras);

	mWindows.insert(Windows::value_type(name, win));
	return win;
}

Window* WindowManager::getWindow(const String& name)
{
	auto win = mWindows.find(name);
	if (win == mWindows.end())
	{
		ST_EXCEPT(L"specify window is not existed", L"WindowManager");
		return nullptr;		
	}
	return win->second;
}

void WindowManager::destroyWindow(const String& name)
{
	auto win = mWindows.find(name);
	if (win == mWindows.end()) return;

	win->second->getFactory()->destroyWindowImpl(win->second);
	mWindows.erase(win);
}

void WindowManager::registerFactory(const String& name, WindowFactory* factory)
{
	auto ret = mFactorys.insert(Factorys::value_type(name, factory));
	if (!ret.second)
		ST_EXCEPT(L"specify factory is existed", L"WindowManager::registerFactory");
}

WindowFactory* WindowManager::unregisterFactory(const String& name)
{
	auto ret = mFactorys.find(name);
	if (ret != mFactorys.end())
	{
		WindowFactory* wf = ret->second;
		mFactorys.erase(ret);
		return wf;
	}

	ST_EXCEPT(L"specify factory is not existed", L"WindowManager::unregisterFactory");
	return nullptr;
}

bool WindowManager::isWindowExisted(const String& name)const
{
	return mWindows.find(name) != mWindows.end();
}

