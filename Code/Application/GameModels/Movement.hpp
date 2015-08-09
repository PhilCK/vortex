
#ifndef MOVEMENT_INCLUDED
#define MOVEMENT_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class Movement : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(Movement)


	// ** Component Interaction ** //

	inline void			moveUp(const float moveAmt)			{ m_pendingMovement.z += moveAmt;	}
	inline void			moveRight(const float moveAmt)		{ m_pendingMovement.x += moveAmt;	}
	inline void			jump(const float force)				{ m_jumpForce += force;				}
	inline bool			isJumping() const					{ return m_isJumping;				}


private:


	// ** Engine Hooks ** //
	
	void				onParams(const CaffUtil::Param &params) override;
	void				onStart() override;
	void				onLateThink(const float dt) override;

	
private:


	float				m_moveScale			= 1.f;
	float				m_moveMomentum		= 0.f;
	float				m_moveFallOff		= 50.0f;
	float				m_jumpGravity		= 1.f;
	float				m_jumpForce			= 0.f;
	float				m_jumpVel			= 0.f;
	float				m_jumpPosition		= 0.f;
	CaffMath::Vector3	m_pendingMovement	= CaffMath::Vector3Init(0.f, 0.f, 0.f);
	float				m_moveEventID		= 0;
	bool				m_isJumping			= false;

}; // class


COMPONENT_FACTORY_INTERFACE(Movement)


#endif // include guard