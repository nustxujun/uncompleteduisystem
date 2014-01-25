#ifndef _STVertex_H_
#define _STVertex_H_

#include "STVector.h"
#include "STColour.h"

namespace ST
{
	class ShoutenExport Vertex
	{
	public:
		Vector3F position;
		Vector2F texcoords;
		Colour colour;
	};
}

#endif