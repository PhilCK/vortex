
/*
	ModelLoader.hpp
	---------------
	
	Project: Caffeine
*/

#ifndef CAFFEINE_SYSTEMS_MODEL_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_MODEL_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <memory>


namespace Caffeine {
namespace Systems {


struct ModelData
{
	time_t				xmlTimeStamp;
	time_t				modelTimeStamp;
	
	std::string			xmlFileName;
	std::string			modelFileName;
	
	std::unique_ptr<CaffApp::Model>	model;
	std::vector<CaffApp::VertexID>	vertBufferIDs;
	std::vector<CaffApp::IndexID>	indexBufferIDs;
	
}; // struct



class ModelManager : private CaffUtil::NonCopyable
{
public:

	explicit				ModelManager(const std::string & directory, const std::string & extension, const bool hotLoading, CaffApp::Renderer & renderer);
	explicit				ModelManager(const CaffUtil::Param &params, CaffApp::Renderer &renderer);
	
							~ModelManager();
	
	//TODO: Make this ptr as the model may not exists.
	CaffSys::ModelData &	getModelData(const std::string & modelName);
	
	void					checkForUpdates();
	
private:

	struct Impl;
	std::unique_ptr<Impl>	m_pImpl;

}; // class

} // namespace
} // namespace


#endif // include guard
