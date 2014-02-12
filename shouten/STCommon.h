#ifndef _STCommon_H_
#define _STCommon_H_

#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <hash_map>

namespace ST
{
	typedef wchar_t	Char;
	typedef std::wstring String;
	
	typedef std::map<String, String> CustomParameters;
	typedef std::vector<String>	StringVec;

	class Geometry;

	class Matrix3;

	class RenderObject;
	class RenderObjectFactory;
	class RenderTarget;
	class RenderWindow;

	class ScriptSystem;

	class Texture;

	class Window;
	class WindowFactory;
	class WindowManager;
	class WindowRenderer;
	class WindowSystem;

}

#include "STPlatform.h"


#endif