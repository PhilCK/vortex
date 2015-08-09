
#include <UnitTest.hpp>
#include <Caffeine/Math/Transform.hpp>



TEST(GetPositionOffset)
{
	CaffMath::Transform transform1, transform2;
	transform1.position = {10.f, 3.f, 6.f};
	transform2.position = {11.f, 2.f, 5.f};

	const CaffMath::Vector3 offset = CaffMath::TransformGetRelativePosition(transform1, transform2);

	ASSERT_IS_EQUAL(offset.x, +1.f)
	ASSERT_IS_EQUAL(offset.y, -1.f)
	ASSERT_IS_EQUAL(offset.z, -1.f)
}



TEST(SetScale)
{
	CaffMath::Transform transform1;
	transform1.position = {3.f, 0.f, 0.f};

	const CaffMath::Vector3 updatedScale = {0.5f, 0.5f, 0.5f};
	const CaffMath::Transform transform2 = CaffMath::TransformScaleAndPosition(transform1, updatedScale);

	ASSERT_IS_EQUAL(transform2.position.x, 1.5f)
	ASSERT_IS_EQUAL(transform2.position.y, 0.f)
	ASSERT_IS_EQUAL(transform2.position.z, 0.f)

	ASSERT_IS_EQUAL(transform2.scale.x, 0.5f)
	ASSERT_IS_EQUAL(transform2.scale.y, 0.5f)
	ASSERT_IS_EQUAL(transform2.scale.z, 0.5f)
}



TEST()



int main()
{
	Test::RunTests();
}
