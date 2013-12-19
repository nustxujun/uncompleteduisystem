#ifndef _STWindowManager_H_
#define _STWindowManager_H_

#include "STCommon.h"
#include "STWindow.h"
#include "Touten.h"
#include "TTBind.h"

namespace ST
{
	class WindowManager
	{
	public :
		WindowManager();
		~WindowManager();

		Window* createWindow(const String& name, const CustomParameters* paras = 0, const String& factory = DefaultWinFactory::NAME);
		Window* getWindow(const String& name);
		void destroyWindow(const String& name);

		void registerFactory(const String& name, WindowFactory* factory);
		WindowFactory* unregisterFactory(const String& name);

		bool isWindowExisted(const String& name)const;

		void initializeScript();
		void uninitialzeScript();
	private:
		TT::Touten mTouten;
		TT::Bind mBind;

		typedef std::map<String , WindowFactory*> Factorys;
		Factorys mFactorys;

		typedef std::hash_map<String, Window*> Windows;
		Windows mWindows;

		DefaultWinFactory mDefaultFactory;
	};

}

#endif