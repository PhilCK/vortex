
#include <UnitTest.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/EventManager.hpp>

namespace CaffSys = ::Caffeine::Systems;

// Event ID's setup.
const std::string FOO = "FOO_MSG";
const std::string BAR = "BAR_MSG";


class ControllerOne : public CaffSys::Eventable
{
public:

	explicit ControllerOne() {}

	void eventController(const CaffSys::Event *data)
	{
		const_cast<CaffSys::Event *>(data)->left.uiData++;
	}
};

class ControllerTwo : public CaffSys::Eventable
{
public:

	explicit ControllerTwo() {}

	void eventController(const CaffSys::Event *data)
	{
		const_cast<CaffSys::Event *>(data)->left.uiData++;
	}
};


TEST(Initialize)
{
	CaffSys::EventManager evtMgr;

	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 0)
}



TEST(SendInstant)
{
	CaffSys::EventManager evtMgr;

	// Event Controllers
	ControllerOne controllerOne;
	ControllerTwo controllerTwo;

	// Event Data
	CaffSys::Event eventDataOne;
	eventDataOne.id = FOO;
	eventDataOne.left.uiData = 0;

	CaffSys::Event eventDataTwo;
	eventDataTwo.id = BAR;
	eventDataTwo.left.uiData = 0;

	// Subscribe to events.
	ASSERT_IS_TRUE(evtMgr.addController(&controllerOne, FOO))
	ASSERT_IS_FALSE(evtMgr.addController(&controllerOne, FOO))
	ASSERT_IS_TRUE(evtMgr.addController(&controllerOne, BAR))
	ASSERT_IS_TRUE(evtMgr.addController(&controllerTwo, BAR))
	ASSERT_IS_FALSE(evtMgr.addController(&controllerTwo, BAR))

	// Fire Instant Events
	evtMgr.sendInstantEvent(&eventDataOne);
	evtMgr.sendInstantEvent(&eventDataTwo);

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 2)

	// Remove Controllers.
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerOne, FOO))
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerOne, BAR))
	ASSERT_IS_FALSE(evtMgr.removeController(&controllerOne, FOO))

	// Fire Instant Events.
	evtMgr.sendInstantEvent(&eventDataOne);
	evtMgr.sendInstantEvent(&eventDataTwo);

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 3)

	// Remove remaining controllers.
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerTwo, BAR))
	ASSERT_IS_FALSE(evtMgr.removeController(&controllerTwo, BAR))

	// Fire Instant Events.
	evtMgr.sendInstantEvent(&eventDataOne);
	evtMgr.sendInstantEvent(&eventDataTwo);

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 3)
}


TEST(SendQueued)
{
	CaffSys::EventManager evtMgr;

	// Event controllers.
	ControllerOne controllerOne;
	ControllerTwo controllerTwo;

	// Event data.
	CaffSys::Event eventDataOne;
	eventDataOne.id = FOO;
	eventDataOne.left.uiData = 0;

	CaffSys::Event eventDataTwo;
	eventDataTwo.id = BAR;
	eventDataTwo.left.uiData = 0;

	// Add controllers.
	ASSERT_IS_TRUE(evtMgr.addController(&controllerOne, FOO))
	ASSERT_IS_FALSE(evtMgr.addController(&controllerOne, FOO))
	ASSERT_IS_TRUE(evtMgr.addController(&controllerOne, BAR))
	ASSERT_IS_TRUE(evtMgr.addController(&controllerTwo, BAR))
	ASSERT_IS_FALSE(evtMgr.addController(&controllerTwo, BAR))

	// Queue up events.
	evtMgr.sendQueuedEvent(&eventDataOne);
	evtMgr.sendQueuedEvent(&eventDataTwo);

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 0)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 0)

	// Check data is actually queued.
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 2)

	// Fire events.
	evtMgr.deliverQueuedEvents();

	// Check data has been removed
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 0)

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 2)

	// Remove controllers.
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerOne, FOO))
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerOne, BAR))
	ASSERT_IS_FALSE(evtMgr.removeController(&controllerOne, FOO))

	// Queue up events.
	evtMgr.sendQueuedEvent(&eventDataOne);
	evtMgr.sendQueuedEvent(&eventDataTwo);

	// Check data is queued.
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 2)

	// Fire events.
	evtMgr.deliverQueuedEvents();

	// Check data has been removed
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 0)

	// Check test data.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 3)

	// Remove controllers
	ASSERT_IS_TRUE(evtMgr.removeController(&controllerTwo, BAR))
	ASSERT_IS_FALSE(evtMgr.removeController(&controllerTwo, BAR))

	// Again queue up events.
	evtMgr.sendQueuedEvent(&eventDataOne);
	evtMgr.sendQueuedEvent(&eventDataTwo);

	// Check events are queued.
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 2)

	// Send events.
	evtMgr.deliverQueuedEvents();

	// Check data has been removed
	ASSERT_IS_EQUAL(evtMgr.numberOfQueuedEvents(), 0)

	// Check test data hasn't changed.
	ASSERT_IS_EQUAL(eventDataOne.left.uiData, 1)
	ASSERT_IS_EQUAL(eventDataTwo.left.uiData, 3)
}



int main()
{
	Test::RunTests();

	return 0;
}
