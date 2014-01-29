#ifndef _STRect_H_
#define _STRect_H_

#include <algorithm>
#include <assert.h>
#include "STMatrix.h"

namespace ST
{
	enum RectType
	{
		RT_NULL,
		RT_FINITE,
		RT_INFINITE,

	};

	template<class T>
	class Rect
	{
	public:

	public :
		T left, right, top, bottom;

		T width() const { return right - left; }
		T height()const{ return bottom - top; }

		Rect(const Rect& rc)
		{
			*this = rc;
		}

		template<class U>
		Rect(const Rect<U>& rc)
		{
			operator=<U>(rc);
		}

		Rect() :mType(RT_NULL)
		{}

		Rect(const T& l, const T& t, const T& r, const T& b) :
			left(l), right(r), top(t), bottom(b), mType(RT_FINITE)
		{}

		Rect& operator=(const Rect& rc)
		{
			left = rc.left;
			right = rc.right;
			top = rc.top;
			bottom = rc.bottom;
			mType = rc.mType;
			return *this;
		}

		template<class U>
		Rect& operator=(const Rect<U>& rc)
		{
			left = (const T)rc.left;
			right = (const T)rc.right;
			top = (const T)rc.top;
			bottom = (const T)rc.bottom;
			mType = rc.getType();
			return *this;
		}

		void setWidth(const T& w) { right = left + w; }
		void setHeight(const T& h) { bottom = top + h; }

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
			top = y;
			return *this;
		}

		Rect& merge(const T& x, const T& y)
		{
			switch (mType)
			{
			case RT_NULL:
				left = right = x;
				top = bottom = y;
				mType = RT_FINITE;
				return *this;
			case RT_FINITE:
				{
					using namespace std;
					left = min(x, left);
					right = max(x, right);
					top = min(y, top);
					bottom = max(y, bottom);
				}
				return *this;
			case RT_INFINITE:
				break;
			}

			assert(false && "fail");
			return *this;
		}

		Rect& merge(const Rect& rect)
		{
			switch (mType)
			{
			case RT_NULL:
				*this = rect;
				return *this;
			case RT_FINITE:
				{
					using namespace std;
					left = min(rect.left, left);
					right = max(rect.right, right);
					top = min(rect.top, top);
					bottom = max(rect.bottom, bottom);
				}
				return *this;
			case RT_INFINITE:
				break;
			}

			assert(false && "fail");
			return *this;

		}

		bool inside(const T& x, const T& y)const
		{
			switch (mType)
			{
			case RT_NULL:
				return false;
			case RT_FINITE:
				return x >= left && x <= right && y >= top && y <= bottom;
			case RT_INFINITE:
				return true;
			}

			assert(false && "test fail");
			return false;
		}

		void transform(const Matrix4& mat)
		{
			if (mType != RT_FINITE) return;

			Vector3 oldmin((float)left, (float)top, 0);
			Vector3 oldmax((float)right, (float)bottom, 0);
			Vector3 corner, cur;

			setNull();

			//left top
			corner = oldmin;
			cur = mat * corner;
			merge((T)cur.x, (T)cur.y);

			//right top
			corner.x = oldmax.x;
			cur = mat * corner;
			merge((T)cur.x, (T)cur.y);

			//right bottom
			corner.y = oldmax.y;
			cur = mat * corner;
			merge((T)cur.x, (T)cur.y);

			//left bottom
			corner.x = oldmin.x;
			cur = mat * corner;
			merge((T)cur.x, (T)cur.y);


		}

		void setNull()
		{
			mType = RT_NULL;
		}

		RectType getType()const
		{
			return mType;
		}
	private:
		RectType mType;

	};

	typedef Rect<int> RectI;
	typedef Rect<float> RectF;
}

#endif