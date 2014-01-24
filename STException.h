#ifndef _STException_H_
#define _STException_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport Exception
	{
	public :
		Exception(const String& content, const String& type);

		virtual ~Exception();
		const String& getMessage();

	protected:

		String mContent;
		String mType;
	};

#define ST_EXCEPT(x, y) {throw Exception(x, y);}
}

#endif