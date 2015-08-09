
#ifndef CAFFEINE_COMPONENTS_INPUT_INCLUDED
#define CAFFEINE_COMPONENTS_INPUT_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Application/Input.hpp>


namespace Caffeine {
namespace Components {
    

namespace InputUtils {
        
    void SetInput(CaffApp::Input *input);
    
}


class InputComponent : public CaffEnt::Component
{
public:
    
    COMPONENT_RTTI(Input)
    
    explicit                InputComponent();
    
    
    bool                    isKeyDown(const CaffApp::KeyID key) const;
    bool                    isKeyUp(const CaffApp::KeyID key) const;
    
    void                    setMouseHold(const bool setHold);
    bool                    isMouseHeld() const;
    
    float                   getMouseDeltaX() const;
    float                   getMouseDeltaY() const;
    
	uint16_t				getMouseX() const;
	uint16_t				getMouseY() const;
	
	virtual void			onMouseMoveDelta(const uint16_t x, const uint16_t y) {}
	virtual void			onMouseMovePosition(const uint16_t x, const uint16_t y) {}
	virtual void			onKeyChange(const CaffApp::KeyID key, const CaffApp::KeyState state) {}

    
}; // class
 
    
} // namespace
} // namespace


#endif // include guard
