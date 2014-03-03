#include "STGDIRenderer.h"
#include "STGDIGeometry.h"
#include "STGDITextrue.h"
#include "STException.h"


#include <gdiplus.h> 
#pragma comment(lib, "gdiplus.lib")


using namespace ST;
using namespace Gdiplus;

GDIRenderer::GDIRenderer(int width, int height)
{
	::GdiplusStartupInput si;
	::GdiplusStartup(&mGDIPToken, &si, NULL);

	mDefaultTarget = new GDIRenderTarget(this, width, height);
}

GDIRenderer::~GDIRenderer()
{
	//ÊÍ·Åsharedptr
	mTextureMap.clear();

	if (mDefaultTarget) delete mDefaultTarget;
	mDefaultTarget = nullptr;

	::GdiplusShutdown(mGDIPToken);
}

Geometry* GDIRenderer::createGeometry()
{
	GDIGeometry* geom = new GDIGeometry(this);
	mGeometrySet.insert(geom);
	return geom;
}

void GDIRenderer::destroyGeometry(Geometry* geom)
{
	auto ret = mGeometrySet.find((GDIGeometry*)geom);
	if (ret == mGeometrySet.end())
	{
		ST_EXCEPT(L"cannt find geometry in renderer", L"GDIRenderer::destroyGeometry");
		return;
	}

	delete geom;
	mGeometrySet.erase(ret);
}


Texture::Ptr GDIRenderer::createTexture(const String& name, const String& filename)
{
	GDITexture::Ptr tex(new GDITexture());
	auto ret = mTextureMap.insert(TextureMap::value_type(name, tex));
	if (!ret.second)
	{
		ST_EXCEPT(L"same name tex is existed", L"GDIRenderer::createTexture");
		return Texture::Ptr();
	}

	tex->loadFromFile(filename);
	return ret.first->second;
}

Texture::Ptr GDIRenderer::createOrRetrieveTexture(const String& name, const String& filename)
{
	auto ret = mTextureMap.find(name);
	if (mTextureMap.end() != ret)
		return ret->second;

	return createTexture(name, filename);
}

void GDIRenderer::destroyTexture(const String& name)
{
	auto ret = mTextureMap.find(name);
	if (mTextureMap.end() == ret) return;
	mTextureMap.erase(ret);
}

void GDIRenderer::destroyTexture(Texture::Ptr tex)
{

}

Texture::Ptr GDIRenderer::getTexture(const String& name)
{
	auto ret = mTextureMap.find(name);
	if (mTextureMap.end() == ret) return Texture::Ptr();
	return ret->second;
}

RenderTarget* GDIRenderer::getDefaultRenderTarget()
{
	return mDefaultTarget;
}

void GDIRenderer::beginRendering()
{

}

void GDIRenderer::endRendering()
{

}


void GDIRenderer::setCurrentDC(HDC dc)
{
	mCurrentDC = dc;
}

void GDIRenderer::setRenderSize(int width, int height)
{
	mRenderSize.width = width;
	mRenderSize.height = height;

	mDefaultTarget->resize(width, height);
}

const SizeI& GDIRenderer::getRenderSize()const
{
	return mRenderSize;
}

//================================================================================


void GDIRenderer::drawPoint(int x, int y, const Colour& color, int radius)
{
	Gdiplus::Graphics graphics(mCurrentDC);
	Gdiplus::SolidBrush  brush(Gdiplus::Color(color.getAsARGB()));
	Status s = graphics.FillEllipse(&brush, x, y, radius, radius);
}

void GDIRenderer::drawLine(int x1, int y1, int x2, int y2, const Colour& color, float width)
{
	Gdiplus::Graphics graphics(mCurrentDC);
	Gdiplus::Pen pen(Gdiplus::Color(color.getAsARGB()),width);
	Status s = graphics.DrawLine(&pen, x1, y1, x2, y2);
}

void GDIRenderer::drawRect(const RectI& rect, const Colour& color, float width)
{
	Gdiplus::Graphics graphics(mCurrentDC);
	Gdiplus::Pen pen(Gdiplus::Color(color.getAsARGB()), width);
	Status s = graphics.DrawRectangle(&pen, rect.left, rect.top, rect.width(), rect.height());
}

void GDIRenderer::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Colour& color)
{
	const Point points[] = { { x1, y1 }, { x2, y2 }, { x3, y3 } };
	Gdiplus::Graphics graphics(mCurrentDC);
	Gdiplus::SolidBrush  brush(Gdiplus::Color(color.getAsARGB()));
	Status s = graphics.FillPolygon(&brush, points, 3);
}


void GDIRenderer::drawImage(GDITexture* tex, const RectI& rect)
{
	Gdiplus::Graphics graphics(mCurrentDC);
	graphics.DrawImage(tex->getImage(), rect.left, rect.top, rect.width(), rect.height());

}

void GDIRenderer::fillRect(const RectI& rect, const Colour& color)
{
	Gdiplus::Graphics graphics(mCurrentDC);
	Gdiplus::SolidBrush  brush(Gdiplus::Color(color.getAsARGB()));
	graphics.FillRectangle(&brush, rect.left, rect.top, rect.width(), rect.height());
}

