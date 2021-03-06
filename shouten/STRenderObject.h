#ifndef _STRenderObject_H_
#define _STRenderObject_H_

#include "STCommon.h"
#include "STVector.h"
#include "STRect.h"
#include "STMatrix.h"
namespace ST
{
	class ShoutenExport RenderObject
	{
	public:
		RenderObject(Window* win, WindowRenderer* renderer);
		virtual ~RenderObject();
		
		Window* getWindow();
		WindowRenderer* getRenderer();
		RenderTarget* getRenderTarget();

		Geometry* createGeometry();
		size_t getGeometryCount()const;
		Geometry* getGeometry(size_t index);
		void removeGeometry(size_t index);
		void removeAllGeometry();

		const Matrix4& getFullTransform();
		virtual RectI getWorldAABB();
		virtual void notifyUpdateWindow(unsigned int dirty);
		virtual void render() ;
	private:
		WindowRenderer* mRenderer;
		RenderTarget* mTarget;
		Window* mWindow;

		using GeometryVec = std::vector<Geometry*>;
		GeometryVec mGeometryVec;

		Matrix4 mFullTransform;
		bool mTransformOutOfData;
	};
}

#endif