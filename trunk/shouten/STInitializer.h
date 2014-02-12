#ifndef _STInitializer_H_
#define _STInitializer_H_

/*
	为什么会出现这个类
	条件：
	1.模块需要初始化和构造分离
	2.模块初始化函数和反初始化函数不想被随意多次调用
	3.阻止忘记反初始化
	4.懒
*/

namespace ST
{
	template<class ... Args>
	class Initializer
	{
	public:
		Initializer() :mIsInitialized(false){}
		virtual ~Initializer(){	uninitialize();}

		void initialize(Args ... args)
		{
			if (mIsInitialized) return;
			mIsInitialized = true;
			initializeImpl(args...);
		}

		void uninitialize()
		{
			if (!mIsInitialized) return;
			uninitializeImpl();
		}

		virtual void initializeImpl(Args ... args) = 0;
		virtual void uninitializeImpl() = 0;

	private:
		bool mIsInitialized;
	};

	template<>
	class Initializer<void>
	{
	public:
		Initializer() :mIsInitialized(false){}
		virtual ~Initializer(){ uninitialize(); }

		void initialize()
		{
			if (mIsInitialized) return;
			mIsInitialized = true;
			initializeImpl();
		}

		void uninitialize()
		{
			if (!mIsInitialized) return;
			uninitializeImpl();
		}

		virtual void initializeImpl() = 0;
		virtual void uninitializeImpl() = 0;

	private:
		bool mIsInitialized;
	};

	typedef Initializer<void> NormalInitializer;
}

#endif