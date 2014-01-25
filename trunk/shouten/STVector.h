#ifndef _STVector_H_
#define _STVector_H_

namespace ST
{
	template<class Type>
	class ShoutenExport Vector2
	{
	public :
		Type x, y;
	};

	template<class Type>
	class ShoutenExport Vector3
	{
	public :
		Type x, y, z;
	};


	typedef Vector2<float>	Vector2F;
	typedef Vector2<int>	Vector2I;

	typedef Vector3<float>	Vector3F;
	typedef Vector3<int>	Vector3I;
}

#endif