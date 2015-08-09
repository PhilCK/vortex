
#ifndef CAFFEINE_COMPONENTS_TRANSFORM_INCLUDED
#define CAFFEINE_COMPONENTS_TRANSFORM_INCLUDED



#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Math/Transform.hpp>



namespace Caffeine {
namespace Components {



class TransformComponent final : public Entity::Component
{
public:



	COMPONENT_RTTI(Transform)

	explicit					TransformComponent();



	#pragma mark - Engine Hooks
	
	void						onParams(const CaffUtil::Param &params) override;
	void						onAttach() override;
	void						onDisattach() override;
    
    #ifndef CAFF_DEBUG // This is only used as a way to break point into transforms during debug.
    void                        onThink(const float dt) override
    {
        
    }
    #endif
	
	
	
	#pragma mark - Directional Vectors

	CaffMath::Vector3			getForwardVec() const;
	CaffMath::Vector3			getUpVec() const;
	CaffMath::Vector3			getRightVec() const;
	


	#pragma mark - Rotation

	inline CaffMath::Quaternion	getRotation() const                                                         { return m_transform.rotation;                                                      }
	CaffMath::Quaternion		getLocalRotation() const;
	inline CaffMath::Vector3	getRotationAsEuler() const                                                  { return CaffMath::QuaternionGetEulerAnglesInDegrees(m_transform.rotation);         }
	CaffMath::Vector3			getLocalRotationAsEuler() const;
	void                        setRotation(const CaffMath::Quaternion &rotation,  const bool updatePhys = true);
	inline void					setRotationWithEuler(const float xDeg, const float yDeg, const float zDeg)  { m_transform.rotation = CaffMath::QuaternionInitFromEulerAngles(xDeg, yDeg, zDeg); }
	
	
	
	#pragma mark - Position
	
	inline CaffMath::Vector3	getPosition() const { return m_transform.position; }
	CaffMath::Vector3			getLocalPosition() const;
	void						setPosition(const CaffMath::Vector3 &worldPos, const bool setPhys = true);
	
	
	
	#pragma mark - Scale
	
	inline CaffMath::Vector3	getScale() const { return m_transform.scale; }
	CaffMath::Vector3			getLocalScale() const;
	void						setScale(const CaffMath::Vector3 &scale);



	#pragma mark - Matrices
	
	CaffMath::Matrix33			getRotationMatrix() const;
	CaffMath::Matrix33			getLocalRotationMatrix() const;
	CaffMath::Matrix33			getScaleMatrix() const;
	CaffMath::Matrix44			getTranslationMatrix() const;
	CaffMath::Matrix44			getWorldMatrix() const;
	void						setWorldMatrix(const CaffMath::Matrix44 &setWorld);



private:


	
    CaffMath::Transform m_transform;

	
	
}; // class



} // namespace
} // namespace



#endif // include guard
