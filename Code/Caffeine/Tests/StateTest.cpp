

// TODO: Should break this down into smaller tests.

#include <UnitTest.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/State.hpp>

namespace CaffSys = ::Caffeine::Systems;


bool loaded, reloaded, unloaded;
void ResetTestData() { loaded = reloaded = unloaded = false; }


class Foo : public CaffSys::State
{
public:

	explicit Foo()
	: State("FooState")
	{}

	void onLoad() { loaded = true; }
	void onReLoad() { reloaded = true; }
	void onUnLoad() { unloaded = true; }
};


class Bar : public CaffSys::State
{
public:

	explicit Bar()
	: State("BarState")
	{}

	void onLoad() { loaded = true; }
	void onReLoad() { reloaded = true; }
	void onUnLoad() { unloaded = true; }
};


class Boo : public CaffSys::State
{
public:

	explicit Boo()
	: State("BooState")
	{}

	void onLoad() { loaded = true; }
	void onReLoad() { reloaded = true; }
	void onUnLoad() { unloaded = true; }
};


TEST(CreateAddState)
{
	CaffSys::StateManager stateMgr;
	ResetTestData();

	std::unique_ptr<Foo> fooState(new Foo);

	ASSERT_IS_EQUAL(stateMgr.size(), 0)
	ASSERT_IS_FALSE(stateMgr.hasStates())

	ASSERT_IS_FALSE(loaded)
	ASSERT_IS_FALSE(reloaded)
	ASSERT_IS_FALSE(unloaded)
	ResetTestData();

	stateMgr.pushState(std::move(fooState));

	ASSERT_IS_EQUAL(stateMgr.size(), 0)
	ASSERT_IS_FALSE(stateMgr.hasStates())

	stateMgr.processPending();

	ASSERT_IS_EQUAL(stateMgr.size(), 1)
	ASSERT_IS_TRUE(stateMgr.hasStates())
	ASSERT_IS_EQUAL(stateMgr.topState().getName(), "FooState")

	ASSERT_IS_TRUE(loaded)
	ASSERT_IS_FALSE(reloaded)
	ASSERT_IS_FALSE(unloaded)
	ResetTestData();
}


TEST(StatePushingPopping)
{
	CaffSys::StateManager stateMgr;
	ResetTestData();

	std::unique_ptr<Foo> fooState(new Foo);
	std::unique_ptr<Bar> barState(new Bar);
	std::unique_ptr<Boo> booState(new Boo);

	// Push 1 state.
	{
		stateMgr.pushState(std::move(fooState));
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 1)
		ASSERT_IS_TRUE(stateMgr.hasStates())
		ASSERT_IS_EQUAL(stateMgr.topState().getName(), "FooState")

		ASSERT_IS_TRUE(loaded)
		ASSERT_IS_FALSE(reloaded)
		ASSERT_IS_FALSE(unloaded)
		ResetTestData();
	}

	// Push 2 state.
	{
		Foo *foo = static_cast<Foo*>(&stateMgr.topState());

		stateMgr.pushState(std::move(barState));
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 2)
		ASSERT_IS_TRUE(stateMgr.hasStates())
		ASSERT_IS_EQUAL(stateMgr.topState().getName(), "BarState")

		ASSERT_IS_TRUE(loaded)
		ASSERT_IS_FALSE(reloaded)
		ASSERT_IS_FALSE(unloaded)
		ResetTestData();
	}

	// Push 3 state.
	{
		Bar *bar = static_cast<Bar*>(&stateMgr.topState());

		stateMgr.pushState(std::move(booState));
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 3)
		ASSERT_IS_TRUE(stateMgr.hasStates())
		ASSERT_IS_EQUAL(stateMgr.topState().getName(), "BooState")

		ASSERT_IS_TRUE(loaded)
		ASSERT_IS_FALSE(reloaded)
		ASSERT_IS_FALSE(unloaded)
		ResetTestData();
	}

	// Pop 3 state.
	{
		stateMgr.popState();
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 2)
		ASSERT_IS_TRUE(stateMgr.hasStates())
		ASSERT_IS_EQUAL(stateMgr.topState().getName(), "BarState")

		ASSERT_IS_FALSE(loaded)
		ASSERT_IS_TRUE(reloaded)
		ASSERT_IS_TRUE(unloaded)
		ResetTestData();
	}

	// Pop 2 state.
	{
		stateMgr.popState();
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 1)
		ASSERT_IS_TRUE(stateMgr.hasStates())
		ASSERT_IS_EQUAL(stateMgr.topState().getName(), "FooState")

		ASSERT_IS_FALSE(loaded)
		ASSERT_IS_TRUE(reloaded)
		ASSERT_IS_TRUE(unloaded)
		ResetTestData();
	}

	// Pop 1 state.
	{
		stateMgr.popState();
		stateMgr.processPending();

		ASSERT_IS_EQUAL(stateMgr.size(), 0)
		ASSERT_IS_FALSE(stateMgr.hasStates())
	}
}


int main()
{
	Test::RunTests();

	return 0;
}
