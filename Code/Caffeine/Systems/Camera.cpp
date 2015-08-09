
#include <Caffeine/Systems/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Caffeine/Math/Math.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace Caffeine {
namespace Systems {

Camera::Camera(const unsigned int width, const unsigned int height, const float fov, const float nearPlane, const float farPlane)
: m_position(0.f,0.f,0.f)
, m_rotation(0.f,0.f,0.f)
, m_forward(0.f,0.f,0.f)
, m_up(0.f,0.f,0.f)
, m_right(0.f,0.f,0.f)
, m_projMat()
, m_viewMat()
{
	m_projMat = glm::perspective(fov, (float)width / (float)height, nearPlane, farPlane);
	
	// Build initial view matrix.
	updateDelta(glm::value_ptr(m_position), glm::value_ptr(m_rotation));
}



//glm::mat4 Camera::updateDelta(const float *positionVec3, const float *rotationVec3)
//{
//	const glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
//	const glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
//	const glm::vec3 zAxis(0.0f, 0.0f, -1.0f); // TODO: should this be -1?
//	
//	// Accumulate Rotations
//	m_rotation.x += -rotationVec3[0]; // pitch
//	m_rotation.y += -rotationVec3[1]; // yaw
//	m_rotation.z += -rotationVec3[2]; // roll
//
//	glm::mat4 rotViewMat = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
//	
//	m_forward	= glm::vec3(rotViewMat * glm::vec4(zAxis, 0));
//	m_up		= glm::vec3(rotViewMat * glm::vec4(yAxis, 0));
//	m_right		= glm::vec3(rotViewMat * glm::vec4(xAxis, 0));
//	
//	m_forward   = glm::normalize(m_forward);
//	m_up		= glm::normalize(m_up);
//	m_right	    = glm::normalize(m_right);
//	
//	m_forward = glm::normalize(m_forward);
//	
//	m_position += (m_forward * positionVec3[2]);
//	m_position += (m_up * positionVec3[1]);
//	m_position += (m_right * positionVec3[0]);
//	
//	m_viewMat = glm::lookAt(m_position, m_position + m_forward, m_up);
//	
//	// Return View Proj
//	return getViewProjMat();
//}



glm::mat4 Camera::updateDeltaPosition(const float *positionVec3)
{
	std::array<float, 3> zero = {{0.f, 0.f, 0.f}};
	return updateDelta(positionVec3, &zero[0]);
}



glm::mat4 Camera::updateDeltaRotation(const float *rotationVec3)
{
	std::array<float, 3> zero = {{0.f, 0.f, 0.f}};
	return updateDelta(&zero[0], rotationVec3);
}



glm::mat4 Camera::lookAt(glm::vec3 &eye, glm::vec3 &lookat, glm::vec3 &up)
{
	m_viewMat = glm::lookAt(eye, lookat, up);
	
	return getViewProjMat();
}



glm::mat4 Camera::updateAbsolute(const float *positionVec3, const float *rotationVec3)
{

	memcpy(&m_position[0], positionVec3, sizeof(float) * 3);
	memcpy(&m_rotation[0], rotationVec3, sizeof(float) * 3);

	/*
	m_position.x = positionVec3[0];
	m_position.y = positionVec3[1];
	m_position.z = positionVec3[2];
	
	m_rotation.x = rotationVec3[0];
	m_rotation.y = rotationVec3[1];
	m_rotation.z = rotationVec3[2];
	*/
	
	const glm::vec3 zeroVec(0,0,0);
	
	return Camera::updateDelta(glm::value_ptr(zeroVec), glm::value_ptr(zeroVec));
}



glm::mat4 Camera::updateAbsolutePosition(const float *positionVec3)
{
	return updateAbsolute(positionVec3, &m_rotation[0]);
}



glm::mat4 Camera::updateAbsoluteRotation(const float *rotationVec3)
{
	return updateAbsolute(&m_position[0], rotationVec3);
}


} // namespace
} // namespace
