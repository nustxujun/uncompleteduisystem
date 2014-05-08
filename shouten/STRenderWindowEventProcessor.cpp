#include "STRenderWindowEventProcessor.h"
#include "STRenderWindow.h"
#include "STWindow.h"
#include "STException.h"
#include "STWindowSystem.h"

using namespace ST;

RenderWindowEventProcessor::RenderWindowEventProcessor()
{
	HINSTANCE inst = GetModuleHandle(0);
	mHook = SetWindowsHookEx(WH_GETMESSAGE, handleHook, inst, GetCurrentThreadId());
}

RenderWindowEventProcessor::~RenderWindowEventProcessor()
{
	UnhookWindowsHookEx(mHook);
}

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
	switch (msg)
	{//ignore events
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		return S_OK;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void RenderWindowEventProcessor::GlobalMessageProc(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	WindowSystem* ws = WindowSystem::getSingletonPtr();
	assert(ws);

	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			//Window* root = i->second->getWindow();
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			ws->operateRoot(&Window::injectMouseButtonDown, Mouse::LeftButton);
		}
		break;
		case WM_LBUTTONUP:
		{
			//Window* root = i->second->getWindow();
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			ws->operateRoot(&Window::injectMouseButtonUp, Mouse::LeftButton);

		}
		//SendMessage(hWnd, WM_NCLBUTTONUP, HTCAPTION, lParam);
		break;
		case WM_MOUSEMOVE:
		{
			//Window* root = i->second->getWindow();
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			static int lastx = x;
			static int lasty = y;
			ws->operateRoot(&Window::injectMouseMove, x, y, x - lastx, y - lasty);
			lastx = x;
			lasty = y;
		}
		break;
	}
}


LRESULT CALLBACK RenderWindowEventProcessor::handleHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	static RenderWindowEventProcessor* proc = RenderWindowEventProcessor::getSingletonPtr();
	assert(proc);

	if (nCode == HC_ACTION)
	{
		MSG *msg = (MSG*)lParam;
		if (wParam & PM_REMOVE)
			proc->GlobalMessageProc(msg->message, msg->hwnd, msg->wParam, msg->lParam);
	}

	return CallNextHookEx(proc->mHook, nCode, wParam, lParam);
}

