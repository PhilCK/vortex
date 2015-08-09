
/*
	EventManager.cpp
	----------------
	
	Project: Caffeine
*/

#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Utilities/ParamNode.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>


namespace Caffeine {
namespace Systems {


EventManager::EventManager()
: m_controllers()
//, m_queuedEvents()
, m_showDebugInfo(false)
, m_eventIDs()
{
	const uint32_t QUEUED_EVENT_STD_VEC_HINT = 128;
	m_eventPool.reserve(QUEUED_EVENT_STD_VEC_HINT);
}



EventManager::~EventManager()
{	
	m_controllers.clear();
	//m_queuedEvents.clear();
}



bool EventManager::addController(CaffSys::EventCallback *controller, const CaffSys::EventID eventID)
{
	assert(controller);

	ControllerList & controllerList = m_controllers[eventID];

	// If new list reserve.
	if(!controllerList.capacity() && controllerList.empty())
	{
		const uint32_t NUM_CONTROLLERS_STD_VEC_HINT = 8;
		controllerList.reserve(NUM_CONTROLLERS_STD_VEC_HINT);
	}

	// Check to see if the controller has already subscribed to this event.
	for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
	{
		if((*it).callback == controller && (*it).isActive == true)
		{
			// Already subscribed
			return false;
		}
	}
	
	ControllerDetails newController = {controller, true};
	
	controllerList.push_back(newController);
	
	return true;
}



bool EventManager::removeController(const CaffSys::EventCallback *controller, const CaffSys::EventID eventID)
{
	assert(controller);

	ControllerList & controllerList = m_controllers[eventID];
	
	// Check to see if the controller has subscribed to this event.
	for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
	{
		if((*it).callback == controller)
		{
			(*it).isActive = false;
			
			return true;
		}
	}

	// None found to remove.
	return false;
}



bool EventManager::removeController(const CaffSys::EventCallback *controller)
{
	assert(controller);
	
	bool hasRemoved = false;
	
	// Loop through all registered events to look for controller.
	for(auto it = m_controllers.begin(); it != m_controllers.end(); ++it)
	{
		if(removeController(controller, it->first))
		{
			hasRemoved = true;
		}
	}

	// None found to remove.
	return hasRemoved;
}



void EventManager::deliverQueuedEvents()
{
    
    // Remove dead controllers
    {
        for(auto &controllers : m_controllers)
        {
            ControllerList & controllerList = controllers.second;

            if(!controllerList.empty())
            {
                auto cit = controllerList.begin();
                while(cit != controllerList.end())
                {
                    if((*cit).isActive == false)
                    {
                        cit = controllerList.erase(cit);
                    }
                    else
                    {
                        ++cit;
                    }
                }
            }
        }
    }
    
	// Loop through all stored events and send them.
//	for(auto it = m_queuedEvents.begin(); it != m_queuedEvents.end(); ++it)
//	{
//		sendInstantEvent(*(*it).get());
//	}
//	
//	// Clear all Controllers marked as not active.
//	//for(auto it2 = m_controllers.begin(); it2 != m_controllers.end(); ++it2)
//	for(auto &controllers : m_controllers)
//	{
//		ControllerList & controllerList = controllers.second;
//		
//		if(!controllerList.empty())
//		{
//			auto cit = controllerList.begin();
//			while(cit != controllerList.end())
//			{
//				if((*cit).isActive == false)
//				{
//					cit = controllerList.erase(cit);
//				}
//				else
//				{
//					++cit;
//				}
//			}
//		}
//	}
//	
//	m_queuedEvents.clear();
}



CaffSys::Event* EventManager::createEvent(const uint32_t eventID)
{
	//m_eventPool.push_back(new CaffSys::Event(eventID), false);

	enum { EVENT = 0, USED, };

	//return std::get<EVENT>(m_eventPool.back()).get();
	return nullptr;
}



void EventManager::sendQueuedEvent(std::unique_ptr<CaffSys::Event> data)
{
	assert(data);

	//m_queuedEvents.push_back(std::move(data));
}



void EventManager::sendInstantEvent(const CaffSys::Event &data)
{
	ControllerList & controllerList = m_controllers[data.getID()];
	
	// Find the controller and send the msg data.
	//for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
    for(auto &controller : controllerList)
	{
		if(controller.isActive)
		{
			if((*controller.callback)(data))
			{
				break;
			}
		}
	}
}



void EventManager::sendInstantEvent(const CaffSys::EventID id,
                                    const CaffSys::EventArg &arg1,
                                    const CaffSys::EventArg &arg2,
                                    const CaffSys::EventArg &arg3,
                                    const CaffSys::EventArg &arg4,
                                    const CaffSys::EventArg &arg5,
                                    const CaffSys::EventArg &arg6,
                                    const CaffSys::EventArg &arg7,
                                    const CaffSys::EventArg &arg8)
{
	std::unique_ptr<Event> event(new Event(id, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
	
	sendInstantEvent(*event);
}



void EventManager::sendQueuedEvent(const CaffSys::EventID id)
{
	std::unique_ptr<CaffSys::Event> evt(new CaffSys::Event(id));
	
	sendQueuedEvent(std::move(evt));
}



void EventManager::sendInstantEvent(const CaffSys::EventID id)
{
	CaffSys::Event evt = CaffSys::Event(id);
	
	sendInstantEvent(evt);
}


    
std::size_t EventManager::getEventID(const std::string &id)
{
	if(m_eventIDs.count(id) == 0)
	{
		m_eventIDs[id] = m_eventIDs.size() + 1;
	}
	
	return m_eventIDs[id];
}
    


} // namespace
} // namespace
