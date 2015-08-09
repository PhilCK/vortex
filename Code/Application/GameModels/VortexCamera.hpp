
#ifndef APP_VORTEX_CAMERA_INCLUDED
#define APP_VORTEX_CAMERA_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <vector>


class VortexCamera : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(VortexCamera)
    
private:
    
    
    // ** Engine Hooks ** //
    
    void                onParams(const CaffUtil::Param &params) override;
	void				onStart() override;
    void                onThink(const float dt) override;
	bool				onEvent(const CaffSys::Event &data) override;
	
	
private:
	
	
	CaffMath::Vector3	m_targetPosition = CaffMath::Vector3Init(0,0,0);
    CaffMath::Vector3   m_targetLookAt   = CaffMath::Vector3Init(0,0,-1.f);
	float				m_rateChange	 = 1.7f;
	float				m_cameraPlane    = 25.f;
	CaffSys::EventID	m_followEvent	 = 0;
	
	
}; // class


COMPONENT_FACTORY_INTERFACE(VortexCamera)


#endif // include guard
