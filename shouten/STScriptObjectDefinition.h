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

		static const Char* ADD_WINDOW_FUNCTION;
	};
}

#endif