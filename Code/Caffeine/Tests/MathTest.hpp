
#include <UnitTest.hpp>
#include <Caffeine/Math/Math.hpp>

namespace MathTests {

namespace CaffMath = ::Caffeine::Math;


TEST(Trig)
{
	float precision = 0.01f;

	// Sin
	float sin1 = CaffMath::Sin(1);
	float sin2 = CaffMath::Sin(0.1f);
	float sin3 = CaffMath::Sin(103.3f);

	ASSERT_IS_NEAR(sin1, 0.8415f, precision)
	ASSERT_IS_NEAR(sin2, 0.0998f, precision)
	ASSERT_IS_NEAR(sin3, 0.3639f, precision)

	// ASin
	float asin1 = CaffMath::ASin(0);
	float asin2 = CaffMath::ASin(0.34f);
	float asin3 = CaffMath::ASin(-0.9f);

	ASSERT_IS_NEAR(asin1, 0.0f, 	precision)
	ASSERT_IS_NEAR(asin2, 0.3469f,  precision)
	ASSERT_IS_NEAR(asin3, -1.1198f, precision)

	// Cos
	float cos1 = CaffMath::Cos(4);
	float cos2 = CaffMath::Cos(-3);
	float cos3 = CaffMath::Cos(546.3);

	ASSERT_IS_NEAR(cos1, -0.6536f, precision)
	ASSERT_IS_NEAR(cos2, -0.9899f, precision)
	ASSERT_IS_NEAR(cos3, 0.94371f, precision)

	// ACos
	float acos1 = CaffMath::ACos(-1);
	float acos2 = CaffMath::ACos(0.5f);
	float acos3 = CaffMath::ACos(-0.4f);

	ASSERT_IS_NEAR(acos1, CaffMath::Pi(), precision)
	ASSERT_IS_NEAR(acos2, 1.0472f,  precision)
	ASSERT_IS_NEAR(acos3, 1.9823f,  precision)

	// Tan

	// ATan

	// ATan2
	float atan21 = CaffMath::ATan2(2, 3);
	float atan22 = CaffMath::ATan2(5, 0);
	float atan23 = CaffMath::ATan2(-0.45f, 5.67f);

	ASSERT_IS_NEAR(atan21, 0.5880f, 			precision)
	ASSERT_IS_NEAR(atan22,  CaffMath::Pi() / 2,  	precision)
	ASSERT_IS_NEAR(atan23, -0.0791f,  			precision)
}

TEST(DegreeRadians)
{
	ASSERT_IS_NEAR(CaffMath::DegToRad(60.3f), 1.052f, 0.1f)
	ASSERT_IS_NEAR(CaffMath::RadToDeg(1.052f), 60.3f, 0.1f)

	ASSERT_IS_NEAR(CaffMath::DegToRad(560.6f), 9.7843f, 0.1f)
	ASSERT_IS_NEAR(CaffMath::RadToDeg(9.7843f), 560.6f, 0.1f)
}

TEST(MinMax)
{
	ASSERT_IS_EQUAL(CaffMath::Min(3.2f, 16.2f), 3.2f)
	ASSERT_IS_EQUAL(CaffMath::Max(53.4f, 1.3f), 53.4f)

	ASSERT_IS_EQUAL(CaffMath::Min(16.1f, 16.2f), 16.1f)
	ASSERT_IS_EQUAL(CaffMath::Max(1.02f, 1.01f), 1.02f)	
}

int RunTests()
{
	Test::RunTests();

	return 0;
}
    
} // namespace
