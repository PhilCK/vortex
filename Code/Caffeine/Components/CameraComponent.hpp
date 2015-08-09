
/*
	Camera Component
	----------------
	
	Project:
	Caffeine
		
	Camera component.
	
	Currently this is just maksing CaffSys::Camera as a component.
	Currently each time you change a setting the object gets rebuilt.
	This can cause some issues and requires another pass but waiting for
	FBO support and the CameraManager to be updated first.
*/

#ifndef CAFFEINE_COMPONENTS_CAMERA_INCLUDED
#define CAFFEINE_COMPONENTS_CAMERA_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <glm/glm.hpp>


namespace Caffeine {
namespace Components {



namespace CameraUtils {

    void SetCameraManager(CaffSys::CameraManager *camManager);
	void SetWindow(CaffApp::Window *setWindow);

}



class CameraComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(CameraComponent)
	
	explicit							CameraComponent();
	
	void								onParams(const CaffUtil::Param &params) override;
	
	void								makeActiveCamera();
	bool								isCameraActive() const;
	
	inline void							setFOV(const uint32_t fovDegrees)   { m_fov = CaffMath::Clamp<uint32_t>(fovDegrees, 0, 360); }
	inline uint32_t						getFOV() const                      { return m_fov; }
	
	void								setViewPort(const uint32_t width, const uint32_t height);
	void								setViewPortToWindowResolution();
	inline uint32_t						getViewPortWidth() const  { return m_width;  }
	inline uint32_t						getViewPortHeight() const { return m_height; }
	
	void								setNearPlane(const float near);
	inline float						getNearPlane() const { return m_nearPlane; }
	
	void								setFarPlane(const float far);
	inline float						getFarPlane() const { return m_farPlane; }
	
	glm::mat4                           getViewMatrix() const;
    glm::mat4                           getProjMatrix() const;
	glm::mat4                           getViewProjMatrix() const;
    glm::mat4                           getWorldMatrix() const;
    glm::vec3                           getLookAtVector() const;
    glm::mat4                           getOrthProjMatrix() const;
    glm::vec3                           getPosition() const;
	
private:
	
	// New Camera
	mutable glm::mat4					m_projMat; // TODO: Want to calc this once per frame.
	mutable glm::mat4					m_viewMat; // TODO: Want to calc this once per frame.
	
	uint32_t							m_fov;
	float								m_nearPlane;
	float								m_farPlane;
	uint32_t							m_width;
	uint32_t							m_height;

}; // class


} // namespace
} // namespace


#endif // include guard
