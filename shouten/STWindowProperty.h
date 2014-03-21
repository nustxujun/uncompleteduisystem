#ifndef _STWindowProperty_H_
#define _STWindowProperty_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport WindowProperty
	{
	public:
		static const Char* PROPERTY_NULL;
		static const Char* BACKGROUND_TEXTURE;
		static const Char* BACKGROUND_COLOUR;
		static const Char* TEXT;
		static const Char* TEXT_FONT;
		static const Char* TEXT_COLOUR;
		static const Char* TEXT_LENGTH;

		static const Char* RENDERER;
		static const Char* RENDER_OBJECT_FACTORY;
		static const Char* SCRIPT_INITIALIZER;
	};
}

#endif