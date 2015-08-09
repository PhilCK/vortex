
/*
	ModelLoader.cpp
	---------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

// TODO: Hotloading, remove or impliment.

#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <time.h>
#include <memory>
#include <map>


namespace Caffeine {
namespace Systems {



struct ModelManager::Impl
{
	std::map<std::string, std::unique_ptr<ModelData>>	loadedModels;
	CaffApp::Renderer &					renderer;
	bool								isHotLoading;
	
	explicit Impl(CaffApp::Renderer & renderer)
	: loadedModels()
	, isHotLoading(false)
	, renderer(renderer)
	{
	}
	
	
	~Impl()
	{
	}
	

	void addModel(const std::string &filename)
	{
		if(filename.empty())
		{
			return;
		}
		
		// New model data.
		std::unique_ptr<ModelData> data(new ModelData);
		{
			data->modelFileName = filename;
		}
		
		// Create the model
		{
			// TODO: Pass scale and rotation as local matrix.
			std::unique_ptr<CaffApp::Model> model(new CaffApp::Model);
			
			// Failed to create.
			if(!model->loadModel(data->modelFileName)) {
				return;
			}
			
			data->model	= std::move(model);
		}
		
		// Timestamps
		{
			//TODO: Bring back the time stamps.
		}
					
		// Load up all the meshes.
		{
			const std::size_t noMeshes = data->model->getNumberOfMeshes();
			
			data->vertBufferIDs.reserve(noMeshes);
			data->indexBufferIDs.reserve(noMeshes);
			
			for(uint16_t i = 0; i < noMeshes; ++i)
			{
				CaffApp::VertexContainer meshData = data->model->getMesh(i).getGLVertexBuffer();
				
				const CaffApp::VertexID vertID = renderer.addVertexBuffer(&meshData[0], sizeof(float) * data->model->getMesh(i).getGLVertices(), CaffApp::BufferAccess::STATIC);
				
				data->vertBufferIDs.push_back(vertID);
				
				// TODO: Index arrays.
			}
		}
		
		// Insert into map
		const std::string modelName = CaffUtil::GetFileNameFromPath(data->modelFileName);
		loadedModels[modelName] = std::move(data);
	}
	
	void updateModels()
	{
		// TODO: Hotloading has been removed.
	}
	
}; // impl struct



ModelManager::ModelManager(const std::string &directory, const std::string &extension, const bool hotLoading, CaffApp::Renderer &renderer)
: m_pImpl(new Impl(renderer))
{
	m_pImpl->isHotLoading = hotLoading;
	const std::string dirPath = CaffSys::Properties::GetApplicationDir() + directory;
	
	std::vector<std::string> filenames;
	
	Utilities::GetFileNamesOfTypeInDirectory(filenames, extension, dirPath);
	
	// Load all files
	for(auto &file : filenames)
	{
		m_pImpl->addModel(file);
	}
}



ModelManager::ModelManager(const CaffUtil::Param &params, CaffApp::Renderer &renderer)
: ModelManager::ModelManager(params["directory"].asStdString("Models/"),
							 params["extension"].asStdString("obj"),
							 params["hot"].asBool(false),
							 renderer)
{
}



ModelManager::~ModelManager()
{
}



CaffSys::ModelData & ModelManager::getModelData(const std::string & modelName)
{
	assert(m_pImpl->loadedModels.count(modelName)); // Didn't find the model you requested. // TODO: Should this return ptr? Is it acceptable to request models that dont exist?
	return *m_pImpl->loadedModels[modelName];
}



void ModelManager::checkForUpdates()
{
	// If models and hotloading then check for updates.
	if(!m_pImpl->loadedModels.empty() && m_pImpl->isHotLoading)
	{
		m_pImpl->updateModels();
	}
}



} // namespace
} // namespace
