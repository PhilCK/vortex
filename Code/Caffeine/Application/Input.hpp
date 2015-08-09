
/*
	Input.hpp
	--
	Project: Caffeine
*/

// TODO: Not full featured yet.

#ifndef CAFFEINE_APPLICATION_INPUT_INCLUDED
#define CAFFEINE_APPLICATION_INPUT_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <functional>
#include <memory>
#include <map>

namespace Caffeine {
namespace Application {

enum class KeyState {
	DOWN = 0,
	UP,
};

enum class KeyID
{
	KB_A = SDLK_a,
	KB_B = SDLK_b,
	KB_C = SDLK_c,
	KB_D = SDLK_d,
	KB_E = SDLK_e,
	KB_F = SDLK_f,
	KB_G = SDLK_g,
	KB_H = SDLK_h,
	KB_I = SDLK_i,
	KB_J = SDLK_j,
	KB_K = SDLK_k,
	KB_L = SDLK_l,
	KB_M = SDLK_m,
	KB_N = SDLK_n,
	KB_O = SDLK_o,
	KB_P = SDLK_p,
	KB_Q = SDLK_q,
	KB_R = SDLK_r,
	KB_S = SDLK_s,
	KB_T = SDLK_t,
	KB_U = SDLK_u,
	KB_V = SDLK_v,
	KB_W = SDLK_w,
	KB_X = SDLK_x,
	KB_Y = SDLK_y,
	KB_Z = SDLK_z,
	
	KB_1 = SDLK_1,
	KB_2 = SDLK_2,
	KB_3 = SDLK_3,
	KB_4 = SDLK_4,
	KB_5 = SDLK_5,
	KB_6 = SDLK_6,
	KB_7 = SDLK_7,
	KB_8 = SDLK_8,
	KB_9 = SDLK_9,
	KB_0 = SDLK_0,
	
	KB_UP	 = SDLK_UP,
	KB_DOWN  = SDLK_DOWN,
	KB_LEFT  = SDLK_LEFT,
	KB_RIGHT = SDLK_RIGHT,
	
	KB_ESCAPE	 = SDLK_ESCAPE,
	KB_RSHIFT	 = SDLK_RSHIFT,
	KB_LSHIFT	 = SDLK_LSHIFT,
	KB_SPACE	 = SDLK_SPACE,
	KB_RCTRL	 = SDLK_RCTRL,
	KB_LCTRL	 = SDLK_LCTRL,
	KB_RALT		 = SDLK_RALT,
	KB_LALT		 = SDLK_LALT,
	KB_TAB		 = SDLK_TAB,
	KB_CAPS		 = SDLK_CAPSLOCK,
	KB_NUM		 = SDLK_NUMLOCKCLEAR,
	KB_ENTER	 = SDLK_KP_ENTER,
	KB_RETURN    = SDLK_RETURN,
	KB_BACKSPACE = SDLK_BACKSPACE,
	KB_DELETE	 = SDLK_DELETE,
	
    MS_LEFT,
    MS_CENTER,
    MS_RIGHT,
	MS_XAXIS,
	MS_YAXIS,
	
	GP_LEFT_XAXIS,
	GP_LEFT_YAXIS,

	GP_RIGHT_XAXIS,
	GP_RIGHT_YAXIS,
};


class Input : private CaffUtil::NonCopyable
{
public:

	explicit					Input();
								~Input();
	
	
	#pragma mark - Mouse Control
	
	// Position of the mouse pointer.
	uint16_t					getMouseX() const;
	uint16_t					getMouseY() const;
	
	// Difference in the mouse coords per frame.
	inline float				getMouseDeltaX() const { return m_xDelta; }
	inline float				getMouseDeltaY() const { return m_yDelta; }
	
	// Holds the mouse inside the screen.
	void						holdMouse(const bool isHeld);
	
	// Update Mouse info.
	void						updateMouseDelta(const float deltaX, const float deltaY);
		
	#pragma mark - Key Control
	
	// Checks if a key is up or down.
	void						setTextStream(const bool setTextStream);
	std::string					getKeyName(const KeyID keyID) const;
	bool						isDown(const KeyID keyID) const;
	bool						isUp(const KeyID keyID) const;
	bool						isDownOnFrame(const KeyID keyID) const;
	bool						isUpOnFrame(const KeyID keyID) const;
	float						getAxis(const KeyID keyID) const;
	
	
	void						updateKeyState(const KeyID keyID, const KeyState state);
	void						streamText(const std::string &str);
	void						flushFrameData();
	
	std::function<void (const KeyID)> onButtonUpEvent, onButtonDownEvent; // TODO: Deprecated
	std::function<void (const float, const float)> onMouseMovePositionEvent, onMouseDeltaEvent;
	std::function<void (const KeyID, const KeyState)> onKeyChangeEvent;
	std::function<void (const std::string &)> onTextStreamEvent;
	
private:

	std::map<KeyID, KeyState>	m_keystates;
	std::map<KeyID, KeyState>	m_frameKeystates;
	std::map<KeyID,	float>		m_axis;
	
	float						m_xDelta;
	float						m_yDelta;

}; // class

} // namespace
} // namespace

#endif // include guard
