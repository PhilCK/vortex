
#include <Caffeine/Components/PhysicsComponent.hpp>

#include <Caffeine/Components/ColliderComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/ModelComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>

#include <LinearMath/btAabbUtil2.h>
//#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <functional>



namespace Caffeine {
namespace Components {



#pragma mark - Physics Components

PhysicsComponent::PhysicsComponent(CaffSys::PhysicsManager *physMgr)
: Entity::Component()
, m_isTrigger(false)
, m_isDirty(true)
#ifndef NDEBUG
, m_showDebug(true)
#else
, m_showDebug(false)
#endif
, m_mass(0.f)
, m_rotAxis(X_AXIS | Y_AXIS | Z_AXIS)
, m_posAxis(X_AXIS | Y_AXIS | Z_AXIS)
, m_motionState(nullptr)
, m_rigidbody(nullptr)
, m_rbodyInertia(0, 0, 0)
, m_physicsMgr(physMgr)
{
	assert(m_physicsMgr);
}



PhysicsComponent::~PhysicsComponent()
{
	// When entities get destroyed ins't guarenteed, consider onEnd();
	onEnd(); // This is just incase.
}



void PhysicsComponent::onAttachToEntity()
{
	getOwner().getParent()->getComponent<PhysicsComponent>()->updateRigidbody();
	getOwner().getComponent<PhysicsComponent>()->updateRigidbody();
}



void PhysicsComponent::onParams(const CaffUtil::Param &params)
{
    m_isTrigger = params["is_trigger"].asBool(m_isTrigger);
    m_mass      = params["mass"].asFloat(m_mass);
	m_showDebug = params["show_debug"].asBool(m_showDebug);
	
	// Freeze axis
	{
		auto ResetAndEnableAxis = [](const std::vector<std::string> &enabledAxis, uint8_t &axisType)
		{
			axisType = 0; // Reset for incoming paramaters
		
			for(auto &axis : enabledAxis)
			{
				if(axis == "x")
				{
					axisType |= X_AXIS;
				}
				else if(axis == "y")
				{
					axisType |= Y_AXIS;
				}
				else if(axis == "z")
				{
					axisType |= Z_AXIS;
				}
			}
		};
		
		const std::string defaultAxis = "x y z";
	
		// Rotation
		if(params.doesMemberExist("rotation_axis"))
		{
			std::vector<std::string> rotAxis = CaffUtil::SplitStringBySpaces(params["rotation_axis"].asStdString(defaultAxis));
			ResetAndEnableAxis(rotAxis, m_rotAxis);
		}

		// Position
		if(params.doesMemberExist("position_axis"))
		{
			std::vector<std::string> posAxis = CaffUtil::SplitStringBySpaces(params["position_axis"].asStdString(defaultAxis));
			ResetAndEnableAxis(posAxis, m_posAxis);
		}
	}
}



void PhysicsComponent::onEnd()
{
	if(m_rigidbody.get() && m_physicsMgr)
	{
		m_physicsMgr->removeRigidbody(m_rigidbody.get());
		m_rigidbody.reset();
	}
}
    

    
void PhysicsComponent::onAwake()
{
	getOwner().getParent()->getComponent<PhysicsComponent>()->updateRigidbody();
	getOwner().getComponent<PhysicsComponent>()->updateRigidbody();
}



void PhysicsComponent::onSleep()
{
	m_physicsMgr->removeRigidbody(m_rigidbody.get());
	getOwner().getParent()->getComponent<PhysicsComponent>()->updateRigidbody();
}

    

void PhysicsComponent::setTransform(const CaffMath::Transform &setTransform, const bool updateRot, const bool updatePos)
{
    if(m_rigidbody)
    {
        btTransform transform = m_rigidbody->getWorldTransform();
        
        if(updatePos)
        {
            const btVector3 setPos(setTransform.position.x, setTransform.position.y, setTransform.position.z);
            transform.setOrigin(setPos);
        }

        if(updateRot)
        {
            const btQuaternion setRot(setTransform.rotation.x, setTransform.rotation.y, setTransform.rotation.z, setTransform.rotation.w);
            transform.setRotation(setRot);
        }
        
        //m_rigidbody->setWorldTransform(transform);
        //const uint32_t prevColFlags     = m_rigidbody->getCollisionFlags();
        //m_prevActivationState = m_rigidbody->getActivationState();
        
        //m_rigidbody->setCollisionFlags(m_rigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        //m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
        
        m_rigidbody->setWorldTransform(transform);
        //m_rigidbody->getMotionState()->setWorldTransform(transform);
        
        //m_rigidbody->setCollisionFlags(prevColFlags);
        //m_rigidbody->setActivationState(prevActivationState);
    }
}


void PhysicsComponent::onSuperEarlyThink(const float dt)
{
	if(m_isDirty)
	{
		updateRigidbody();
		m_isDirty = false;
	}

	// Update the transform
	if(m_rigidbody)
	{
		CaffComp::TransformComponent *transform = getOwner().getComponent<TransformComponent>();
		assert(transform);
		
		// Update Rigidbody collider
		// TODO: Doing this every frame?
		{
			const CaffMath::Vector3 scale = transform->getScale();
			const btVector3 colliderScale(scale.x, scale.y, scale.z);
			
			m_rigidbody->getCollisionShape()->setLocalScaling(colliderScale);
		}
		
		// Update Transform
		{
			btTransform trans;
			m_rigidbody->getMotionState()->getWorldTransform(trans);
			
			const btVector3 btPos = trans.getOrigin();
			const CaffMath::Vector3 pos = CaffMath::Vector3Init(btPos.x(), btPos.y(), btPos.z());
			transform->setPosition(pos, false);
			
			const btQuaternion btRot = trans.getRotation();
			const CaffMath::Quaternion rot = CaffMath::QuaternionInit(btRot.w(), btRot.x(), btRot.y(), btRot.z());
			transform->setRotation(rot, false);
		}
	}
}



void PhysicsComponent::onLateThink(const float dt)
{
	//if(m_mass == 0)
	{
//		CaffComp::TransformComponent *transform = getOwner()->getComponent<TransformComponent>();
//		assert(transform);
//		
//		const CaffMath::Vector3 preserveScale   = transform->getScale();
//		const CaffMath::Vector3 normalizedScale = {1.f, 1.f, 1.f};
//		transform->setScale(normalizedScale);
//	
//		#ifdef CAFF_ON_NIX
//		std::array<float, 16> mat __attribute__ ((aligned (16)));
//		#elif defined(CAFF_ON_WINDOWS)
//		__declspec(align(16)) std::array<float, 16> mat;
//		#endif
//		
//		mat.at(15) = 1;
//		
//		const CaffMath::Matrix44 worldMat = transform->getWorldMatrix();
//		memcpy(&mat[0], &worldMat._11, sizeof(float) * 16);
//		
//		btTransform worldTrans;
//		worldTrans.setIdentity();
//		worldTrans.setFromOpenGLMatrix(&mat[0]);
//
//		m_rigidbody->setWorldTransform(worldTrans);
//		
//		btVector3 colliderScale(preserveScale.x, preserveScale.y, preserveScale.z);
//		btVector3 localScale = m_rigidbody->getCollisionShape()->getLocalScaling();
//		
//		//m_rigidbody->getCollisionShape()->setLocalScaling(btVector3(1,1,1));
//		m_rigidbody->getCollisionShape()->setLocalScaling(colliderScale);
//        
//		transform->setScale(preserveScale);
	}
}



void PhysicsComponent::onSuperLateThink(const float dt)
{
	//m_rigidbody->setActivationState(m_prevActivationState);
}



//void PhysicsComponent::setColliderShape(Collider &collider)
//{
//	m_colliderShapes.clear();
//	
//	// If collider is a compound collider then we reserve for that.
//	const std::size_t numberOfColliders = collider.numberOfColliders();
//	m_colliderShapes.reserve(numberOfColliders);
//	
//	m_colliderShapes.push_back(std::move(collider.getCollider()));
//
//	// If compound get the pointers of the children so we can delete them later.
//	if(numberOfColliders > 1)
//	{
//		btCompoundShape *compoundShape = static_cast<btCompoundShape *>(m_colliderShapes.front().get());
//
//		for(std::size_t i = 0; i < compoundShape->getNumChildShapes(); ++i)
//		{
//			std::unique_ptr<btCollisionShape> shape(compoundShape->getChildShape((int)i));
//			m_colliderShapes.push_back(std::move(shape));
//		}
//	}
//
//	updateRigidbody();
//}



void PhysicsComponent::setMass(const float mass)
{
	m_mass = mass;
	updateRigidbody();
}



void PhysicsComponent::setPositionAxis(const uint8_t axis)
{
	m_posAxis = axis;
	updateRigidbody();
}



void PhysicsComponent::setRotationAxis(const uint8_t axis)
{
	m_rotAxis = axis;
	updateRigidbody();
}



void PhysicsComponent::setTrigger(const bool isTrigger)
{
	m_isTrigger = isTrigger;
	updateRigidbody();
}



void PhysicsComponent::applyForce(const float *dir)
{
	if(m_rigidbody)
	{
		m_rigidbody->activate(true);
		m_rigidbody->applyCentralForce(btVector3(dir[0], dir[1], dir[2]));
	}
	
	//m_pImpl->rigidBody->applyTorque(btVector3(0, 0, 1000));
	//m_pImpl->rigidBody->applyTorque(btVector3(0, 0, 1));
	//m_pImpl->rigidBody->setAngularVelocity(btVector3(0, 50, 0));
}



void PhysicsComponent::applyLocalForce(const float *dir, const float *offset)
{
	btVector3 force(dir[0], dir[1], dir[2]);
	btVector3 wForce = m_rigidbody->getWorldTransform().getBasis() * force;
	
	btVector3 relOffset(offset[0], offset[1], offset[2]);
	btVector3 wOffset = m_rigidbody->getWorldTransform().getOrigin() + relOffset;
	
	m_rigidbody->activate(true);
	m_rigidbody->applyForce(wForce, wOffset);
}



void PhysicsComponent::applyLocalForce(const float *dir)
{
	btVector3 force(dir[0], dir[1], dir[2]);
	btVector3 wForce = m_rigidbody->getWorldTransform().getBasis() * force;
	
	m_rigidbody->activate(true);
	m_rigidbody->applyCentralForce(wForce);
}



void PhysicsComponent::applyTorque(const float *dir)
{
	m_rigidbody->activate(true);
	
	btVector3 torque(dir[0], dir[1], dir[2]);
	btVector3 wTorque = m_rigidbody->getWorldTransform().getBasis() * torque;
	
	//m_pImpl->rigidBody->applyTorque(btVector3(dir[0], dir[1], dir[2]));
	m_rigidbody->applyTorque(wTorque);
}



void PhysicsComponent::updateRigidbody()
{
    const CaffComp::TransformComponent *transform = getOwner().getComponent<TransformComponent>();
	
    assert(transform);      // Did you forget to add a transform or remove it?
	assert(m_physicsMgr);   // We need a physics world!

	if(m_rigidbody)
	{
		m_physicsMgr->removeRigidbody(m_rigidbody.get());
	}
	
    // Create Collider
    {
        // We make all colliders compound.
        btCompoundShape *compoundCollider = new btCompoundShape();
        
        const std::size_t COLLIDER_RESERVE = 10;
		m_colliderShapes.clear();
        m_colliderShapes.reserve(COLLIDER_RESERVE);
        m_colliderShapes.push_back(std::unique_ptr<btCollisionShape>(compoundCollider));
        
        // Search for colliders to add to the
        std::function<void(CaffEnt::Entity*, bool)> AddFreeCollider = [&](CaffEnt::Entity *entity, const bool ignoreRigidbody)
        {
            assert(entity);
            
            // If has rigidbody that isn't parent we ignore.
            if(entity->getComponent<PhysicsComponent>() && !ignoreRigidbody)
            {
                const bool isActive = entity->getComponent<PhysicsComponent>()->isActive();
                
                if(isActive)
                {
                    return;
                }
            }
            
            // Look for collider.
            ColliderComponent *collider = entity->getComponent<ColliderComponent>();
			//const std::string colliderName = getOwner()->getName() + "_" + CaffUtil::ToString(getOwner()->getID());
			//collider->setName(colliderName.c_str());
            
            if(collider)
            {
                // Requires a trasnform for offset.
                const CaffComp::TransformComponent *colliderTransform = entity->getComponent<TransformComponent>();
                assert(transform);      // Did you forget to add a transform or remove it?
                
                CaffMath::Vector3 scale  = colliderTransform->getScale();
                CaffMath::Vector3 offset = colliderTransform->getPosition();
                
                CaffComp::ModelComponent *model = entity->getComponent<ModelComponent>();

                if(model && model->getModelData())
                {
					const CaffMath::Vector3 transformScale = colliderTransform->getScale();
                    const CaffApp::AABB		bounds         = model->getModelData()->getBounds();
                    
                    scale.x = bounds.getSize().x;
                    scale.y = bounds.getSize().y;
                    scale.z = bounds.getSize().z;
                    
                    offset.x -= (bounds.getCenter().x * transformScale.x);
                    offset.y -= (bounds.getCenter().y * transformScale.y);
                    offset.z -= (bounds.getCenter().z * transformScale.z);
                }
                
                m_colliderShapes.push_back(collider->getColliderShape().getCollider(scale));
                
                const CaffMath::Vector3 pos = CaffMath::Vector3ComponentSubtract(colliderTransform->getPosition(), offset);
                
                const btQuaternion	rotation(0.f, 0.f, 0.f);
                const btVector3		position(pos.x, pos.y, pos.z);
                const btTransform	trans(rotation, position);
                
                compoundCollider->addChildShape(trans, m_colliderShapes.back().get());
            }
            
            // Look through child entiteies
            {
                for(std::size_t i = 0; i < entity->numberOfChildren(); ++i)
                {
                    AddFreeCollider(entity->findChild(i), false);
                }
            }
        };
        
		AddFreeCollider(&getOwner(), true);
    }
    
    // Create rigidbody.
    if(!m_colliderShapes.empty())
	{
		// Setup and Create rigidbody
		{
			const CaffMath::Vector3 pos = transform->getPosition();
			
			const btVector3		rPos(pos.x, pos.y, pos.z);
			const btQuaternion	rRot(0.f, 0.f, 0.f, 1.f);
			const btScalar		rMass(m_mass);
			
			m_motionState.reset(new btDefaultMotionState(btTransform(rRot, rPos)));
			
			m_colliderShapes.front()->calculateLocalInertia(rMass, m_rbodyInertia);
			
			const btRigidBody::btRigidBodyConstructionInfo rbodyCI(rMass, m_motionState.get(), m_colliderShapes.front().get(), m_rbodyInertia);
			
			m_rigidbody.reset(new btRigidBody(rbodyCI));
			
			m_rigidbody->setDamping(0.75f, 0.75f);
			m_rigidbody->setRestitution(0.05f);
			m_rigidbody->setFriction(0.05f);
			m_rigidbody->setUserPointer((void*)&getOwner());
		}
		
		// Constrain axis.
		{
		const btVector3 axisMovement((btScalar)(m_posAxis >> 0 & 1), (btScalar)(m_posAxis >> 1 & 1), (btScalar)(m_posAxis >> 2 & 1));
		const btVector3 axisRotation((btScalar)(m_rotAxis >> 0 & 1), (btScalar)(m_rotAxis >> 1 & 1), (btScalar)(m_rotAxis >> 2 & 1));
			
			m_rigidbody->setLinearFactor(axisMovement);
			m_rigidbody->setAngularFactor(axisRotation);
		}
		
		// Collision flags.
		{
			int flags = m_rigidbody->getCollisionFlags();
			
			flags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
			//flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
			
			if(m_isTrigger)
			{
				flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
			}
			if(!m_showDebug)
			{
				flags |= btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
			}
			
			m_rigidbody->setCollisionFlags(flags);
		}
		
		m_physicsMgr->addRigidbody(m_rigidbody.get());
	}
}



} // namespace
} // namespace
