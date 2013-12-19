#ifndef _STWindow_H_
#define _STWindow_H_

#include "STCommon.h"
namespace ST
{
	class Window
	{
	public :
		Window(const String& name, const CustomParameters* paras, const WindowFactory* f);
		~Window();

		const String& getName()const;
		const WindowFactory* getFactory()const;
	private:
		String mName;
		const WindowFactory* mFactory;
		WindowRenderer* mRenderer;
	};

	class WindowFactory
	{
	public :
		virtual Window* createWindowImpl(const String& name, const CustomParameters* paras)const = 0;
		virtual void destroyWindowImpl(Window* window)const = 0;
	};

	class DefaultWinFactory: public WindowFactory
	{
	public :
		Window* createWindowImpl(const String& name, const CustomParameters* paras)const;
		void destroyWindowImpl(Window* window)const;

		static const String NAME;
	};
}

#endif