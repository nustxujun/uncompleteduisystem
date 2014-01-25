#ifndef _STStringUtil_H_
#define _STStringUtil_H_

#include "STCommon.h"
#include "STColour.h"

namespace ST
{
	typedef std::vector<String> StringVector;

	class ShoutenExport StringUtil
	{
	public :
		static int toInt(const String& str);
		static Colour toColour(const String& str);

		template<class T>
		static String toString(T val)
		{
			std::wstringstream ss;
			ss << val;
			String ret;
			ss >> ret;
			return ret;
		}

		static StringVector split(const String& str, const String& delims);

	};
}

#endif