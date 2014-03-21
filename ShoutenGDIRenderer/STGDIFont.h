#ifndef _STGDIFont_H_
#define _STGDIFont_H_

#include "STGDIRendererCommon.h"
#include "STRenderObject.h"
#include "STRenderObjectFactory.h"

namespace ST
{
	/*
		简单使用gdi的font封装
	*/
	class ShoutenGDIRenderer GDIFont : public RenderObject
	{
	public:
		GDIFont(Window* win, WindowRenderer* renderer);
		~GDIFont();

		RectI getWorldAABB();
		void render();
	};

	class ShoutenGDIRenderer GDIFontFactory : public RenderObjectFactory
	{
	public:
		RenderObject* createImpl(Window* win, WindowRenderer* renderer);
		void destroyImpl(RenderObject* ro);

		static const Char* NAME;
	};
}

#endif