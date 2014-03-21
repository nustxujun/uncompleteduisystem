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
#include "../touten/TTbind.h"
#include <fstream>

//#define MEM_DEBUG

size_t memsize = 0;


struct REC
{
	int create;
	int release;
	std::set<void*> addr;

	REC():
		create(0), release(0)
	{}
};
std::map<int, REC > memrecord;

class Meminfo
{
public:
	void* ptr;
	size_t size;

	bool operator<(const Meminfo& inf)const
	{
		if (ptr < inf.ptr)
		{
			if ((size_t)ptr + size < (size_t)inf.ptr)
				return true;
			else
				throw 1;
		}
		else return false;
	}
};

std::set<Meminfo> ptrset;
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
		printf("%x total:%10d, current:- %d\n",s, memsize, *s);
#endif
		//memset(s, 0xfe, *s + 1);
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
			printf("%x total:%10d, current:- %d\n",s, memsize, *s);
#endif
		}
		s = (int*)::realloc(s, nsize + 4);
		*s = nsize;
		optr = s + 1;
		memsize += nsize;
#ifdef MEM_DEBUG
		memrecord[*s].create += 1;
		memrecord[*s].addr.insert(optr);
		printf("%x total:%10d, current:+ %d\n", s, memsize, *s);

#endif
	}
#endif
	return optr;
}

#include "../ShoutenGDIRenderer/STGDIRenderer.h"
#include "../ShoutenGDIRenderer/STGDIRenderWindow.h"
#include "../ShoutenGDIRenderer/STGDIFont.h"

#include <iostream>
//#pragma comment(lib, "Msimg32.lib") 
#include "../Touten/TTMemoryAllocator.h"
#pragma comment(lib, "touten.lib") 


void Print(const ST::String& text)
{
	std::wcout << text << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int winWidth = GetSystemMetrics(SM_CXSCREEN);
	int winHeight = GetSystemMetrics(SM_CYSCREEN);

	using namespace ST;
	{
		TT::MemoryAllocator::setupMethod(alloc);
		WindowSystem ws;
		GDIRenderer renderer(winWidth, winHeight);
		GDIRenderWindowFactory rwFactory;
		BasicImageFactory biFactory;
		RenderWindowEventProcessor rwep;

		GDIFontFactory	ffactory;

		ws.getScriptBind()->bind(L"print", Print);

		ws.addRenderer(L"gdirenderer", &renderer);
		ws.addRenderObjectFactory(GDIRenderWindowFactory::NAME, &rwFactory);
		ws.addRenderObjectFactory(BasicImageFactory::NAME, &biFactory);
		ws.addRenderObjectFactory(GDIFontFactory::NAME, &ffactory);


		//ws.loadScript(L"test.tt");
		

		renderer.createTexture(L"back",L"pic.jpg");

		WindowManager wm;

		StringVec sv;
		sv.push_back(L"WindowManager.tt");
		sv.push_back(L"Window.tt");
		sv.push_back(L"RenderWindow.tt");
		sv.push_back(L"WindowEventCollection.tt");

		sv.push_back(L"test.tt");

		wm.initialize(sv);

		ws.getScriptBind()->call<void>(L"main");

	

		Window* w = wm.getWindow(L"child2");
		DWORD timer = GetTickCount();
		while (true)
		{
			RenderWindowEventProcessor::pumpMessage();
		
			ws.render();
			DWORD t = GetTickCount() - timer;
			double fps =  (double)t;
			w->setProperty(L"text", StringUtil::toString(fps));
			timer = GetTickCount();
		}



	}

	assert(memsize == 0);

	return 0;
}

