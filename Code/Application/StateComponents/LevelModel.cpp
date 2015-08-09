
#include <Application/StateComponents/LevelModel.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>



void LevelModel::onParams(const CaffUtil::Param &params)
{
}



void LevelModel::onStart()
{
}



void LevelModel::onThink(const float dt)
{
	switch(m_state)
	{
		// Startup event.
		case State::START_UP:
		{
			// Event
			{
				CaffSys::Event startUp(LevelStart::GetID());
				startUp.at(LevelStart::OBJ_ID) = CaffSys::EventArg(getOwner().getID());
				getOwner().getEventManager().sendInstantEvent(startUp);
			}
			
			m_currentLevel = 1;
			m_state = State::GAME;
		
			break;
		}
		
		// Are we ready to spawn something.
		case State::GAME:
		{
			
			break;
		}
		
		//
		case State::GAME_OVER:
		{
			// Event
			{
				CaffSys::Event gameOver(LevelStart::GetID());
				gameOver.at(LevelStart::OBJ_ID) = CaffSys::EventArg(getOwner().getID());
				getOwner().getEventManager().sendInstantEvent(gameOver);
			}

			m_currentLevel = 0;
			m_state = State::WAIT_FOR_RESET;
		
			break;
		}
		
		// Wait for reset
		case State::WAIT_FOR_RESET:
		break;
		
		// Shouldn't be here boyo.
		default:
			assert(false);
	}
}



COMPONENT_FACTORY_SOURCE(LevelModel)
