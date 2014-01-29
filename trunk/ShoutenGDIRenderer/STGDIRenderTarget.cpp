#include "STGDIRenderTarget.h"
#include "STGeometry.h"
#include "STGDIRenderer.h"
using namespace ST;

GDIRenderTarget::GDIRenderTarget(GDIRenderer* renderer, int width, int height) :
mRenderer(renderer), mHeight(height), mWidth(width)
{
	HDC screen = ::GetDC(NULL);
	mDC = ::CreateCompatibleDC(screen);
	//mBitmap = ::CreateCompatibleBitmap(screen, width, height);


	BITMAPINFOHEADER bi = { 0 };
	bi.biSize = sizeof(bi);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount  = 32;
	bi.biCompression = BI_RGB;
	mBitmap = ::CreateDIBSection(screen, (BITMAPINFO*)&bi, 0, (void**)&mBuffer, 0, 0);

	HRESULT ret = GetLastError();

	::ReleaseDC(NULL, screen);

	SelectObject(mDC, mBitmap);

}

GDIRenderTarget::~GDIRenderTarget()
{
	::DeleteObject(mBitmap);
	::DeleteDC(mDC);
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

	size_t count = mWidth * mHeight;

	//dc�ϵ������Ǿ�����alpha������ֵ
	// RGB = srcRGB * A / 255;
	//alphaֵ����
	//Ҳ����˵  0x80ffffff ʵ���ǲ�͸���� 
	//Ӧ��Ϊ 0x80808080
	Colour real = color * color.a;
	real.a = color.a;

	ARGB c = real.getAsARGB();
	//ARGB c = color.getAsARGB();

	ARGB* head = (ARGB*)mBuffer;
	for (size_t i = 0; i < count; ++i)
		*head++ = c;

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
