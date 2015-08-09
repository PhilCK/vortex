
#include <Caffeine/Components/ColliderComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/ModelComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>


namespace Caffeine {
namespace Components {


#pragma mark - Colliders

Collider::Collider()
{}


BoxCollider::BoxCollider(const float xExtents, const float yExtnets, const float zExtents)
: Collider()
, m_extents()
{
	enum { X = 0, Y, Z};

	m_extents.at(X) = xExtents;
	m_extents.at(Y) = yExtnets;
	m_extents.at(Z) = zExtents;
}


std::unique_ptr<btCollisionShape> BoxCollider::getCollider(const CaffMath::Vector3 &scale)
{
	enum { X = 0, Y, Z};

	//const btVector3 size(m_extents.at(X), m_extents.at(Y), m_extents.at(Z));
    const btVector3 size(0.5f, 0.5f, 0.5f);
	std::unique_ptr<btCollisionShape> shape(new btBoxShape(size));
	
	const btVector3 extentsScale(m_extents.at(X) * 2.f, m_extents.at(Y) * 2.f, m_extents.at(Z) * 2.f);
	const btVector3 colliderScale(extentsScale.x() * scale.x, extentsScale.y() * scale.y, extentsScale.z() * scale.z);
	
	shape->setLocalScaling(colliderScale);
	
	return shape;
}


CubeCollider::CubeCollider(const float extents)
: BoxCollider(extents, extents, extents)
{
}


StaticPlaneCollider::StaticPlaneCollider(const float xNormal, const float yNormal, const float zNormal, const float offset)
: Collider()
, m_normal()
, m_offset(offset)
{
	enum { X = 0, Y, Z};
	
	m_normal.at(X) = xNormal;
	m_normal.at(Y) = yNormal;
	m_normal.at(Z) = zNormal;
}


std::unique_ptr<btCollisionShape> StaticPlaneCollider::getCollider(const CaffMath::Vector3 &scale)
{
	enum { X = 0, Y, Z};
	
	const btVector3 normal(m_normal.at(X), m_normal.at(Y), m_normal.at(Z));
	std::unique_ptr<btCollisionShape> shape(new btStaticPlaneShape(normal, m_offset));
	
	return shape;
}



CompoundCollider::CompoundCollider()
: Collider()
, m_shapes()
{
	const uint32_t SHAPE_VEC_HINT = 4;
	m_shapes.reserve(SHAPE_VEC_HINT);
}


CompoundCollider::CompoundCollider(const CompoundCollider &o)
{
	m_shapes = o.m_shapes;
}


void CompoundCollider::addCollider(const glm::vec3 &offsetPos, const glm::vec3 &offsetRot, Collider &collider)
{
	//std::unique_ptr<ColliderDetails> detail(new ColliderDetails(offsetPos, offsetRot, collider));
	//std::shared_ptr<ColliderDetails> detail(new ColliderDetails(offsetPos, offsetRot, collider));
	std::shared_ptr<Collider> hCollider(collider.getClone());
	
	ColliderDetails detail(offsetPos, offsetRot, hCollider);
	
	//m_shapes.push_back(std::move(detail));
	m_shapes.push_back(detail);
}


std::unique_ptr<btCollisionShape> CompoundCollider::getCollider(const CaffMath::Vector3 &scale)
{
	std::unique_ptr<btCollisionShape> compoundShape(new btCompoundShape());
	
	for(auto &detail : m_shapes)
	{
		std::unique_ptr<btCollisionShape> shape = detail.coll->getCollider(scale);
		assert(shape.get()); // didn't create a shape.
		
		const btQuaternion	rotation(detail.rot.x, detail.rot.y, detail.rot.z);
		const btVector3		position(detail.pos.x, detail.pos.y, detail.pos.z);
		const btTransform	trans(rotation, position);
		
		static_cast<btCompoundShape*>(compoundShape.get())->addChildShape(trans, shape.get());
		
		shape.release();
	}
	
	return compoundShape;
}
    
#pragma mark - Collider Component  
    
ColliderComponent::ColliderComponent()
: CaffEnt::Component()
, m_collider(new CubeCollider(0.5f))
{
}



void ColliderComponent::onParams(const CaffUtil::Param &params)
{
	// TODO: Add support for multiplie types (ie multiplie types in param).
	const std::string colliderType = CaffUtil::ToLower(params["type"]["name"].asStdString(""));
	
	if(colliderType == "box")
	{
		const std::array<float, 3> defaultExtents = {{0.5f, 0.5f, 0.5f}};
		const std::array<float, 3> extents = params["type"]["extents"].asFloat3(defaultExtents);
		
		enum { X = 0, Y, Z };
		
		m_collider.reset(BoxCollider(extents.at(X), extents.at(Y), extents.at(Z)).getClone());
	}
	else if(colliderType == "cube")
	{
		const float extents = params["type"]["extents"].asFloat(1.f);
		
		m_collider.reset(CubeCollider(extents).getClone());
	}
	else if(colliderType == "static_plane")
	{
		const float defaultOffset = 0.f;
		const float offset = params["type"]["offset"].asFloat(defaultOffset);
		
		const std::array<float, 3> defaultNormal = {{0.f, 1.f, 0.f}};
		const std::array<float, 3> normal = params["type"]["normal"].asFloat3(defaultNormal);
		
		enum { X = 0, Y, Z };
		
		m_collider.reset(StaticPlaneCollider(normal.at(X), normal.at(Y), normal.at(Z), offset).getClone());
	}
}



void ColliderComponent::setColliderShape(Collider &collider)
{
    m_collider.reset(collider.getClone());
}



Collider& ColliderComponent::getColliderShape() const
{
	assert(m_collider.get());
    return *m_collider.get();
}
   

    
} // namespace
} // namespace
