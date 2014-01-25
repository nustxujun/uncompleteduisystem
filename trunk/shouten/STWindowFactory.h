#ifndef _STWindowFactory_H_
#define _STWindowFactory_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport WindowFactory
	{
	public:
		virtual Window* createWindowImpl(const String& name, WindowManager* wm)const = 0;
		virtual void destroyWindowImpl(Window* window)const = 0;
	};

	class ShoutenExport DefaultWinFactory : public WindowFactory
	{
	public:
		Window* createWindowImpl(const String& name, WindowManager* wm)const;
		void destroyWindowImpl(Window* window)const;

		static const Char* NAME;
	};
}

#endif