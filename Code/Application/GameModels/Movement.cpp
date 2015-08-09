
#include <Application/GameModels/Movement.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Math/Quaternion.hpp>
#include <Caffeine/Utilities/Logging.hpp>



void Movement::onParams(const CaffUtil::Param &params)
{
	m_moveScale		= params["move_scale"].asFloat(m_moveScale);
	m_moveFallOff	= params["move_falloff"].asFloat(m_moveFallOff);
	m_jumpGravity	= params["jump_gravity"].asFloat(m_jumpGravity);
}



void Movement::onStart()
{
	// Get inital jump position
	{
		VortexTransform *vortTransform = getOwner().getComponent<VortexTransform>();
		assert(vortTransform);
			
		m_jumpPosition = vortTransform->getDepth();
	}
}



void Movement::onLateThink(const float dt)
{
	VortexTransform *vortTransform = getOwner().getComponent<VortexTransform>();
	assert(vortTransform);

	// Apply pending movement
	{
		const float moveScale = m_moveScale * dt;
		const CaffMath::Vector3 moveDir = CaffMath::Vector3Scale(m_pendingMovement, moveScale);
		
		// set Angle
		{
			const float newAngle = vortTransform->getAngle() + CaffMath::Vector3GetX(moveDir);
			vortTransform->setAngle(newAngle);
		}
		
		// set Depth
		{
			const float newDepth = vortTransform->getDepth() + CaffMath::Vector3GetZ(moveDir);
			vortTransform->setDepth(newDepth);
		}
		
		// Move Event.
		{
			const float error = 0.001f;
		
			if(CaffMath::Vector3Magnitude(moveDir) > error)
			{
				CaffSys::Event moveEvt(ObjectMove::GetID());
				
				moveEvt.at(ObjectMove::OBJ_ID)		= CaffSys::EventArg(getOwner().getID());
				moveEvt.at(ObjectMove::POSITION)	= CaffSys::EventArg(vortTransform->getPosition());
				moveEvt.at(ObjectMove::DIRECTION)	= CaffSys::EventArg(moveDir);
				
				getOwner().getState()->getEventManager().sendInstantEvent(moveEvt);
			}
		}
		
		m_moveMomentum		+= CaffMath::Vector3GetX(moveDir);
		m_pendingMovement	=  CaffMath::Vector3Init(0.f, 0.f, 0.f);
	}
	
	// Jump Logic
	{
		// Are we jumping?
		if(m_jumpForce > 0.f)
		{
			m_isJumping = true;
			m_jumpPosition = vortTransform->getDepth();
			
			m_jumpVel	= 1.f * (m_jumpForce * dt);
			m_jumpForce	= 0.f;
			
			// Jump Event
			{
				CaffSys::Event jumpEvt(ObjectJump::GetID());
				
				jumpEvt.at(ObjectJump::OBJ_ID)		= CaffSys::EventArg(getOwner().getID());
				jumpEvt.at(ObjectJump::POSITION)	= CaffSys::EventArg(vortTransform->getPosition());
				jumpEvt.at(ObjectJump::SPEED)		= CaffSys::EventArg(CaffMath::Abs(m_jumpVel));
				
				getOwner().getState()->getEventManager().sendInstantEvent(jumpEvt);
			}
		}
		
		// Update Position
		if(isJumping())
		{
			m_jumpVel -= (m_jumpGravity * dt);
			
			const float currDepth = vortTransform->getDepth();
			const float newDepth  = currDepth + m_jumpVel;
			
			vortTransform->setDepth(CaffMath::Max(newDepth, m_jumpPosition));
			
			if(newDepth <= m_jumpPosition)
			{
				m_jumpForce = m_jumpVel = 0.f;
				m_isJumping = false;
			}
		}
	}
	
	// Apply rotation to point ship correction, with momentum tilt.
	{
		CaffComp::TransformComponent *transform = getOwner().getComponent<CaffComp::TransformComponent>();
		assert(transform);
	
		const float getAngle = vortTransform->getAngle() + CaffMath::QuartTau() + m_moveMomentum;
		const float newAngle = CaffMath::FMod(getAngle, CaffMath::Tau());
		
		const CaffMath::Vector3 rotationAxis = CaffMath::Vector3Init(0.f, 0.f, -1.f);
		const CaffMath::Quaternion rotation  = CaffMath::QuaternionInitFromAngleAxis(-newAngle, rotationAxis);
		
		transform->setRotation(rotation);
		
		const float fallOffMax	= 0.95f;
		const float fallOff		= CaffMath::Min(m_moveFallOff * dt, fallOffMax);
		m_moveMomentum			*= fallOff;
	}
}



COMPONENT_FACTORY_SOURCE(Movement)
