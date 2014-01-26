#ifndef _STGDIGeometry_H_
#define _STGDIGeometry_H_

#include "STGDIRendererCommon.h"
#include "STGeometry.h"
#include "STRect.h"
#include "STColour.h"
#include "STGDITextrue.h"

namespace ST
{
	class GDIGeometry: public Geometry
	{
	public:
		GDIGeometry(GDIRenderer* renderer);
		~GDIGeometry();

		void addVertex(const Vertex& vert);
		void addVertex(const Vertex* verts, size_t count);
		void setTexture(Texture::Ptr tex);
		void clear();
		void draw()const;

	private:
		GDIRenderer* mRenderer;
		RectI mRect;
		HDC mDC;
		
		using Vertexs = std::vector<Vertex>;
		Vertexs mVertexs;

		Texture::Ptr	mTexture;
	};
}

#endif