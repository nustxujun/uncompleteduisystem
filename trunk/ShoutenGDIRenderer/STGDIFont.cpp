#include "STGDIFont.h"
#include "STGDIRenderer.h"
#include "STWindow.h"
using namespace ST;

GDIFont::GDIFont(Window* win, WindowRenderer* renderer):
RenderObject(win, renderer)
{
	
}

GDIFont::~GDIFont()
{}

RectI GDIFont::getWorldAABB()
{
	RectI aabb = getWindow()->getRect();
	aabb.transform(getFullTransform());

	return aabb;
}

void GDIFont::render()
{
	GDIRenderer* renderer = (GDIRenderer*)getRenderer();
	Window* win = getWindow();
	const String& str = win->getProperty(WindowProperty::TEXT);
	const String& font = win->getProperty(WindowProperty::TEXT_FONT);
	renderer->drawString(str,12,0xffff0000,font,win->getWorldAABB());
}


RenderObject* GDIFontFactory::createImpl(Window* win, WindowRenderer* renderer)
{
	return new GDIFont(win, renderer);
}

void GDIFontFactory::destroyImpl(RenderObject* ro)
{
	delete (GDIFont*)ro;
}

const Char* GDIFontFactory::NAME = L"GDIFontFactory";