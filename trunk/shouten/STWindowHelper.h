#ifndef _STWindowHelper_H_
#define _STWindowHelper_H_

#include "TTBind.h"
#include "STWindow.h"
#include "STStringUtil.h"

namespace ST
{
	class WindowHelper
	{
	public :
		template<class O, class R, class ... Args>
		void registerFunction(TT::Bind& bind, const String& membername, O* win, R(O::*func)(Args...))
		{
			String realName = StringUtil::toString(win) + win->getName() + L"::" + membername;
			if (bind.getTouten()->isValidFunction(realName)) return;
			bind.bind(realName, win, func);
			bind.call<void>(ScriptObject::ADD_WINDOW_FUNCTION, win->getName().c_str(), membername.c_str(), realName.c_str());
			win->mFuncNames.push_back(realName);
		}

		template<class O, class R, class ... Args>
		void registerFunction(TT::Bind& bind, const String& membername, O* win, R(O::*func)(Args...)const)
		{
			registerFunction(bind, membername, win, (R(O::*)(Args...))func);
		}

		template<class O, class R, class ... Args>
		void registerFunction(TT::Bind& bind, const String& membername, O* win, R(O::*func)(Args...)volatile)
		{
			registerFunction(bind, membername, win, (R(O::*)(Args...))func);
		}
	};
}

#endif