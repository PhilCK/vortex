
#ifndef LEVEL_MODEL_INCLUDED
#define LEVEL_MODEL_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class LevelModel : public CaffEnt::Component
{
public:
    
    
	COMPONENT_RTTI(LevelModel)
	
	// ** Component Interface ** //
	
	inline uint32_t		getLevel()		{ return m_currentLevel;				}
	inline void			setGameOver()	{ m_state = State::GAME_OVER;			}
	inline bool			isGameOver()	{ return m_state == State::GAME_OVER;	}
	inline void			resetModel()	{ m_state = State::START_UP;			}
	
    
private:
    
    
    // ** Engine Hooks ** //
    
    void                onParams(const CaffUtil::Param &params) override;
    void                onStart() override;
    void                onThink(const float dt) override;
	   
    
private:

    
	enum class State { START_UP, GAME, GAME_OVER, WAIT_FOR_RESET };
    
	State				m_state			= State::START_UP;
	uint32_t			m_currentLevel	= 1;
    
    
}; // class


COMPONENT_FACTORY_INTERFACE(LevelModel)


#endif // included guard
