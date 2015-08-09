
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Entity/Entity.hpp>


namespace Caffeine {
namespace Entity {


Component::Component()
: m_owner(nullptr)
, m_name("Component")
, m_isActive(true)
// , m_eventable()
{
	// Setup eventable call back.
	m_eventCallback = std::bind(&Component::onEvent, this, std::placeholders::_1);
}


Component::~Component()
{
	getOwner().getEventManager().removeController(&m_eventCallback);
}


bool Component::registerForEvent(const CaffSys::EventID eventID)
{
	return getOwner().getEventManager().addController(&m_eventCallback, eventID);
}


bool Component::unRegisterForEvent(const CaffSys::EventID eventID)
{
	return getOwner().getEventManager().removeController(&m_eventCallback, eventID);
}


} // namespace
} // namespace
