
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/Param.hpp>


namespace Caffeine {
namespace Systems {


State::State(const std::string stateName, const CaffUtil::Param &params)
: m_name(stateName)
, m_evtCallback()
, m_eventMgr()
, m_stateMgr(nullptr)
, m_entities()
, m_params(params)
, m_queuedEntities()
{
	m_entities.reserve(10); // reserves from params
    m_newEntities.reserve(10); // reserves from params
	m_queuedEntities.reserve(10);
	
	m_evtCallback = std::bind(&State::onEvent, this, std::placeholders::_1);
}

    
bool State::onEvent(const CaffSys::Event &data)
{
    return false;
}


void State::onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID key)
{
	for(auto &ent : m_entities)
	{
		ent->onKeyboardChange(state, key);
	}
}



void State::onTextStream(const std::string &text)
{
	for(auto &ent : m_entities)
	{
		ent->onTextStream(text);
	}
}


void State::updateEntities(const float dt)
{
    // Remove deleted objects.
    {
        auto it = m_entities.begin();
        
        while(it != m_entities.end())
        {
            CaffEnt::Entity *entity = (*it).get();
            
            if(entity->isDestroyed())
            {
                entity->onEnd();
                it = m_entities.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    
    // Add new entities
    if(!m_newEntities.empty())
    {
        for(auto &newEntity : m_newEntities)
        {
            //newEntity->onStart();
            m_entities.push_back(std::move(newEntity));
        
            // Will the next push cause an allocation.
            #ifndef NDEBUG
            if(m_entities.size() == m_entities.capacity())
            {
				CaffUtil::LogWarning("State - Entity container running out of space " + CaffUtil::ToString(m_entities.capacity()) + ", next push will cause reallocation.");
            }
            #endif
        }
        
        m_newEntities.clear();
    }
	
	// Deliver queued msgs
	{
		m_eventMgr.deliverQueuedEvents();
	}
	
	// Update objects.
	auto thinkHook = [](std::vector<std::unique_ptr<CaffEnt::Entity > > &container, void (CaffEnt::Entity::*hook)(const float), const float dt)
	{
		for(auto it = container.begin(); it != container.end(); ++it)
		{	
			if((*it)->isActive())
			{
				((*it).get()->*hook)(dt);
			}
		}
	};
	
    thinkHook(m_entities, &CaffEnt::Entity::onSuperEarlyThink,	dt);
	thinkHook(m_entities, &CaffEnt::Entity::onEarlyThink,		dt);
	thinkHook(m_entities, &CaffEnt::Entity::onThink,			dt);
	thinkHook(m_entities, &CaffEnt::Entity::onLateThink,		dt);
	thinkHook(m_entities, &CaffEnt::Entity::onSuperLateThink,	dt);
	
	// Render
	{
		for(auto &entity : m_entities)
		{
			if(entity->isActive())
			{
				entity->onRender();
			}
		}
	}
}

    

#pragma mark - Entity Management

CaffEnt::Entity* State::pushNewEntity(std::unique_ptr<CaffEnt::Entity> addEntity)
{
	assert(addEntity);

	m_newEntities.push_back(std::move(addEntity));

	CaffEnt::Entity &newEntity = *m_newEntities.back().get();

	newEntity.setState(this);
	newEntity.setEventManager(&m_eventMgr);
    newEntity.onStart();
	
	return m_newEntities.back().get();
}


    
CaffEnt::Entity* State::pushNewEntity(const std::string &name)
{
    std::unique_ptr<CaffEnt::Entity> newEntity = getStateManager().getEntityFactory().createInstance(name, &getEventManager(), &getStateManager());
        
    if(newEntity)
    {
        return pushNewEntity(std::move(newEntity));
    }
    else
    {
        CaffUtil::LogError("State - Couldn't push new entity  '" + name + "'");
		return nullptr;
    }
}
    


CaffEnt::Entity* State::getFirstEntityByTag(const std::string &tag)
{
	for(auto &e : m_entities)
	{
		if(e->hasTag(tag))
		{
			return e.get();
		}
	}
	
	return nullptr;
}



std::vector<CaffEnt::Entity*> State::getEntitiesByTag(const std::string &tag)
{
	std::vector<CaffEnt::Entity*> taggedEntities;
	
	const uint16_t VEC_HINT = 4;
	taggedEntities.reserve(VEC_HINT);

	for(auto &e : m_entities)
	{
		if(e->hasTag(tag))
		{
			taggedEntities.push_back(e.get());
		}
	}
	
	return taggedEntities;
}



CaffEnt::Entity* State::getFirstEntityByName(const std::string &name)
{
	for(auto &e : m_entities)
	{
		if(e->getName() == name)
		{
			return e.get();
		}
	}
	
	return nullptr;
}



std::vector<CaffEnt::Entity*> State::getEntitiesByName(const std::string &name)
{
	std::vector<CaffEnt::Entity*> namedEntities;
	
	const uint16_t VEC_HINT = 4;
	namedEntities.reserve(VEC_HINT);

	for(auto &e : m_entities)
	{
		if(e->getName() == name)
		{
			namedEntities.push_back(e.get());
		}
	}
	
	return namedEntities;
}



CaffEnt::Entity* State::getEntityByID(const std::size_t id)
{
	for(auto &e : m_entities)
	{
		if(e->getID() == id)
		{
			return e.get();
		}
	}
	
	return nullptr;
}



std::unique_ptr<CaffEnt::Entity> State::removeEntityByID(const std::size_t id)
{
	//for(auto &e : m_entities)
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if((*it)->getID() == id)
		{
			std::unique_ptr<CaffEnt::Entity> removeEnt = std::move(*it);
			m_entities.erase(it);
			
			return std::move(removeEnt);
		}
	}
	
	return std::unique_ptr<CaffEnt::Entity>();
}


    
namespace
{
    inline void ProcessParams(State &state, const CaffUtil::Param &paramNode)
    {
        const std::size_t createEntities = paramNode.numberOfMembersOfName("instantiate_entity");
        
        for(std::size_t i = 0; i < createEntities; ++i)
        {
            CaffUtil::LogInfo("State - Instantiated entity \"" + paramNode["instantiate_entity"][i]["name"].asStdString() + "\".");
			
			const std::string entityName = paramNode["instantiate_entity"][i]["name"].asStdString();
			
			if(!entityName.empty())
			{
				std::unique_ptr<CaffEnt::Entity> newEntity = state.getStateManager().getEntityFactory().createInstance(entityName, &state.getEventManager(), &state.getStateManager());
				state.pushNewEntity(std::move(newEntity));
			}
        }
    }
}


    
void State::onCreate()
{
    ProcessParams(*this, m_params["on_create"]);
}



void State::onFocus()
{
    ProcessParams(*this, m_params["on_focus"]);
}



void State::onReFocus()
{
    ProcessParams(*this, m_params["on_re_focus"]);
}



void State::onDeFocus()
{
    ProcessParams(*this, m_params["on_de_focus"]);
}



void State::onEnd()
{
    ProcessParams(*this, m_params["on_end"]);
}



} // namespace
} // namespace
