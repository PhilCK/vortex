
/*
	CameraManager.hpp
	-----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef CAFFEINE_SYSTEMS_CAMERA_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_CAMERA_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>

namespace Caffeine {
namespace Systems {


class CameraManager : private CaffUtil::NonCopyable
{
public:

	explicit							CameraManager();

	inline CaffComp::CameraComponent &	getCurrentCamera() const { return *m_currentCamera; }
	void								setCurrentCamera(CaffComp::CameraComponent * camera);
	
private:

	CaffComp::CameraComponent *			m_currentCamera;

}; // class


} // namespace
} // namespace

#endif
