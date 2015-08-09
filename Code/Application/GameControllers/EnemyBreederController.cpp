
#include <Application/GameControllers/EnemyBreederController.hpp>
#include <Application/GameModels/Movement.hpp>
#include <Application/GameModels/Health.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Math/Math.hpp>



void EnemyBreederController::onParams(const CaffUtil::Param &params)
{
	m_numberOfChildren	= params["number_of_children"].asUInt32(m_numberOfChildren);
	m_breedRate			= params["breed_rate"].asFloat(m_breedRate);
}



void EnemyBreederController::onStart()
{
}



void EnemyBreederController::onThink(const float dt)
{
	switch(m_state)
	{
		case(State::SPAWN_AND_MOVE):
		{
			m_state = State::MOVE;
			break;
		}
		
		case(State::SPAWN_AND_BREED):
		{
			m_state = State::BREED;
			break;
		}
		
		case(State::MOVE):
		{
			VortexTransform *vortTransform = getOwner().getComponent<VortexTransform>();
			assert(vortTransform);
			
			Movement *movement = getOwner().getComponent<Movement>();
			assert(movement);
				
			movement->moveUp(8.1f);
			
			const float finalDepth = 7;
			
			if(vortTransform->getDepth() >= finalDepth)
			{
				vortTransform->setDepth(finalDepth);
				m_state = State::BREED;
			}
			
			break;
		}
		
		case(State::BREED):
		{
			if(m_currentChildren < m_numberOfChildren)
			{
				m_currentBreedTime += dt;
				
				if(m_currentBreedTime > m_breedRate)
				{
					CaffEnt::Entity *newChild = getOwner().getState()->pushNewEntity("EnemyBreederShip");
					assert(newChild);
					
					EnemyBreederController *controller = newChild->getComponent<EnemyBreederController>();
					assert(controller);
					
					controller->setSpawnAndBreed();
					
					const VortexTransform *vortTransform = getOwner().getComponent<VortexTransform>();
					assert(vortTransform);
					
					VortexTransform *childVortTransform = newChild->getComponent<VortexTransform>();
					assert(childVortTransform);
					
					childVortTransform->setDepth(vortTransform->getDepth());
					childVortTransform->setAngle(vortTransform->getAngle() + CaffMath::RandFloatRange(-0.2f, 0.2f));
					
					m_currentChildren += 1;
					m_currentBreedTime = 0;
				}
			}
			else
			{
				m_state = State::SLEEP;
			}
			
			break;
		}
		
		case(State::SLEEP):
		{
			break;
		}
		
		default:
			assert(false); // Why you here!
	}
}



bool EnemyBreederController::onEvent(const CaffSys::Event &data)
{
	if(data.is(VortexCollision::GetID()))
	{
		Health *health = getOwner().getComponent<Health>();
		assert(health);
		
		health->takeDamage(1);
		
		return CaffSys::EventNotSwallowed;
	}

	return CaffSys::EventNotSwallowed;

	return false;
}



COMPONENT_FACTORY_SOURCE(EnemyBreederController)
