#include "STWindowHelper.h"
#include "TTFunctor.h"
#include "STWindowFactory.h"
#include "STWindowSystem.h"
#include "STGeometry.h"

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


	class SetGeometry:public Functor
	{
	public:
		Window* win;
		SetGeometry(Window* w) :win(w){}

		void operator()(const ObjectPtr* paras, int paracount, Object* ret)
		{
			if (paracount == 0) return;
			if (paras[0]->val->type != OT_ARRAY) return;
			
			RenderObject* ro = win->getRenderObject();
			

			auto convertGeom = [](Geometry* geom, const ObjectPtr& obj)->bool
			{
				if (obj->val->type != OT_ARRAY) return false;
				Array* geomarr = obj->val->arr;
				if (!geomarr->isArray()) return false;

				auto getfloat = [](float& val, const Char* key, const Array* arr)->bool
				{
					ObjectPtr& obj = arr->get(key);
					if (obj.isNull()) return false;

					if (obj->val->type != OT_DOUBLE &&
						obj->val->type != OT_INTEGER)
						return false;

					val = Caster::castToReal(*obj);
					return true;

				};

				int oldcount = geom->getVertexCount();

				for (int i = 0;; ++i)
				{
					ObjectPtr& v = geomarr->get(i);
					if (v.isNull() || v->val->type != OT_ARRAY) break;
					Array* arr = v->val->arr;

					Vertex vert;
					float x, y, z, w;

					ObjectPtr& pos = arr->get(L"position");
					if (pos.isNull() || pos->val->type != OT_ARRAY) return false;

					if (!getfloat(x, L"x", pos->val->arr)) return false;
					if (!getfloat(y, L"y", pos->val->arr)) return false;
					if (!getfloat(z, L"z", pos->val->arr)) return false;

					vert.position = Vector3(x, y, z);

					ObjectPtr& texcd = arr->get(L"texcoords");
					if (!texcd.isNull())
					{

						if (getfloat(x, L"x", texcd->val->arr) && 
							getfloat(y, L"y", texcd->val->arr)) 
							vert.texcoords = Vector2(x, y);
					}

					ObjectPtr& color = arr->get(L"colour");
					if (!color.isNull())
					{

						if (getfloat(w, L"a", color->val->arr) &&
							getfloat(x, L"r", color->val->arr) &&
							getfloat(y, L"g", color->val->arr) &&
							getfloat(z, L"b", color->val->arr))
							vert.colour = Colour(x, y, z, w);
					}


					if (i < oldcount)
						geom->getVertex(i) = vert;
					else
						geom->addVertex(vert);
				}

				return true;
			};
			int oldcount = ro->getGeometryCount();
			Array* arr = paras[0]->val->arr;

			if (arr->isArray() && arr->get((size_t)0)->val->type == OT_ARRAY && 
				arr->get(1)->val->arr->isArray())
			{
				for (int i = 0;; ++i)
				{
					Geometry* geom;
					if (i < oldcount)
						geom = ro->getGeometry(i);
					else
						geom = ro->createGeometry();

					convertGeom(geom, arr->get(i));
				}
			}
			else
			{
				Geometry* geom;
				if (oldcount == 0)
					geom = ro->createGeometry();
				else
					geom = ro->getGeometry(0);

				convertGeom(geom, paras[0]);
			}
		}
	};

	registerFunction<SetGeometry>(bind, L"SetGeometry", win, win);


}
