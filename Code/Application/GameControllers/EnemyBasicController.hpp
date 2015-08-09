
#ifndef ENEMY_BASIC_CONTROLLER_INCLUDED
#define ENEMY_BASIC_CONTROLLER_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class EnemyBasicController : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(EnemyBasicController)
	
	
private:


	// ** Engine Hooks ** //
	
	void				onParams(const CaffUtil::Param &params) override;
	void				onStart() override;
	void				onThink(const float dt) override;
	bool				onEvent(const CaffSys::Event &data) override;
	

private:

    

}; // class


COMPONENT_FACTORY_INTERFACE(EnemyBasicController)


#endif // include guard
