
/*
	Application.cpp
	--
	Project: Caffeine
*/

#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Utilities/Timer.hpp>
#include <Caffeine/Utilities/Logging.hpp>


namespace Caffeine {
namespace Application {


Application::Application(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen)
: m_window(nullptr)
, m_renderer(nullptr)
, m_input(nullptr)
, m_gameTimer()
, m_deltaTime(0.f)
, m_shouldQuit(false)
{
    // Init SDL.
	const Uint32 flags = SDL_INIT_EVERYTHING;
	if(SDL_Init(flags) != 0)
	{
		std::runtime_error(SDL_GetError()); // TODO: Errr!
	}
	
	// SDL Attribute Hints.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	
	// Create Input, Window and Renderer.
	m_window.reset(new Window(title, width, height, isFullscreen));
	m_renderer.reset(new Renderer(m_window->getSDLWindow()));
	m_input.reset(new Input());
}


Application::Application(const CaffUtil::Param &appParams)
: m_window(nullptr)
, m_renderer(nullptr)
, m_input(nullptr)
, m_gameTimer()
, m_deltaTime(0.f)
, m_shouldQuit(false)
{
    // Init SDL.
	const Uint32 flags = SDL_INIT_EVERYTHING;
    if(SDL_Init(flags) != 0)
	{
		std::runtime_error(SDL_GetError());
	}
	
	// SDL Attribute Hints.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,	1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,	4);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,			appParams["renderer"]["red_bits"].asUInt32(5));
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,			appParams["renderer"]["green_bits"].asUInt32(5));
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,			appParams["renderer"]["blue_bits"].asUInt32(5));
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,			appParams["renderer"]["alpha_bits"].asUInt32(0));
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,			appParams["renderer"]["depth_bits"].asUInt32(24));

	// Window	
	const std::string title		   = appParams["window"]["title"].asStdString("CaffApp");
	const uint16_t	  width		   = appParams["window"]["width"].asUInt32(640);
	const uint16_t	  height	   = appParams["window"]["height"].asUInt32(420);
	const bool		  isFullscreen = appParams["window"]["fullscreen"].asBool(false);
	
	m_window.reset(new Window(title, width, height, isFullscreen));
	
	// Input/Renderer
	m_renderer.reset(new Renderer(m_window->getSDLWindow()));
	m_input.reset(new Input());
	
	SDL_StartTextInput();
}


Application::~Application()
{
	m_renderer.release();
	m_window.release();

	SDL_ERROR("Application::~Application");

	SDL_Quit();
}


void Application::startFrame()
{
	m_deltaTime = m_gameTimer.split() * 0.001f;
	
	onTick();
}


void Application::endFrame()
{
	// Swap buffer.
	SDL_GL_SwapWindow(m_window->getSDLWindow());
	SDL_ERROR("Application::endFrame()");
}


void Application::onTick()
{
	{
		SDL_ERROR("Application::onTick()");
	}

	// Reset Input
	{
		m_input->updateMouseDelta(0, 0);
		m_input->flushFrameData();
	}
	
	// SDL Event Checking
	{
		SDL_Event sdlEvent;
		
		while(SDL_PollEvent(&sdlEvent))
		{
			// Check for quit message.
			if(sdlEvent.type == SDL_QUIT)
			{
				m_shouldQuit = true;
			}
			
			if(sdlEvent.type == SDL_TEXTINPUT)
			{
				const std::string streamTxt = sdlEvent.text.text;
				m_input->streamText(streamTxt);
			}
			
			// Input Events TODO: Move into input.
			if(sdlEvent.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = sdlEvent.key.keysym.sym;
				
				m_input->updateKeyState((Caffeine::Application::KeyID)key, KeyState::DOWN);
			}

			if(sdlEvent.type == SDL_KEYUP)
			{
				SDL_Keycode key = sdlEvent.key.keysym.sym;
				
				m_input->updateKeyState((Caffeine::Application::KeyID)key, KeyState::UP);
			}
			
			if(sdlEvent.type == SDL_MOUSEMOTION)
			{
				m_input->updateMouseDelta(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
			}
            
            if(sdlEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if(sdlEvent.button.button == SDL_BUTTON_LEFT)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_LEFT, KeyState::DOWN);
                }
                else if(sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_CENTER, KeyState::DOWN);
                }
                else if(sdlEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_RIGHT, KeyState::DOWN);
                }
            }
            
            if(sdlEvent.type == SDL_MOUSEBUTTONUP)
            {
                if(sdlEvent.button.button == SDL_BUTTON_LEFT)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_LEFT, KeyState::UP);
                }
                else if(sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_CENTER, KeyState::UP);
                }
                else if(sdlEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    m_input->updateKeyState(CaffApp::KeyID::MS_RIGHT, KeyState::UP);
                }
            }
		}
	}
}

bool Application::hasLostFocus() const { return false; }
bool Application::hasGainedFocus() const { return false; }
bool Application::isInFocus() const { return true; }


#pragma mark - Application Modules

Renderer & Application::getRenderer() const {
	return *m_renderer;
}

Window & Application::getWindow() const {
	return *m_window;
}

Input & Application::getInput() const {
	return *m_input;
}

} // namespace
} // namespace

