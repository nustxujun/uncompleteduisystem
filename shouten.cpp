//

#include "stdafx.h"
#include "STWindowManager.h"
#include "STScriptObjectDefinition.h"
#include "TTMemoryAllocator.h"

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


int _tmain(int argc, _TCHAR* argv[])
{
	{
		using namespace ST;
		TT::MemoryAllocator::setupMethod(alloc);


		WindowManager wm;
		Window* w = wm.createWindow(L"a", 0);
	}

	assert(memsize == 0);

	return 0;
}

