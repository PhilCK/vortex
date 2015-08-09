
/*
	CameraManager.cpp
	-----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Utilities/ParamNode.hpp>

namespace Caffeine {
namespace Systems {

CameraManager::CameraManager()
: m_currentCamera(nullptr)
{
}


void CameraManager::setCurrentCamera(CaffComp::CameraComponent *camera)
{
	m_currentCamera = camera;
}

} // namespace
} // namespace
