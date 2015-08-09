
#include <Caffeine/Systems/TextRenderer.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Math/Math.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assert.h>
#include <vector>
#include <fstream>
#include <functional>



namespace
{
	// Character vertex stuff.
	GLfloat vertexData[] =
	{
		0.0f,  0.5f, 0.0f,  // Top-left
	};


	GLuint indexData[] =
	{
		0,
	};

	std::array<CaffApp::FormatDesc, 2> vertFmt = {{
		{"inPosition",	CaffApp::AttrFormat::FLOAT, 3},
	}};

	CaffApp::ShaderID		textShaderID;
	CaffApp::VertexID		billboardID;
	CaffApp::IndexID		indexID;
	CaffApp::VertexFormatID vertFmtID;

} // namespace



namespace Caffeine {
namespace Systems {



TextRenderer::TextRenderer(CaffApp::Renderer &renderer, CaffSys::TextureManager &texMgr, CaffSys::ModelManager &modelMgr, CaffSys::CameraManager &camMgr)
: m_renderer(renderer)
, m_textureMgr(texMgr)
, m_modelMgr(modelMgr)
, m_camMgr(camMgr)
, m_fonts()
, m_textToProcess()
{
	const uint32_t bufferReserve(16);
	m_textToProcess.reserve(bufferReserve);

	// Load a default font
	{
		std::ifstream fontDetails(CaffSys::Properties::GetApplicationDir() + "Textures/fontfoo.txt");
		std::string line;
		
		Font font;
		font.maxWidth  = 0;
		font.maxHeight = 0;
		
		while(std::getline(fontDetails, line))
		{
			const std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(line);
			
			enum TokenID { CHAR = 0, ID, X, Y, WIDTH, HEIGHT, XOFFSET, YOFFSET, XADVANCE };
			
			if(!tokens.empty() && tokens.at(CHAR) == "char")
			{
				auto GetValue = [](const std::string &str) -> uint16_t
				{
					return CaffUtil::StringToInt(str.substr(str.find("=") + 1, str.length()));
				};
				
				Font::CharInfo charInfo;
				charInfo.x			= GetValue(tokens.at(X));
				charInfo.y			= GetValue(tokens.at(Y));
				charInfo.width		= GetValue(tokens.at(WIDTH));
				charInfo.height		= GetValue(tokens.at(HEIGHT));
				charInfo.xOffset	= GetValue(tokens.at(XOFFSET));
				charInfo.yOffset	= GetValue(tokens.at(YOFFSET));
				charInfo.xAdvance	= GetValue(tokens.at(XADVANCE));
				
				font.maxWidth  = CaffMath::Max(font.maxWidth,  charInfo.width);
				font.maxHeight = CaffMath::Max(font.maxHeight, charInfo.height);
				
				font.characters[GetValue(tokens.at(ID))] = charInfo;
			}
		}
		
		m_fonts["prop"] = font;
	}

	// Setup Render Stuff
	{
		vertFmtID = m_renderer.addVertexFormat(&vertFmt[0], vertFmt.size());
		assert(vertFmtID);
		
		billboardID = m_renderer.addVertexBuffer(&vertexData[0], sizeof(vertexData), CaffApp::BufferAccess::STATIC);
		assert(billboardID);
		
		indexID = m_renderer.addIndexBuffer(&indexData[0], 6, sizeof(indexData), CaffApp::BufferAccess::STATIC);
		assert(indexID);
		
		textShaderID = m_renderer.addShader(CaffUtil::GetPathDir() + "AppAssets/Shaders/Text.shd");
		assert(textShaderID);
	}
}



void TextRenderer::render2DScreenText(const std::string &text, const CaffMath::Vector2 &position, const std::string &font, const float scale, const bool fixedWidth, const bool processMarkup)
{
	const CaffMath::Vector3 pos = { position.x, position.y, 2.f };

	m_textToProcess.emplace_back();
	m_textToProcess.back().textToRender = text;
	m_textToProcess.back().renderSpace	= TextPosition::SCREEN_SPACE;
	m_textToProcess.back().textType		= TextType::TEXT_2D;
	m_textToProcess.back().position		= pos;
	m_textToProcess.back().fontName		= font;
	m_textToProcess.back().scale		= scale;
	m_textToProcess.back().markup		= processMarkup;
	m_textToProcess.back().fixedWidth	= fixedWidth;
}



void TextRenderer::renderPendingText()
{
	for(auto &text : m_textToProcess)
	{
		// Get the font
		const CaffSys::TextureData	*textureData = m_textureMgr.getTextureData("fontfoo.png");
		
		// Get the matices
		const CaffComp::CameraComponent &currentCam = m_camMgr.getCurrentCamera();
		const glm::mat4 proj  = currentCam.getProjMatrix();
		const glm::mat4 view  = currentCam.getViewMatrix();
			  glm::mat4 world = currentCam.getWorldMatrix();
		
		// Customise world mat
		{
			if(text.renderSpace == TextPosition::SCREEN_SPACE)
			{
				const glm::vec3 offset(text.position.x, text.position.y, -2.f);
				world = glm::translate(world, offset);
			}
			
			const float fontScale = text.scale * 0.125f;
			world = glm::scale(world, fontScale, fontScale, fontScale);
		}
		
		const Font &font = m_fonts.at("prop");
		
		float xAdvance = 0;
		float yAdvance = 0;
		
		// markup
		/*
			[red]text[/red]
			[green]text[/green]
			[blue]...
			[yellow]...
			[purple]
			[grey]
			[black]
			[white]
			[blink]
			[flash]
		*/
		
		enum MarkupTag { RED, GREEN, BLUE, YELLOW, GREY, BLACK, WHITE, BLINK, FLASH, MARKUP_SIZE, };
		const std::array<std::string, MARKUP_SIZE> markupTagNames = {{ "red", "green", "blue", "yellow", "grey", "black", "white", "blink", "flash", }};
		
		std::vector<MarkupTag> markupStack;
		const uint16_t markSizeHint = 4;
		markupStack.reserve(markSizeHint);
		
		markupStack.push_back(WHITE); // Get a default one in there.
		
		struct Markup
		{
			glm::vec4 color;
			bool isBlinking;
		};
		
		std::vector<Markup> markup;
		
		for(std::size_t i = 0; i < text.textToRender.size(); ++i)
		{
			// If we pushed a markup we don't want to render that.
			bool wasMarkupPushed = false;
		
			// Current letter we are processing.
			const std::string current = text.textToRender.substr(i, 1);
			
			// If new line move yAdvance
			if(current == "\n")
			{
				yAdvance += font.maxHeight;
				xAdvance = 0;
				continue;
			}
			
			// Does this look like its going to be a markup tag?
			{
				if(current == "[")
				{
					auto SearchTags = [&](const std::string &openTag, const std::string &closeTag, std::function<void(std::vector<MarkupTag> &, const MarkupTag)> func)
					{
						for(uint16_t m(0); m < markupTagNames.size(); ++m)
						{
							const std::string tag = openTag + markupTagNames.at(m) + closeTag;
							
							if((text.textToRender.size() - i) >= tag.size())
							{
								const std::string search = text.textToRender.substr(i, tag.size());
								
								if(tag == search)
								{
									func(markupStack, (MarkupTag)m);
									i += tag.size() - 1;
									wasMarkupPushed = true;
									break;
								}
							}
						}
					};

					// Search Open Tags.
					auto OpenTag = [](std::vector<MarkupTag> &markup, const MarkupTag markupTag)
					{
						markup.push_back(markupTag);
					};

					SearchTags("[", "]", OpenTag);

					// Search Close Tags.
					auto CloseTag = [](std::vector<MarkupTag> &markup, const MarkupTag markupTag)
					{
						if(!markup.empty() && markup.back() == (MarkupTag)markupTag)
						{
							markup.pop_back();
						}
					};

					SearchTags("[/", "]", CloseTag);
				}
				
				// If we pushed/popped a markup lets move on.
				if(wasMarkupPushed)
				{
					continue;
				}
			} // Markup tags.
		
			m_renderer.reset();
					
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			m_renderer.setShader(textShaderID);
			m_renderer.setVertexFormat(vertFmtID);
			m_renderer.setVertexBuffer(billboardID);
			m_renderer.setIndexBuffer(indexID);
			
			m_renderer.setShaderMatrix44f("worldMat", &world[0][0]);
			m_renderer.setShaderMatrix44f("viewMat",  &view[0][0]);
			m_renderer.setShaderMatrix44f("projMat",  &proj[0][0]);
			
			m_renderer.setTexture("uniFontMap", textureData->textureID);
			
			const glm::vec2 scale(1.f, 1.f);
			m_renderer.setShader2f("uniFontScale", &scale[0]);
			
			static int blinkCount = 0; // TODO: Make timer.
			static bool isBlink = true;
			
			blinkCount++;
			
			if(!(blinkCount % 6000))
			{
				isBlink = !isBlink;
			}

			// Set the color of the text.
			{
				std::function<void(std::vector<MarkupTag>::reverse_iterator&)> SelectColor = [&](std::vector<MarkupTag>::reverse_iterator &markupIt)
				{
					auto SetColor = [&](const float r, const float g, const float b, const float a)
					{
						const glm::vec4 color(r, g, b, a);
						m_renderer.setShader4f("uniFontColor", &color[0]);
					};
				
				
					switch(*markupIt)
					{
						case(RED):		SetColor(1.f, 0.f, 0.f, 1.f);		break;
						case(GREEN):	SetColor(0.f, 1.f, 0.f, 1.f);		break;
						case(BLUE):		SetColor(0.f, 0.f, 1.f, 1.f);		break;
						case(YELLOW):	SetColor(1.f, 1.f, 0.f, 1.f);		break;
						case(GREY):		SetColor(0.7f, 0.7f, 0.7f, 1.f);	break;
						case(BLACK):	SetColor(1.f, 0.f, 0.f, 1.f);		break;
						case(WHITE):	SetColor(1.f, 1.f, 1.f, 1.f);		break;
						case(BLINK):
						{
							if(isBlink)
							{
								SetColor(0.f, 0.f, 0.f, 0.f);
							}
							else
							{
								++markupIt;
								
								if(markupIt != markupStack.rend())
								{
									SelectColor(markupIt);
								}
							}
							
							break;
						}
						case(FLASH):
						{
							if(isBlink)
							{
								markupIt += 2;
								
								if(markupIt != markupStack.rend())
								{
									SelectColor(markupIt);
								}
							}
							else
							{
								++markupIt;
								
								if(markupIt != markupStack.rend())
								{
									SelectColor(markupIt);
								}
							}
							
							break;
						}
						default:
							SetColor(0.5f, 0.25f, 0.5f, 1.f); break;
						break;
					};
				};
				
				auto it = markupStack.rbegin();
				SelectColor(it);
				
			}
					
			const uint16_t id = text.textToRender.at(i);
			
			// Catch unknown characters.
			glm::vec2 characterSize;
			glm::vec2 characterStart;
			
			if(font.characters.count(id) > 0)
			{
				characterStart.x = font.characters.at(id).x;
				characterStart.y = font.characters.at(id).y;
				characterSize.x  = font.characters.at(id).width;
				characterSize.y  = font.characters.at(id).height;
				
				m_renderer.setShader1f("uniXAdvance", &xAdvance);
				m_renderer.setShader1f("uniYAdvance", &yAdvance);
				
				xAdvance += text.fixedWidth ? font.maxWidth : font.characters.at(id).xAdvance;
				
				const glm::vec2 offset((float)font.characters.at(id).xOffset, (float)font.characters.at(id).yOffset);
				m_renderer.setShader2f("uniOffset", &offset[0]);
			}
			else
			{
				characterStart.x = 2;
				characterStart.y = 2;
				characterSize.x  = 0;
				characterSize.y  = 0;
				
				m_renderer.setShader1f("uniXAdvance", &xAdvance);
				m_renderer.setShader1f("uniYAdvance", &yAdvance);
				
				const glm::vec2 offset(0.f, 0.f);
				m_renderer.setShader2f("uniOffset", &offset[0]);
			}
			
			const glm::vec2 fontMapSize(512.f, 512.f);
			
			m_renderer.setShader2f("uniFontMapSize",	&fontMapSize[0]);
			m_renderer.setShader2f("uniCharacterSize",  &characterSize[0]);
			m_renderer.setShader2f("uniCharacterStart", &characterStart[0]);
			
			const glm::vec2 maxScale(font.maxWidth, font.maxHeight);
			m_renderer.setShader2f("uniMaxScale", &maxScale[0]);
			m_renderer.apply();

			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
	
	m_textToProcess.clear();
}



} // namespace
} // namespace
