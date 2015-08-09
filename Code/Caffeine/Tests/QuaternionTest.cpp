

#include <UnitTest.hpp>
#include <Caffeine/Math/Quaternion.hpp>
#include <iostream>



namespace
{
	void ToString(CaffMath::Quaternion quat) {
		std::cout << "Quat: " << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << std::endl;
	}

	void ToString(CaffMath::Vector3 vec) {
		std::cout << "Vec3: " << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
	}

	void ToString(CaffMath::Matrix33 mat)
	{
		std::cout << "Mat: " << std::endl;
		std::cout << mat._11 << ", " << mat._12 << ", " << mat._13 << std::endl;
		std::cout << mat._21 << ", " << mat._22 << ", " << mat._23 << std::endl;
		std::cout << mat._31 << ", " << mat._32 << ", " << mat._33 << std::endl;		
	}

	const CaffMath::Vector3 xAxis = {1.f, 0.f, 0.f};
	const CaffMath::Vector3 yAxis = {0.f, 1.f, 0.f};
	const CaffMath::Vector3 zAxis = {0.f, 0.f, 1.f};	
}



TEST(InitQuat)
{
	const CaffMath::Quaternion quat = CaffMath::QuaternionInit(1.0f,2.0f,3.0f,4.0f);

	ASSERT_IS_NEAR(CaffMath::QuaternionGetW(quat), 1.f, 0.0001f)
	ASSERT_IS_NEAR(CaffMath::QuaternionGetX(quat), 2.f, 0.0001f)
	ASSERT_IS_NEAR(CaffMath::QuaternionGetY(quat), 3.f, 0.0001f)
	ASSERT_IS_NEAR(CaffMath::QuaternionGetZ(quat), 4.f, 0.0001f)
}



TEST(InitQuatWithAxisAngle)
{
	// 180 around x
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::HalfTau(), xAxis);

		ASSERT_IS_NEAR(CaffMath::QuaternionGetW(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetX(quat), 1.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetY(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetZ(quat), 0.f, 0.0001f)
	}

	// 90 around y
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), yAxis);

		ASSERT_IS_NEAR(CaffMath::QuaternionGetW(quat), 0.70711f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetX(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetY(quat), 0.70711f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetZ(quat), 0.f, 0.0001f)
	}

	// 360 around z
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::Tau(), zAxis);

		ASSERT_IS_NEAR(CaffMath::QuaternionGetW(quat), -1.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetX(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetY(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetZ(quat), 0.f, 0.0001f)
	}

	// 45 around y
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::Tau() / 8, yAxis);

		ASSERT_IS_NEAR(CaffMath::QuaternionGetW(quat), 0.92388f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetX(quat), 0.f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetY(quat), 0.38268f, 0.0001f)
		ASSERT_IS_NEAR(CaffMath::QuaternionGetZ(quat), 0.f, 0.0001f)
	}
}



TEST(InitWithRotationMatrix)
{

}



TEST(InitWithEuler)
{
	
}



TEST(RotateAround)
{
	// Rot a point around z-axis
	{
		const CaffMath::Vector3 point 		 = {1.f, 0.f, 0.f};
		const CaffMath::Quaternion quat 	 = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), zAxis);
		const CaffMath::Vector3 rotatedPoint = CaffMath::QuaternionRotate(quat, point);
	
		ASSERT_IS_NEAR(rotatedPoint.x, 0.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.y, 1.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.z, 0.f, 0.0001f)
	}

	// Rot a point around y-axis
	{
		const CaffMath::Vector3 point 		 = {1.f, 0.f, 0.f};
		const CaffMath::Quaternion quat 	 = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), yAxis);
		const CaffMath::Vector3 rotatedPoint = CaffMath::QuaternionRotate(quat, point);
	
		ASSERT_IS_NEAR(rotatedPoint.x, 0.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.y, 0.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.z, -1.f, 0.0001f)
	}

	// Rot a point around x-axis
	{
		const CaffMath::Vector3 point 		 = {0.f, 1.f, 0.f};
		const CaffMath::Quaternion quat 	 = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), xAxis);
		const CaffMath::Vector3 rotatedPoint = CaffMath::QuaternionRotate(quat, point);
	
		ASSERT_IS_NEAR(rotatedPoint.x, 0.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.y, 0.f, 0.0001f)
		ASSERT_IS_NEAR(rotatedPoint.z, 1.f, 0.0001f)
	}
}



TEST(Multiply)
{
	{
		const CaffMath::Quaternion quat1  = CaffMath::QuaternionInit(0, 1, 0, 0);
		const CaffMath::Quaternion quat2  = CaffMath::QuaternionInit(0, 0, 1, 0);
		const CaffMath::Quaternion result = CaffMath::QuaternionMultiply(quat1, quat2);

		ASSERT_IS_NEAR(result.w, 0, 0.0001f)
		ASSERT_IS_NEAR(result.x, 0, 0.0001f)
		ASSERT_IS_NEAR(result.y, 0, 0.0001f)
		ASSERT_IS_NEAR(result.z, 1, 0.0001f)
	}
}



TEST(Conjugate)
{

}



TEST(RotationMatrix)
{
	// 90 around y
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), yAxis);
		const CaffMath::Matrix33    mat = CaffMath::QuaternionToRotationMatrix(quat);

		ASSERT_IS_NEAR(mat._11, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._12, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._13, 1, 0.00001f)

		ASSERT_IS_NEAR(mat._21, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._22, 1, 0.00001f)
		ASSERT_IS_NEAR(mat._23, 0, 0.00001f)

		ASSERT_IS_NEAR(mat._31, -1, 0.00001f)
		ASSERT_IS_NEAR(mat._32, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._33, 0, 0.00001f)
	}

	// 45 around y
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::Tau() / 8, yAxis);
		const CaffMath::Matrix33    mat = CaffMath::QuaternionToRotationMatrix(quat);

		ASSERT_IS_NEAR(mat._11, 0.707108, 0.00001f)
		ASSERT_IS_NEAR(mat._12, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._13, 0.707106, 0.00001f)

		ASSERT_IS_NEAR(mat._21, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._22, 1, 0.00001f)
		ASSERT_IS_NEAR(mat._23, 0, 0.00001f)

		ASSERT_IS_NEAR(mat._31, -0.707106, 0.00001f)
		ASSERT_IS_NEAR(mat._32, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._33, 0.707108, 0.00001f)
	}

	// 180 around x
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::HalfTau(), xAxis);
		const CaffMath::Matrix33    mat = CaffMath::QuaternionToRotationMatrix(quat);

		ASSERT_IS_NEAR(mat._11, 1, 0.00001f)
		ASSERT_IS_NEAR(mat._12, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._13, 0, 0.00001f)

		ASSERT_IS_NEAR(mat._21, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._22, -1, 0.00001f)
		ASSERT_IS_NEAR(mat._23, 0, 0.00001f)

		ASSERT_IS_NEAR(mat._31, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._32, 0, 0.00001f)
		ASSERT_IS_NEAR(mat._33, -1, 0.00001f)
	}
}



TEST(MultiplyAndRotatePoint)
{

}



TEST(ToEuler)
{
	// 180 around 1 axis
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::HalfTau(), xAxis);
		const CaffMath::Vector3 euler   = CaffMath::QuaternionGetEulerAnglesInDegrees(quat);

		ToString(euler);
	}

	// 90 around 1 axis
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::QuartTau(), xAxis);
		const CaffMath::Vector3 euler   = CaffMath::QuaternionGetEulerAnglesInDegrees(quat);

		ToString(euler);
	}

	// with multiplication.
	{
		const CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(CaffMath::HalfTau(), xAxis);
		const CaffMath::Vector3 euler   = CaffMath::QuaternionGetEulerAnglesInDegrees(quat);

		ToString(euler);
	}

}



int main()
{
	Test::RunTests();

	return 0;
}