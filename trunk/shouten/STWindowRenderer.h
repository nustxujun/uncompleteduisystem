#ifndef _STWindowRenderer_H_
#define _STWindowRenderer_H_

#include "STCommon.h"
#include "STTexture.h"

namespace ST
{
	class ShoutenExport WindowRenderer
	{
	public:
	
		virtual Geometry* createGeometry() = 0;
		virtual void destroyGeometry(Geometry* geom) = 0;

		virtual Texture::Ptr createTexture(const String& name, const String& filename) = 0;
		virtual Texture::Ptr createOrRetrieveTexture(const String& name, const String& filename) = 0;
		virtual void destroyTexture(const String& name) = 0;
		virtual void destroyTexture(Texture::Ptr tex) = 0;
		virtual Texture::Ptr getTexture(const String& name) = 0;

		virtual RenderTarget* getDefaultRenderTarget() = 0;

		virtual void beginRendering() = 0;
		virtual void endRendering() = 0;
	};
}

#endif