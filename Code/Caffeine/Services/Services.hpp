
/*
	Services
	--------
	
	Project:
	Caffeine
	
	Purpose:
	Services are a stop gap to aid in development of Caffeine
	but not hold up game development to much.
	
	This serves as the public face of the engine from which components
	can define there behaviour.
	
	Currently the plan is to remove most of these in favour of full events.
	
	However while some of these things are in flux this provides a convenient
	alternitive to waiting to the event system to be fully functional.
*/

#ifndef CAFFEINE_SERVICES_INCLUDED
#define CAFFEINE_SERVICES_INCLUDED

#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>

namespace Caffeine {
namespace Services {


Systems::EventManager &		EventManager();

Systems::StateManager &		StateManager();

Systems::CameraManager &	CameraManager();

Systems::ModelManager &		ModelManager();

Systems::PhysicsManager &	PhysicsManager();

Systems::TextureManager &	TextureManager();

Systems::EntityFactory &	EntityFactory();

Application::Input &		InputManager();

Application::Window &		WindowManager();

Application::Renderer &		RendererManager();


void TimeManager();


// The non-private private setter, ** DO NOT USE THIS **

void SetApplication(Application::Application * app);


}
} // namespace

#endif // include guard
