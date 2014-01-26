#ifndef _STGDIRenderTarget_H_
#define _STGDIRenderTarget_H_

#include "STGDIRendererCommon.h"
#include "STRenderTarget.h"

namespace ST
{
	class ShoutenGDIRenderer GDIRenderTarget : public RenderTarget
	{
	public:
		GDIRenderTarget(GDIRenderer* renderer, int width, int height);
		~GDIRenderTarget();

		void draw(const Geometry* geom) ;
		void activate() ;
		void deactivate() ;

		void fill(const Colour& color);

		size_t getWidth()const;
		size_t getHeight()const;

		HDC getDC();

	private:
		HDC mDC;
		HBITMAP mBitmap;
		GDIRenderer* mRenderer;
		size_t mHeight;
		size_t mWidth;
	};
}

#endif