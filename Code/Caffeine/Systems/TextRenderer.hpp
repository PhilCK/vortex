
#ifndef CAFFEINE_SYSTEMS_TEXT_RENDERER_INCLUDED
#define CAFFEINE_SYSTEMS_TEXT_RENDERER_INCLUDED



#include <Caffeine/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <string>
#include <vector>
#include <map>



namespace Caffeine {
namespace Systems {



class TextRenderer : private CaffUtil::NonCopyable
{
	struct Font
	{
		struct CharInfo
		{
			uint16_t x;
			uint16_t y;
			uint16_t width;
			uint16_t height;
			uint16_t xOffset;
			uint16_t yOffset;
			uint16_t xAdvance;
		};
		
		uint16_t maxWidth;
		uint16_t maxHeight;
		
		std::map<uint16_t, CharInfo> characters;
	};
	
	enum class TextPosition { SCREEN_SPACE,	};
	enum class TextType		{ TEXT_2D,		};
	
	struct TextToRender
	{
		std::string			textToRender	= "";
		TextPosition		renderSpace		= TextPosition::SCREEN_SPACE;
		TextType			textType		= TextType::TEXT_2D;
		CaffMath::Vector3	position = { { 0.f, 0.f, 0.f } };
		std::string			fontName		= "";
		float				scale			= 1.f;
		bool				markup			= false;
		bool				fixedWidth		= false;
	};

public:
	
	explicit							TextRenderer(CaffApp::Renderer &renderer, CaffSys::TextureManager &texMgr, CaffSys::ModelManager &modelMgr, CaffSys::CameraManager &camMgr);
	// TODO: Requres params ctor.
										~TextRenderer() {}
	
	void								render2DScreenText(const std::string &text, const CaffMath::Vector2 &position, const std::string &font = "", const float scale = 1.f, const bool fixedWidth = false, const bool processMarkup = false);
	void								renderPendingText();
	
private:

	CaffApp::Renderer					&m_renderer;
	CaffSys::TextureManager				&m_textureMgr;
	CaffSys::ModelManager				&m_modelMgr;
	CaffSys::CameraManager				&m_camMgr;
	
	std::map<std::string, Font>			m_fonts;
	
	std::vector<TextToRender>			m_textToProcess;
	
}; // class



} // namespace
} // namespace


#endif // include guard
