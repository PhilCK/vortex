
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/PhysicsDebugRenderer.hpp>
#include <Caffeine/Utilities/ParamNode.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/Event.hpp>


namespace Caffeine {
namespace Systems {


#pragma mark - Impl

struct PhysicsManager::Impl
{
	explicit Impl()
	: gravityVec(0, -10, 0)
	//World
	, broadphase(new btDbvtBroadphase)
	, collisionConfig(new btDefaultCollisionConfiguration)
	, dispatcher(new btCollisionDispatcher(collisionConfig))
	, solver(new btSequentialImpulseConstraintSolver)
	, dynamicsWorld(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig))
	, debugDraw()
	{
		dynamicsWorld->setGravity(btVector3(gravityVec.x, gravityVec.y, gravityVec.z));
		dynamicsWorld->setDebugDrawer(&debugDraw);
	}
	
	
	~Impl()
	{
		delete dynamicsWorld;
		delete solver;
		delete dispatcher;
		delete collisionConfig;
		delete broadphase;
	}
	
	
	void updateWorld(const float dt)
	{
		//for(int i = 0; i < 16; ++i)
		{
			dynamicsWorld->stepSimulation(dt, 40);
		}
		
		
			
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i=0;i<numManifolds;i++)
		{
			btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			
			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			for (int j=0;j<numContacts;j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f)
				{
					// Get set user pointers
					void* userPointerA = obA->getUserPointer();
					void* userPointerB = obB->getUserPointer();
					
					// TODO: Sort Collisions
					if(userPointerA)
					{
						CaffEnt::ColliderDetails collider = {static_cast<CaffEnt::Entity *>(userPointerB), 123};
						//collider.collidedWith = static_cast<CaffEnt::Entity *>(userPointerB);
					
						CaffEnt::Entity *obj = static_cast<CaffEnt::Entity *>(userPointerA);
						//obj->onEvent(collider);
						obj->onCollision(collider);
						
						//CaffEvent::EventData evt = CaffEvent::Collision(userPointerB);
						//obj->onEvent(evt);
					}
					
					if(userPointerB)
					{
						CaffEnt::ColliderDetails collider = {static_cast<CaffEnt::Entity *>(userPointerA), 456};
						//collider.collidedWith = static_cast<CaffEnt::Entity *>(userPointerA);
						
						CaffEnt::Entity *obj = static_cast<CaffEnt::Entity *>(userPointerB);
						//obj->onEvent(collider);
						obj->onCollision(collider);
						
						//CaffEvent::EventData evt = CaffEvent::Collision(userPointerA);
						//obj->onEvent(evt);
					}
					
					const btVector3& ptA		= pt.getPositionWorldOnA();
					const btVector3& ptB		= pt.getPositionWorldOnB();
					const btVector3& normalOnB	= pt.m_normalWorldOnB;
				}
			}
		}
	}
	
	
	// *** Variables *** //
	
	glm::vec3							gravityVec;
	
	btBroadphaseInterface				*broadphase;
	btDefaultCollisionConfiguration		*collisionConfig;
	btCollisionDispatcher				*dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld				*dynamicsWorld;
	
	CaffSys::PhysicsDebugRenderer		debugDraw;
	
}; // struct


#pragma mark - PhysicsManager

PhysicsManager::PhysicsManager()
: m_pImpl(new Impl)
{}


PhysicsManager::~PhysicsManager()
{}


void PhysicsManager::updateWorld(const float dt)
{
	m_pImpl->updateWorld(dt);
}


void PhysicsManager::renderDebug() const
{
	m_pImpl->dynamicsWorld->debugDrawWorld();
}


void PhysicsManager::setGravity(const float* gravityVec)
{
	m_pImpl->gravityVec = glm::vec3(gravityVec[0], gravityVec[1], gravityVec[2]);
	btVector3 gravity(gravityVec[0], gravityVec[1], gravityVec[2]);
	
	m_pImpl->dynamicsWorld->setGravity(gravity);
}


glm::vec3 PhysicsManager::getGravity() const
{
	return m_pImpl->gravityVec;
}


void PhysicsManager::addRigidbody(btRigidBody *rigidBody)
{
	m_pImpl->dynamicsWorld->addRigidBody(rigidBody);
}


void PhysicsManager::removeRigidbody(btRigidBody *rigidbody)
{
	m_pImpl->dynamicsWorld->removeRigidBody(rigidbody);
}


btDiscreteDynamicsWorld& PhysicsManager::getWorld()
{
	return *m_pImpl->dynamicsWorld;
}


} // namespace
} // namespace
