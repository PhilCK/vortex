
#include <Application/GameControllers/BulletController.hpp>
#include <Application/GameModels/Movement.hpp>
#include <Caffeine/Entity/Entity.hpp>




void BulletController::onParams(const CaffUtil::Param &params)
{
	m_velocity = params["velocity"].asFloat(m_velocity);
}



void BulletController::onThink(const float dt)
{
	// Move bullet in direction.
	{
        Movement *movement = getOwner().getComponent<Movement>();
		assert(movement);
		
		movement->moveUp(m_velocity);
	}
}




COMPONENT_FACTORY_SOURCE(BulletController)
