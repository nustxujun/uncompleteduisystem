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
	public:
		WindowSystem();
		~WindowSystem();

		WindowRenderer* getRenderer(const String& name);
		void addRenderer(const String& name, WindowRenderer* wr);

		void addRenderRoot(Window* root);
		void removeRenderRoot(Window* root);

		void loadScript(const String& filename);

		void render();

		TT::Bind* getScriptBind();
	private:
		using Renderers = std::hash_map<String, WindowRenderer*>;
		Renderers mRenderers;

		using RenderRoots = std::vector<Window*>;
		RenderRoots mRoots;

		TT::Touten* mTouten;
		TT::Bind* mBind;
	};
}

#endif