#ifndef _STSingleton_H_
#define _STSingleton_H_

#include <assert.h>

namespace ST
{
	template<class T>
	class Singleton
	{
	public :
		Singleton()
		{
			assert(instance() == nullptr);
			instance() = (T*)this;
		}

		~Singleton()
		{
			assert(instance());
			instance() = nullptr;
		}

		static T& getSingleton()
		{
			return *instance();
		}

	private:
		static T*& instance()
		{
			static T* inst = nullptr;
			return inst;
		}
	};
}


#endif