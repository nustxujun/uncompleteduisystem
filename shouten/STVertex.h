#ifndef _STVertex_H_
#define _STVertex_H_

#include "STVector.h"
#include "STColour.h"

namespace ST
{
	class ShoutenExport Vertex
	{
	public:
		Vector3 position;
		Vector2 texcoords;
		Colour colour;
	};
}

#endif