

#include <UnitTest.hpp>
#include <Caffeine/Math/Vector.hpp>


TEST(InitVectors)
{
	// Vec2
	{
		CaffMath::Vector2 vec2 = CaffMath::Vector2Init(1.2f, 3.4f);
		ASSERT_IS_EQUAL(CaffMath::Vector2GetX(vec2), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector2GetY(vec2), 3.4f)

		ASSERT_IS_EQUAL(CaffMath::Vector2GetS(vec2), 1.2f);
		ASSERT_IS_EQUAL(CaffMath::Vector2GetT(vec2), 3.4f);

		ASSERT_IS_EQUAL(CaffMath::Vector2GetU(vec2), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector2GetV(vec2), 3.4f)

		vec2 = CaffMath::Vector2Init(4.4f);
		ASSERT_IS_EQUAL(CaffMath::Vector2GetX(vec2), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector2GetY(vec2), 4.4f)
	}

	// Vec3
	{
		CaffMath::Vector3 vec3 = CaffMath::Vector3Init(1.2f, 3.4f, 5.6f);
		ASSERT_IS_EQUAL(CaffMath::Vector3GetX(vec3), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetY(vec3), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetZ(vec3), 5.6f)

		ASSERT_IS_EQUAL(CaffMath::Vector3GetR(vec3), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetG(vec3), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetB(vec3), 5.6f)

		ASSERT_IS_EQUAL(CaffMath::Vector3GetS(vec3), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetT(vec3), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetP(vec3), 5.6f)

		ASSERT_IS_EQUAL(CaffMath::Vector3GetU(vec3), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetV(vec3), 3.4f)

		vec3 = CaffMath::Vector3Init(4.4f);
		ASSERT_IS_EQUAL(CaffMath::Vector3GetX(vec3), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetY(vec3), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector3GetZ(vec3), 4.4f)
	}

	// Vec4
	{
		CaffMath::Vector4 vec4 = CaffMath::Vector4Init(1.2f, 3.4f, 5.6f, 7.8f);
		ASSERT_IS_EQUAL(CaffMath::Vector4GetX(vec4), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetY(vec4), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetZ(vec4), 5.6f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetW(vec4), 7.8f)

		ASSERT_IS_EQUAL(CaffMath::Vector4GetR(vec4), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetG(vec4), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetB(vec4), 5.6f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetA(vec4), 7.8f)

		ASSERT_IS_EQUAL(CaffMath::Vector4GetS(vec4), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetT(vec4), 3.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetP(vec4), 5.6f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetQ(vec4), 7.8f)

		ASSERT_IS_EQUAL(CaffMath::Vector4GetU(vec4), 1.2f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetV(vec4), 3.4f)

		vec4 = CaffMath::Vector4Init(4.4f);
		ASSERT_IS_EQUAL(CaffMath::Vector4GetX(vec4), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetY(vec4), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetZ(vec4), 4.4f)
		ASSERT_IS_EQUAL(CaffMath::Vector4GetW(vec4), 4.4f)
	}	
}



TEST(TestComponentWiseMath)
{
	// Vec2
	{
		CaffMath::Vector2 vec2a = CaffMath::Vector2Init(1.2f, 3.4f);
		CaffMath::Vector2 vec2b = CaffMath::Vector2Init(5.6f, 7.8f);
		CaffMath::Vector2 vec2r = CaffMath::Vector2Init(0.f);

		vec2r = Vector2ComponentAdd(vec2a, vec2b);
		ASSERT_IS_NEAR(CaffMath::Vector2GetX(vec2r), 6.8f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector2GetY(vec2r), 11.2f, 0.001f)

		vec2r = Vector2ComponentSubtract(vec2a, vec2b);
		ASSERT_IS_NEAR(CaffMath::Vector2GetX(vec2r), -4.4f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector2GetY(vec2r), -4.4f, 0.001f)

		vec2r = Vector2ComponentMultiply(vec2a, vec2b);
		ASSERT_IS_NEAR(CaffMath::Vector2GetX(vec2r), 6.72f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector2GetY(vec2r), 26.52f, 0.001f)

		vec2r = Vector2ComponentDivide(vec2a, vec2b);
		ASSERT_IS_NEAR(CaffMath::Vector2GetX(vec2r), 0.2142, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector2GetY(vec2r), 0.4358, 0.001f)
	}

	// Vec3
	{
		CaffMath::Vector3 vec3a = CaffMath::Vector3Init(1.2f, 3.4f, 5.6f);
		CaffMath::Vector3 vec3b = CaffMath::Vector3Init(5.6f, 7.8f, 9.9f);
		CaffMath::Vector3 vec3r = CaffMath::Vector3Init(0.f);

		vec3r = Vector3ComponentAdd(vec3a, vec3b);
		ASSERT_IS_NEAR(CaffMath::Vector3GetX(vec3r), 6.8f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetY(vec3r), 11.2f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetZ(vec3r), 15.5f, 0.001f)

		vec3r = Vector3ComponentSubtract(vec3a, vec3b);
		ASSERT_IS_NEAR(CaffMath::Vector3GetX(vec3r), -4.4f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetY(vec3r), -4.4f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetZ(vec3r), -4.3f, 0.001f)


		vec3r = Vector3ComponentMultiply(vec3a, vec3b);
		ASSERT_IS_NEAR(CaffMath::Vector3GetX(vec3r), 6.72f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetY(vec3r), 26.52f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetZ(vec3r), 55.44f, 0.001f)

		vec3r = Vector3ComponentDivide(vec3a, vec3b);
		ASSERT_IS_NEAR(CaffMath::Vector3GetX(vec3r), 0.2142, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetY(vec3r), 0.4358, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetZ(vec3r), 0.5656, 0.001f)
	}

	// Vec4
	{
		CaffMath::Vector4 vec4a = CaffMath::Vector4Init(1.2f, 3.4f, 5.6f, 7.8f);
		CaffMath::Vector4 vec4b = CaffMath::Vector4Init(5.6f, 7.8f, 9.9f, 1.2f);
		CaffMath::Vector4 vec4r = CaffMath::Vector4Init(0.f);

		vec4r = Vector4ComponentAdd(vec4a, vec4b);
		ASSERT_IS_NEAR(CaffMath::Vector4GetX(vec4r), 6.8f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetY(vec4r), 11.2f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetZ(vec4r), 15.5f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetW(vec4r), 9.0f, 0.001f)

		vec4r = Vector4ComponentSubtract(vec4a, vec4b);
		ASSERT_IS_NEAR(CaffMath::Vector4GetX(vec4r), -4.4f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetY(vec4r), -4.4f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetZ(vec4r), -4.3f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetW(vec4r), 6.6f, 0.001f)

		vec4r = Vector4ComponentMultiply(vec4a, vec4b);
		ASSERT_IS_NEAR(CaffMath::Vector4GetX(vec4r), 6.72f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetY(vec4r), 26.52f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetZ(vec4r), 55.44f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetW(vec4r), 9.36f, 0.001f)

		vec4r = Vector4ComponentDivide(vec4a, vec4b);
		ASSERT_IS_NEAR(CaffMath::Vector4GetX(vec4r), 0.2142, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetY(vec4r), 0.4358, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetZ(vec4r), 0.5656, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector4GetW(vec4r), 6.5, 0.001f)
	}	
}



TEST(Scale)
{
	// Vec2
	{
		CaffMath::Vector2 vec2 = CaffMath::Vector2Init(1.f, 1.f);

		float mag = Vector2Magnitude(vec2);
		ASSERT_IS_NEAR(mag, CaffMath::Root2(), 0.001f)

		CaffMath::Vector2 vec2r = Vector2Scale(vec2, 2.0f);
		ASSERT_IS_NEAR(Vector2Magnitude(vec2r), CaffMath::Root2() * 2.0f, 0.001f)

		CaffMath::Vector2 norm = Vector2Normalize(vec2r);
		ASSERT_IS_NEAR(Vector2Magnitude(norm), 1.0f, 0.001f)
	}

	// Vec3
	{
		CaffMath::Vector3 vec3 = CaffMath::Vector3Init(1.f, 1.f, 1.f);

		float mag = Vector3Magnitude(vec3);
		ASSERT_IS_NEAR(mag, 1.732f, 0.001f)

		CaffMath::Vector3 vec3r = Vector3Scale(vec3, 2.0f);
		ASSERT_IS_NEAR(Vector3Magnitude(vec3r), 1.732f * 2.0f, 0.001f)

		CaffMath::Vector3 norm = Vector3Normalize(vec3r);
		ASSERT_IS_NEAR(Vector3Magnitude(norm), 1.0f, 0.001f)
	}

	// Vec4
	{
		CaffMath::Vector4 vec4 = CaffMath::Vector4Init(1.f, 1.f, 1.f, 1.f);

		float mag = Vector4Magnitude(vec4);
		ASSERT_IS_NEAR(mag, 2.0f, 0.001f)

		CaffMath::Vector4 vec4r = Vector4Scale(vec4, 2.0f);
		ASSERT_IS_NEAR(Vector4Magnitude(vec4r), 2.0f * 2.0f, 0.001f)

		CaffMath::Vector4 norm = Vector4Normalize(vec4r);
		ASSERT_IS_NEAR(Vector4Magnitude(norm), 1.0f, 0.001f)
	}	
}



TEST(CrossDot)
{
	// Vec2
	{
		CaffMath::Vector2 vec2a = CaffMath::Vector2Init(2.2f, 3.3f);
		CaffMath::Vector2 vec2b = CaffMath::Vector2Init(4.4f, 5.5f);

		float dot = CaffMath::Vector2DotProduct(vec2a, vec2b);
		ASSERT_IS_NEAR(dot, 27.83f, 0.001f)

		float cross = CaffMath::Vector2CrossProduct(vec2a, vec2b);
		ASSERT_IS_NEAR(cross, -2.42f, 0.001f)
	}

	// Vec3
	{
		CaffMath::Vector3 vec3a = CaffMath::Vector3Init(2.2f, 3.3f, 4.4f);
		CaffMath::Vector3 vec3b = CaffMath::Vector3Init(4.4f, 5.5f, 6.6f);

		float dot = CaffMath::Vector3DotProduct(vec3a, vec3b);
		ASSERT_IS_NEAR(dot, 56.87f, 0.001f)

		CaffMath::Vector3 cross = CaffMath::Vector3CrossProduct(vec3a, vec3b);
		ASSERT_IS_NEAR(CaffMath::Vector3GetX(cross), -2.42f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetY(cross),  4.84f, 0.001f)
		ASSERT_IS_NEAR(CaffMath::Vector3GetZ(cross), -2.42f, 0.001f)
	}	

	// Vec4
	{
		CaffMath::Vector4 vec4a = CaffMath::Vector4Init(2.2f, 3.3f, 4.4f, 5.5f);
		CaffMath::Vector4 vec4b = CaffMath::Vector4Init(4.4f, 5.5f, 6.6f, 7.7f);

		float dot = CaffMath::Vector4DotProduct(vec4a, vec4b);
		ASSERT_IS_NEAR(dot, 99.22f, 0.001f)
	}	
}


int main()
{
	Test::RunTests();

	return 0;
}
