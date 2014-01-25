#ifndef _STRenderTarget_H_
#define _STRenderTarget_H_

#include "STCommon.h"

namespace ST
{
	class RenderTarget
	{
	public :
		virtual void draw(const Geometry* geom) = 0;
		virtual void activate() = 0;
		virtual void deactivate() = 0;
	};
}

#endif