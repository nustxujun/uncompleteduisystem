#include "STGDIRenderTarget.h"
#include "STGeometry.h"
#include "STGDIRenderer.h"
using namespace ST;

GDIRenderTarget::GDIRenderTarget(GDIRenderer* renderer, int width, int height) :
mRenderer(renderer), mSize(width, height)
{
	createDCObject();
}

void GDIRenderTarget::createDCObject()
{
	HDC screen = ::GetDC(NULL);
	mDC = ::CreateCompatibleDC(screen);

	BITMAPINFOHEADER bi = { 0 };
	bi.biSize = sizeof(bi);
	bi.biWidth = mSize.width;
	bi.biHeight = mSize.height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	mBitmap = ::CreateDIBSection(screen, (BITMAPINFO*)&bi, 0, (void**)&mBuffer, 0, 0);

	assert(mBitmap && L"CreateDIBSection failed");

	::ReleaseDC(NULL, screen);

	HGDIOBJ old = SelectObject(mDC, mBitmap);
	::DeleteObject(old);
}

void GDIRenderTarget::clear()
{
	::DeleteObject(mBitmap);
	::DeleteDC(mDC);
}


GDIRenderTarget::~GDIRenderTarget()
{
	clear();
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

	size_t count = mSize.square();

	//dc上的像素是经过其alpha折算后的值
	// RGB = srcRGB * A / 255;
	//alpha值不变
	//也就是说  0x80ffffff 实际是不透明的 
	//应该为 0x80808080
	Colour real = color * color.a;
	real.a = color.a;

	ARGB c = real.getAsARGB();
	//ARGB c = color.getAsARGB();

	ARGB* head = (ARGB*)mBuffer;
	for (size_t i = 0; i < count; ++i)
		*head++ = c;

	deactivate();

}

const SizeI& GDIRenderTarget::getSize()const
{
	return mSize;
}

void GDIRenderTarget::resize(int width, int height)
{
	mSize.width = width;
	mSize.height = height;

	clear();
	createDCObject();
}

HDC GDIRenderTarget::getDC()
{
	return mDC;
}
