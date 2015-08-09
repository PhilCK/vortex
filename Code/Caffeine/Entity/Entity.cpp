
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>

#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

#include <Caffeine/Utilities/Param.hpp>


namespace Caffeine {
namespace Entity {


Entity::Entity(const std::string & name)
: m_parent(nullptr)
, m_tags()
, m_children()
, m_components()
, m_name(name)
, m_isActive(true)
, m_isDestroyed(false)
, m_entityID(0)
, m_windowPtr(nullptr)
, m_inputPtr(nullptr)
, m_rendererPtr(nullptr)
, m_eventManagerPtr(nullptr)
, m_stateManagerPtr(nullptr)
, m_cameraManagerPtr(nullptr)
, m_modelManagerPtr(nullptr)
, m_textureManagerPtr(nullptr)
, m_textRendererPtr(nullptr)
, m_entityFactoryPtr(nullptr)
, m_ownedState(nullptr)
{
	// Vector Setup
	{
		const std::size_t CHILD_STD_VEC_HINT(4);
		const std::size_t TAGS_STD_VEC_HINT(4);
		const std::size_t COMP_STD_VEC_HINT(8);

		m_tags.reserve(TAGS_STD_VEC_HINT);
		m_children.reserve(CHILD_STD_VEC_HINT);
		m_components.reserve(COMP_STD_VEC_HINT);
	}
}



Entity::~Entity()
{
}



#pragma mark - General Entity Settings

std::string Entity::getName() const
{
	if(!m_name.empty())
	{
		return m_name;
	}
	else
	{
		return m_parent ? "Sub-Entity" : "Entity";
	}
}



void Entity::setName(const std::string & setName)
{
	m_name = setName;
}



bool Entity::isActive() const
{
	if(m_isDestroyed)
	{
		return false;
	}
	
	return m_isActive;
}



void Entity::setActive(const bool isActive)
{
    isActive ? onAwake() : onSleep();
    
	m_isActive = isActive;
}



void Entity::addTag(const std::string &tag)
{
	if(!hasTag(tag))
	{
		m_tags.push_back(tag);
	}
}



bool Entity::hasTag(const std::string &tag) const
{
	auto it = std::find(m_tags.begin(), m_tags.end(), tag);
	
	return (it != m_tags.end());
}



void Entity::clearTags()
{
	m_tags.clear();
}



void Entity::removeTag(const std::string &tag)
{
	auto it = std::find(m_tags.begin(), m_tags.end(), tag);
	
	if(it != m_tags.end())
	{
		m_tags.erase(it);
	}
}



#pragma mark - Useage Hooks

namespace
{
	// Updates hooks that take no paramaters.
	template<typename S, typename T>
	void GenericOnHook(S &container, void (T::*hook)(void))
	{
		//for(auto it = container.begin(); it != container.end(); ++it)
		auto it = container.begin();
		while(it != container.end())
		{
			if((*it) && (*it)->isActive())
			{
				((*it).get()->*hook)();
			}
			
			++it;
		}
	}

	// Updates hooks that take deltatime
	template<typename S, typename T, typename V>
	void OnThinkHook(S &container, void (T::*hook)(const V), const V val)
	{
		for(auto it = container.begin(); it != container.end(); ++it)
		{
			if((*it)->isActive())
			{
				((*it).get()->*hook)(val);
			}
		}
	}
	
	
	template<typename S, typename T, typename V>
	void OnOneRefHook(S &container, void (T::*hook)(const V&), const V val)
	{
		for(auto it = container.begin(); it != container.end(); ++it)
		{
			if((*it)->isActive())
			{
				((*it).get()->*hook)(val);
			}
		}
	}
	
	
	template<typename S, typename T, typename U, typename V>
	void OnTwoArgHook(S &container, void (T::*hook)(const U, const V), const U val1, const V val2)
	{
		for(auto it = container.begin(); it != container.end(); ++it)
		{
			if((*it)->isActive())
			{
				((*it).get()->*hook)(val1, val2);
			}
		}
	}
	

	// Updates hooks that take deltatime
	template<typename S, typename T>
	bool OnEventHook(S &container, bool (T::*hook)(const CaffSys::Event &), const CaffSys::Event &data)
	{
		bool swallowed = false;
		
		for(auto it = container.begin(); it != container.end(); ++it)
		{
			if((*it)->isActive())
			{
				swallowed = ((*it).get()->*hook)(data);
				
				if(swallowed)
				{
					return swallowed; // return true;
				}
			}
		}
		
		return swallowed;
	}
	
	// Sorts out queue.
	template<typename S, typename T, typename U>
	void ProcessQueue(S &queue, T &container, void(U::*hook)())
	{
		const std::size_t length = (container.size());
		
		while(!queue.empty())
		{
			container.push_back(std::move(queue.front()));
			queue.pop();
		}
		
		auto it = container.begin() + length;
		
		// Call hook
		for(; it != container.end(); ++it)
		{
			((*it).get()->*hook)();
		}
	}
}



void Entity::onStart()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);
	
	//GenericOnHook(m_components, &Component::onStart);
	//GenericOnHook(m_children,   &Entity::onStart);
}



void Entity::onEnd()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

	GenericOnHook(m_components, &Component::onEnd);
	GenericOnHook(m_children,   &Entity::onEnd);
}



void Entity::onAttachToEntity()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

    if(isActive())
	{
		GenericOnHook(m_components, &Component::onAttachToEntity);
		GenericOnHook(m_children,   &Entity::onAttachToEntity);
	}
}



void Entity::onDisattachFromEntity()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

	if(isActive())
	{
		GenericOnHook(m_components, &Component::onDisattachFromEntity);
		GenericOnHook(m_children,   &Entity::onDisattachFromEntity);
	}
}
    

    
void Entity::onAwake()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

    if(isActive())
    {
        GenericOnHook(m_components, &Component::onAwake);
        GenericOnHook(m_children,   &Entity::onAwake);
    }
}
    
    
    
void Entity::onSleep()
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

    if(isActive())
    {
        GenericOnHook(m_components, &Component::onSleep);
        GenericOnHook(m_children,   &Entity::onSleep);
    }
}

    
void Entity::onSuperEarlyThink(const float dt)
{
    ProcessQueue(m_componentQueue, m_components, &Component::onStart);
    ProcessQueue(m_childQueue, m_children, &Entity::onStart);
    
    if(isActive())
    {
        OnThinkHook(m_components, &Component::onSuperEarlyThink, dt);
        OnThinkHook(m_children,   &Entity::onSuperEarlyThink,    dt);
    }
}


void Entity::onEarlyThink(const float dt)
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

	if(isActive())
	{
		OnThinkHook(m_components, &Component::onEarlyThink, dt);
		OnThinkHook(m_children,   &Entity::onEarlyThink,    dt);
	}
}



void Entity::onThink(const float dt)
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

	if(isActive())
	{
		OnThinkHook(m_components, &Component::onThink, dt);
		OnThinkHook(m_children,   &Entity::onThink,    dt);
	}
}



void Entity::onLateThink(const float dt)
{
	if(isActive())
	{
		OnThinkHook(m_components, &Component::onLateThink, dt);
		OnThinkHook(m_children,   &Entity::onLateThink,	   dt);
	}
	
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);
}
    
    
    
void Entity::onSuperLateThink(const float dt)
{
    if(isActive())
    {
        OnThinkHook(m_components, &Component::onSuperLateThink, dt);
        OnThinkHook(m_children,   &Entity::onSuperLateThink,	dt);
    }
    
    ProcessQueue(m_componentQueue, m_components, &Component::onStart);
    ProcessQueue(m_childQueue, m_children, &Entity::onStart);
}



void Entity::onTextStream(const std::string &text)
{
	if(isActive())
    {
        OnOneRefHook(m_components, &Component::onTextStream, text);
        OnOneRefHook(m_children,   &Entity::onTextStream, text);
    }
    
    ProcessQueue(m_componentQueue, m_components, &Component::onStart);
    ProcessQueue(m_childQueue, m_children, &Entity::onStart);
}



void Entity::onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID id)
{
    if(isActive())
    {
        OnTwoArgHook(m_components, &Component::onKeyboardChange, state, id);
        OnTwoArgHook(m_children,   &Entity::onKeyboardChange, state, id);
    }
    
    ProcessQueue(m_componentQueue, m_components, &Component::onStart);
    ProcessQueue(m_childQueue, m_children, &Entity::onStart);
}



bool Entity::onEvent(const CaffSys::Event &data)
{
	ProcessQueue(m_componentQueue, m_components, &Component::onStart);
	ProcessQueue(m_childQueue, m_children, &Entity::onStart);

	if(isActive())
	{
		if(!OnEventHook(m_components, &Component::onEvent, data))
		{
			return OnEventHook(m_children, &Entity::onEvent, data);
		}
		else
		{
			return true; // event got swallowed.
		}
	}
	
	return false;
}



void Entity::onPreRender()
{
}



void Entity::onRender()
{
	if(isActive())
	{
		GenericOnHook(m_components, &Component::onRender);
		GenericOnHook(m_children,	&Entity::onRender);
	}
}



void Entity::onPostRenderer()
{
}



void Entity::onCollision(const ColliderDetails &details)
{
	if(isActive())
	{
		OnOneRefHook(m_components,	&Component::onCollision, details);
		OnOneRefHook(m_children,	&Entity::onCollision,	 details);
	}
}
    
	
    
void Entity::onEntityChange(const EntityChangeAction changeAction)
{
	if(isActive())
	{
		for(auto &comp : m_components)
		{
			comp->onEntityChange(changeAction);
		}
		
		for(auto &child : m_children)
		{
			child->onEntityChange(changeAction);
		}
	}
}



#pragma mark - System Modules

namespace
{
	template<typename S, typename T>
	void SetChildren(CaffEnt::Entity *entity, void (S::*hook)(T*), T *element)
	{
		assert(entity);
		
		for(std::size_t i = 0; i < entity->numberOfChildren(); ++i)
		{
			CaffEnt::Entity *child = entity->findChild(i);
		
			(child->*hook)(element);
		}
	}
}

void Entity::setWindow(CaffApp::Window *setWindow)						{ SetChildren(this, &Entity::setWindow, setWindow);				m_windowPtr = setWindow;			}
void Entity::setInput(CaffApp::Input *setInput)							{ SetChildren(this, &Entity::setInput, setInput);				m_inputPtr = setInput;				}
void Entity::setRenderer(CaffApp::Renderer *setRenderer)				{ SetChildren(this, &Entity::setRenderer, setRenderer);			m_rendererPtr = setRenderer;		}
void Entity::setEventManager(CaffSys::EventManager *evtMgr)				{ SetChildren(this, &Entity::setEventManager, evtMgr);			m_eventManagerPtr = evtMgr;			}
void Entity::setStateManager(CaffSys::StateManager *stateMgr)			{ SetChildren(this, &Entity::setStateManager, stateMgr);		m_stateManagerPtr = stateMgr;		}
void Entity::setState(CaffSys::State *state)							{ SetChildren(this, &Entity::setState, state);					m_ownedState = state;				}
void Entity::setCameraManager(CaffSys::CameraManager *camMgr)			{ SetChildren(this, &Entity::setCameraManager, camMgr);			m_cameraManagerPtr = camMgr;		}
void Entity::setModelManager(CaffSys::ModelManager *modelMgr)			{ SetChildren(this, &Entity::setModelManager, modelMgr);		m_modelManagerPtr = modelMgr;		}
void Entity::setTextureManager(CaffSys::TextureManager *textureMgr)		{ SetChildren(this, &Entity::setTextureManager, textureMgr);	m_textureManagerPtr = textureMgr;	}
void Entity::setTextRenderer(CaffSys::TextRenderer *textRendererMgr)	{ SetChildren(this, &Entity::setTextRenderer, textRendererMgr);	m_textRendererPtr = textRendererMgr;}
void Entity::setEntityFactory(CaffSys::EntityFactory *entityMgr)		{ SetChildren(this, &Entity::setEntityFactory, entityMgr);		m_entityFactoryPtr = entityMgr;		}
void Entity::setPhysicsManager(CaffSys::PhysicsManager *physMgr)		{ SetChildren(this, &Entity::setPhysicsManager, physMgr);		m_physicsManagerPtr = physMgr;		}

    

#pragma mark - Relationships
    
    
EntityPtr Entity::getRootParent() const
{
    if(getParent())
    {
        return getParent()->getRootParent();
    }

    return const_cast<Entity*>(this);
}
    
    


void Entity::addChild(EntityUniquePtr & setChild)
{
	setChild->m_parent = this;
	setChild->setWindow(m_windowPtr);
	setChild->setInput(m_inputPtr);
	setChild->setRenderer(m_rendererPtr);
	setChild->setEventManager(m_eventManagerPtr);
	setChild->setStateManager(m_stateManagerPtr);
	setChild->setState(m_ownedState);
	setChild->setCameraManager(m_cameraManagerPtr);
	setChild->setModelManager(m_modelManagerPtr);
	setChild->setTextureManager(m_textureManagerPtr);
	setChild->setTextRenderer(m_textRendererPtr);
	setChild->setEntityFactory(m_entityFactoryPtr);
	setChild->setPhysicsManager(m_physicsManagerPtr);
	
	m_children.push_back(std::move(setChild));
	m_children.back()->onAttachToEntity();
    onEntityChange(EntityChangeAction::ENTITY_ADDED);
}



EntityUniquePtr Entity::removeChild(const std::string &childName)
{
	return removeChild(findChild(childName));
}



EntityUniquePtr Entity::removeChild(const EntityPtr childPtr)
{
	assert(childPtr);
	
	// Remove child.
	auto it  = m_children.begin();
	auto end = m_children.end();
	
	for(; it != end; ++it)
	{
		if((*it).get() == childPtr)
		{
			EntityUniquePtr child = std::move(*it);
			child->m_parent = nullptr;
			
			m_children.erase(it);
            
            onEntityChange(EntityChangeAction::ENTITY_REMOVED);
			
			return child;
		}
	}
	
	return EntityUniquePtr();
}



EntityPtr Entity::findChild(const std::string &name) const
{
	auto it  = m_children.begin();
	auto end = m_children.end();
	
	for(; it != end; ++it)
	{
		if((*it)->getName() == name)
		{
			return (*it).get();
		}
	}

	return nullptr;
}



#pragma mark - Components

bool Entity::addComponent(const std::string &component, const CaffUtil::Param &params)
{
	assert(m_entityFactoryPtr);
	
	std::unique_ptr<CaffEnt::Component> newComp = m_entityFactoryPtr->createComponent(component);
	
	if(newComp)
	{
		addComponent(newComp);
		return true;
	}
	
	return false;
}


    
void Entity::addComponent(ComponentUniquePtr &setComponent, const CaffUtil::Param &params)
{
	m_componentQueue.push(std::move(setComponent));
	m_componentQueue.back()->setOwner(this);
	m_componentQueue.back()->onParams(params);
	m_componentQueue.back()->onAttach();
	
    onEntityChange(EntityChangeAction::COMPONENT_ATTACH);
}



CaffEnt::Component * Entity::getComponentByName(const std::string &name) const
{
	for(auto &comp : m_components)
	{
		if(comp->getName() == name)
		{
			return comp.get();
		}
	}
	
	return nullptr;
}


   
} // namespace
} // namespace
