#include "STRenderObject.h"
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

Geometry* RenderObject::createGeometry()
{
	Geometry* geom = mRenderer->createGeometry();
	mGeometryVec.push_back(geom);
	return geom;
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
	auto endi = mGeometryVec.end();
	for (auto i = mGeometryVec.begin(); i != endi; ++i)
	{
		mTarget->draw(*i);
	}
}

void RenderObject::notifyUpdateWindow()
{
}


