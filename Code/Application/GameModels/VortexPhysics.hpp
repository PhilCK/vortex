
#ifndef VORTEX_PHYSICS_INCLUDED
#define VORTEX_PHYSICS_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <string>
#include <vector>


/*
	Events
	--
	vortex_collision - Sends direct to entity that it has collided.
*/
class VortexPhysics : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(VortexPhysics)


private:


	// ** Engine Hooks ** //
	
	void				onParams(const CaffUtil::Param &params) override;
	void				onStart() override;
	void				onThink(const float dt) override;
	void				onCollision(const CaffEnt::ColliderDetails &details) override;
	
	
private:


	std::vector<std::string>	m_collideWithTags;
	

}; // class


COMPONENT_FACTORY_INTERFACE(VortexPhysics)


#endif // include guard
