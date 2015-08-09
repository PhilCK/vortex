
#include <Caffeine/Components/InputComponent.hpp>


namespace {
    
    CaffApp::Input *inputMgr(nullptr);
    
}


namespace Caffeine {
namespace Components {
    

namespace InputUtils {
    
    void SetInput(CaffApp::Input *input) { inputMgr = input; }
    
}

    

InputComponent::InputComponent()
: CaffEnt::Component()
{
	if(inputMgr)
	{
		inputMgr->onKeyChangeEvent = std::bind(&InputComponent::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
	}
}
    
    

bool InputComponent::isKeyDown(const CaffApp::KeyID key) const
{
    return inputMgr->isDown(key);
}
    
    

bool InputComponent::isKeyUp(const CaffApp::KeyID key) const
{
    return inputMgr->isUp(key);
}
    
    

void InputComponent::setMouseHold(const bool setHold)
{
    return inputMgr->holdMouse(setHold);
}
    
    

bool InputComponent::isMouseHeld() const
{
    return false; // TODO: Don't return false.
}
    
    
    
float InputComponent::getMouseDeltaX() const { return inputMgr->getMouseDeltaX(); }
float InputComponent::getMouseDeltaY() const { return inputMgr->getMouseDeltaY(); }
    
uint16_t InputComponent::getMouseX() const { return inputMgr->getMouseX(); }
uint16_t InputComponent::getMouseY() const { return inputMgr->getMouseY(); }
    
    
} // namespace
} // namespace
