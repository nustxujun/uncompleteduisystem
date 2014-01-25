#ifndef _STRenderState_H_
#define _STRenderState_H_

namespace ST
{
	enum PrimitiveType
	{
		PT_POINTS,
		PT_LINELIST,
		PT_LINESTRIP,
		PT_TRIANGLELIST,
		PT_TRIANGLESTRIP,
		PT_TRIANGLEFAN,
	};

	class RenderState
	{
	public:
		PrimitiveType primitiveType;
	};
}

#endif