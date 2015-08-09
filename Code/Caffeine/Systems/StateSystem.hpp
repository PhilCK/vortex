
#ifndef CAFFEINE_SYSTEMS_STATE_SYSTEM_INCLUDED
#define CAFFEINE_SYSTEMS_STATE_SYSTEM_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <vector>
#include <memory>
#include <map>


namespace Caffeine {
namespace Systems {


    
    class StateSystemBase : private CaffUtil::NonCopyable
    {
    protected:
        
        inline State*       top() { return nullptr; }
        inline void         push() {}
        inline void         pop() {}
        
        inline void         processPending() {}
        
        inline void         size() const {}
        inline void         getState(const std::size_t i) {}
        
    private:
        
        
        
    }; // class
    
    

/*
	StateSystem
	-----------
	A very simple state system based off a stack system.
	
	TODO: Rename file to StateManager as it more accuretly represents its nature.
 */
class StateManager : private CaffUtil::NonCopyable
{

friend class CaffeineMain;

public:

	explicit						StateManager(const std::string &stateDir, const std::string &stateExt, CaffSys::EntityFactory *entityFactory = nullptr);
    explicit                        StateManager(const CaffUtil::Param &params, CaffSys::EntityFactory *entityFactory = nullptr);
									~StateManager();

	// Returns the active state.
	inline State &					topState() const { assert(!m_states.empty()); return *m_states.back().get(); }
	
	// Find a state.
	State*							find(const std::string &stateName);
	
	// Process pending pops / pushes, pops will be processed first, returns true if a state changed.
	bool							processPending();
	
	// Returns true if states exist.
	inline bool						hasStates() const { return !m_states.empty(); }
	
	// Pushes a new pending state. Will not become active until processPending() has
	// been called, by default the last push has priority.
	void							pushState(std::unique_ptr<State> pushState);
    void                            pushState(const std::string &stateName);
	
	// Removes the current state from the stack, the state will not be removed
	// until processPending() has been called.
	inline void						popState() { m_pendingPop = true; }
	
	inline std::size_t				size() const { return m_states.size(); }
	
	inline CaffSys::EntityFactory&	getEntityFactory() { assert(m_entityFactory); return *m_entityFactory; }

private:

	std::vector<std::unique_ptr<State> > m_states;		// Stack of states currently in use.
	std::unique_ptr<State>			m_pendingState;		// Push this state when next processed.
	bool							m_pendingPop; 		// Pop current state when next processed.
	
	/* In dev
	enum class Operation { PUSH, POP, CHANGE, };
	
	struct NextState
	{
		Operation	stateOperation;
		std::unique_ptr<State> state;
	}; // struct
	*/
	std::vector<int>				m_pendingStates;
    
    std::map<std::string, CaffUtil::Param>  m_stateInfo;
	CaffSys::EntityFactory			*m_entityFactory;

}; // class


} // namespace
} // namespace


#endif // include guard
