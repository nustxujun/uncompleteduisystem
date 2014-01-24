#ifndef _BasicImage_H_
#define _BasicImage_H_

#include "STCommon.h"
#include "STRenderObject.h"

namespace ST
{
	class ShoutenExport BasicImage : public RenderObject
	{
	public:
		BasicImage(Window* win, WindowRenderer* renderer);
		~BasicImage();

		void notifyUpdateWindow();

	private:
		Geometry* mGeom;
	};
}

#endif