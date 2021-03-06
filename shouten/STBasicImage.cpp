#include "STBasicImage.h"
#include "STWindowRenderer.h"
#include "STVertex.h"
#include "STGeometry.h"
#include "STWindowProperty.h"
#include "STWindow.h"
#include "STStringUtil.h"

using namespace ST;

BasicImage::BasicImage(Window* win, WindowRenderer* renderer):
	RenderObject(win, renderer)
{
	mGeom = createGeometry();

	//notifyUpdateWindow();
}

void BasicImage::notifyUpdateWindow(unsigned int dirty)
{
	RenderObject::notifyUpdateWindow(dirty);

	Window* win = getWindow();
	if (!win->isDirty(DT_SIZE | DT_PROPERTY)) return;

	WindowRenderer* renderer = getRenderer();
	mGeom->clear();

	Colour color = StringUtil::toColour(win->getProperty(WindowProperty::BACKGROUND_COLOUR));

	RectI rect = win->getAbsRect();
	float width = rect.width();
	float height = rect.height();

	Vertex verts[6];
	verts[0].position.x = 0;
	verts[0].position.y = 0;
	verts[0].position.z = 0;
	verts[0].colour = color;
	verts[0].texcoords.x = 0;
	verts[0].texcoords.y = 0;


	verts[1].position.x = width;
	verts[1].position.y = 0;
	verts[1].position.z = 0;
	verts[1].colour = color;
	verts[1].texcoords.x = 1;
	verts[1].texcoords.y = 0;

	verts[2].position.x = 0;
	verts[2].position.y = height;
	verts[2].position.z = 0;
	verts[2].colour = color;
	verts[2].texcoords.x = 0;
	verts[2].texcoords.y = 1;

	verts[3].position.x = width;
	verts[3].position.y = 0;
	verts[3].position.z = 0;
	verts[3].colour = color;
	verts[3].texcoords.x = 1;
	verts[3].texcoords.y = 0;

	verts[4].position.x = width;
	verts[4].position.y = height;
	verts[4].position.z = 0;
	verts[4].colour = color;
	verts[4].texcoords.x = 0;
	verts[4].texcoords.y = 1;

	verts[5].position.x = 0;
	verts[5].position.y = height;
	verts[5].position.z = 0;
	verts[5].colour = color;
	verts[5].texcoords.x = 1;
	verts[5].texcoords.y = 1;

	mGeom->addVertex(verts, 6);
	mGeom->setPrimitiveType(PT_TRIANGLELIST);

	const String& tex = win->getProperty(WindowProperty::BACKGROUND_TEXTURE);
	if (tex != WindowProperty::PROPERTY_NULL)
		mGeom->setTexture(renderer->getTexture(tex));
}


BasicImage::~BasicImage()
{

}


RenderObject* BasicImageFactory::createImpl(Window* win, WindowRenderer* renderer)
{
	return new BasicImage(win, renderer);
}

void BasicImageFactory::destroyImpl(RenderObject* ro)
{
	delete (BasicImage*)ro;
}

const Char* BasicImageFactory::NAME = L"BasicImageFactory";