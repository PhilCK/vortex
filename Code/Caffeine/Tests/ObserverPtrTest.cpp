
#include <UnitTest.hpp>
#include <Caffeine/Utilities/ObserverPtr.hpp>
#include <string>

class Entity
{
public:

	explicit Entity()
	: factory(this)
	{
		name = 1234;
	}

	CaffUtil::ObserverPtrFactory<Entity> factory;

	int name = 0;
};


TEST(BasicUse)
{
	Entity entity;
	CaffUtil::ObserverPtr<Entity> obvPtr = entity.factory.create();

	ASSERT_IS_FALSE(obvPtr.expired())
	ASSERT_IS_EQUAL(obvPtr->name, 1234)

	ASSERT_IS_EQUAL(&entity, obvPtr.get())
}


TEST(Scopes)
{
	CaffUtil::ObserverPtr<Entity> obvPtr;
	ASSERT_IS_TRUE(obvPtr.expired())
	ASSERT_IS_FALSE(obvPtr)

	{
		Entity entity;
		obvPtr = entity.factory.create();

		ASSERT_IS_FALSE(obvPtr.expired())
		ASSERT_IS_TRUE(obvPtr)
	}

	ASSERT_IS_TRUE(obvPtr.expired())
	ASSERT_IS_FALSE(obvPtr)
}


TEST(AsignsAndCopy)
{

}


TEST(ReferenceCount)
{
	Entity entity;
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 0)

	CaffUtil::ObserverPtr<Entity> obvPtr1 = entity.factory.create();
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 1)

	// In and out of scope.
	{
		CaffUtil::ObserverPtr<Entity> obvPtr2 = entity.factory.create();
		ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 2)
	}

	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 1)

	// Copy and Asign
	CaffUtil::ObserverPtr<Entity> obvPtr3;
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 1)

	obvPtr3 = obvPtr1;
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 2)

	CaffUtil::ObserverPtr<Entity>obvPtr4(obvPtr3);
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 3)

	// Dump all
	entity.factory.flush();
	ASSERT_IS_EQUAL(entity.factory.numberOfObservers(), 0)	
	ASSERT_IS_TRUE(obvPtr1.expired())
	ASSERT_IS_TRUE(obvPtr3.expired())
	ASSERT_IS_TRUE(obvPtr4.expired())
}


TEST(Const)
{

}


int main()
{
	Test::RunTests();

	return 0;
}
