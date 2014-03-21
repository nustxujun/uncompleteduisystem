#ifndef _STGDIRenderer_H_
#define _STGDIRenderer_H_

#include "STGDIRendererCommon.h"
#include "STWindowRenderer.h"
#include "STGDITextrue.h"
#include "STGDIRenderTarget.h"
#include "STRect.h"
#include "STColour.h"
#include "STVector.h"
#include "STVertex.h"

namespace ST
{
	class ShoutenGDIRenderer GDIRenderer : public WindowRenderer
	{
	public:
		GDIRenderer(int width, int height);
		~GDIRenderer();

		Geometry* createGeometry() ;
		void destroyGeometry(Geometry* geom) ;

		Texture::Ptr createTexture(const String& name, const String& filename);
		Texture::Ptr createOrRetrieveTexture(const String& name, const String& filename);
		void destroyTexture(const String& name);
		void destroyTexture(Texture::Ptr tex) ;
		Texture::Ptr getTexture(const String& name) ;

		RenderTarget* getDefaultRenderTarget() ;

		void beginRendering();
		void endRendering();

		void setRenderSize(int width, int height);
		const SizeI& getRenderSize()const;

		//gdirenderer的所有绘制方法将对当前dc进行操作
		void setCurrentDC(HDC dc);

		void drawPoint(int x, int y, const Colour& color, int radius = 1);
		void drawLine(int x1, int y1, int x2, int y2, const Colour& color, float width = 1.0f);
		void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Colour& color);
		void drawRect(const RectI& rect, const Colour& color, float width = 1.0f);
		void drawImage(GDITexture* tex, const RectI& rect);
		void drawString(const String& cont,int length, const Colour& color, const String& font, const RectI& rect);

		void fillRect(const RectI& rect, const Colour& color);

	private:
		SizeI mRenderSize;

		using TextureMap = std::hash_map<String, GDITexture::Ptr>;
		TextureMap mTextureMap;

		using GeometrySet = std::set<GDIGeometry*>;
		GeometrySet mGeometrySet;

		GDIRenderTarget* mDefaultTarget;
		HDC mCurrentDC;
		ULONG_PTR mGDIPToken;
	};
}

#endif