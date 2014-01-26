#include "STGDITextrue.h"

#include <gdiplus.h> 
#pragma comment(lib, "gdiplus.lib")

using namespace ST;
using namespace Gdiplus;

GDITexture::GDITexture():
mImage(nullptr)
{
	
}

GDITexture::~GDITexture()
{
	if (mImage) delete mImage;
	mImage = nullptr;
}

Gdiplus::Image* GDITexture::getImage()
{
	return mImage;
}

void GDITexture::loadFromFile(const String& filename)
{
	mImage = Gdiplus::Image::FromFile(filename.c_str());
}
