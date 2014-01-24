#ifndef _STRect_H_
#define _STRect_H_

#include <algorithm>

namespace ST
{
	template<class T>
	class Rect
	{
	public :
		T left, right, top, bottom;

		T width() const { return right - left; }
		T height()const{ return bottom - top; }

		void setWidth(const T& w) { right = left + w; }
		void setHeight(const T& h) { bottom = top + h; }

		Rect():
			left(0), right(0), top(0), bottom(0)
		{}

		Rect& move(const T& x, const T& y)
		{
			left += x;
			right += x;
			top += y;
			bottom += y;
			return *this;
		}

		Rect& moveTo(const T& x, const T& y)
		{
			right += x - left;
			bottom += y - top;
			left = x;
			right = y;
			return *this;
		}

		Rect& merge(const T& x, const T& y)
		{
			using namespace std;
			left = min(x, left);
			right = max(x, right);
			top = min(y, top);
			bottom = max(y, bottom);
			return *this;
		}

		Rect& merge(const Rect<T>& rect)
		{
			using namespace std;
			left = min(rect.left, left);
			right = max(rect.right, right);
			top = min(rect.top, top);
			bottom = max(rect.bottom, bottom);
			return *this;
		}

	};

	typedef Rect<int> RectI;
	typedef Rect<float> RectF;
}

#endif