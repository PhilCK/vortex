
#ifndef CAFFEINE_ENTITY_ENTITY_INCLUDED
#define CAFFEINE_ENTITY_ENTITY_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <assert.h>


namespace Caffeine {
namespace Entity {



class EntityObserver
{
public:

	explicit EntityObserver(const uint32_t entityID);

	bool isValid();
	
	CaffEnt::Entity* operator->();
	
private:

	const uint32_t m_entityID;
	CaffEnt::Entity *m_entity = nullptr;

};



typedef Entity*						EntityPtr;
typedef std::unique_ptr<Entity>		EntityUniquePtr;
    
// TODO: is this internal still required?
enum class EntityChangeAction       { COMPONENT_ATTACH, COMPONENT_DISATTACH, ENTITY_ADDED, ENTITY_REMOVED };

/*
	Entity
	------
	Entities form the game objects, they are not to be inheritied from.
	UniquePtrs are used extensivly here, an Entity will own its child entities.
	it will also own its components. You can remove child enties and components
	and you will get a UniquePtrs in return that you can attach to other entities.
	
	Entities contain some sugar methods to access common components, these will return 
	nullptr if they are not added to the entity. As the renderable component is inherited
	only the first attached component will be visible in the sugar.
*/

struct ColliderDetails
{
	Entity* collidedWith;
	int test;
	// normal etc.
};

class Entity final
{
public:
    
    explicit						Entity(const std::string & name = "");
									~Entity();
	
	
	#pragma mark - General Entity Settings
	
	// Name of the entity.
    std::string						getName() const;
	void							setName(const std::string & setName);
	
	inline std::size_t				getID() const { return m_entityID; }
	inline void						setID(const std::size_t id) { m_entityID = id; }
	
	// Tagging can be used to quickly identify an object.
	void							addTag(const std::string &tag);
	bool							hasTag(const std::string &tag) const;
	void							clearTags();
	void							removeTag(const std::string &tag);
	inline std::vector<std::string>	getTags() const { return m_tags; }
	
	// Inactive objects will not have their hooks called.
	bool							isActive() const;
	void							setActive(const bool isActive);
	
	
	// Destroy will render an entity unuseable, however it may not be removed from memory
	// straight away.
	inline void						destroy()			{ m_isDestroyed = true; }
	inline bool						isDestroyed() const { return m_isDestroyed; }
	
    
private:
    
    #pragma mark - Useage Hooks
    
    friend class ::CaffSys::State;
    friend class ::CaffSys::PhysicsManager;
    // onEvent needs a friend I think.
	
	void							onStart();
	void							onEnd();
    
	void							onAttachToEntity();
	void							onDisattachFromEntity();
    
    void                            onAwake();
    void                            onSleep();
	
	// Access to the components and subentity update functions.
    void                            onSuperEarlyThink(const float dt);
	void							onEarlyThink(const float dt);
	void							onThink(const float dt);
	void							onLateThink(const float dt);
    void                            onSuperLateThink(const float dt);
	
public:
    
    // public so that direct events can be sent.
	bool							onEvent(const CaffSys::Event &data);
    
private:
	
	// Access to the components render fuctions. TODO: This is in alot flux.
	void							onPreRender(); // TODO: Unsure how this is goint to work atm.
	void							onRender();
	void							onPostRenderer(); // TODO: Unsure how this is going to work atm.
	
	void							onTextStream(const std::string &text);
	void							onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID id);
	
	void							onCollision(const ColliderDetails &details);
	void							onTrigger(); // TODO: DO
	
	void							onStateChange(); // TODO: DO
	
	void							onApplicationExit(); // TODO: DO
	void							onApplicationLostFocus(); // TODO: DO
	void							onApplicationGainedFocus(); // TODO: DO
    
    void                            onEntityChange(const EntityChangeAction changeAction);
	
public:
	
	#pragma mark - System Modules
	
	inline CaffApp::Window&         getWindow() const										{ assert(m_windowPtr); return *m_windowPtr;					}
	void							setWindow(CaffApp::Window *setWindow);

	inline CaffApp::Input&			getInput() const										{ assert(m_inputPtr); return *m_inputPtr;					}
	void							setInput(CaffApp::Input *setInput);

	inline CaffApp::Renderer&		getRenderer() const										{ assert(m_rendererPtr); return *m_rendererPtr;				}
	void							setRenderer(CaffApp::Renderer *setRenderer);
		
	inline CaffSys::EventManager&	getEventManager() const									{ assert(m_eventManagerPtr); return *m_eventManagerPtr;		}
	void							setEventManager(CaffSys::EventManager *evtMgr);
		
	inline CaffSys::StateManager&	getStateManager() const									{ assert(m_stateManagerPtr); return *m_stateManagerPtr;		}
	void							setStateManager(CaffSys::StateManager *stateMgr);
		
	inline CaffSys::State*			getState() const										{ return m_ownedState;										}
	void							setState(CaffSys::State *state);
		
	inline CaffSys::CameraManager&	getCameraManager() const								{ assert(m_cameraManagerPtr); return *m_cameraManagerPtr;	}
	void							setCameraManager(CaffSys::CameraManager *camMgr);
		
	inline CaffSys::ModelManager&	getModelManager() const									{ assert(m_modelManagerPtr); return *m_modelManagerPtr;		}
	void							setModelManager(CaffSys::ModelManager *modelMgr);
		
	inline CaffSys::TextureManager&	getTextureManager() const								{ assert(m_textureManagerPtr); return *m_textureManagerPtr; }
	void							setTextureManager(CaffSys::TextureManager *textureMgr);
		
	inline CaffSys::TextRenderer&	getTextRenderer() const									{ assert(m_textRendererPtr); return *m_textRendererPtr;		}
	void							setTextRenderer(CaffSys::TextRenderer *textRendererMgr);
		
	inline CaffSys::EntityFactory&  getEntityFactory() const								{ assert(m_entityFactoryPtr); return *m_entityFactoryPtr;	}
	void							setEntityFactory(CaffSys::EntityFactory *entityMgr);
		
	inline CaffSys::PhysicsManager& getPhysicsManager() const								{ assert(m_physicsManagerPtr); return *m_physicsManagerPtr; }
	void							setPhysicsManager(CaffSys::PhysicsManager *entityMgr);
	
	
	#pragma mark - Relationships
	
	// Returns the attached parent of a given entity, nullptr if none.
	inline EntityPtr				getParent() const { return m_parent; }
    EntityPtr                       getRootParent() const;
	
	// Adds a child entity to the hierachy, it will take ownership of the object.
	void							addChild(EntityUniquePtr & setChild);
	
	// Removes a child from the hierachy, takes a string or a pointer. Returns a unique ptr
	// so it can be attached to another entity, or nullptr if none is found.
	EntityUniquePtr					removeChild(const std::string & childName);
	EntityUniquePtr					removeChild(const EntityPtr childPtr);
	
	// Will find the first instance of a child with the same name, nullptr if none.
	EntityPtr						findChild(const std::string & name) const;
    
    // Will find a child based on its index.
    inline EntityPtr                findChild(const std::size_t & i) const { return m_children.at(i).get(); }
	
	inline std::size_t				numberOfChildren() const { return m_children.size(); }
	
	
	#pragma mark - Components
	
	// Adds a component. Only one component per type can be attached per entity.
	bool							addComponent(const std::string &component, const CaffUtil::Param &params = CaffUtil::Param("EMPTY"));
	void							addComponent(ComponentUniquePtr & setComponent, const CaffUtil::Param &params = CaffUtil::Param("EMPTY"));
	
	// Removes a component, returns a unique ptr so it can be attached to another entity,
	// or nullptr if none is found.
	template<typename T>
	ComponentUniquePtr				removeComponent(const ComponentPtr componentPtr)
	{
		assert(componentPtr);
		
		// Remove child.
		auto it  = m_components.begin();
		auto end = m_components.end();
		
		for(; it != end; ++it)
		{
			if((*it).get() == componentPtr)
			{
                componentPtr->onDisattach();
				ComponentUniquePtr component = std::move(*it);
				m_components.erase(it);
                
                onEntityChange(EntityChangeAction::COMPONENT_DISATTACH);
				
				
				return component;
			}
		}
		
		return ComponentUniquePtr();

	}
	
	// Gets the component of requested type, returns nullptr if none is found.
	CaffEnt::Component *			getComponentByName(const std::string &name) const;
			
	template<typename T>
	T*								getComponent() const
	{
		const std::string componentID = T::getTypeID();
		
		auto it  = m_components.begin();
		auto end = m_components.end();
		
		for(; it != end; ++it)
		{
			if((*it)->getInstTypeID() == componentID)
			{
				return static_cast<T*>((*it).get());
			}
		}
		
		return nullptr;
	}
    
	inline std::size_t				numberOfComponents() const { return m_components.size(); }
	
private:

	EntityPtr						m_parent;
	
	std::vector<std::string>		m_tags;
	
	std::queue<EntityUniquePtr>		m_childQueue;
	std::queue<EntityUniquePtr>		m_childRemovealQueue;
    std::vector<EntityUniquePtr> 	m_children;
	
	std::queue<ComponentUniquePtr>	m_componentQueue;
	std::queue<ComponentUniquePtr>	m_componentRemovealQueue;
    std::vector<ComponentUniquePtr>	m_components;
    
    
	
	std::string						m_name;
	bool							m_isActive;
	bool							m_isDestroyed;
	
	std::size_t						m_entityID;
	
	CaffApp::Window					*m_windowPtr;
	CaffApp::Input					*m_inputPtr;
	CaffApp::Renderer				*m_rendererPtr;
	CaffSys::EventManager			*m_eventManagerPtr;
	CaffSys::StateManager			*m_stateManagerPtr;
	CaffSys::CameraManager			*m_cameraManagerPtr;
	CaffSys::ModelManager			*m_modelManagerPtr;
	CaffSys::TextureManager			*m_textureManagerPtr;
	CaffSys::TextRenderer			*m_textRendererPtr;
	CaffSys::EntityFactory			*m_entityFactoryPtr;
	CaffSys::PhysicsManager			*m_physicsManagerPtr;
	CaffSys::State					*m_ownedState;
	
}; // class
    
} // namespace
} // namespace

#endif // include guard
