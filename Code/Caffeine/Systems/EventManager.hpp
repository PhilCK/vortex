
/*
	EventManager.hpp
	----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef CAFFEINE_SYSTEMS_EVENT_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_EVENT_MANAGER_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <map>


namespace Caffeine {
namespace Systems {


/*
	EventManager
	------------
	Controls the events in the game.
	
	// TODO: Unique_ptr
 */
 
class EventManager : private CaffUtil::NonCopyable
{
public:
	
	explicit				EventManager();
							~EventManager();

	// Add/Remove controllers for events.
	bool					addController(CaffSys::EventCallback *controller, const CaffSys::EventID eventID);
	bool					removeController(const CaffSys::EventCallback *controller, const CaffSys::EventID eventID);
	bool					removeController(const CaffSys::EventCallback *controller);
	
	// Deleivers all the events that have been queued, this should be a per frame thing.
	void					deliverQueuedEvents();
	
	// Creats an event.
	CaffSys::Event*			createEvent(const uint32_t eventID);
	
	// Send events to the controllers, queued will be stored until the next frame.
	void					sendQueuedEvent(std::unique_ptr<CaffSys::Event> data);
	void					sendInstantEvent(const CaffSys::Event &data);
	
	void					sendInstantEvent(const CaffSys::EventID id,
                                             const CaffSys::EventArg &arg1,
                                             const CaffSys::EventArg &arg2 = nullptr,
                                             const CaffSys::EventArg &arg3 = nullptr,
                                             const CaffSys::EventArg &arg4 = nullptr,
                                             const CaffSys::EventArg &arg5 = nullptr,
                                             const CaffSys::EventArg &arg6 = nullptr,
                                             const CaffSys::EventArg &arg7 = nullptr,
                                             const CaffSys::EventArg &arg8 = nullptr);
	
	// Same as above but these are for sending events that have not data.
	void					sendQueuedEvent(const CaffSys::EventID id);
	void					sendInstantEvent(const CaffSys::EventID id);

	inline std::size_t		eventsInPool() const { return m_eventPool.size(); }
	
	std::size_t				getEventID(const std::string &id);

private:

	struct ControllerDetails
	{
		CaffSys::EventCallback *		callback;
		bool							isActive;
	
	}; // struct
	
	
	struct EventDetails
	{
		std::unique_ptr<CaffSys::Event >	eventData;
		bool								hasBeenSent;
	}; // struct
	

	std::map<std::string, std::size_t> m_eventIDs;
	
	//typedef std::vector<EventCallback *> ControllerList;
	typedef std::vector<ControllerDetails> ControllerList;
	
	// List of controllers, and queued events.
	std::map<CaffSys::EventID, ControllerList>          m_controllers;
	//std::vector<std::unique_ptr<CaffSys::Event > >      m_queuedEvents;
	
	std::vector<std::tuple<std::unique_ptr<CaffSys::Event >, bool> >							 m_eventPool;
	
	bool						m_showDebugInfo;

};


} // namespace
} // namespace

#endif
