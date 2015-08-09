
/*
	Window.hpp
	----------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

// TODO: setFullscreen, setTitle not implimented yet.

#ifndef CAFFEINE_APPLICATION_WINDOW_INCLUDED
#define CAFFEINE_APPLICATION_WINDOW_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <string>
#include <memory>


namespace Caffeine {
namespace Application {


struct WindowParams
{
	std::string		title;
	uint16_t		width;
	uint16_t		height;
	bool			isFullscreen;
};

/*
	Window
	------
	Deals with the window context only.
	You can set the properties of the window but this
	will not affect the renderer's buffers in anyway.
 */
 
class Window : private CaffUtil::NonCopyable
{
public:
    
    explicit				Window(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen);
	explicit				Window(const WindowParams &params);
							~Window();


	#pragma mark - Window size settings
    
    bool					setResolution(const uint16_t width, const uint16_t height);
	uint16_t				getWidth() const;
    uint16_t				getHeight() const;
	
	
	#pragma mark - Fullscreen settings.
	
	bool					setFullscreen(bool const isFullscreen);
	inline bool				isFullscreen() const { return m_isFullscreen; }
	
	
	#pragma mark - Title settings
	
	bool					setTitle(const std::string & setTitle);
	inline std::string		getTitle() const { return m_title; }
	
	
	#pragma mark - Misc.
	
	inline SDL_Window*		getSDLWindow() const { return m_sdlWindow; }
	
private:
	
	SDL_Window				*m_sdlWindow;
	std::string				m_title;
	bool					m_isFullscreen;
	uint32_t				m_width;
	uint32_t				m_height;
    
}; // class
    
} // namespace
} // namespace

#endif // include guard
