
#ifndef CAFFEINE_SYSTEMS_ENTITY_FACTORY_INCLUDED
#define CAFFEINE_SYSTEMS_ENTITY_FACTORY_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <tinyxml2/tinyxml2.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

// Helper Macros for defining component factories. Enter the name of component
// which will produce a factory with the same name and Factory appended at the end.
// ie. COMPONENT_FACTORY_HEADER(FooComponent) = FooComponentFactory
// Warning: breaking this convention will lead to issues as name is used to identify
// The component as well as create the name.
// This one is for the interface of the factory.
#define COMPONENT_FACTORY_INTERFACE(name)					\
	class name##Factory : public CaffSys::ComponentFactory	\
	{														\
	public:													\
		explicit name##Factory() {}							\
				 ~name##Factory() {}						\
															\
		std::unique_ptr<CaffEnt::Component> create() const; \
	};														\

// Helper Macros for defining component factories. Enter the name of component
// which will produce a factory with the same name and Factory appended at the end.
// ie. COMPONENT_FACTORY_HEADER(FooComponent) = FooComponentFactory
// Warning: breaking this convention will lead to issues as name is used to identify
// The component as well as create the name.
// This one is for the soruce of the factory
#define COMPONENT_FACTORY_SOURCE(name)									\
	std::unique_ptr<CaffEnt::Component> name##Factory::create() const { \
		return CaffEnt::ComponentUniquePtr(new name);					\
		}																\


namespace Caffeine {
namespace Systems {

//struct EntityData
//{
//	time_t					xmlTimeStamp;
//	time_t					modelTimeStamp;
//	
//	std::string				xmlFileName;
//	std::string				modelFileName;
//	
//}; // struct

struct EntityComponentDetails
{
	std::string			name;
	CaffUtil::Param     *params;
};


struct EntityDetails
{
	time_t			timeStamp;

	std::string							name;
	bool								isVisible;
	std::vector<std::string>			tags;
	std::vector<EntityComponentDetails> components;
	std::vector<EntityDetails>			children;
};



class EntityFactory : private CaffUtil::NonCopyable
{
public:
	
	explicit							EntityFactory(CaffApp::Window *window, CaffApp::Input *input, CaffApp::Renderer *renderer, CaffSys::EventManager *eventManager, CaffSys::StateManager *stateManager, CaffSys::CameraManager *cameraManager, CaffSys::ModelManager *modelManager, CaffSys::TextureManager *textureManager, CaffSys::TextRenderer *textRenderer, ScriptManager *scriptMgr, CaffSys::PhysicsManager *physMgr);
	explicit							EntityFactory(CaffApp::Window *window, CaffApp::Input *input, CaffApp::Renderer *renderer, CaffSys::EventManager *eventManager, CaffSys::StateManager *stateManager, CaffSys::CameraManager *cameraManager, CaffSys::ModelManager *modelManager, CaffSys::TextureManager *textureManager, CaffSys::TextRenderer *textRenderer, ScriptManager *scriptMgr, CaffSys::PhysicsManager *physMgr, const CaffUtil::Param &params);
	
										~EntityFactory();

	void								checkForUpdates();
				
	std::unique_ptr<CaffEnt::Entity>	createInstance(const std::string &id, CaffSys::EventManager *evtMgr = nullptr, CaffSys::StateManager *stateMgr = nullptr);
	std::unique_ptr<CaffEnt::Entity>	createInstance();
	std::unique_ptr<CaffSys::State>		createState(const std::string &name);
	std::unique_ptr<CaffEnt::Component> createComponent(const std::string &name);
	
	bool								registerComponent(const std::string & factoryName, ComponentFactory *factory);
	void								clearFactories();

	inline std::size_t					numberOfFactories() const { return m_registeredComponentFactories.size(); }
	inline std::size_t					numberOfEntities() const  { return m_registeredEntities.size(); }
	
	
	// SetComponents required for special components.
	void								setPhysicsManager(CaffSys::PhysicsManager *physMgr);
	//void								setRenderer(CaffApp::Renderer *renderer);
	//void								setCameraMgr(CaffSys::CameraManager *camMgr);
	// .. other render stuff .. //
	
private:

	struct EntityData
	{
		time_t					fileTimeStamp;
		CaffUtil::Param			details;
	}; // struct

	std::map<std::string, EntityDetails>			m_registeredEntities;
	std::map<std::string, EntityData>				m_registeredEntityData;
	std::map<std::string, const ComponentFactory*>	m_registeredComponentFactories;
    
    CaffApp::Window                                 *m_windowModule;
	CaffApp::Input									*m_inputModule;
	CaffApp::Renderer								*m_rendererModule;
	CaffSys::EventManager							*m_eventManagerModule;
	CaffSys::StateManager							*m_stateManagerModule;
	CaffSys::CameraManager							*m_cameraManagerModule;
	CaffSys::ModelManager							*m_modelManagerModule;
	CaffSys::TextureManager							*m_textureManagerModule;
	CaffSys::TextRenderer							*m_textRendererModule;
	CaffSys::PhysicsManager							*m_physicsManagerModule;

}; // class



# pragma mark - Factorys for system components.

class ComponentFactory
{
public:

	ComponentFactory() {}
	virtual ~ComponentFactory() {}
	
	virtual std::unique_ptr<Entity::Component> create() const = 0;

}; // class

class TransformComponentFactory : public ComponentFactory
{
public:
	explicit TransformComponentFactory() {}
	~TransformComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class PhysicsComponentFactory : public ComponentFactory
{
public:
	explicit PhysicsComponentFactory() {}
	~PhysicsComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class ColliderComponentFactory : public ComponentFactory
{
public:
	explicit ColliderComponentFactory() {}
	~ColliderComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class CameraComponentFactory : public ComponentFactory
{
public:
	explicit CameraComponentFactory() {}
	~CameraComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class ModelComponentFactory : public ComponentFactory
{
public:
	explicit ModelComponentFactory() {}
	~ModelComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class DirectionalLightComponentFactory : public ComponentFactory
{
public:
	explicit DirectionalLightComponentFactory() {}
	~DirectionalLightComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class PointLightComponentFactory : public ComponentFactory
{
public:
	explicit PointLightComponentFactory() {}
	~PointLightComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class FullbrightMaterialComponentFactory : public ComponentFactory
{
public:
	explicit FullbrightMaterialComponentFactory() {}
	~FullbrightMaterialComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class BlinnPhongMaterialComponentFactory : public ComponentFactory
{
public:
	explicit BlinnPhongMaterialComponentFactory() {}
	~BlinnPhongMaterialComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

} // namespace
} // namespace

#endif // namespace
