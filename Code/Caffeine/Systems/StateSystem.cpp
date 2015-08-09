
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Utilities/ParamNode.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <assert.h>


namespace Caffeine {
namespace Systems {


StateManager::StateManager(const std::string &stateDir, const std::string &stateExt, CaffSys::EntityFactory *entityFactory)
: m_states()
, m_pendingState(nullptr)
, m_pendingPop(false)
, m_stateInfo()
, m_entityFactory(entityFactory)
{
	// Vectors
	{
		const uint8_t STATES_STD_VEC_HINT(6);
		const uint8_t PENDING_STATES_STD_VEC_HINT(2);
		
		m_states.reserve(STATES_STD_VEC_HINT);
		m_pendingStates.reserve(PENDING_STATES_STD_VEC_HINT);
	}
    
    // Get State Desc's
    {
        const std::string path = CaffSys::Properties::GetApplicationDir() + stateDir;
        
        std::vector<std::string> filenames;
        
        Utilities::GetFileNamesOfTypeInDirectory(filenames, stateExt, path);
        
        for(const auto &file : filenames)
        {
            const CaffUtil::Param stateDetail(file, CaffUtil::Param::FileType::XML);
            
            const std::string defaultedName = "GameState_" + CaffUtil::ToString(m_stateInfo.size());
            const std::string stateName     = stateDetail["state"]["name"].asStdString(defaultedName);
            
            m_stateInfo[stateName] = stateDetail["state"];
        }
    }
}



StateManager::StateManager(const CaffUtil::Param &param, CaffSys::EntityFactory *entityFactory)
: StateManager(param["directory"].asStdString("States"), param["extension"].asStdString("xml"), entityFactory)
{
    
}
    


StateManager::~StateManager()
{
}



State* StateManager::find(const std::string &stateName)
{
	State* foundState = nullptr;
	
	for(auto &state : m_states)
	{
		if(state->getName() == stateName)
		{
			foundState = state.get();
			break;
		}
	}
	
	return foundState;
}



bool StateManager::processPending()
{
	bool hasStateChanged = false;

	// Pop current state.
	if(m_pendingPop)
	{
		DEBUG_LOG("StateManager: Popping State - " + m_states.back()->getName());
		
		m_states.back()->onEnd();
		m_states.pop_back();
		
		m_pendingPop = false;
		
		if(!m_states.empty() && !m_pendingState)
		{
			DEBUG_LOG("StateManager: Reloading State - " + m_states.back()->getName());
		}
		
		hasStateChanged = true;
	}
	
	// Push next state.
	if(m_pendingState)
	{
		if(!m_states.empty() && !hasStateChanged)
		{
			m_states.back()->onDeFocus(); // TODO: Process on_exit in state.
		}
	
		m_states.push_back(std::move(m_pendingState));
		m_states.back()->onFocus(); // TODO: Process on_start in state.
		
		hasStateChanged = true;
		
		DEBUG_LOG("StateManager: Pushing State - " + m_states.back()->getName());
	}
	else if(hasStateChanged)
	{
		m_states.back()->onFocus();
	}
	
	// New state has entered
	if(hasStateChanged)
	{
		if(!m_states.empty())
		{
			//m_states.back()->onFocus();
		}
	}
	
	return hasStateChanged;
}



void StateManager::pushState(std::unique_ptr<State> pushState)
{
	assert(pushState);
	pushState->setStateManager(this);
	
    pushState->onCreate();
	m_pendingState = std::move(pushState);
}



void StateManager::pushState(const std::string &stateName)
{
    if(m_stateInfo.count(stateName))
    {
        const CaffUtil::Param &stateParam = m_stateInfo[stateName];
        
        std::unique_ptr<CaffSys::State> newState(new State(stateParam["name"].asStdString("none"), stateParam));
        pushState(std::move(newState));
    }
    else
    {
        WARNING_LOG("StateManager: Failed to find state - " + stateName)
    }
}


} // namespace
} // namespace
