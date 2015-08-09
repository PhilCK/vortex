
#include <Application/GameModels/VortexCamera.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Application/Common/GameObjectTags.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/State.hpp>



void VortexCamera::onParams(const CaffUtil::Param &params)
{
	m_rateChange  = params["rate_change"].asFloat(m_rateChange);
	m_cameraPlane = params["camera_plane"].asFloat(m_cameraPlane);
}



void VortexCamera::onStart()
{
	registerForEvent(VortexMove::GetID());

	// Set Start Pos.
	{
		CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
		assert(transform);
		
		const CaffMath::Vector3 pos		 = transform->getPosition();
		const CaffMath::Vector3 startPos = CaffMath::Vector3Init(pos.x, pos.y, m_cameraPlane);
		
		transform->setPosition(startPos);
	}
}



void VortexCamera::onThink(const float dt)
{
	CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
	assert(transform);

	// update position
	{
		const float changeRate = m_rateChange * dt;
	
		const CaffMath::Vector3 direction = CaffMath::Vector3ComponentSubtract(m_targetPosition, transform->getPosition());
		const CaffMath::Vector3 change	  = CaffMath::Vector3Scale(direction, changeRate);
		const CaffMath::Vector3 newPos	  = CaffMath::Vector3ComponentAdd(transform->getPosition(), change);
	
		transform->setPosition(newPos);
	}
	
	// Set Rotation
	// TODO: Double check this math.
	{
		const CaffMath::Vector3 up          = CaffMath::Vector3Init(0, 1, 0);
		const CaffMath::Vector3 lookatDir   = CaffMath::Vector3ComponentSubtract(m_targetLookAt, transform->getPosition());
		
		const CaffMath::Vector3 UCrossL     = CaffMath::Vector3CrossProduct(up, lookatDir);
		const CaffMath::Vector3 UCrossLNorm = CaffMath::Vector3Normalize(UCrossL);
		
		const float targetPosLen            = CaffMath::Vector3Magnitude(up);
		const float targetLookAtLen         = CaffMath::Vector3Magnitude(lookatDir);
		
		const float w = CaffMath::SquareRoot((targetPosLen * targetPosLen) * (targetLookAtLen * targetLookAtLen)) + CaffMath::Vector3DotProduct(up, lookatDir);
		
		const CaffMath::Quaternion rotation = CaffMath::QuaternionInit(w, UCrossLNorm.x, UCrossLNorm.y, UCrossLNorm.z);
		const CaffMath::Quaternion rotNorm  = CaffMath::QuaternionNormalize(rotation);
		
		transform->setRotation(rotNorm);
	}
}



bool VortexCamera::onEvent(const CaffSys::Event &data)
{
	// Player Moved.
	if(data.is(VortexMove::GetID()))
	{		
        // What object moved
        CaffEnt::Entity *entity = getOwner().getState()->getEntityByID(data.at(VortexMove::OBJ_ID).get<std::size_t>());
        
        // Calc position
        if(entity && entity->hasTag(GameObjectTag::player))
        {
            m_targetPosition = data.at(1).get<CaffMath::Vector3>();
            m_targetPosition.z = m_cameraPlane;
        }
		
		return false;
	}

	return false;
}



COMPONENT_FACTORY_SOURCE(VortexCamera)
