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
		size_t getVertexCount()const;
		const Vertex& getVertex(size_t index)const;

		void setTexture(Texture::Ptr tex);
		void clear();
		void draw(const Matrix4& mat)const;
		const RectF& getAABB()const ;

	private:
		GDIRenderer* mRenderer;
		RectF mRect;
		HDC mDC;
		
		using Vertexs = std::vector<Vertex>;
		Vertexs mVertexs;

		Texture::Ptr	mTexture;
	};
}

#endif