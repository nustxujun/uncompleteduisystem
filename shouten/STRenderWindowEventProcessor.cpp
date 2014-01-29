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
			//case WM_LBUTTONDOWN:
			//{
			//					   ST::Window* win = root->getHitWindow(LOWORD(lParam), HIWORD(lParam));
			//					   if (win) win->injectMouseButtonDown(ST::Mouse::LeftButton);
			//}
			//	//SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
			//	break;
			//case WM_LBUTTONUP:
			//{
			//					 ST::Window* win = root->getHitWindow(LOWORD(lParam), HIWORD(lParam));
			//					 if (win) win->injectMouseButtonUp(ST::Mouse::LeftButton);
			//}
			//	//SendMessage(hWnd, WM_NCLBUTTONUP, HTCAPTION, lParam);
			//	break;
		case WM_MOUSEMOVE:
		{
							 Window* root = (*i)->getWindow();
							 Window* win = root->getChild(L"child");
							  //root->injectMouseMove(LOWORD(lParam), HIWORD(lParam), 0, 0);
							 // ST::Window* win = root->getHitWindow(LOWORD(lParam), HIWORD(lParam));
							 if (win) win->injectMouseMove(LOWORD(lParam), HIWORD(lParam), 0, 0);
		}
			break;
		}

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void RenderWindowEventProcessor::addListener(RenderWindow* win)
{
	mWindows.insert(win);
}

void RenderWindowEventProcessor::removeListener(RenderWindow* win)
{
	mWindows.erase(win);
}