
#include <Application/GameControllers/EnemyBasicController.hpp>
#include <Application/GameModels/Movement.hpp>
#include <Application/GameModels/Health.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>



void EnemyBasicController::onParams(const CaffUtil::Param &params)
{
}



void EnemyBasicController::onStart()
{
}



void EnemyBasicController::onThink(const float dt)
{
	// Movement
	{
		Movement *movement = getOwner().getComponent<Movement>();
		assert(movement);
		
		movement->moveUp(1.1f);
	}
}



bool EnemyBasicController::onEvent(const CaffSys::Event &data)
{	
	if(data.is(VortexCollision::GetID()))
	{
		Health *health = getOwner().getComponent<Health>();
		assert(health);
		
		health->takeDamage(1);
		
		return CaffSys::EventNotSwallowed;
	}

	return CaffSys::EventNotSwallowed;
}



COMPONENT_FACTORY_SOURCE(EnemyBasicController)
