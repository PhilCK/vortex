

#ifndef CAFFEINE_COMPONENTS_COLLIDER_INCLUDED
#define CAFFEINE_COMPONENTS_COLLIDER_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>

#include <btBulletDynamicsCommon.h>


namespace Caffeine {
namespace Components {

#pragma mark - Colliders


// Collider base class.
class Collider
{
protected:

public:
	
	explicit Collider();
	virtual ~Collider() {}
	
	virtual std::unique_ptr<btCollisionShape>	getCollider(const CaffMath::Vector3 &scale)				{ return nullptr;	};
	virtual bool								hasCollider() const			{ return false;		}
	virtual std::size_t							numberOfColliders() const	{ return 0;			}
	virtual Collider*							getClone() const = 0;
};



// Collider box can has different x,y and z.
class BoxCollider : public Collider
{
	std::array<float, 3> m_extents;

public:
	// TODO: Decide what our vector solution is going to be.
	explicit BoxCollider(const float xExtents, const float yExtents, const float zExtents);
	
	std::unique_ptr<btCollisionShape>	getCollider(const CaffMath::Vector3 &scale) override;
	bool								hasCollider() const override		{ return true; }
	std::size_t							numberOfColliders() const override	{ return 1; }
	Collider*							getClone() const override			{ return new BoxCollider(*this); }
};



// Collider cube is a box with equal sides.
class CubeCollider : public BoxCollider
{
public:

	explicit							CubeCollider(const float extents);
	Collider*							getClone() const override			{ return new CubeCollider(*this); }
};



// Static plane shape.
class StaticPlaneCollider : public Collider
{
	std::array<float, 3> m_normal;
	float m_offset;

public:
	// TODO: Decide what our vector solution is going to be.
	explicit StaticPlaneCollider(const float xNormal, const float yNormal, const float zNormal, const float offset);
	
	std::unique_ptr<btCollisionShape>	getCollider(const CaffMath::Vector3 &scale) override;
	bool								hasCollider() const override		{ return true; }
	std::size_t							numberOfColliders() const override	{ return 1; }
	Collider*							getClone() const override			{ return new StaticPlaneCollider(*this); }
};



// Compound collider can be made up from other colliders.
class CompoundCollider : public Collider
{
	struct ColliderDetails
	{
		glm::vec3 rot;
		glm::vec3 pos;
		std::shared_ptr<Collider> coll;
		
		ColliderDetails(const glm::vec3 &offsetPos, const glm::vec3 &offsetRot, std::shared_ptr<Collider> collider)
		: rot(offsetRot)
		, pos(offsetPos)
		, coll(collider)
		{
		}
	
		ColliderDetails(const ColliderDetails& o)
		: coll(o.coll)
		{
			rot		 = o.rot;
			pos		 = o.pos;
			//coll	 = std::move(o.coll);
		}
		
	};
	
	//std::vector<std::shared_ptr<ColliderDetails> > m_shapes; // TODO: make unique again, shared_ptr to solve the headache of implicitly deleted copy-ctor.
	std::vector<ColliderDetails> m_shapes;

public:

	explicit CompoundCollider();
				~CompoundCollider() {}
			 CompoundCollider(const CompoundCollider& o);

	void addCollider(const glm::vec3 &offsetPos, const glm::vec3 &offsetRot, Collider &collider);
	
	std::unique_ptr<btCollisionShape>	getCollider(const CaffMath::Vector3 &scale) override;
	bool								hasCollider() const override		{ return true; }
	std::size_t							numberOfColliders() const override	{ return m_shapes.size(); }
	Collider*							getClone() const override			{ return new CompoundCollider(*this); }
};

#pragma mark - Collider Component


class ColliderComponent : public CaffEnt::Component
{
public:
    
    COMPONENT_RTTI(Collider)
    
    explicit                        ColliderComponent();
    
	void							onParams(const CaffUtil::Param &params) override;
	
    void                            setColliderShape(Collider &collider);
    Collider&                       getColliderShape() const;
    
private:

	std::unique_ptr<Collider>		m_collider;
    
}; // class


} // namespace
} // namespace


#endif // include guard
