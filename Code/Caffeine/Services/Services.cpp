
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>


namespace
{
	Caffeine::Application::Application *caffApp;
}


namespace Caffeine {
namespace Services {

//
//Systems::EventManager &		EventManager()		{ return caffApp->getEventManager();	}
//
//Systems::StateManager &		StateManager()		{ return caffApp->getStateManager();	}
//
//Systems::CameraManager &	CameraManager()		{ return caffApp->getCameraManager();	}
//
//Systems::ModelManager &		ModelManager()		{ return caffApp->getModelManager();	}
//
//Systems::PhysicsManager &	PhysicsManager()	{ return caffApp->getPhysicsManager();  }
//
//Systems::TextureManager &	TextureManager()	{ return caffApp->getTextureManager();  }
//
//Systems::EntityFactory &	EntityFactory()		{ return caffApp->getEntityFactory();	}

Application::Input &		InputManager()		{ return caffApp->getInput();			}

Application::Window &		WindowManager()		{ return caffApp->getWindow();			}

Application::Renderer &		RendererManager()	{ return caffApp->getRenderer();		}


// !!!DO NOT USE!!! //
// FIXME: This!!!!
void SetApplication(Application::Application * app)
{
	if(!caffApp) {
		caffApp = app;
	}
}

} // namespace
} // namespace