#ifndef _STRenderObjectFactory_H_
#define _STRenderObjectFactory_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport RenderObjectFactory
	{
	public: 
		virtual RenderObject* createImpl(Window* win, WindowRenderer* renderer);
		virtual void destroyImpl(RenderObject* ro);

		static const Char* NAME;
	};
}

#endif