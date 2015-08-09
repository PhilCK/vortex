
#ifndef CAFFEINE_UTILITIES_SIMPLE_CAMERA_INCLUDED
#define CAFFEINE_UTILITIES_SIMPLE_CAMERA_INCLUDED

#include <glm/glm.hpp>


namespace Caffeine {
namespace Utilities {

// Simple Camera class.
// updateDelta good for FPS style camera
// updateAbsolute good for other stuff

class SimpleCam
{
public:

	explicit SimpleCam(const unsigned int width, const unsigned int height, const float fov, const float nearPlane = 0.1f, const float farPlane = 100.0f);
	
	// Updates the view matrix.
	// position (vec3) is the distance traveled since the last frame along normalised axis.
	// rotation (vec3) is the rotation since the last frame.
	// - updateDelta() applies incremental changes to position and rotation.
	// - updateAbsolute() applies new values to position and rotation.
	glm::mat4 updateDelta(const float *positionVec3, const float *rotationVec3);
	glm::mat4 updateAbsolute(const float *positionVec3, const float *rotationVec3);
	
	// Various matrices
	inline glm::mat4 getViewProjMat() const { return m_projMat * m_viewMat;	}
	inline glm::mat4 getViewMat() const		{ return m_viewMat;				}
	inline glm::mat4 getProjMat() const		{ return m_projMat;				}

	// Direction vectors
	inline glm::vec3 getForwardVec() const  { return m_forward;  }
	inline glm::vec3 getRightVec() const    { return m_right;	 }
	inline glm::vec3 getUpVec() const		{ return m_up;		 }
	
private:
	
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;
	
	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;
};

} // namespace
} // namespace


#endif