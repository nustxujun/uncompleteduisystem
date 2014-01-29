#ifndef _STGDIRenderWindow_H_
#define _STGDIRenderWindow_H_

#include "STGDIRendererCommon.h"
#include "STRenderWindow.h"
#include "STRenderObjectFactory.h"

namespace ST
{
	class ShoutenGDIRenderer GDIRenderWindow : public RenderWindow
	{
	public:
		GDIRenderWindow(Window* win, WindowRenderer* renderer);
		~GDIRenderWindow();

		RectI getWorldAABB();
		void notifyUpdateWindow();
		void render();


	private:
		HWND mWnd;
	};

	class ShoutenGDIRenderer GDIRenderWindowFactory : public RenderObjectFactory
	{
	public:
		RenderObject* createImpl(Window* win, WindowRenderer* renderer);
		void destroyImpl(RenderObject* ro);

		static const Char* NAME;
	};

	

}

#endif