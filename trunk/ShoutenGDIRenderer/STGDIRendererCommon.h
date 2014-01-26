#ifndef _STGDIRendererCommon_H_
#define _STGDIRendererCommon_H_

namespace ST
{
	class GDIGeometry;
	class GDIRenderer;
	class GDIRenderTarget;
	class GDITexture;
}


#ifdef SHOUTEN_STATIC_LIB
#	define ShoutenGDIRenderer
#else
#	ifdef SHOUTEN_GDIRENDERER_EXPORTS
#		define ShoutenGDIRenderer __declspec( dllexport )
#	else
#		define ShoutenGDIRenderer __declspec( dllimport )
#	endif
#endif

#pragma warning(disable:4251)

#include <Windows.h>

#endif