
/*
	Application.hpp
	---------------
	
	Project:
	Caffeine
	
	Purpose:
	Basic application class.
*/

#ifndef CAFFEINE_APPLICATION_APPLICATION_INCLUDED
#define CAFFEINE_APPLICATION_APPLICATION_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Utilities/Timer.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <string>
#include <memory>

namespace Caffeine {
namespace Application {

/*
	Application
	-----------
	Main application class.

 */

class Application final : private CaffUtil::NonCopyable
{
public:
    
	//! Setup the window with a title, width, height and if it is fullscreen or not.
	//! This will apply the same width/height to the renderer settings.
    explicit					Application(const std::string &title, const uint32_t width, const uint32_t height, const bool isFullscreen);
	
	//! Initalize using the setup param structs, this gives for greater initialisation control.
	explicit					Application(const CaffUtil::Param &appParams);
	
	virtual						~Application();
	
	//! Application does not define a game loop, you need to specifiy the start of a frame in your game loop.
	void						startFrame();
	
	//! Application does not define a game loop, you need to specifiy the end of a frame in your game loop.
	void						endFrame();
	
	// onTick gets called by startFrame, which allows for customisation.
	virtual void				onTick();
	
	//! Get the time the last frame took.
	inline float				getDeltaTime() const { return m_deltaTime; }
	
	//! The user or application has requested that the app should quit.
	inline bool					shouldQuit() const { return m_shouldQuit; }
	
	bool						hasLostFocus() const;
	bool						hasGainedFocus() const;
	bool						isInFocus() const;
	
	#pragma mark - Application Modules
	
	Renderer &					getRenderer() const;
	Window &					getWindow() const;
	Input &						getInput() const;
	
private:
	
	std::unique_ptr<Window>		m_window;
	std::unique_ptr<Renderer>	m_renderer;
	std::unique_ptr<Input>		m_input;
	
	Utilities::Timer			m_gameTimer;
	float						m_deltaTime;
	bool						m_shouldQuit;
	
}; // class


} // namespace
} // namespace

#endif // include guard
