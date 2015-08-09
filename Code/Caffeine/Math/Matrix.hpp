
#ifndef CAFFEINE_MATH_MATRIX_INCLUDED
#define CAFFEINE_MATH_MATRIX_INCLUDED


#include <Caffeine/Math/Vector.hpp>


namespace Caffeine {
namespace Math {


struct Matrix44
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};


struct Matrix33
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;
};
    
    



inline Matrix33 Matrix33InitScaleMatrix(const Vector3 &scale)
{
	const Matrix33 returnMat = {Vector3GetX(scale), 0, 0, 0, Vector3GetY(scale), 0, 0, 0, Vector3GetZ(scale)};
	
	return returnMat;
}


inline Matrix33 Matrix33Multiply(const Matrix33 &left, const Matrix33 &right)
{
	Matrix33 returnMat = {0,0,0,0,0,0,0,0,0};
	
	returnMat._11 = (left._11 * right._11) + (left._12 * right._21) + (left._13 * right._31);
	returnMat._12 = (left._11 * right._12) + (left._12 * right._22) + (left._13 * right._32);
	returnMat._13 = (left._11 * right._13) + (left._12 * right._23) + (left._13 * right._33);
	
	returnMat._21 = (left._21 * right._11) + (left._22 * right._21) + (left._23 * right._31);
	returnMat._22 = (left._21 * right._12) + (left._22 * right._22) + (left._23 * right._32);
	returnMat._23 = (left._21 * right._13) + (left._22 * right._23) + (left._23 * right._33);
	
	returnMat._31 = (left._31 * right._11) + (left._32 * right._21) + (left._33 * right._31);
	returnMat._32 = (left._31 * right._12) + (left._32 * right._22) + (left._33 * right._32);
	returnMat._33 = (left._31 * right._13) + (left._32 * right._23) + (left._33 * right._33);
	
	return returnMat;
}


} // namespace
} // namespace


#endif // include guard
