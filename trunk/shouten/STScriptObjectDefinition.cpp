#include "STScriptObjectDefinition.h"

using namespace ST;


//windowmanager.tt
const Char* ScriptObject::SCRIPT_WINDOW_MANAGER = L"ShoutenScriptWindowManager";//script window manager instance

const Char* ScriptObject::INITIALIZE_WINDOW_MANAGER = L"InitializeWindowManager";//init function
const Char* ScriptObject::UNINITIALIZE_WINDOW_MANAGER = L"UninitializeWindowManager";//uninit function
const Char* ScriptObject::GET_WINDOW_MANAGER = L"GetWindowManager";

//window.tt
const Char* ScriptObject::INITIALIZE_WINDOW = L"InitializeWindow";

const Char* ScriptObject::ADD_WINDOW_FUNCTION = L"AddWindowFunction";


const Char* ScriptObject::INJECT_MOUSE_MOVE = L"InjectMouseMove";
const Char* ScriptObject::INJECT_MOUSE_BUTTON_DOWN = L"InjectMouseButtonDown";
const Char* ScriptObject::INJECT_MOUSE_BUTTON_UP = L"InjectMouseButtonUp";
const Char* ScriptObject::INJECT_MOUSE_WHEEL = L"InjectMouseWheel";
const Char* ScriptObject::INJECT_KEY_DOWN = L"InjectKeyDown";
const Char* ScriptObject::INJECT_KEY_UP = L"InjectKeyUp";