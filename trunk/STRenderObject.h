#ifndef _STRenderObject_H_
#define _STRenderObject_H_

#include "STCommon.h"

namespace ST
{
	class ShoutenExport RenderObject
	{
	public:
		RenderObject(Window* win, WindowRenderer* renderer);
		virtual ~RenderObject();
		
		Window* getWindow();
		WindowRenderer* getRenderer();

		Geometry* createGeometry();
		void removeGeometry(size_t index);
		void removeAllGeometry();

		virtual void notifyUpdateWindow();

		virtual void render() ;
	private:
		WindowRenderer* mRenderer;
		RenderTarget* mTarget;
		Window* mWindow;

		using GeometryVec = std::vector<Geometry*>;
		GeometryVec mGeometryVec;
	};
}

#endif