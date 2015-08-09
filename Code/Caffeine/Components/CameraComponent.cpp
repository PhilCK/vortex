
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace
{
    CaffSys::CameraManager  *cameraManager(nullptr);
	CaffApp::Window			*window(nullptr);
}


namespace Caffeine {
namespace Components {


    
namespace CameraUtils {

    void SetCameraManager(CaffSys::CameraManager *camManager)	{ cameraManager = camManager; }
	void SetWindow(CaffApp::Window *setWindow)					{ window = setWindow; }
	
}

    

CameraComponent::CameraComponent()
: Entity::Component()
, m_projMat()
, m_viewMat()
, m_fov(60)
, m_nearPlane(1.0f)
, m_farPlane(1000.f)
, m_width(800) // TODO: default to window size.
, m_height(600) // TODO: default to window size.
{
    if(window)
    {
        m_width  = window->getWidth();
        m_height = window->getHeight();
    }
}



void CameraComponent::onParams(const CaffUtil::Param &params)
{
    m_fov           = params["fov"].asUInt32(m_fov);
    m_nearPlane     = params["near"].asFloat(m_nearPlane);
    m_farPlane      = params["far"].asFloat(m_farPlane);
    m_width         = params["width"].asUInt32(m_width);
	m_height		= params["height"].asUInt32(m_height);
    
	if(params["active"].asBool(true))
    {
        makeActiveCamera();
    }
}



void CameraComponent::makeActiveCamera()
{
	if(cameraManager)
	{
		cameraManager->setCurrentCamera(this);
	}
}



bool CameraComponent::isCameraActive() const
{
	if(cameraManager)
	{
		if(this == &cameraManager->getCurrentCamera())
		{
			return true;
		}
	}
	
	return false;
}



void CameraComponent::setViewPort(const uint32_t width, const uint32_t height)
{
	m_width  = width;
	m_height = height;
}



void CameraComponent::setViewPortToWindowResolution()
{
    assert(window); // Requires window to get width/height from.
    setViewPort(window->getWidth(), window->getHeight());
}



void CameraComponent::setNearPlane(const float near)
{
	const float minNear = 0.1f;
	m_nearPlane = CaffMath::Max(minNear, near);
}



void CameraComponent::setFarPlane(const float far)
{
    assert(far >= m_nearPlane); // Far plane is closer than the near plane.
	m_farPlane = far;
}

    
    
glm::mat4 CameraComponent::getViewMatrix() const
{
    const TransformComponent *transform = getOwner().getComponent<TransformComponent>();
    assert(transform);
    
	const glm::vec3 pos(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z);
	const glm::vec3 fwd(transform->getForwardVec().x, transform->getForwardVec().y, transform->getForwardVec().z);
	const glm::vec3 up(transform->getUpVec().x, transform->getUpVec().y, transform->getUpVec().z);
	const glm::vec3 target(0.f, 0.f, 0.f);
	
	m_viewMat = glm::lookAt(pos, pos + fwd, up);
	//m_viewMat = glm::lookAt(pos, target, up);
	
    return m_viewMat;
}
    
    

glm::mat4 CameraComponent::getProjMatrix() const
{
    m_projMat = glm::perspective((float)m_fov, (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
    
    return m_projMat;
}
    
    
    
glm::mat4 CameraComponent::getViewProjMatrix() const
{
    return getProjMatrix() * getViewMatrix();
}
    
    

glm::mat4 CameraComponent::getWorldMatrix() const
{
    const TransformComponent *transform = getOwner().getComponent<TransformComponent>();
    assert(transform);
    
    const CaffMath::Matrix44 world = transform->getWorldMatrix();
    
    glm::mat4 returnWorld;
    memcpy(&returnWorld[0][0], &world._11, sizeof(float) * 16);
    
    return returnWorld;
}


    
glm::vec3 CameraComponent::getLookAtVector() const
{
    const TransformComponent *transform = getOwner().getComponent<TransformComponent>();
    assert(transform);
    
    const CaffMath::Vector3 fwd = transform->getForwardVec();
    const glm::vec3 retFwd(fwd.x, fwd.y, fwd.z);
    
    return retFwd;
}
    
    
glm::mat4 CameraComponent::getOrthProjMatrix() const
{
    //m_projMat = glm::perspective((float)m_fov, (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
    glm::mat4 orth = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.f, 100.f);
    
    return orth;
}
    
    
glm::vec3 CameraComponent::getPosition() const
{
    const TransformComponent *transform = getOwner().getComponent<TransformComponent>();
    assert(transform);
    
    CaffMath::Vector3 pos = transform->getPosition();
    
    glm::vec3 retPos(pos.x, pos.y, pos.z);
    
    return retPos;
}
    

} // namespace
} // namespace
