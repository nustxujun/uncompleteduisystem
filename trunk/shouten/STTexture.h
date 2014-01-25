#ifndef _STTexture_H_
#define _STTexture_H_

#include "STCommon.h"
#include <memory>

namespace ST
{
	class ShoutenExport Texture
	{
	public:
		using Ptr = std::shared_ptr<Texture>;

	public:
		virtual ~Texture(){};
	};
}

#endif