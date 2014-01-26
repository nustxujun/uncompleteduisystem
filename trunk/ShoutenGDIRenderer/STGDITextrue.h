#ifndef _STGDITexture_H_
#define _STGDITexture_H_

#include "STTexture.h"
#include "STGDIRendererCommon.h"

namespace Gdiplus
{
	class Image;
}


namespace ST
{

	class GDITexture : public Texture
	{
	public:
		using Ptr = std::shared_ptr<GDITexture>;

	public:
		GDITexture();
		~GDITexture();
		Gdiplus::Image* getImage();

		void loadFromFile(const String& filename);

	private:
		Gdiplus::Image* mImage;
	};
}

#endif