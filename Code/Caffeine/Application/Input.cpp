
/*
	Application.hpp
	---------------
	
	Project: Caffeine
*/

#include <Caffeine/Application/Input.hpp>
#include <string>
#include <iostream>


namespace Caffeine {
namespace Application {



#pragma mark - Input

Input::Input()
: m_keystates()
, m_frameKeystates()
, m_axis()
, m_xDelta(0.f)
, m_yDelta(0.f)
{
}



Input::~Input()
{
}



#pragma mark - Mouse Control

uint16_t Input::getMouseX() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return x;
}



uint16_t Input::getMouseY() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return y;
}



void Input::holdMouse(const bool isHeld)
{
	SDL_SetRelativeMouseMode(isHeld ? SDL_TRUE : SDL_FALSE);
}



void Input::updateMouseDelta(const float deltaX, const float deltaY)
{
	m_xDelta = deltaX;
	m_yDelta = deltaY;
	
	m_axis[KeyID::MS_XAXIS] = deltaX;
	m_axis[KeyID::MS_YAXIS] = deltaY;
	
	m_keystates[KeyID::MS_XAXIS] = deltaX != 0 ? KeyState::DOWN : KeyState::UP;
	m_keystates[KeyID::MS_YAXIS] = deltaY != 0 ? KeyState::DOWN : KeyState::UP;
	
	if(onMouseDeltaEvent)
	{
		onMouseDeltaEvent(m_xDelta, m_yDelta);
	}
	
	if(onMouseMovePositionEvent)
	{
		onMouseMovePositionEvent(getMouseX(), getMouseY());
	}
}



#pragma mark - Key Control

void Input::setTextStream(const bool setTextStream)
{
	setTextStream ? SDL_StartTextInput() : SDL_StopTextInput();
}


std::string Input::getKeyName(const KeyID keyID) const
{
	const std::string name = SDL_GetKeyName((int)keyID);
	return name;
}



bool Input::isDown(const KeyID keyID) const
{
	auto it = m_keystates.find(keyID);
	
	if(it != m_keystates.end())
	{
		if(it->second == KeyState::DOWN) {
			return true;
		}
	}
	
	return false;
}



bool Input::isUp(const KeyID keyID) const {
	return !isDown(keyID);
}



float Input::getAxis(const KeyID keyID) const
{
	if((unsigned int)keyID >= (unsigned int)KeyID::MS_XAXIS)
	{
		return m_axis.count(keyID) ? m_axis.at(keyID) : 0.f;
	}
	
	else
	{
		return m_keystates.at(keyID) == KeyState::DOWN ? 1.f : 0.f;
	}
}



bool Input::isDownOnFrame(const KeyID keyID) const
{
	auto it = m_frameKeystates.find(keyID);
	
	if(it != m_frameKeystates.end())
	{
		if(it->second == KeyState::DOWN)
		{
			return true;
		}
	}
	
	return false;
}



bool Input::isUpOnFrame(const KeyID keyID) const
{
	return !isDownOnFrame(keyID);
}



void Input::updateKeyState(const KeyID keyID, const KeyState state)
{
	m_keystates[keyID] = state;
	m_frameKeystates[keyID] = state;
	
	if(state == KeyState::UP)
	{
		if(onButtonDownEvent)
		{
			onButtonUpEvent(keyID);
		}
	}
	else if(state == KeyState::DOWN)
	{
		if(onButtonDownEvent)
		{
			onButtonDownEvent(keyID);
		}
	}
	
	if(onKeyChangeEvent)
	{
		onKeyChangeEvent(keyID, state);
	}
}



void Input::streamText(const std::string &str)
{
	onTextStreamEvent(str);
}



void Input::flushFrameData()
{
	m_frameKeystates.clear();
}


} // namespace
} // namespace
