
/*
	Platform.hpp
	------------
	
	Project:
	Caffeine
	
	Purpose:
	Lazy includes for lazy times.
	
*/

#ifndef CAFFEINE_LAZY_INCLUDES_INCLUDED
#define CAFFEINE_LAZY_INCLUDES_INCLUDED

// TODO: Update this list.

#include <Caffeine/Platform.hpp>
#include <Caffeine/Version.hpp>

// Application
#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Window.hpp>

// Components
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>

// Entity
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Entity/Entity.hpp>

// Math
#include <Caffeine/Math/Math.hpp>

// Services
#include <Caffeine/Services/Services.hpp>

// Systems
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/PhysicsDebugRenderer.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/TextureManager.hpp>

// Utils
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Utilities/SimpleCam.hpp>
#include <Caffeine/Utilities/Timer.hpp>


#endif // Include guard
