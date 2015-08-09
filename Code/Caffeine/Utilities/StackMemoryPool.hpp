
#ifndef CAFFEINE_UTILITIES_STACK_MEMORY_POOL_INCLUDED
#define CAFFEINE_UTILITIES_STACK_MEMORY_POOL_INCLUDED


#include <Caffeine/Platform.hpp>


namespace Caffeine {
namespace Utilities {


class StackMemoryPool
{

	typedef unsigned char byte;

public:


	explicit StackMemoryPool(const uint32_t byteSizeOfPool)
	{
		
	}


	virtual ~StackMemoryPool()
	{

	}


	void* alloc(const std::size_t size)
	{
		assert(size); // Eeek 0 size object sounds kinda dodgy to me.

		byte *location = m_nextAllocation;

		m_nextAllocation += size;

		++m_allocations;
	}


	void clear()
	{
		m_allocations = 0;
		m_nextAllocation = m_memoryPool[0];

		// make sure we call destroy objects.
	}


	uint32_t numberOfAllocations()
	{
		return m_allocations;
	}


private:

	byte 			*m_memoryPool;
	byte 			**m_nextAllocation;
	uint32_t 		m_allocations;

}; // class


} // namespace
} // namespace


#endif // include guard
