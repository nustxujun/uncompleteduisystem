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

size_t GDIGeometry::getVertexCount()const
{
	return mVertexs.size();
}

const Vertex& GDIGeometry::getVertex(size_t index)const
{
	return mVertexs[index];
}

Vertex& GDIGeometry::getVertex(size_t index)
{
	return mVertexs[index];
}


const RectF& GDIGeometry::getAABB()const
{
	return mRect;
}

void GDIGeometry::setTexture(Texture::Ptr tex)
{
	mTexture = tex;
}

void GDIGeometry::clear()
{
	mVertexs.clear();
	mRect.setNull();
}

void GDIGeometry::draw(const Matrix4& mat)const
{
	size_t vertCount = mVertexs.size();
	if (vertCount == 0) return;

	const RenderState& state = getRenderState();

	std::vector<Vector3> poss;
	poss.resize(vertCount);

	for (size_t i = 0; i < vertCount; ++i)
	{
		poss[i] = mat * mVertexs[i].position;
	}


	if (mTexture)
	{
		mRenderer->drawImage((GDITexture*)mTexture.get(), mRect);
		return;
	}
	else
	{

		Colour color = mVertexs[0].colour;

		switch (state.primitiveType)
		{

			case PT_POINTS:
			{
				std::for_each(poss.begin(), poss.end(), [this, poss, color](const Vector3& pos)
				{
					mRenderer->drawPoint(pos.x, pos.y, color);
				});
			}
			break;
			case PT_LINELIST:
			{
				size_t count = poss.size() & (~1);
				for (size_t i = 0; i < count;)
				{
					const Vector3& pos1 = poss[i];
					const Vector3& pos2 = poss[i + 1];
					mRenderer->drawLine(pos1.x, pos1.y,
										pos2.x, pos2.y,
										color);

					i += 2;
				}
			}
			break;
			case PT_LINESTRIP:
			{
				size_t count = poss.size() - 1;
				for (size_t i = 0; i < count; ++i)
				{
					const Vector3& pos1 = poss[i];
					const Vector3& pos2 = poss[i + 1];
					mRenderer->drawLine(pos1.x, pos1.y,
										pos2.x, pos2.y,
										color);
				}
			}
			break;
			case PT_TRIANGLELIST:
			{
				size_t count = poss.size() / 3;
				for (size_t i = 0; i < count; ++i)
				{
					const Vector3& pos1 = poss[i * 3 + 0];
					const Vector3& pos2 = poss[i * 3 + 1];
					const Vector3& pos3 = poss[i * 3 + 2];
					mRenderer->drawTriangle(pos1.x, pos1.y,
											pos2.x, pos2.y,
											pos3.x, pos3.y,
											color);
				}

			}
			break;
			case PT_TRIANGLESTRIP:
			{
				size_t count = poss.size() - 2;
				for (size_t i = 0; i < count; ++i)
				{
					const Vector3& pos1 = poss[i + 0];
					const Vector3& pos2 = poss[i + 1];
					const Vector3& pos3 = poss[i + 2];
					mRenderer->drawTriangle(pos1.x, pos1.y,
											pos2.x, pos2.y,
											pos3.x, pos3.y,
											color);
				}
			}
			break;
			case PT_TRIANGLEFAN:
			{}
			break;
		}

	}

}
