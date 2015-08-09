
/*
	SystemFwd.hpp
	-------------
	
	Project: Caffeine
*/

#ifndef CAFFEINE_SYSTEM_FWD_INCLUDED
#define CAFFEINE_SYSTEM_FWD_INCLUDED

#include <functional>

/*
	System forward declerations.
*/
namespace Caffeine {
namespace Systems {

class StateManager;
class State;
class EventManager;
class Eventable;
class Event;
class CameraManager;
class Camera;
class PhysicsManager;
class ModelManager;
struct ModelData;
class TextureManager;
class TextRenderer;
class ShaderManager;
class EntityFactory;
class ComponentFactory;
class ScriptManager;

// yourEventCallBack(const Event &);
typedef std::function<bool(const Event &)> EventCallback;
    
    
} // namespace
} // namespace


#endif // include guard
