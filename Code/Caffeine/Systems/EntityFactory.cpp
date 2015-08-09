
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/ColliderComponent.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Components/ModelComponent.hpp>
#include <Caffeine/Components/LightComponent.hpp>
#include <Caffeine/Components/MaterialComponent.hpp>

#include <Caffeine/Entity/Entity.hpp>

#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <Caffeine/Utilities/Param.hpp>

#include <string>
#include <iostream>
#include <functional>


namespace
{
	uint32_t entityCounter(0); // Counter of how many entities we have made.

	// Stuff for special components
	CaffSys::PhysicsManager *physicsManager(nullptr);
}


namespace Caffeine {
namespace Systems {


EntityFactory::EntityFactory(CaffApp::Window *window, CaffApp::Input *input, CaffApp::Renderer *renderer, CaffSys::EventManager *eventManager, CaffSys::StateManager *stateManager, CaffSys::CameraManager *cameraManager, CaffSys::ModelManager *modelManager, CaffSys::TextureManager *textureManager, CaffSys::TextRenderer *textRenderer, ScriptManager *scriptMgr, CaffSys::PhysicsManager *physMgr)
: m_registeredEntities()
, m_registeredComponentFactories()
, m_windowModule(window)
, m_inputModule(input)
, m_rendererModule(renderer)
, m_eventManagerModule(eventManager)
, m_stateManagerModule(stateManager)
, m_cameraManagerModule(cameraManager)
, m_modelManagerModule(modelManager)
, m_textureManagerModule(textureManager)
, m_textRendererModule(textRenderer)
, m_physicsManagerModule(physMgr)
{
	checkForUpdates();

	// Register the default components.
	registerComponent("Transform",			new TransformComponentFactory());
	registerComponent("Rigidbody",			new PhysicsComponentFactory());
	registerComponent("Collider",			new ColliderComponentFactory());
	registerComponent("Camera",				new CameraComponentFactory());
	registerComponent("Model",				new ModelComponentFactory());
	registerComponent("DirectionalLight",	new DirectionalLightComponentFactory());
	registerComponent("PointLight", 		new PointLightComponentFactory());
	registerComponent("FullbrightMaterial", new FullbrightMaterialComponentFactory());
	registerComponent("BlinnPhongMaterial", new BlinnPhongMaterialComponentFactory());
	
	// Put it in global scope to allow PhysicsComponent to grab it, this might be moot now.
	// TODO: Is there better way? entity will have physics manager already.
	physicsManager = m_physicsManagerModule;
}



EntityFactory::EntityFactory(CaffApp::Window *window, CaffApp::Input *input, CaffApp::Renderer *renderer, CaffSys::EventManager *eventManager, CaffSys::StateManager *stateManager, CaffSys::CameraManager *cameraManager, CaffSys::ModelManager *modelManager, CaffSys::TextureManager *textureManager, CaffSys::TextRenderer *textRenderer, ScriptManager *scriptMgr, CaffSys::PhysicsManager *physMgr, const CaffUtil::Param &params)
: EntityFactory(window, input, renderer, eventManager, stateManager, cameraManager, modelManager, textureManager, textRenderer, scriptMgr, physMgr)
{
	// TODO: impliment Param settings for directories.
}



EntityFactory::~EntityFactory()
{
	clearFactories();
}



void EntityFactory::checkForUpdates()
{
	// Get all the entities in the directory.
	const std::string dirPath = CaffSys::Properties::GetApplicationDir() + "Entities/"; // TODO: Die.
	
	std::vector<std::string> filenames;
	
	Utilities::GetFileNamesOfTypeInDirectory(filenames, "xml", dirPath);
	
	// Load all files
	//for(auto it = filenames.begin(); it != filenames.end(); ++it)
	for(const std::string &file : filenames)
	{
        const CaffUtil::Param details(file, CaffUtil::Param::FileType::XML);
		const time_t timeStamp(0);
		
		EntityData newEntity;
		newEntity.fileTimeStamp = timeStamp;
		newEntity.details		= details;
		
		const std::string entityName = CaffUtil::GetFileNameFromPath(file);
        
        const std::string mapName = newEntity.details["entity"]["name"].asStdString().empty() ? entityName : newEntity.details["entity"]["name"].asStdString();
		
		m_registeredEntityData[mapName] = newEntity;
	}
}



std::unique_ptr<CaffEnt::Entity> EntityFactory::createInstance(const std::string & id, CaffSys::EventManager *evtMgr, CaffSys::StateManager *stateMgr)
{
	if(m_registeredEntityData.count(id))
	{
		const EntityData &entityData = m_registeredEntityData.at(id);
		
		// Creates Entity/Components and Children recusivly.
		std::function<std::unique_ptr<CaffEnt::Entity> (const CaffUtil::Param&)> Instantiate = [&](const CaffUtil::Param &entDetail) -> std::unique_ptr<CaffEnt::Entity>
		{
			std::unique_ptr<CaffEnt::Entity> newEntity = createInstance();
			
			// Set given mgr's
			if(evtMgr)		{ newEntity->setEventManager(evtMgr);	}
			if(stateMgr)	{ newEntity->setStateManager(stateMgr); }
		
			// Setup Entity
			{
				// Name and ID
				newEntity->setName(entDetail["name"].asStdString("Entity"));
				newEntity->setID(++entityCounter);
				
				// Tags
				std::vector<std::string> tags = CaffUtil::SplitStringBySpaces(entDetail["tags"].asStdString());
				
				for(auto &tag : tags)
				{
					newEntity->addTag(tag);
				}
			}
			
			// Components
			{
				const std::size_t numberOfComponents = entDetail["components"].numberOfMembersOfName("component");
				
				for(std::size_t i = 0; i < numberOfComponents; ++i)
				{
					const CaffUtil::Param &compParam = entDetail["components"]["component"][i];
					const std::string componentName  = compParam["name"].asStdString();
					
					if(!componentName.empty())
					{
						// Look for C++ Components
						if(m_registeredComponentFactories.count(componentName))
						{
							std::unique_ptr<CaffEnt::Component> newComp = createComponent(componentName);
                            newEntity->addComponent(newComp, compParam["params"]);
							
							continue;
						}
                        
                        ERROR_LOG("Couldn't find requested component '" + componentName + "'")
					}
					else
					{
						ERROR_LOG("Entity Detail has an un-named component.")
					}
				}
			}
			
			// Child entities.
			{
				const std::size_t numberOfChildren = entDetail["children"].numberOfMembersOfName("entity");
				
				for(std::size_t i = 0; i < numberOfChildren; ++i)
				{
					std::unique_ptr<CaffEnt::Entity> childEntity = Instantiate(entDetail["children"]["entity"][i]);
					newEntity->addChild(childEntity);
				}
			}
            
            return  newEntity;
		};
		
		// Return new entity.
		return Instantiate(entityData.details["entity"]);
	}
	
	// Couldn't find entity.
	return Entity::EntityUniquePtr();
}



std::unique_ptr<CaffEnt::Entity> EntityFactory::createInstance()
{
	Entity::EntityUniquePtr newEntity(new CaffEnt::Entity());
	
	newEntity->setWindow(m_windowModule);
	newEntity->setInput(m_inputModule);
	newEntity->setRenderer(m_rendererModule);
	newEntity->setEventManager(m_eventManagerModule); // null
	newEntity->setStateManager(m_stateManagerModule); // null
	newEntity->setCameraManager(m_cameraManagerModule);
	newEntity->setModelManager(m_modelManagerModule);
	newEntity->setTextureManager(m_textureManagerModule);
	newEntity->setTextRenderer(m_textRendererModule);
	newEntity->setPhysicsManager(m_physicsManagerModule);
	newEntity->setEntityFactory(this);
	
	return newEntity;
}



std::unique_ptr<CaffEnt::Component> EntityFactory::createComponent(const std::string &name)
{
	if(m_registeredComponentFactories.count(name))
	{
		std::unique_ptr<CaffEnt::Component> newComp(m_registeredComponentFactories.at(name)->create());
		newComp->setName(name);
		
		return newComp;
	}
	
	return std::unique_ptr<CaffEnt::Component>();
}



bool EntityFactory::registerComponent(const std::string & factoryName, ComponentFactory *factory)
{
	if(!m_registeredComponentFactories.count(factoryName))
	{
		m_registeredComponentFactories[factoryName] = factory;
		return true;
	}

	return false;
}



void EntityFactory::clearFactories()
{
	auto it = m_registeredComponentFactories.begin();
	auto end = m_registeredComponentFactories.end();
	
	for(; it != end; ++it)
	{
		delete it->second;
	}
	
	m_registeredComponentFactories.clear();
}



void EntityFactory::setPhysicsManager(CaffSys::PhysicsManager *setPhysMgr)
{
	physicsManager = setPhysMgr;
}



# pragma mark - Factorys for system components.

std::unique_ptr<Entity::Component> TransformComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::TransformComponent());
}



std::unique_ptr<Entity::Component> PhysicsComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::PhysicsComponent(physicsManager));
}



std::unique_ptr<Entity::Component> ColliderComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::ColliderComponent());
}



std::unique_ptr<Entity::Component> CameraComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::CameraComponent());
}



std::unique_ptr<Entity::Component> ModelComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::ModelComponent());
}

std::unique_ptr<Entity::Component> DirectionalLightComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::DirectionalLightComponent());
}

std::unique_ptr<Entity::Component> PointLightComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::PointLightComponent());
}

std::unique_ptr<Entity::Component> FullbrightMaterialComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::FullbrightMaterialComponent());
}

std::unique_ptr<Entity::Component> BlinnPhongMaterialComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::BlinnPhongMaterialComponent());
}





} // namespace
} // namespace
