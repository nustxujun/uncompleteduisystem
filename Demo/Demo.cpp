// Demo.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <memory>
class TestMem
{
public:
	~TestMem(){ _CrtDumpMemoryLeaks(); }
};

TestMem mem;

#pragma comment(lib, "shouten.lib")
#pragma comment(lib, "shoutengdirenderer.lib")

#include <assert.h>
#include <set>
#include <map>

#include "Shouten.h"
#include <Windows.h>





//#define MEM_DEBUG

size_t memsize = 0;

struct REC
{
	int create;
	int release;
	std::set<void*> addr;
};
std::map<int, REC > memrecord;

void* alloc(void* optr, size_t nsize)
{
#ifndef _DEBUG
	return ::realloc(optr, nsize);
#else
	if (nsize == 0)
	{
		int* s = (int*)optr - 1;
		memsize -= *s;
#ifdef MEM_DEBUG
		memrecord[*s].release += 1;
		memrecord[*s].addr.erase(optr);
		printf("total:%10d, current:- %d\n", memsize, *s);
#endif
		::free(s);
	}
	else
	{
		int* s = 0;
		if (optr)
		{
			s = (int*)optr - 1;
			memsize -= *s;
#ifdef MEM_DEBUG
			memrecord[*s].release += 1;
			memrecord[*s].addr.erase(optr);
			printf("total:%10d, current:- %d\n", memsize, *s);
#endif
		}
		s = (int*)::realloc(s, nsize + 4);
		*s = nsize;
		optr = s + 1;
		memsize += nsize;
#ifdef MEM_DEBUG
		memrecord[*s].create += 1;
		memrecord[*s].addr.insert(optr);
		printf("total:%10d, current:+ %d\n", memsize, *s);
#endif
	}
#endif
	return optr;
}

#include "../ShoutenGDIRenderer/STGDIRenderer.h"
#pragma comment(lib, "Msimg32.lib") 

ST::Window* root = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (root)
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
							 root->injectMouseMove(LOWORD(lParam), HIWORD(lParam), 0, 0);
							// ST::Window* win = root->getHitWindow(LOWORD(lParam), HIWORD(lParam));
							// if (win) win->injectMouseMove(LOWORD(lParam), HIWORD(lParam), 0, 0);
		}
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE instance = ::GetModuleHandle(NULL);

	WNDCLASSEX wc;
	memset(&wc,0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = 0; 
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = NULL; 
	wc.lpszClassName = L"Demo"; 
	::RegisterClassEx(&wc);


	HWND wnd = ::CreateWindowEx(WS_EX_LAYERED /*^ WS_EX_TOOLWINDOW*/, L"Demo", L"Demo", 0,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, instance, NULL);



	::ShowWindow(wnd, SW_SHOW);
	::UpdateWindow(wnd);

	using namespace ST;
	{
		WindowSystem ws;

		ws.loadScript(L"WindowManager.tt");
		ws.loadScript(L"Window.tt");

		GDIRenderer renderer;

		RECT winrect;
		::GetWindowRect(wnd, &winrect);
		size_t winwidth = winrect.right - winrect.left;
		size_t winheight = winrect.bottom - winrect.top;
		renderer.initialise(winwidth, winheight );
		ws.addRenderer(L"gdirenderer", &renderer);

		renderer.createTexture(L"back",L"pic.jpg");

		WindowManager wm;
		CustomParameters paras;
		paras[L"renderer"] = L"gdirenderer";
		paras[WindowProperty::BACKGROUND_COLOUR] = L"255,255,255";
		Window* w = wm.createWindow(L"a", &paras);
		w->setPosition(100, 100);
		w->setSize(100, 50);
		w->setProperty(WindowProperty::BACKGROUND_COLOUR, L"32,255,0,0");
		//w->setProperty(WindowProperty::BACKGROUND_TEXTURE, L"back");
		ws.addRenderRoot(w);

		root = w;
		MSG msg;
		while (true)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{

				GDIRenderTarget* target = (GDIRenderTarget*)renderer.getDefaultRenderTarget();
				target->fill(0x80ffffff);
				ws.render();

				HDC dc = GetDC(wnd);
				//BOOL ret = ::BitBlt(dc, 0, 0, w->getWidth(), w->getHeight(), target->getDC(), 0, 0, SRCCOPY);

				::GetWindowRect(wnd, &winrect);
				size_t winwidth = winrect.right - winrect.left;
				size_t winheight = winrect.bottom - winrect.top;
				POINT dstpos = { winrect.left, winrect.top };
				POINT srcpos = { 0, 0 };
				SIZE size = { winwidth, winheight };
				BLENDFUNCTION bf = { 0 };
				bf.AlphaFormat = AC_SRC_ALPHA;
				bf.BlendFlags = 0;
				bf.BlendOp = AC_SRC_OVER;
				bf.SourceConstantAlpha = 0xff;

				BOOL ret = ::UpdateLayeredWindow(wnd, dc, &dstpos, &size, target->getDC(), &srcpos, 0, &bf, ULW_ALPHA);

				

				ReleaseDC(wnd, dc);
			
			}
		}



	}

	assert(memsize == 0);

	return 0;
}

