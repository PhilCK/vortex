
#include <UnitTest.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <memory>
#include <iostream>

namespace CaffEntity = ::Caffeine::Entity;

// Test components.
class TestComponentOne : public CaffEntity::Component {};
class TestComponentTwo : public CaffEntity::Component {};


// Check to see if components attach
TEST(AttachEntity)
{
	CaffEntity::EntityUniquePtr 	entity(new CaffEntity::Entity);
	CaffEntity::ComponentUniquePtr 	component1(new CaffEntity::Component);
	//CaffEntity::ComponentUniquePtr 	component2(new CaffEntity::Component);

	entity->addComponent(component1);

	// Check 

	//entity->addComponent(component2);

	// Check
}

// Detach components.
TEST(DetachEntity)
{
	// Write me!
}

int main()
{
	Test::RunTests();

	return 0;
}