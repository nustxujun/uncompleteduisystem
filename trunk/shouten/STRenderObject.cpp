#include "STRenderObject.h"
#include "STWindow.h"
#include "STWindowRenderer.h"
#include "STException.h"
#include "STGeometry.h"
#include "STRenderTarget.h"

using namespace ST;

RenderObject::RenderObject(Window* win, WindowRenderer* renderer) :
	mWindow(win), mRenderer(renderer)
{
	mTarget = renderer->getDefaultRenderTarget();
}

RenderObject::~RenderObject()
{
	removeAllGeometry();
}

Window* RenderObject::getWindow()
{
	return mWindow;
}

WindowRenderer* RenderObject::getRenderer()
{
	return mRenderer;
}

RenderTarget* RenderObject::getRenderTarget()
{
	return mTarget;
}


Geometry* RenderObject::createGeometry()
{
	Geometry* geom = mRenderer->createGeometry();
	mGeometryVec.push_back(geom);
	return geom;
}

size_t RenderObject::getGeometryCount()const
{
	return mGeometryVec.size();
}

Geometry* RenderObject::getGeometry(size_t index)
{
	return mGeometryVec[index];
}

void RenderObject::removeGeometry(size_t index)
{
	if (index >= mGeometryVec.size())
	{
		ST_EXCEPT(L"out of range", L"RenderObject::removeGeometry");
	}
	auto i = mGeometryVec.begin() + index;
	mRenderer->destroyGeometry(*i);

	mGeometryVec.erase(i);
}

void RenderObject::removeAllGeometry()
{
	std::for_each(mGeometryVec.begin(), mGeometryVec.end(), 
		[this](Geometry* geom){
		mRenderer->destroyGeometry(geom); });
}


void RenderObject::render()
{
	Matrix4 mat;
	Vector3 pos((float)mWindow->getAbsX(), (float)mWindow->getAbsY(), 0);
	Quaternion orien = Quaternion::IDENTITY;
	Vector3 scale(1, 1, 1);
	mat.makeTransform(pos, scale, orien);

	mTarget->activate();

	auto endi = mGeometryVec.end();
	for (auto i = mGeometryVec.begin(); i != endi; ++i)
	{
		(*i)->draw(mat);
	}

	mTarget->deactivate();
}

RectI RenderObject::getWorldAABB()
{
	RectF aabb;
	std::for_each(mGeometryVec.begin(), mGeometryVec.end(), [&aabb](const Geometry* geom)
	{
		aabb.merge(geom->getAABB());
	});

	return aabb;
}


void RenderObject::notifyUpdateWindow()
{
}


