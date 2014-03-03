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
#if	1
//#ifndef _DEBUG
	//if (optr)
	//{
	//	Meminfo mi;
	//	mi.ptr = optr;
	//	mi.size = 0;
	//	ptrset.erase(mi);
	//}
	void* ptr = ::realloc(optr, nsize);
	//if (nsize)
	//{
	//	Meminfo mi;
	//	mi.ptr = ptr;
	//	mi.size = nsize;

	//	if (!ptrset.insert(mi).second)
	//		throw 1;
	//}

	return ptr;
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

		ws.getScriptBind()->bind(L"print", Print);

		ws.addRenderer(L"gdirenderer", &renderer);
		ws.addRenderObjectFactory(GDIRenderWindowFactory::NAME, &rwFactory);
		ws.addRenderObjectFactory(BasicImageFactory::NAME, &biFactory);

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

		//CustomParameters paras;
		//paras[L"renderer"] = L"gdirenderer";
		//paras[L"renderobjectfactory"] = GDIRenderWindowFactory::NAME;
		//paras[WindowProperty::BACKGROUND_COLOUR] = L"0x00000000";
		//paras[WindowProperty::SCRIPT_INITIALIZER] = L"InitializeRenderWindow";
		//Window* w = wm.createWindow(L"root", &paras);
		//w->setPosition(100, 100);
		//w->setSize(winWidth, winHeight);
		//w->setProperty(WindowProperty::BACKGROUND_COLOUR, L"0,0,0,0");
		//w->setProperty(WindowProperty::BACKGROUND_TEXTURE, L"back");
		//ws.addRenderRoot(w);

		//paras[L"renderobjectfactory"] = BasicImageFactory::NAME;
		//paras[WindowProperty::BACKGROUND_COLOUR] = L"0x80ff0000";
		//paras[WindowProperty::SCRIPT_INITIALIZER] = L"InitializeWindow";
		//w = w->createChild(L"child", &paras);
		//w->setSize(100, 50);
		//w->setPosition(0, 0);

		//paras[L"renderobjectfactory"] = BasicImageFactory::NAME;
		//paras[WindowProperty::BACKGROUND_COLOUR] = L"0x80ffff00";
		//paras[WindowProperty::SCRIPT_INITIALIZER] = L"InitializeWindow";
		//w = w->createChild(L"child1", &paras);
		//w->setSize(50, 25);
		//w->setPosition(0, 0);

		while (true)
		{
			RenderWindowEventProcessor::pumpMessage();
		
			ws.render();
			
		}



	}

	assert(memsize == 0);

	return 0;
}

