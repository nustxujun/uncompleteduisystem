#ifndef _STRenderWindow_H_
#define _STRenderWindow_H_

#include "STCommon.h"
#include "STRenderObject.h"

namespace ST
{
	class ShoutenExport RenderWindow : public RenderObject
	{
	public:
		RenderWindow(Window* win, WindowRenderer* renderer);
		virtual ~RenderWindow();


	};
}

#endif