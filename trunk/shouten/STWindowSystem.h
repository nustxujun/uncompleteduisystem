#ifndef _STWindowSystem_H_
#define _STWindowSystem_H_

#include "STCommon.h"
#include "STSingleton.h"

namespace TT
{
	class Touten;
	class Bind;
};

namespace ST
{
	class ShoutenExport WindowSystem : public Singleton<WindowSystem>
	{
	public :
		typedef void* (*AllocMethod) (void* optr, size_t nsize);

	public:
		WindowSystem(AllocMethod func = nullptr);
		~WindowSystem();

		WindowRenderer* getRenderer(const String& name);
		void addRenderer(const String& name, WindowRenderer* wr);

		void addRenderRoot(Window* root);
		void removeRenderRoot(Window* root);

		void addRenderObjectFactory(const String& name, RenderObjectFactory* fac);
		RenderObjectFactory* getRenderObjectFactory(const String& name);
		RenderObjectFactory* removeRenderObjectFactory(const String& name);

		bool loadScript(const String& filename);

		void render();

		TT::Bind* getScriptBind();
	private:
		using Renderers = std::hash_map<String, WindowRenderer*>;
		Renderers mRenderers;

		using RenderRoots = std::vector<Window*>;
		RenderRoots mRoots;

		using RenderObjectFactorys = std::map<String, RenderObjectFactory*>;
		RenderObjectFactorys mROFactorys;

		TT::Touten* mTouten;
		TT::Bind* mBind;
	};
}

#endif