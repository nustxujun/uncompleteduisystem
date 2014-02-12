#include "STWindowHelper.h"
#include "TTFunctor.h"
#include "STWindowFactory.h"
#include "STWindowSystem.h"

using namespace ST;
using namespace TT;
void WindowHelper::registerBaseFunctions(Window* win)
{
	TT::Bind& bind = *WindowSystem::getSingleton().getScriptBind();

	class CreateChildWrapper : public Functor
	{
	public:
		Window* win;
		CreateChildWrapper(Window* w) :win(w){}

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

			win->createChild(Caster::castToString(*paras[0]), &customs, fac);

		}
	};

	registerFunction<CreateChildWrapper>(bind, L"createChildImpl", win, win);

}
