#ifndef _STWindowManager_H_
#define _STWindowManager_H_

#include "STCommon.h"
#include "STSingleton.h"
#include "STWindowFactory.h"
#include "STInitializer.h"

namespace ST
{

	class ShoutenExport WindowManager:
		public Singleton<WindowManager> ,
		public Initializer<const StringVec&>
	{
	public :
		WindowManager();
		~WindowManager();

		void initializeImpl(const StringVec& files);
		void uninitializeImpl();

		Window* createWindow(const String& name, const CustomParameters* paras = nullptr, const String& factory = DefaultWinFactory::NAME);
		Window* getWindow(const String& name);
		void destroyWindow(const String& name);

		void registerFactory(const String& name, WindowFactory* factory);
		WindowFactory* unregisterFactory(const String& name);

		bool isWindowExisted(const String& name)const;

		void initializeScript(const StringVec& files);
		void uninitialzeScript();

	private:
		typedef std::map<String , WindowFactory*> Factorys;
		Factorys mFactorys;

		typedef std::hash_map<String, Window*> Windows;
		Windows mWindows;

		DefaultWinFactory mDefaultFactory;
	};

}

#endif