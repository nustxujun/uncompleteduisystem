#ifndef _STSize_H_
#define _STSize_H_

namespace ST
{
	template<class T>
	class Size
	{
	public:
		T width, height;

		Size(const T& w, const T& h) :
			width(w), height(h)
		{
		}

		Size()
		{
		}

		T square()const
		{
			return width * height;
		}
	};



	typedef Size<int> SizeI;
	typedef Size<float> SizeF;
}

#endif//_STSize_H_