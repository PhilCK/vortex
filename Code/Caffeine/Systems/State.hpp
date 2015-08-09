
#ifndef CAFFEINE_SYSTEMS_STATE_INCLUDED
#define CAFFEINE_SYSTEMS_STATE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <string>
#include <vector>


namespace Caffeine {
namespace Systems {


/*
	State
	-----
	Base class for game states.
	
	Inherit and override the virutal functions.
	The internal ones are mostly for Logging and adding aditional
	functionality.
 */

#define ENTITY_LIST_STD_VEC_HINT 256
#define NEW_ENTITY_LIST_STD_VEC_HINT 8


class State : private CaffUtil::NonCopyable
{

friend class StateManager;

public:

	explicit							State(const std::string stateName, const CaffUtil::Param &params = CaffUtil::Param());
	virtual								~State() {}
	
	#pragma mark - Internal State Interactions
	
	bool                                onEvent(const CaffSys::Event &data);
	void								onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID key);
	void								onTextStream(const std::string &text);
    
	void                                updateEntities(const float dt);
	
	inline std::string                  getName() const			 { return m_name; }
	
	inline StateManager &               getStateManager() const		{ assert(m_stateMgr); return *m_stateMgr; }
	inline EventManager &               getEventManager()			{ return m_eventMgr; }
	
	#pragma mark - Entity Management
	
	// Add an entity to the state.
	CaffEnt::Entity*					pushNewEntity(std::unique_ptr<CaffEnt::Entity> addEntity);
    CaffEnt::Entity*					pushNewEntity(const std::string &name);
	
	CaffEnt::Entity*					getFirstEntityByTag(const std::string &tag);
	std::vector<CaffEnt::Entity*>		getEntitiesByTag(const std::string &tag);
	
	CaffEnt::Entity*					getFirstEntityByName(const std::string &name);
	std::vector<CaffEnt::Entity*>		getEntitiesByName(const std::string &name);
	
	CaffEnt::Entity*					getEntityByID(const std::size_t id);
	std::unique_ptr<CaffEnt::Entity>	removeEntityByID(const std::size_t id);
	
	inline std::size_t					numberOfEntitiesInState() const { return m_entities.size(); }
	
private:
    
    void                                onCreate();
    void                                onFocus();
    void                                onReFocus();
    void                                onDeFocus();
	void                                onEnd();
    

	inline void                         setStateManager(StateManager *stateMgr) { assert(stateMgr); m_stateMgr = stateMgr; }
	
private:

	const std::string					m_name;
	
	CaffSys::EventCallback				m_evtCallback;
	EventManager						m_eventMgr;
	StateManager						*m_stateMgr;
	
	std::vector<std::unique_ptr<CaffEnt::Entity> > m_entities;
    std::vector<std::unique_ptr<CaffEnt::Entity> > m_newEntities;
    
    const CaffUtil::Param				m_params;
    std::vector<std::string>			m_queuedEntities;
	
}; // class


} // namespace
} // namespace


#endif // include guard
