#ifndef _BasicImage_H_
#define _BasicImage_H_

#include "STCommon.h"
#include "STRenderObject.h"

#include "STRenderObjectFactory.h"

namespace ST
{
	class ShoutenExport BasicImage : public RenderObject
	{
	public:
		BasicImage(Window* win, WindowRenderer* renderer);
		~BasicImage();

		void notifyUpdateWindow(unsigned int dirty);

	private:
		Geometry* mGeom;
	};

	class ShoutenExport BasicImageFactory : public RenderObjectFactory
	{
	public:
		RenderObject* createImpl(Window* win, WindowRenderer* renderer);
		void destroyImpl(RenderObject* ro);

		static const Char* NAME;

	};
}

#endif