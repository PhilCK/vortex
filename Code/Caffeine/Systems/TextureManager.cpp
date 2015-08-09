
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <time.h>
#include <map>

namespace Caffeine {
namespace Systems {

#pragma mark - Impl

struct TextureManager::Impl
{
	explicit Impl(CaffApp::Renderer & renderer, const bool hotLoading)
	: loadedTextures()
	, isHotLoading(hotLoading)
	, renderer(renderer)
	{
	}
		
	~Impl()
	{
	}
	
	void addTexture(const std::string &filename)
	{
		if(filename.empty())
		{
			return;
		}

		std::unique_ptr<TextureData> data(new TextureData);
		
		data->textureFilename = filename;
		
		// Timestamps
		{
			// TODO: Bring back timestamps
		}
		
		// Load up the texture
		{
			data->textureID = renderer.addTexture(data->textureFilename);
			
			if(data->textureID != CaffApp::NO_TEXTURE)
			{
				data->isLoaded = true;
			}
			else
			{
				return;
			}
		}

		// Insert into map
		const std::string name = CaffUtil::GetFileNameFromPath(filename);
		loadedTextures[name] = std::move(data);
		
	} // addTexture
	

	void updateTextures()
	{
		// TODO: Bring back hotloading.
	} // updateTextures

	

	std::map<std::string, std::unique_ptr<TextureData>>	loadedTextures;
	bool								isHotLoading;
	CaffApp::Renderer &					renderer;

}; // struct


#pragma mark - Texture Manager

TextureManager::TextureManager(const std::string &directory, const std::string &extension, const bool hotLoading, CaffApp::Renderer &renderer)
: m_pImpl(new Impl(renderer, hotLoading))
{
	m_pImpl->isHotLoading = hotLoading;
	std::string dirPath = CaffSys::Properties::GetApplicationDir() + directory;
	
	std::vector<std::string> filenames;
	Utilities::GetFileNamesOfTypeInDirectory(filenames, extension, dirPath);
	
	// Load all files
	for(auto &file : filenames)
	{
		m_pImpl->addTexture(file);
	}
}


TextureManager::TextureManager(const CaffUtil::Param &params, CaffApp::Renderer & renderer)
: TextureManager::TextureManager(params["directory"].asStdString("Textures/"),
								 params["extension"].asStdString("png"),
								 params["hot"].asBool(false),
								 renderer)
{
}



TextureManager::~TextureManager()
{
}



CaffSys::TextureData * TextureManager::getTextureData(const std::string &textureName)
{
	return m_pImpl->loadedTextures[textureName].get();
}



void TextureManager::checkForUpdates()
{
	// If textures and hotloading then check for updates.
	if(!m_pImpl->loadedTextures.empty() && m_pImpl->isHotLoading)
	{
		m_pImpl->updateTextures();
	}
}



} // namespace
} // namespace
