
#include <UnitTest.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>


namespace EntityTests {

using namespace Caffeine::Entity;


TEST(EntityCreation)
{
	Entity *ent1 = new Entity;

	ASSERT_IS_EQUAL(ent1->numberOfChildren(), 0)
	ASSERT_IS_EQUAL(ent1->getName(), "Entity")
	ASSERT_IS_FALSE(ent1->getParent())
	ASSERT_IS_EQUAL(ent1->numberOfComponents(), 0)

	ent1->setName("FooBar");
	ASSERT_IS_EQUAL(ent1->getName(), "FooBar")
	ASSERT_IS_NOT_EQUAL(ent1->getName(), "foobar")

	delete ent1;
}


TEST(ParentAndChildren)
{
	EntityUniquePtr ent1 = EntityUniquePtr(new Entity);
	EntityUniquePtr ent2 = EntityUniquePtr(new Entity);
	ent2->setName("barFoo");
	EntityPtr ent2Raw = ent2.get();
	EntityUniquePtr ent3 = EntityUniquePtr(new Entity);

	// Simple add / get child
	{
		ent1->addChild(std::move(ent2));

		bool isFalse = ent2 ? true : false;

		ASSERT_IS_FALSE(isFalse)
		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 1)
		ASSERT_IS_FALSE(ent1->getParent())

		//EntityPtr child = ent1->getChild(ent2Raw);

		//ASSERT_IS_EQUAL(ent2->getParent(), ent1.get());
		//ASSERT_IS_EQUAL(ent2->getName(), "Sub-Entity")
/*
		bool removeChildCheck = ent2->setParent(nullptr);

		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 0)
		ASSERT_IS_TRUE(removeChildCheck)
		ASSERT_IS_FALSE(ent2->getParent())
		ASSERT_IS_NOT_EQUAL(ent2->getParent(), ent1)
		ASSERT_IS_EQUAL(ent2->getName(), "Entity")
		*/
	}

	/*
	// Simple add 2 different
	{
		bool addChildCheck1 = ent1->addChild(ent2);
		bool addChildCheck2 = ent1->addChild(ent3);

		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 2)
		ASSERT_IS_TRUE(addChildCheck1)
		ASSERT_IS_TRUE(addChildCheck2)
		ASSERT_IS_FALSE(ent1->getParent())
		ASSERT_IS_EQUAL(ent2->getParent(), ent1)
		ASSERT_IS_EQUAL(ent3->getParent(), ent1)

		bool removeChildCheck1 = ent2->setParent(nullptr);

		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 1)
		ASSERT_IS_TRUE(removeChildCheck1)
		ASSERT_IS_FALSE(ent2->getParent())
		
		bool removeChildCheck2 = ent3->setParent(nullptr);

		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 0)
		ASSERT_IS_TRUE(removeChildCheck2)
		ASSERT_IS_FALSE(ent3->getParent())
	}
	*/

	// Add same twice
	{
		/*
		bool shouldBeTrue  = ent1->addChild(ent2);
		bool shouldBeFalse = ent1->addChild(ent2);

		ASSERT_IS_EQUAL(ent1->numberOfChildren(), 1)
		ASSERT_IS_TRUE(shouldBeTrue)
		ASSERT_IS_FALSE(shouldBeFalse)
		ASSERT_IS_FALSE(ent1->getParent())
		ASSERT_IS_EQUAL(ent2->getParent(), ent1);
		*/
	}
	
	// Add 

	/*
	delete ent1;
	delete ent2;
	delete ent3;
	delete ent4;
	delete ent5;
	*/
}


TEST(TryAndScrewUpThings)
{

}


//int main()
//{
//	Test::RunTests();
//
//	return 0;
//}


} // namespace
