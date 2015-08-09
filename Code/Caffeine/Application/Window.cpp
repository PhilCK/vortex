
/*
	Window.cpp
	----------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Utilities/Logging.hpp>

namespace Caffeine {
namespace Application {


#pragma mark - Window

Window::Window(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen)
	: m_sdlWindow(nullptr)
	, m_title(title)
	, m_isFullscreen(isFullscreen)
	, m_width(width)
	, m_height(height)
{
	// Setup the SDL Window.
	Uint32 flags = m_isFullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL;

	m_sdlWindow = SDL_CreateWindow(m_title.c_str(),
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								m_width,
								m_height,
								flags);
	SDL_ERROR("Window::Window()");
}


Window::Window(const WindowParams &params)
: Window(params.title, params.width, params.height, params.isFullscreen)
{
}


Window::~Window()
{
	SDL_DestroyWindow(m_sdlWindow);
	SDL_ERROR("Window::~Window()");
}



#pragma mark - Window size settings

bool Window::setResolution(const uint16_t width, const uint16_t height)
{
	DEBUG_LOG("Window: Setting screen size during runtime is not currently supported on this platform.")
	return false;
}


uint16_t Window::getWidth() const
{
	int w, h;
	
	SDL_GetWindowSize(m_sdlWindow, &w, &h);
	SDL_ERROR("Window::getWidth()");
    
	return (uint16_t)w;
}


uint16_t Window::getHeight() const
{
	int w, h;
	
	SDL_GetWindowSize(m_sdlWindow, &w, &h);
	SDL_ERROR("Window::getHeight()");

	return (uint16_t)h;
}


#pragma mark - Fullscreen settings.

bool Window::setFullscreen(const bool isFullscreen)
{
	m_isFullscreen = isFullscreen;
	
	SDL_SetWindowFullscreen(m_sdlWindow, m_isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	SDL_ERROR("Window::setFullscreen()");
	
	return true;
}


#pragma mark - Title settings.

bool Window::setTitle(const std::string & setTitle)
{
	m_title = setTitle;
	
	SDL_SetWindowTitle(m_sdlWindow, m_title.c_str());
	SDL_ERROR("Window::setTitle()");
	
	return true;
}


} // namespace
} // namespace
