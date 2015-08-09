
#ifndef CAFFEINE_SYSTEMS_CAFFEINE_MAIN_INCLUDED
#define CAFFEINE_SYSTEMS_CAFFEINE_MAIN_INCLUDED



#include <Caffeine/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <string>
#include <map>
#include <array>
#include <memory>



namespace Caffeine {
namespace Systems {



class CaffeineMain final : private CaffUtil::NonCopyable
{
public:

	explicit						CaffeineMain();
									~CaffeineMain();
	
	// You should register prior to starting the applicaton.
	void							registerComponent(const std::string & factoryName, ComponentFactory *factory);

	// Start the application.
	void							start();
	
private:

	void							onButtonUpCallback(const CaffApp::KeyID key);
	void							onButtonDownCallback(const CaffApp::KeyID key);
	void							onTextStreamCallback(const std::string &text);

private:

	std::unique_ptr<CaffApp::Application>		m_caffApp;
	std::unique_ptr<CaffSys::CameraManager>		m_cameraMgr;
	std::unique_ptr<CaffSys::EventManager>		m_eventMgr;
	std::unique_ptr<CaffSys::ModelManager>		m_modelMgr;
	std::unique_ptr<CaffSys::PhysicsManager>	m_physicsMgr;
	std::unique_ptr<CaffSys::StateManager>		m_stateMgr;
	std::unique_ptr<CaffSys::TextureManager>	m_textureMgr;
	std::unique_ptr<CaffSys::EntityFactory>		m_entityMgr;
	std::unique_ptr<CaffSys::TextRenderer>		m_textRenderer;

}; // class



} // namespace
} // namespace



#endif // include guard
