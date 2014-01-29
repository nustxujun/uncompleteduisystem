#include "STStringUtil.h"
#include <sstream>
#include "STColour.h"
using namespace ST;

int StringUtil::toInt(const String& str)
{
	std::wstringstream ss;
	int val;
	ss << str;
	ss >> val;
	return val;
}

unsigned int StringUtil::toUInt(const String& str, bool hex )
{
	std::wstringstream ss;
	unsigned int val;
	if (hex) ss << std::hex;
	ss << str;
	ss >> val;
	return val;
}

Colour StringUtil::toColour(const String& str)
{
	Colour ret;

	const Char hexhead[] = {'0', 'x'};

	if ((*(int*)str.c_str()) == (*(int*)hexhead))
	{
		ret.setFromARGB(toUInt(str));
		return ret;
	}
	else
	{
		StringVector sv = split(str, L",");
		switch (sv.size())
		{
		case 3:
		{
				  float inv = 1.0f / 255.f;
				  return Colour(toInt(sv[0]) * inv, toInt(sv[1]) * inv, toInt(sv[2]) * inv);
		}
			break;
		case 4:
		{
				  float inv = 1.0f / 255.f;
				  return Colour(toInt(sv[1]) * inv, toInt(sv[2]) * inv, toInt(sv[3]) * inv, toInt(sv[0]) * inv);
		}
			break;
		default:
			return Colour::BLACK;
		}
	}


	return ret;
}

StringVector StringUtil::split(const String& str, const String& delims)
{
	StringVector ret;
	ret.reserve(8);

	size_t start = 0;
	size_t pos;

	do
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == String::npos)
		{
			ret.push_back(str.substr(start));
		}
		else
		{
			ret.push_back(str.substr(start, pos - start));

			start = pos + 1;
		
		}
	
	} while (pos != String::npos);
	return ret;
}

