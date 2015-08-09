
/*
	ModelLoader.hpp
	---------------
	
	Project: Caffeine
*/

#ifndef CAFFEINE_SYSTEMS_TEXTURE_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_TEXTURE_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <memory>
#include <string>

namespace Caffeine {
namespace Systems {


struct TextureData
{
	time_t				xmlTimeStamp;
	time_t				textureTimeStamp;
	
	std::string			xmlFileName;
	std::string			textureFilename;
	
	CaffApp::TextureID	textureID;
	
	bool				isLoaded;
	
}; // struct


class TextureManager : private CaffUtil::NonCopyable
{
public:

	explicit					TextureManager(const std::string &directory, const std::string &extension, const bool hotLoading, CaffApp::Renderer &renderer);
	explicit					TextureManager(const CaffUtil::Param &params, CaffApp::Renderer &renderer);
	
								~TextureManager();
				
	
	CaffSys::TextureData *		getTextureData(const std::string & textureName);
	
	void						checkForUpdates();
	
private:

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;

}; // class


} // namespace
} // namespace

#endif // include guard
