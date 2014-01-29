#ifndef _STRenderTarget_H_
#define _STRenderTarget_H_

#include "STCommon.h"
#include "STColour.h"

namespace ST
{
	class RenderTarget
	{
	public :
		virtual void activate() = 0;
		virtual void deactivate() = 0;

		virtual void fill(const Colour& color) = 0;

		virtual size_t getWidth()const = 0;
		virtual size_t getHeight()const = 0;
	};
}

#endif