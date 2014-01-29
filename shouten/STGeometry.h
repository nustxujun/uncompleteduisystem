#ifndef _STGeometry_H_
#define _STGeometry_H_

#include "STCommon.h"
#include "STVertex.h"
#include "STTexture.h"
#include "STRenderState.h"
#include "STMatrix.h"
#include "STRect.h"

namespace  ST
{
	class ShoutenExport Geometry
	{
	public :
		Geometry();
		virtual ~Geometry() {}
		virtual void addVertex(const Vertex& vert) = 0;
		virtual void addVertex(const Vertex* verts, size_t count) = 0;
		virtual size_t getVertexCount()const = 0;
		virtual const Vertex& getVertex(size_t index)const = 0;
		virtual void setTexture(Texture::Ptr tex) = 0;
		virtual void clear() = 0;
		virtual void draw(const Matrix4& mat) const = 0;
		virtual const RectF& getAABB()const = 0;

		const RenderState& getRenderState()const;

		void setPrimitiveType(PrimitiveType mode);

	private:
		RenderState mRenderState;
	};
}

#endif