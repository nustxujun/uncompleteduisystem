#include "STRenderObjectFactory.h"
#include "STRenderObject.h"
using namespace ST;

const Char* RenderObjectFactory::NAME = L"RenderObjectFactory";


RenderObject* RenderObjectFactory::createImpl(Window* win, WindowRenderer* renderer)
{
	return new RenderObject(win, renderer);
}

void RenderObjectFactory::destroyImpl(RenderObject* ro)
{
	delete ro;
}
