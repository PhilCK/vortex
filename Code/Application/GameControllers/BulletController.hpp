
#ifndef BULLET_CONTROLLER_INCLUDED
#define BULLET_CONTROLLER_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class BulletController : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(BulletController)
	

private:


	void			onParams(const CaffUtil::Param &param) override;
	void			onThink(const float dt) override;


private:


	float			m_velocity = -1.f;


}; // class


COMPONENT_FACTORY_INTERFACE(BulletController)


#endif // include guard
