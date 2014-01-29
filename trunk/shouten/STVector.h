#ifndef _STVector_H_
#define _STVector_H_

namespace ST
{
	class Vector2
	{
	public :
		float x, y;

	public:
		Vector2()
		{}

		Vector2(const float& a, const float& b) :
			x(a), y(b)
		{
		}
	};

	class Vector3
	{
	public :
		float x, y, z;

		Vector3()
		{
		}

		Vector3(float fX, float fY, float fZ)
			: x(fX), y(fY), z(fZ)
		{
		}
	};


	class Vector4
	{
	public:
		float x, y, z,w;


		Vector4()
		{}

		Vector4(float fX, float fY, float fZ, float fW)
			: x(fX), y(fY), z(fZ), w(fW)
		{
		}
	};


}

#endif