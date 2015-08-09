
#ifndef CAFFEINE_SYSTEMS_CAMERA_INCLUDED
#define CAFFEINE_SYSTEMS_CAMERA_INCLUDED

#include <Caffeine/Platform.hpp>
#include <glm/glm.hpp>

namespace Caffeine {
namespace Systems {

// TODO: Make this component? and use transform component for posiiton etc?

class Camera
{
public:

	explicit Camera(const unsigned int width, const unsigned int height, const float fov, const float nearPlane = 0.1f, const float farPlane = 1000.0f);
	
	// Updates the view matrix.
	// position (vec3) is the distance traveled since the last frame along normalised axis.
	// rotation (vec3) is the rotation since the last frame.
	// - updateDelta() applies incremental changes to position and rotation.
	// - updateAbsolute() applies new values to position and rotation.
	glm::mat4 updateDelta(const float *positionVec3, const float *rotationVec3);
	glm::mat4 updateDeltaPosition(const float *positionVec3);
	glm::mat4 updateDeltaRotation(const float *rotationVec3);
	
	glm::mat4 updateAbsolute(const float *positionVec3, const float *rotationVec3);
	glm::mat4 updateAbsolutePosition(const float *positionVec3);
	glm::mat4 updateAbsoluteRotation(const float *rotationVec3);
	
	// Temp function
	glm::mat4 lookAt(glm::vec3 &eye, glm::vec3 &lookat, glm::vec3 &up);
	
	// Various matrices
	inline glm::mat4 getViewProjMat() const { return m_projMat * m_viewMat;	}
	inline glm::mat4 getViewMat() const		{ return m_viewMat;				}
	inline glm::mat4 getProjMat() const		{ return m_projMat;				}
	
	// Direction vectors
	inline glm::vec3 getForwardVec() const  { return m_forward;  }
	inline glm::vec3 getRightVec() const    { return m_right;	 }
	inline glm::vec3 getUpVec() const		{ return m_up;		 }
	
	inline glm::vec3 getPosition() const	{ return m_position; }
	
private:
	
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;
	
	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;

}; // class

} // namespace
} // namespace

#endif // include guard