
#include <Application/GameModels/VortexTransform.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Systems/EventManager.hpp>



// ** Component Interface ** //

CaffMath::Vector3 VortexTransform::getPosition() const
{
	const CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
	assert(transform);

	return transform->getPosition();
}



// ** Engine Hooks ** //

void VortexTransform::onParams(const CaffUtil::Param &params)
{
	m_depth					= params["depth"].asFloat(m_depth);
	m_angle					= params["angle"].asFloat(m_angle);
	m_vortexRadius			= params["vortex_radius"].asFloat(m_vortexRadius);
	m_nearZone				= params["near_zone"].asFloat(m_nearZone);
	m_farZone				= params["far_zone"].asFloat(m_farZone);
	m_destroyOnOutOfBounds	= params["destroy_on_out_of_bounds"].asBool(m_destroyOnOutOfBounds);
}




void VortexTransform::onStart()
{
	// Add transform if its missing
	{
		const CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
		
		if(!transform)
		{
			CaffUtil::LogWarning("VortexTransform - Missing 'Transform' adding it to entity.");
			std::unique_ptr<CaffEnt::Component> newTransform(new CaffComp::TransformComponent());
			getOwner().addComponent(newTransform);
		}
	}
}



void VortexTransform::onThink(const float dt)
{
	// Set position
	{
		// Circle
		const float x = CaffMath::Cos(m_angle) * m_vortexRadius;
		const float y = CaffMath::Sin(m_angle) * m_vortexRadius;
		const float z = m_depth;

		const CaffMath::Vector3 newPos = CaffMath::Vector3Init(x, y, z);

		CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
		assert(transform);

		// Have we moved?
		const CaffMath::Vector3 currPos  = transform->getPosition();
		const CaffMath::Vector3 movement = CaffMath::Vector3ComponentSubtract(currPos, newPos);
		const float magnitude			 = CaffMath::Vector3Magnitude(movement);
		const float errorMargin			 = 0.0001f;
		
		if(magnitude > errorMargin)
		{
			transform->setPosition(newPos);

			CaffSys::Event moveEvent(VortexMove::GetID());
			moveEvent.at(VortexMove::OBJ_ID)   = CaffSys::EventArg(getOwner().getID());
			moveEvent.at(VortexMove::POSITION) = CaffSys::EventArg(newPos);
		
			getOwner().getEventManager().sendInstantEvent(moveEvent);
		}
	}
    
    // Is out of bounds
    {
        if(CaffMath::InRange(m_depth, m_farZone, m_nearZone))
        {
            const CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
            assert(transform);
            
            CaffSys::Event outOfBoundsEvent(VortexOutOfBounds::GetID());
            outOfBoundsEvent.at(VortexOutOfBounds::OBJ_ID)      = CaffSys::EventArg(getOwner().getID());
            outOfBoundsEvent.at(VortexOutOfBounds::POSITION)    = CaffSys::EventArg(transform->getPosition());
			
			getOwner().getEventManager().sendInstantEvent(outOfBoundsEvent);
			
			if(m_destroyOnOutOfBounds)
			{
				getOwner().destroy();
			}
        }
    }
}



COMPONENT_FACTORY_SOURCE(VortexTransform)
