#ifndef _STScriptObjectDefinition_H_
#define _STScriptObjectDefinition_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport ScriptObject
	{
	public:
		static const Char* SCRIPT_WINDOW_MANAGER;

		static const Char* INITIALIZE_WINDOW_MANAGER;
		static const Char* UNINITIALIZE_WINDOW_MANAGER;
		static const Char* GET_WINDOW_MANAGER;

		static const Char* INITIALIZE_WINDOW;

		static const Char* ADD_WINDOW_FUNCTION;

		static const Char* INJECT_MOUSE_MOVE;
		static const Char* INJECT_MOUSE_BUTTON_DOWN;
		static const Char* INJECT_MOUSE_BUTTON_UP;
		static const Char* INJECT_MOUSE_WHEEL;
		static const Char* INJECT_KEY_DOWN;
		static const Char* INJECT_KEY_UP;

	};
}

#endif