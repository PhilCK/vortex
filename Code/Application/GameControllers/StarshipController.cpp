
#include <Application/GameControllers/StarshipController.hpp>
#include <Application/GameModels/Movement.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Utilities/Logging.hpp>



void StarShipController::onParams(const CaffUtil::Param &params)
{
	m_jumpForce = params["jump_force"].asFloat(m_jumpForce);
}



void StarShipController::onStart()
{
}



void StarShipController::onThink(const float dt)
{
	const CaffApp::Input &input = getOwner().getInput();
	Movement *shipMovement		= getOwner().getComponent<Movement>();
	assert(shipMovement);
	
	// Move
	{
		const float dx = input.getMouseDeltaX() * 0.5f;
		
		shipMovement->moveRight(dx);
	}

	// Jump
	{
		if(input.isDownOnFrame(CaffApp::KeyID::KB_SPACE) && !shipMovement->isJumping())
		{
			shipMovement->jump(m_jumpForce);
		}
	}
	
	// Fire a bullet.
	{
		if(input.isDownOnFrame(CaffApp::KeyID::MS_LEFT))
		{
			CaffEnt::Entity *bullet = getOwner().getState()->pushNewEntity("Bullet");
			assert(bullet);
			
			// Set bullet transform.
			{
				const VortexTransform *vortTransform = getOwner().getComponent<VortexTransform>();
				assert(vortTransform);
				
				VortexTransform *bulletVortTransform = bullet->getComponent<VortexTransform>();
				assert(bulletVortTransform);
				
				bulletVortTransform->setAngle(vortTransform->getAngle());
				bulletVortTransform->setDepth(vortTransform->getDepth());
			}
		}
	}
} // onThink(...)



bool StarShipController::onEvent(const CaffSys::Event &data)
{
	return false;
}



COMPONENT_FACTORY_SOURCE(StarShipController)

