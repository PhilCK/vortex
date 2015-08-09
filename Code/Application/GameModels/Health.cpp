
#include <Application/GameModels/Health.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>



void Health::onParams(const CaffUtil::Param &params)
{
	m_health				= params["health"].asUInt32(m_health);
	m_maxHealth				= params["max_health"].asUInt32(CaffMath::Max(m_maxHealth, m_health));
	m_destroyOnZeroHealth	= params["destoy_on_zero_health"].asBool(m_destroyOnZeroHealth);
}



void Health::onLateThink(const float dt)
{
	// Pending health
	if(m_pendingHealth)
	{
		m_health = CaffMath::Min(m_health + m_pendingHealth, m_maxHealth);
		
		// Health Up Event
		CaffSys::Event healthUp(ObjectHeal::GetID());
		healthUp.at(ObjectHeal::OBJ_ID)			= CaffSys::EventArg(getOwner().getID());
		healthUp.at(ObjectHeal::HEALTH_GAINED)	= CaffSys::EventArg(m_pendingHealth);
		healthUp.at(ObjectHeal::HEALTH_LEFT)	= CaffSys::EventArg(m_health);
		getOwner().getEventManager().sendInstantEvent(healthUp);
		
		m_pendingHealth = 0;
	}
	
	// Pending damage
	if(m_pendingDamage && m_pendingDamage < m_health)
	{
		(m_pendingDamage > m_health) ? (m_health = 0) : (m_health -= m_pendingDamage);
		
		// Damage Event
		CaffSys::Event damage(ObjectDamage::GetID());
		damage.at(ObjectDamage::OBJ_ID)			= CaffSys::EventArg(getOwner().getID());
		damage.at(ObjectDamage::HEALTH_LOST)	= CaffSys::EventArg(m_pendingDamage);
		damage.at(ObjectDamage::HEALTH_LEFT)	= CaffSys::EventArg(m_health);
		getOwner().getEventManager().sendInstantEvent(damage);
        
		m_pendingDamage = 0;
	}
	
	// Did obj just die
	if(m_pendingDamage >= m_health)
	{
		m_health		= 0;
		m_pendingDamage = 0;
		
		// Death Event
		const VortexTransform *transform = getOwner().getComponent<VortexTransform>();
		assert(transform);
		
		CaffSys::Event death(ObjectDestroyed::GetID());
		death.at(ObjectDestroyed::OBJ_ID)	= CaffSys::EventArg(getOwner().getID());
		death.at(ObjectDestroyed::POSITION)	= CaffSys::EventArg(transform->getPosition());
		
		getOwner().getEventManager().sendInstantEvent(death);
		getOwner().onEvent(death);
		
		if(m_destroyOnZeroHealth)
		{
			getOwner().destroy();
		}
	}
}



COMPONENT_FACTORY_SOURCE(Health)


