
#include <Caffeine/Utilities/SimpleCam.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Caffeine {
namespace Utilities {

SimpleCam::SimpleCam(const unsigned int width, const unsigned int height, const float fov, const float nearPlane, const float farPlane)
: m_position(0,0,0)
, m_rotation(0,0,0)
, m_forward(0,0,0)
, m_up(0,0,0)
, m_right(0,0,0)
, m_projMat()
, m_viewMat()
{
	m_projMat = glm::perspective(fov, (float)width / (float)height, nearPlane, farPlane);
	
	// Build initial view matrix.
	updateDelta(glm::value_ptr(m_position), glm::value_ptr(m_rotation));
}


glm::mat4 SimpleCam::updateDelta(const float *positionVec3, const float *rotationVec3)
{
	// Rotation Axis
	glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	glm::vec3 zAxis(0.0f, 0.0f, 1.0f); // TODO: should this be -1?
	
	// Accumulate Rotations
	m_rotation.x += rotationVec3[0]; // pitch
	m_rotation.y += rotationVec3[1]; // yaw
	m_rotation.z += rotationVec3[2]; // roll

	// Calculate Rotation
	glm::mat4 rotViewMat;
	rotViewMat = glm::rotate(rotViewMat, m_rotation.x, xAxis);
	rotViewMat = glm::rotate(rotViewMat, m_rotation.y, yAxis);
	rotViewMat = glm::rotate(rotViewMat, m_rotation.z, zAxis);
	
	// Updated direction vectors
	m_forward = glm::vec3(rotViewMat[0][2], rotViewMat[1][2], rotViewMat[2][2]);
	m_up	  = glm::vec3(rotViewMat[0][1], rotViewMat[1][1], rotViewMat[2][1]);
	m_right	  = glm::vec3(rotViewMat[0][0], rotViewMat[1][0], rotViewMat[2][0]);
			
	m_forward = glm::normalize(m_forward);
	m_up	  = glm::normalize(m_up);
	m_right	  = glm::normalize(m_right);
			
	// Calculate Position
	m_position += (m_forward * positionVec3[2]);
	m_position += (m_up * positionVec3[1]);
	m_position += (m_right * positionVec3[0]);
	
	glm::mat4 translateViewMat;
	translateViewMat = glm::translate(translateViewMat, m_position);
	
	// Calculate view matrix.
	m_viewMat = rotViewMat * translateViewMat;
	
	// Return View Proj
	return getViewProjMat();
}


glm::mat4 SimpleCam::updateAbsolute(const float *positionVec3, const float *rotationVec3)
{
	m_position.x = positionVec3[0];
	m_position.y = positionVec3[1];
	m_position.z = positionVec3[2];
	
	m_rotation.x = rotationVec3[0];
	m_rotation.y = rotationVec3[1];
	m_rotation.z = rotationVec3[2];
	
	return SimpleCam::updateDelta(glm::value_ptr(m_position), glm::value_ptr(m_rotation));
}


} // namespace
} // namespace
