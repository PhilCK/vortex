
#ifndef STARSHIP_CONTROLLER_INCLUDED
#define STARSHIP_CONTROLLER_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class StarShipController : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(StarShipController)


private:


	// ** Engine Hooks ** //
	
	void			onParams(const CaffUtil::Param &params) override;
	void			onStart() override;
	void			onThink(const float dt) override;
	bool			onEvent(const CaffSys::Event &data) override;


private:


	float			m_jumpForce = 25.f;


}; // class


COMPONENT_FACTORY_INTERFACE(StarShipController)


#endif // include guard
