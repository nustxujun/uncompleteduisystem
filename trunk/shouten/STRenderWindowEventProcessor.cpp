#include "STRenderWindowEventProcessor.h"
#include "STRenderWindow.h"
#include "STWindow.h"
using namespace ST;

void RenderWindowEventProcessor::pumpMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

LRESULT CALLBACK RenderWindowEventProcessor::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static RenderWindowEventProcessor* proc = RenderWindowEventProcessor::getSingletonPtr();

	assert(proc);

	auto i = proc->mWindows.begin();
	auto endi = proc->mWindows.end();

	for (; i != endi; ++i)
	{

		switch (msg)
		{
			case WM_LBUTTONDOWN:
			{
				Window* root = (*i)->getWindow();
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				Window* win = root->getHitWindow(x, y);
				if (win) win->injectMouseButtonDown(Mouse::LeftButton);
			}
			break;
			case WM_LBUTTONUP:
			{
				Window* root = (*i)->getWindow();
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				Window* win = root->getHitWindow(x, y);
				if (win) win->injectMouseButtonUp(Mouse::LeftButton);
			}
			//SendMessage(hWnd, WM_NCLBUTTONUP, HTCAPTION, lParam);
			break;
			case WM_MOUSEMOVE:
			{
				Window* root = (*i)->getWindow();
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				static int lastx = x;
				static int lasty = y;
				Window* win = root->getHitWindow(x, y);
				if (win) win->injectMouseMove(x, y, x - lastx, y - lasty);
				lastx = x;
				lasty = y;
			}
			break;
		}

	}
	LRESULT ret =  DefWindowProc(hWnd, msg, wParam, lParam);
	return ret;
}


void RenderWindowEventProcessor::addListener(RenderWindow* win)
{
	mWindows.insert(win);
}

void RenderWindowEventProcessor::removeListener(RenderWindow* win)
{
	mWindows.erase(win);
}