#include "STGDIGeometry.h"
#include "STGDIRenderer.h"

using namespace ST;

GDIGeometry::GDIGeometry(GDIRenderer* renderer) :
mRenderer(renderer)
{
}

GDIGeometry::~GDIGeometry()
{
}

void GDIGeometry::addVertex(const Vertex& vert)
{
	mRect.merge(vert.position.x, vert.position.y);
	mVertexs.push_back(vert);
}

void GDIGeometry::addVertex(const Vertex* verts, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		mRect.merge(verts[i].position.x, verts[i].position.y);
		mVertexs.push_back(verts[i]);
	}
}

void GDIGeometry::setTexture(Texture::Ptr tex)
{
	mTexture = tex;
}

void GDIGeometry::clear()
{
	mVertexs.clear();
}

void GDIGeometry::draw()const
{
	const RenderState& state = getRenderState();

	if (mTexture)
	{
		mRenderer->drawImage((GDITexture*)mTexture.get(), mRect);
		return;
	}



	switch (state.primitiveType)
	{

	case PT_POINTS:
		{
			std::for_each(mVertexs.begin(), mVertexs.end(), [this](const Vertex& vert)
			{
				mRenderer->drawPoint(vert.position.x, vert.position.y, vert.colour);
			});
		}
		break;
	case PT_LINELIST:
		{
			size_t count = mVertexs.size() & (~1);
			for (size_t i = 0; i < count;)
			{
				const Vertex& vert1 = mVertexs[i];
				const Vertex& vert2 = mVertexs[i + 1];
				mRenderer->drawLine(vert1.position.x, vert1.position.y,
					vert2.position.x, vert2.position.y,
					vert1.colour);

				i += 2;
			}
		}
			break;
	case PT_LINESTRIP:
		{
			size_t count = mVertexs.size() - 1;
			for (size_t i = 0; i < count; ++i)
			{
				const Vertex& vert1 = mVertexs[i];
				const Vertex& vert2 = mVertexs[i + 1];
				mRenderer->drawLine(vert1.position.x, vert1.position.y,
					vert2.position.x, vert2.position.y,
					vert1.colour);
			}
		}
		break;
	case PT_TRIANGLELIST:
		{
			size_t count = mVertexs.size() / 3;
			for (size_t i = 0; i < count; ++i)
			{
				const Vertex& vert1 = mVertexs[i * 3 + 0];
				const Vertex& vert2 = mVertexs[i * 3 + 1];
				const Vertex& vert3 = mVertexs[i * 3 + 2];
				mRenderer->drawTriangle(vert1.position.x, vert1.position.y,
					vert2.position.x, vert2.position.y,
					vert3.position.x, vert3.position.y,
					vert1.colour);
			}

		}
		break;
	case PT_TRIANGLESTRIP:
		{
			size_t count = mVertexs.size() - 2;
			for (size_t i = 0; i < count; ++i)
			{
				const Vertex& vert1 = mVertexs[i + 0];
				const Vertex& vert2 = mVertexs[i + 1];
				const Vertex& vert3 = mVertexs[i + 2];
				mRenderer->drawTriangle(vert1.position.x, vert1.position.y,
					vert2.position.x, vert2.position.y,
					vert3.position.x, vert3.position.y,
					vert1.colour);
			}
		}
		break;
	case PT_TRIANGLEFAN:
	{}
		break;

	}

}
