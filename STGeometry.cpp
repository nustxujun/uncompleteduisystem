#include "STGeometry.h"

using namespace ST;

Geometry::Geometry()
{
	mRenderState.primitiveType = PT_TRIANGLELIST;

}

void Geometry::setPrimitiveType(PrimitiveType mode)
{
	mRenderState.primitiveType = mode;
}

const RenderState& Geometry::getRenderState()const
{
	return mRenderState;
}
