#ifndef _STScriptObjectDefinition_H_
#define _STScriptObjectDefinition_H_

#include "STCommon.h"

namespace ST
{
	const String SCRIPT_WINDOW_MANAGER = L"ShoutenScriptWindowManager";//script window manager instance

	const String INITIALIZE_WINDOW_MANAGER = L"initializeWindowManager";//init function
	const String UNINITIALIZE_WINDOW_MANAGER = L"uninitializeWindowManager";//uninit function
	const String GET_WINDOW_MANAGER = L"getWindowManager";

}

#endif