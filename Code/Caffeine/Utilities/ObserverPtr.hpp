
#ifndef CAFFEINE_UTILITIES_OBSERVER_PTR_INCLUDED
#define CAFFEINE_UTILITIES_OBSERVER_PTR_INCLUDED


#include <Caffeine/Platform.hpp>
#include <vector>


namespace Caffeine {
namespace Utilities {


/*
	ObserverPtr
	--
	Observer ptr acts as a type of weak_ptr but without shared_ptr functionality.
	This was designed to allow you to issue ptrs to an object whos life space is 
	undetermined, but without sharing the objects ownership, or costly overhead.

	ObserverPtr is designed to be light and fit a specific purpose, so it isn't thread safe etc.

	This was mainly designed with Entity and Component passing in mind in a single-threaded
	environment. Where access to gameobjects	is required, but the lifespan is unknown.
	So consider carfully if this is the right solution.

	Useage
	--
	struct Entity
	{
		void entityMethod() { ... }

		ObserverPtrFactory<Entity> factory;	
	};

	// Issue an observer.
	Entity entity;
	ObserverPtr<Entity> observer = entity.factory.create();

	// Check valid and use observer as a ptr.
	if(!observer->expired()) {
		observer->entityMethod();
	}
*/


template<typename S>
class ObserverPtr;

/*
	ObserverPtrFactory<S>
	--
	Observer factory has been designed with the intention of being a member var.

	Useage
	--
	Entity::Entity()
	: m_observerPtrFactory(this)
	{}

	// create an observer
	ObserverPtr<Entity> obvPtr = m_observerPtrFactory->create();
*/
template<typename S>
class ObserverPtrFactory
{
	template<typename T>
	friend class ObserverPtr;

public:

	explicit ObserverPtrFactory(S* owner)
	: m_owner(owner)
	, m_observers()
	{
	}

	~ObserverPtrFactory()
	{
		flush();
	}

	// Creates a new observer ptr.
	ObserverPtr<S> create()
	{
		return ObserverPtr<S>(this, m_owner);
	}

	// Invalidates all the observers watching this object.
	void flush()
	{
		for(auto &obs : m_observers)
		{
			obs->invalidate();
		}

		m_observers.clear();
	}

	// The number of observers watching this object.
	std::size_t numberOfObservers() const { return m_observers.size(); }

private:

	void addReference(ObserverPtr<S> *add)
	{
		assert(add);
		m_observers.push_back(add);
	}

	void removeReference(ObserverPtr<S> *remove)
	{
		auto it = m_observers.begin();
		while(it != m_observers.end())
		{
			if((*it) == remove)
			{
				it = m_observers.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:

	S* m_owner = nullptr;
	std::vector<ObserverPtr<S> *> m_observers;

}; // class


/*

*/
template<typename T>
class ObserverPtr final
{

	template<typename S>
	friend class ObserverPtrFactory;

	/*
		Called by the factory's create method, sets up the pointer.
	*/
	explicit ObserverPtr(ObserverPtrFactory<T> *factory, T *observePtr)
	: m_factory(factory)
	, m_rawPtr(observePtr)
	{
		assert(m_factory);
		assert(m_rawPtr);

		m_factory->addReference(this);
	}

	/*
		Removes connection to the object and the factory.
		This is called by the factory's flush method, when it
		Wishes to dismiss its observers.
	*/
	void invalidate()
	{
		m_factory = nullptr;
		m_rawPtr  = nullptr;
	}

public:

	explicit ObserverPtr()
	: m_factory(nullptr)
	, m_rawPtr(nullptr)
	{
	}


	ObserverPtr(const ObserverPtr& other)
	{
		m_factory = other.m_factory;
		m_rawPtr  = other.m_rawPtr;

		if(m_rawPtr && m_factory)
		{
			m_factory->addReference(this);
		}
		else
		{
			release();
		}
	}

	ObserverPtr& operator=(const ObserverPtr& other)
	{
		m_factory = other.m_factory;
		m_rawPtr  = other.m_rawPtr;

		if(m_rawPtr && m_factory)
		{
			m_factory->addReference(this);
		}
		else
		{
			release();
		}

		return *this;
	}


	~ObserverPtr()
	{
		release();
	}

	// Checks to see if the ptr exists.
	bool 		expired() const { return m_rawPtr == nullptr; }
	operator 	bool() const   	{ return !expired(); }

	// Access to the ptr.
	T * const get() const { return m_rawPtr; }
	T * const operator->() const
	{
		assert(m_rawPtr);
		return get();
	}

	// Will inform the factory to stop observing this.
	// And will also null the internal ptr.
	void release()
	{
		m_rawPtr = nullptr;

		if(m_factory)
		{
			m_factory->removeReference(this);
		}
	}

private:

	ObserverPtrFactory<T>* m_factory = nullptr;
	T* m_rawPtr						 = nullptr;

}; // class


} // include guard
} // include guard


#endif // include guard
