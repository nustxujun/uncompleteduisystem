#include "STMatrix.h"

using namespace ST;



void Matrix4::makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
{
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	Matrix3 rot3x3;
	orientation.ToRotationMatrix(rot3x3);

	// Set up final matrix with scale, rotation and translation
	m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
	m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
	m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

	// No projection term
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}