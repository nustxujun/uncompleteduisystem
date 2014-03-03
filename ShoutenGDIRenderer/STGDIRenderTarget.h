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

		void activate() ;
		void deactivate() ;

		void fill(const Colour& color);

		const SizeI& getSize()const ;
		void resize(int width, int height) ;

		HDC getDC();

	private:
		void createDCObject();
		void clear();

	private:
		HDC mDC;
		HBITMAP mBitmap;
		GDIRenderer* mRenderer;
		void* mBuffer;
		SizeI mSize;
	};
}

#endif