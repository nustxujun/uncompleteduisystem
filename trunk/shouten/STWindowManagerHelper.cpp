#include "STWindowManagerHelper.h"
#include "STWindowSystem.h"
#include "TTBind.h"
#include "Touten.h"
#include "TTObject.h"
#include "TTCaster.h"
#include "STWindowManager.h"

using namespace ST;
using namespace TT;

void WindowManagerHelper::registerFunction(WindowManager* wm)
{
	TT::Bind& bind = *WindowSystem::getSingleton().getScriptBind();


	//createWindow
	class CreateWindowWrapper : public Functor
	{
	public:
		WindowManager* winMgr;

		CreateWindowWrapper(WindowManager* wm) :winMgr(wm){	}

		void operator()(const ObjectPtr* paras, int paracount, Object* ret)
		{
			if (paracount == 0) return;

			CustomParameters customs;
			String fac = DefaultWinFactory::NAME;

			if (paracount > 1 && paras[1]->val->type == OT_ARRAY)
			{
				Array* arr = paras[1]->val->arr;
				auto endi = arr->end();
				for (auto i = arr->begin(); i != endi; ++i)
					customs[i.key()] = Caster::castToString(i.value());
			}

			if (paracount > 2 && paras[2]->val->type == OT_STRING)
				fac = Caster::castToString(*paras[2]);

			winMgr->createWindow(Caster::castToString(*paras[0]), &customs, fac);
		}
	};

	bind.bind<CreateWindowWrapper>(L"CreateWindowImpl", wm);


	//addRootWindow
	class AddRootWindowWrapper : public Functor
	{
	public :
		WindowManager* winMgr;

		AddRootWindowWrapper(WindowManager* wm) :winMgr(wm){	}

		void operator()(const ObjectPtr* paras, int paracount, Object* ret)
		{
			if (paracount == 0)return;

			Window* win = winMgr->getWindow(Caster::castToString(**paras));

			WindowSystem::getSingleton().addRenderRoot(win);
		}
	};

	bind.bind<AddRootWindowWrapper>(L"AddRootWindow", wm);


}