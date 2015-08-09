

#ifndef BASE_STATE_SYSTEM_BASE_INCLUDED
#define BASE_STATE_SYSTEM_BASE_INCLUDED


#include <Caffeine/Utilities/Noncopyable.hpp>
#include <vector>
#include <queue>
#include <assert.h>


namespace Base {


/*
	StateSystemBase
	--
	Base class for the state system.
	provides only the interaction with the State data.
*/

template<typename T>
class StateSystemBase : private CaffUtils::Noncopyable
{
protected:

	explicit StateSystemBase() {}
	virtual  StateSystemBase() {}

	

	// *** STATE ACCESS *** //

	// Returns the top state. Will assert if no states exist.
	T& top() { assert(m_states.size()); return *m_states.back().get(); }

	// Lets you index through the stored states. * Use this with care. *
	T& get(const std::size_t i) { assert(m_states.size()); return *m_states.at(i).get(); }
	
	
	
	// *** STATE INFORMATION *** //

	// Return true if states exist.
	bool hasState() { return m_states.size() > 0; }
	
	// How many states exist.
	std::size_t numberOfStates() { return m_states.size(); }

	// Has states waiting to be processed.
	bool hasPendingProcess() { return m_pendingStates.size() > 0; }
	
	// The number of pending processes in the state system.
	std::size_t numberOfPending() { return m_pendingStates.size(); }


	
	// *** STATE INTERACTION *** //

	// Push a new state onto the stack.
	void push(std::unique_ptr<T> state) { assert(m_state.get()); m_pendingStates.emplace({ProcessAction::PUSH, std::move(state)}); }

	// Pops the top state from the stack.
	void pop() { assert(m_state.size()); m_pendingStates.emplace({ProcessAction::POP, nullptr}); }

	// Removes all the states.
	void clear() { m_pendingStates.emplace({ProcessAction::CLEAR, nullptr}); }


	
	// *** PROCESS STATES *** //
	
	// Process the first pending state.
	void processNext()
	{
		ProcessState &pending = m_pendingStates.front();

		switch(m_pending.action)
		{
			case(ProcessState::PUSH)
			{
				m_states.push_back(std::move(m_pending.state));
				statePushed(*m_state.back().get());

				break;
			}
			case(ProcessState::POP)
			{
				assert(m_statePopped.size());

				statePopped(*m_state.back().get());
				m_state.pop_back();

				break;
			}
			case(ProcessState::CLEAR)
			{
				while(m_states.size() > 0)
				{
					statePopped(*m_state.back().get());
					m_state.pop_back();
				}

				break;
			}
			default:
			break;
		};

		m_pendingStates.pop();
	}



	// Processes all the pending states.
	void processAll()
	{
		while(m_pendingStates.size() > 0)
		{
			processNext();
		}
	}



	// *** CHILD CLASS HOOKS *** //

	// When a state is about to get popped off the state stack.
	virtual void statePopped(T &state) 	{}
	
	// When a state is pushed onto the stack.
	virtual void statePushed(T &state) 	{}

private:

	class enum ProcessAction
	{
		NONE,
		PUSH,
		POP,
		CLEAR,
	};

	struct ProcessState
	{
		ProcessAction 		action;
		std::unique_ptr<T> 	state;
	};

	std::vector<std::unique_ptr<T> > 	m_states;
	std::queue<ProcessState> 			m_pendingStates;

	uint32_t 							m_pendingPops = 0;

}; // class


} // namespace


#endif // include guard
