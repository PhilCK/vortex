
#ifndef ENEMY_BREEDER_CONTROLLER_INCLUDED
#define ENEMY_BREEDER_CONTROLLER_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class EnemyBreederController : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(EnemyBreederController)
	
	
	// ** Component Interface ** //
	
	inline void			setSpawnAndBreed() { m_state = State::SPAWN_AND_BREED; }
	
	
private:


	// ** Engine Hooks ** //
	
	void				onParams(const CaffUtil::Param &params) override;
	void				onStart() override;
	void				onThink(const float dt) override;
	bool				onEvent(const CaffSys::Event &data) override;
	

private:


	enum class State { SPAWN_AND_MOVE, SPAWN_AND_BREED, MOVE, BREED, SLEEP, };
	
	State				m_state				= State::SPAWN_AND_MOVE;
	uint32_t			m_numberOfChildren	= 1;
	uint32_t			m_currentChildren	= 0;
	float				m_breedRate			= 0.5f;
	float				m_currentBreedTime	= 0.f;
    

}; // class


COMPONENT_FACTORY_INTERFACE(EnemyBreederController)


#endif // include guard
