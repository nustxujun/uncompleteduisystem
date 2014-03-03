#ifndef _STRenderTarget_H_
#define _STRenderTarget_H_

#include "STCommon.h"
#include "STColour.h"
#include "STSize.h"

namespace ST
{
	class RenderTarget
	{
	public :
		virtual void activate() = 0;
		virtual void deactivate() = 0;

		virtual void fill(const Colour& color) = 0;

		virtual const SizeI& getSize()const = 0;
		virtual void resize(int width, int height) = 0;
	};
}

#endif