#ifndef _STWindow_H_
#define _STWindow_H_

#include "STCommon.h"
#include "STScriptObjectDefinition.h"
#include "STRect.h"
#include "STVector.h"
#include "STInput.h"
#include "STRenderObject.h"

namespace ST
{
	class ShoutenExport Window
	{
		friend class WindowHelper;
	public:
		Window(const String& name, const WindowFactory* f, WindowManager* wm);
		~Window();

		const String& getName()const;
		const WindowFactory* getFactory()const;

		Window* createChild(const String& name);
		Window* getChild(const String& name);
		void destroyChild(const String& name);

		virtual void initializeScript();
		virtual void uninitializeScript();
		virtual void parseParameter(const CustomParameters& paras);

		void unregisterFunction(const String& name);

		void setProperty(const String& prop, const String& val);
		const String& getProperty(const String& key)const;

		Window* getParent();
		RenderObject* getRenderObject();

		const RectI& getRect()const;
		RectI getAbsRect()const;

		int getX() const;
		int getY() const;
		int getAbsX() const;
		int getAbsY() const;
		int getWidth() const;
		int getHeight() const;

		void setRect(const RectI& r);
		void setX(int x);
		void setY(int y);
		void setPosition(int x, int y);
		void setWidth(int w);
		void setHeight(int h);
		void setSize(int w, int h);

		void dirty();
		bool isDirty() const;
		bool isMousePassThroughEnabled()const;

		class WindowHitTest
		{
		public :
			bool operator()(int x, int y, Window* win)
			{
				return win->getRenderObject()->getWorldAABB().inside(x, y);
			}
		};

		template<class Type = WindowHitTest>
		Window* getHitWindow(int x, int y, Type testFunc = WindowHitTest())
		{
			auto i = mChildren.begin();
			auto endi = mChildren.end();
			for (; i != endi; ++i)
			{
				Window* w = i->second->getHitWindow(x, y, testFunc);
				if (w == nullptr) continue;
				return w;
			}
			
			if (isHit(x, y, testFunc)) return this;
			return nullptr;
		}

		template<class Type = WindowHitTest>
		bool isHit(int x, int y, Type testFunc = WindowHitTest())
		{
			return !isMousePassThroughEnabled() && testFunc(x, y, this);
		}

		//common function
		virtual void initialize(const CustomParameters* paras);
		virtual void uninitialize();
		virtual void close();
		virtual void draw();


		//event
		virtual void injectMouseMove(int posx, int posy, int deltax, int deltay);
		virtual void injectMouseButtonDown(Mouse::MouseButton btn);
		virtual void injectMouseButtonUp(Mouse::MouseButton btn);
		virtual void injectMouseWheel(float delta);
		virtual void injectKeyDown(Key::KeyType key);
		virtual void injectKeyUp(Key::KeyType key);

	private:
		String mName;
		const WindowFactory* mFactory;
		WindowManager* mManager;
		RenderObject* mRenderObject;

		using Childs = std::hash_map<String, Window*>;
		Childs mChilds;

		using FuncNames = std::vector<String>;
		FuncNames mFuncNames;

		using Propertys = std::hash_map<String, String>;
		Propertys mPropertys;

	protected:
		void refresh();

	private://property
		Window* mParent;
		using Children = std::hash_map<String, Window*>;
		Children mChildren;

		bool mIsdirty;
		bool mMousePassThrough;
		RectI mRect;



	};
}


#endif