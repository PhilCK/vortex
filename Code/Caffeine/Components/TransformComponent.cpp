
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <Caffeine/Utilities/Logging.hpp>



namespace
{
	// Default world axis.
	const CaffMath::Vector3 xAxis = { +1.f, +0.f, +0.f };
	const CaffMath::Vector3 yAxis = { +0.f, +1.f, +0.f };
	const CaffMath::Vector3 zAxis = { +0.f, +0.f, -1.f };
	
	// Root of the world.
	const CaffMath::Transform rootTransform = CaffMath::TransformInitialize({0,0,0}, {1,1,1});
}



namespace Caffeine {
namespace Components {



TransformComponent::TransformComponent()


: Entity::Component()
, m_transform()
{
}



void TransformComponent::onParams(const CaffUtil::Param &params)
{
	enum { X = 0, Y, Z };
    typedef std::array<float, 3> Float3;

    // Position
    {
        const Float3 atOrigin	= {{0.f, 0.f, 0.f}};
		const Float3 pos		= params["position"].asFloat3(atOrigin);
        m_transform.position	= CaffMath::Vector3Init(pos.at(X), pos.at(Y), pos.at(Z));
    }
    
    // Scale
    {
        const Float3 atOne		= {{1.f, 1.f, 1.f}};
        const Float3 scale		= params["scale"].asFloat3(atOne);
        m_transform.scale		= CaffMath::Vector3Init(scale.at(X), scale.at(Y), scale.at(Z));
    }
	
	// Rotation
	{
		const Float3 zeroRot	= {{0.f, 0.f, 0.f}};
		const Float3 rot		= params["rotation_degrees"].asFloat3(zeroRot);
		m_transform.rotation	= CaffMath::QuaternionInitFromEulerAngles(rot.at(X), rot.at(Y), rot.at(Z));
	}
}



void TransformComponent::onAttach()
{
	// TODO: Adjust transform to fit parent.
}



void TransformComponent::onDisattach()
{
	// TODO: Adjust transform to fit root.
}



#pragma mark - Directional Vectors

CaffMath::Vector3 TransformComponent::getForwardVec() const
{
	return CaffMath::QuaternionRotate(getRotation(), zAxis);
}



CaffMath::Vector3 TransformComponent::getUpVec() const
{
	return CaffMath::QuaternionRotate(getRotation(), yAxis);
}



CaffMath::Vector3 TransformComponent::getRightVec() const
{
	return CaffMath::QuaternionRotate(getRotation(), xAxis);
}



#pragma mark - Rotation

void TransformComponent::setRotation(const CaffMath::Quaternion &rotation, const bool updatePhys)
{
    if(m_transform.rotation.w == rotation.w
       && m_transform.rotation.x == rotation.x
       && m_transform.rotation.y == rotation.y
       && m_transform.rotation.z == rotation.z)
    {
        return;
    }
    
    m_transform.rotation = rotation;
	
	// Need to set the children as well.
	for(std::size_t i = 0; i < getOwner().numberOfChildren(); ++i)
	{
		TransformComponent *childTransform = getOwner().findChild(i)->getComponent<TransformComponent>();
		
		// Move the children the same amount.
		if(childTransform)
		{
			const CaffMath::Quaternion childRot   = childTransform->getRotation();
			const CaffMath::Quaternion updatedRot = CaffMath::QuaternionMultiply(childRot, rotation);
			const CaffMath::Quaternion normRot    = CaffMath::QuaternionNormalize(updatedRot);
			
			childTransform->setRotation(normRot, updatePhys);
		}
		else
		{
			CaffUtil::LogError("TransformComponent: Found a child entity with no transform component!");
		}
	}
	
	// Physics
    if(updatePhys)
    {
        PhysicsComponent *phys = getOwner().getComponent<PhysicsComponent>();
        
        if(phys)
        {
            phys->setTransform(m_transform, true, false);
        }
    }
	
	
}


#pragma mark - Position

CaffMath::Vector3 TransformComponent::getLocalPosition() const
{
	if(getOwner().getParent())
	{
		// localPosition is based offset from parent.
		const TransformComponent *parentTransform = getOwner().getParent()->getComponent<TransformComponent>();
		
		if(parentTransform)
		{
			const CaffMath::Vector3 parentWorld = parentTransform->getPosition();
			const CaffMath::Vector3 childWorld  = getPosition();
			const CaffMath::Vector3 localOffset = CaffMath::Vector3ComponentSubtract(parentWorld, childWorld);
			
			return localOffset;
		}
		else
		{
			CaffUtil::LogError("TransformComponent: Found a parent entity with no transform component!");
		}
	}
	
	// Consider it local to the root of the world. So just return position.
	return m_transform.position;
}



void TransformComponent::setPosition(const CaffMath::Vector3 &worldPos, const bool setPhys)
{
	const CaffMath::Vector3 difference = CaffMath::Vector3ComponentSubtract(worldPos, m_transform.position);
	
	m_transform.position = worldPos;
	
	// Need to set the children as well.
	for(std::size_t i = 0; i < getOwner().numberOfChildren(); ++i)
	{
		TransformComponent *childTransform = getOwner().findChild(i)->getComponent<TransformComponent>();
		
		// Move the children the same amount.
		if(childTransform)
		{
			const CaffMath::Vector3 childPos   = childTransform->getPosition();
			const CaffMath::Vector3 updatedPos = CaffMath::Vector3ComponentAdd(childPos, difference);
			
			childTransform->setPosition(updatedPos);
		}
		else
		{
			CaffUtil::LogError("TransformComponent: Found a child entity with no transform component!");
		}
	}
    
    // update rigidbody
    if(setPhys)
    {
        PhysicsComponent *phys = getOwner().getComponent<PhysicsComponent>();
        
        if(phys)
        {
            phys->setTransform(m_transform, false, true);
        }
    }
}



#pragma mark - Scale

CaffMath::Vector3 TransformComponent::getLocalScale() const
{
	if(getOwner().getParent())
	{
		// localPosition is based offset from parent.
		const TransformComponent *parentTransform = getOwner().getParent()->getComponent<TransformComponent>();
		
		if(parentTransform)
		{
			const CaffMath::Vector3 parentScale = parentTransform->getScale();
			const CaffMath::Vector3 childScale  = getScale();
			const CaffMath::Vector3 localScale  = CaffMath::Vector3ComponentSubtract(parentScale, childScale);
			
			return localScale;
		}
		else
		{
			CaffUtil::LogError("TransformComponent: Found a parent entity with no transform component!");
		}
	}
	
	// Consider it local to the root of the world. So just return scale.
	return m_transform.scale;
}



// TODO: This a) wont work its c/p of position, and b) needs to update the position of the child as well.
void TransformComponent::setScale(const CaffMath::Vector3 &scale)
{
	const CaffMath::Vector3 difference = CaffMath::Vector3ComponentSubtract(m_transform.scale, scale);
	
	m_transform.scale = scale;
	
	// Need to set the children as well.
	for(std::size_t i = 0; i < getOwner().numberOfChildren(); ++i)
	{
		TransformComponent *childTransform = getOwner().findChild(i)->getComponent<TransformComponent>();
		
		// Move the children the same amount.
		if(childTransform)
		{
			const CaffMath::Vector3 childScale   = childTransform->getScale();
			const CaffMath::Vector3 updatedScale = CaffMath::Vector3ComponentAdd(childScale, difference);
			
			childTransform->setScale(updatedScale);
		}
		else
		{
			CaffUtil::LogError("TransformComponent: Found a child entity with no transform component!");
		}
	}
}



#pragma mark - Matrices


CaffMath::Matrix44 TransformComponent::getWorldMatrix() const
{
	// TODO: Scale.

    CaffMath::Matrix44 worldMat;
	{
	    const CaffMath::Matrix33 rotMat		= CaffMath::QuaternionToRotationMatrix(m_transform.rotation);
		const CaffMath::Matrix33 scaleMat	= CaffMath::Matrix33InitScaleMatrix(m_transform.scale);
		const CaffMath::Matrix33 basis		= CaffMath::Matrix33Multiply(rotMat, scaleMat);
		const CaffMath::Vector3  pos		= m_transform.position;
	
		worldMat._11 = basis._11;
		worldMat._12 = basis._12;
		worldMat._13 = basis._13;
		worldMat._14 = 0.f;
		
		worldMat._21 = basis._21;
		worldMat._22 = basis._22;
		worldMat._23 = basis._23;
		worldMat._24 = 0.f;
		
		worldMat._31 = basis._31;
		worldMat._32 = basis._32;
		worldMat._33 = basis._33;
		worldMat._34 = 0.f;
		
		worldMat._41 = pos.x;
		worldMat._42 = pos.y;
		worldMat._43 = pos.z;
		worldMat._44 = 1;
	}
    
    return worldMat;
}
    
    
    
void TransformComponent::setWorldMatrix(const CaffMath::Matrix44 &setWorld)
{
	// Position
    m_transform.position = CaffMath::Vector3Init(setWorld._41, setWorld._42, setWorld._43);
}


} // namespace
} // namespace
