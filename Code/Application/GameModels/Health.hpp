
#ifndef HEALTH_INCLUDED
#define HEALTH_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Math/Math.hpp>


class Health : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(Health)


	// ** Component Interaction ** //
	
	inline void				takeDamage(const uint32_t damage)	{ m_pendingDamage += damage;				}
	inline void				addHealth(const uint32_t addHealth)	{ m_pendingHealth += addHealth;				}
	inline bool				isDead() const						{ return (m_health + m_pendingHealth) <= 0; }


private:


	// ** Engine Hooks ** //

	void					onParams(const CaffUtil::Param &params) override;
	void					onLateThink(const float dt) override;


private:


	uint32_t				m_pendingDamage			= 0;
	uint32_t				m_pendingHealth			= 0;
	uint32_t				m_health				= 1;
	uint32_t				m_maxHealth				= CaffMath::Max((uint32_t)1, m_health);
	bool					m_destroyOnZeroHealth	= true;


}; // class


COMPONENT_FACTORY_INTERFACE(Health)


#endif // include gaurd