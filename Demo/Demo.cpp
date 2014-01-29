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
#include "../ShoutenGDIRenderer/STGDIRenderWindow.h"

#pragma comment(lib, "Msimg32.lib") 

int _tmain(int argc, _TCHAR* argv[])
{
	const int winWidth = 800;
	const int winHeight = 600;

	using namespace ST;
	{
		WindowSystem ws;
		GDIRenderer renderer;
		GDIRenderWindowFactory rwFactory;
		BasicImageFactory biFactory;
		RenderWindowEventProcessor rwep;

		ws.addRenderer(L"gdirenderer", &renderer);
		ws.addRenderObjectFactory(GDIRenderWindowFactory::NAME, &rwFactory);
		ws.addRenderObjectFactory(BasicImageFactory::NAME, &biFactory);


		ws.loadScript(L"WindowManager.tt");
		ws.loadScript(L"Window.tt");

		renderer.initialise(winWidth, winHeight);
		renderer.createTexture(L"back",L"pic.jpg");

		WindowManager wm;
		CustomParameters paras;
		paras[L"renderer"] = L"gdirenderer";
		paras[L"renderobjectfactory"] = GDIRenderWindowFactory::NAME;
		paras[WindowProperty::BACKGROUND_COLOUR] = L"255,255,255";
		Window* w = wm.createWindow(L"root", &paras);
		//w->setPosition(100, 100);
		w->setSize(winWidth, winHeight);
		w->setProperty(WindowProperty::BACKGROUND_COLOUR, L"0,0,0,0");
		//w->setProperty(WindowProperty::BACKGROUND_TEXTURE, L"back");
		ws.addRenderRoot(w);

		paras[L"renderobjectfactory"] = BasicImageFactory::NAME;
		paras[WindowProperty::BACKGROUND_COLOUR] = L"0x80ff0000";
		w = w->createChild(L"child", &paras);
		w->setSize(100, 50);
		w->setPosition(0, 0);
		while (true)
		{
			RenderWindowEventProcessor::pumpMessage();
		
			ws.render();
			
		}



	}

	assert(memsize == 0);

	return 0;
}

