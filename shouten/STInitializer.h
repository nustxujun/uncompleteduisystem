#ifndef _STInitializer_H_
#define _STInitializer_H_

/*
	Ϊʲô����������
	������
	1.ģ����Ҫ��ʼ���͹������
	2.ģ���ʼ�������ͷ���ʼ���������뱻�����ε���
	3.��ֹ���Ƿ���ʼ��
	4.��
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