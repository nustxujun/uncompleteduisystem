#include "STGDIRenderTarget.h"
#include "STGeometry.h"
#include "STGDIRenderer.h"
using namespace ST;

GDIRenderTarget::GDIRenderTarget(GDIRenderer* renderer, int width, int height) :
mRenderer(renderer), mHeight(height), mWidth(width)
{
	HDC screen = ::GetDC(NULL);
	mDC = ::CreateCompatibleDC(screen);
	mBitmap = ::CreateCompatibleBitmap(screen, width, height);
	::ReleaseDC(NULL, screen);

	SelectObject(mDC, mBitmap);

}

GDIRenderTarget::~GDIRenderTarget()
{
	::DeleteObject(mBitmap);
	::DeleteDC(mDC);
}


void GDIRenderTarget::draw(const Geometry* geom)
{
	activate();

	geom->draw();

	deactivate();
}

void GDIRenderTarget::activate()
{
	mRenderer->setCurrentDC(mDC);
}

void GDIRenderTarget::deactivate()
{
}

void GDIRenderTarget::fill(const Colour& color)
{
	activate();

	::SetBkColor(mDC, color.getAsARGB());
	//mRenderer->fillRect(RectI(0, 0, mWidth, mHeight), color);

	deactivate();

}

size_t GDIRenderTarget::getWidth()const
{
	return mWidth;
}

size_t GDIRenderTarget::getHeight()const
{
	return mHeight;
}

HDC GDIRenderTarget::getDC()
{
	return mDC;
}
