#ifndef _STWindow_H_
#define _STWindow_H_

#include "STCommon.h"
#include "STScriptObjectDefinition.h"
#include "STRect.h"

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

		//common function
		virtual void initialize(const CustomParameters* paras);
		virtual void uninitialize();
		virtual void close();
		virtual void draw();

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
		void dirty();
		bool isDirty() const;
		void refresh();


	private://property
		Window* mParent;
		bool mIsdirty;
		RectI mRect;



	};
}


#endif