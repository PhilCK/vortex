
/*
	Physics Component
	-----------------
	
	Project:
	Caffeine
	
	Purpose:
	Provides a rigidbody that will be attached to an entity.
	
	Requirements:
	Must have a transform component attached prior to attaching physics.
	
	General Useage:
	Before the component will create a rigidbody it requires a collision shape.
	
	Useage Without Params:
	std::unique_ptr<Collider> collider(new CubeCollider(1);
	physicsComponent->setColliderShape(std::move(collider)); // A rigid body will be created.
	
	Useage With Params:
	Inside entity xmls you can do this.
	<component>
		<name>Transform</name>
	</component>		

	<component>
		<name>Physics</name>
		<params>
			<param name="mass">50</param>
			<param name="collider">box 2 1.5 10 0 -2 0</param>
			<param name="collider">box 1 2 1 3 1.8 3.5</param>
		</params>			
	</component>
	
	each collider has a set number of attributes look at the colliders to see what is expected.
	
	*when using params, the component will always generate a compound collider.
 */

#ifndef CAFFEINE_COMPONENTS_PHYSICS_INCLUDED
#define CAFFEINE_COMPONENTS_PHYSICS_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>
#include <Caffeine/Math/Transform.hpp>

#include <btBulletDynamicsCommon.h>
//#include <BulletCollision/btBulletCollisionCommon.h>
#include <glm/glm.hpp>

#include <memory>
#include <array>
#include <vector>

namespace Caffeine {
namespace Components {



#pragma mark - Physics Component

/*
	PhysicsComponent
	----------------
	Component handles all the physics of an object.
	This needs alot of work to make it general, currently it should be inherited from
	to add the functionality required.
	
	TODO: needs to replace onEarlyThink, with something else that is called before the updates (or after).
	TODO: Rename this Rigidbody.
 */

class PhysicsComponent : public Entity::Component
{
public:

	enum Axis : uint8_t
	{
		X_AXIS = 1 << 0,
		Y_AXIS = 1 << 1,
		Z_AXIS = 1 << 2,
	};

	COMPONENT_RTTI(Physics)

	explicit					PhysicsComponent(CaffSys::PhysicsManager *physMgr);
								~PhysicsComponent();
				
	// Returns true if a rigidbody has been created.
	inline bool					hasRigidbody() const { return m_rigidbody != nullptr; };
	
	void						onAttachToEntity() override;
    
    void                        onAwake() override;
    void                        onSleep() override;
    void						onEnd() override;
	
	// Takes an xml document in a string.
	void						onParams(const CaffUtil::Param &params) override;
	
	void						onSuperEarlyThink(const float dt) override;
	void						onLateThink(const float dt) override;
	void						onSuperLateThink(const float dt) override;
	
	// Set the collider shape of the rigidbody.
	// Takes ownership of the ptr.
	//void						setColliderShape(std::unique_ptr<Collider> collider);
	//void						setColliderShape(Collider &collider);
	
	// Change the mass of an object.
	// TODO: Currently this will remove and re-add the object is there a better way. setMassProps()?
	void						setMass(const float mass);
	inline float				getMass() const { return m_mass; }
	
	inline void					showDebug(const bool showDebug) { m_showDebug = showDebug; updateRigidbody(); }
	
	// You can constrain movment to an xz field by passing in PhysicsComponent::X_AXIS | PhysicsComponent::Z_AXIS into setPositionAxis()
	// Or limit movment in y axis by passing in just PhysicsComponent::Y_AXIS
	void						setPositionAxis(const uint8_t axis);
	uint8_t						getPositionAxis() const { return m_posAxis; }
	
	void						setRotationAxis(const uint8_t axis);
	uint8_t						getRotationAxis() const { return m_rotAxis; }
    
    void                        setTransform(const CaffMath::Transform &tran, const bool updateRot = true, const bool updatePos = true);
	
	// If you set a rigidbody to setTrigger(true) the rigidbody will have
	// no response to collisions, but you will still get collision callbacks.
	void						setTrigger(const bool isTrigger);
	inline bool					isTrigger() const { return m_isTrigger; }
	
	// URGH!!!
	// TODO: This is a mess.	
	void						applyForce(const float *dir);
	void						applyLocalForce(const float *dir, const float *offset);
	void						applyLocalForce(const float *dir);
	void						applyTorque(const float *data);
	void						applyLocalTorque(const float *data);
	
	// TODO: To remove this we need to fix the transform component.
	inline btRigidBody*			getRigidBody() const { return m_rigidbody.get(); }
	
private:

	// This gets called when a change in the rigid body happens.
	void						updateRigidbody();
	
private:

	// General properties
	bool						m_isTrigger		= false;
	bool						m_isDirty		= true;
	bool						m_showDebug		= false;
	float						m_mass			= 0.f;
	uint8_t						m_rotAxis		= X_AXIS | Y_AXIS | Z_AXIS;
	uint8_t						m_posAxis		= X_AXIS | Y_AXIS | Z_AXIS;

	// Bullet physics stuff
	std::vector<std::unique_ptr<btCollisionShape> > m_colliderShapes;
	std::unique_ptr<btDefaultMotionState>			m_motionState			= nullptr;
	std::unique_ptr<btRigidBody>					m_rigidbody				= nullptr;
	btVector3										m_rbodyInertia			= btVector3(0.f, 0.f, 0.f);
	uint32_t										m_prevActivationState;

	CaffSys::PhysicsManager		*m_physicsMgr	= nullptr;
	
}; // class


} // namespace
} // namespace

#endif // include guard
