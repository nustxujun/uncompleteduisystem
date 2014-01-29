#ifndef _STQuaternion_H_
#define _STQuaternion_H_

#include "STCommon.h"
#include "STVector.h"
#include <assert.h>

namespace ST
{
	class Quaternion
	{
	public:
		Quaternion()
			: w(1), x(0), y(0), z(0)
		{
		}
		Quaternion(
			float fW,
			float fX, float fY, float fZ)
			: w(fW), x(fX), y(fY), z(fZ)
		{
		}

		/// Construct a quaternion from 3 orthonormal local axes
		Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
		{
			this->FromAxes(xaxis, yaxis, zaxis);
		}
		/// Construct a quaternion from 3 orthonormal local axes
		Quaternion(const Vector3* akAxis)
		{
			this->FromAxes(akAxis);
		}
		/// Construct a quaternion from 4 manual w/x/y/z values
		Quaternion(float* valptr)
		{
			memcpy(&w, valptr, sizeof(float)* 4);
		}

		/** Exchange the contents of this quaternion with another.
		*/
		void swap(Quaternion& other)
		{
			std::swap(w, other.w);
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		/// Array accessor operator
		float operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// Array accessor operator
		float& operator [] (const size_t i)
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// Pointer accessor for direct copying
		float* ptr()
		{
			return &w;
		}

		/// Pointer accessor for direct copying
		const float* ptr() const
		{
			return &w;
		}

		void ToRotationMatrix(Matrix3& kRot) const;
		/** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
		@see FromAxes
		*/
		void FromAxes(const Vector3* akAxis);
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		/** Gets the 3 orthonormal axes defining the quaternion. @see FromAxes */
		void ToAxes(Vector3* akAxis) const;
		void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

		/** Returns the X orthonormal axis defining the quaternion. Same as doing
		xAxis = Vector3::UNIT_X * this. Also called the local X-axis
		*/
		Vector3 xAxis(void) const;

		/** Returns the Y orthonormal axis defining the quaternion. Same as doing
		yAxis = Vector3::UNIT_Y * this. Also called the local Y-axis
		*/
		Vector3 yAxis(void) const;

		/** Returns the Z orthonormal axis defining the quaternion. Same as doing
		zAxis = Vector3::UNIT_Z * this. Also called the local Z-axis
		*/
		Vector3 zAxis(void) const;

		Quaternion& operator= (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
		Quaternion operator+ (const Quaternion& rkQ) const;
		Quaternion operator- (const Quaternion& rkQ) const;
		Quaternion operator* (const Quaternion& rkQ) const;
		Quaternion operator* (float fScalar) const;

		Quaternion operator- () const;
		bool operator== (const Quaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) &&
				(rhs.z == z) && (rhs.w == w);
		}
		bool operator!= (const Quaternion& rhs) const
		{
			return !operator==(rhs);
		}
		// functions of a quaternion
		/// Returns the dot product of the quaternion
		float Dot(const Quaternion& rkQ) const;
		/* Returns the normal length of this quaternion.
		@note This does <b>not</b> alter any values.
		*/
		float Norm() const;
		/// Normalises this quaternion, and returns the previous length
		float normalise(void);
		Quaternion Inverse() const;  // apply to non-zero quaternion
		Quaternion UnitInverse() const;  // apply to unit-length quaternion
		Quaternion Exp() const;
		Quaternion Log() const;

		/// Rotation of a vector by a quaternion
		Vector3 operator* (const Vector3& rkVector) const;


		// special values
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

		float w, x, y, z;



	};
}

#endif