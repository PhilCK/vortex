
// Event Data

#include <Caffeine/Systems/Event.hpp>
#include <assert.h>
#include <iostream>


namespace Caffeine {
namespace Systems {


Event::Event(const EventID id,
			 const EventArg &arg1,
			 const EventArg &arg2,
			 const EventArg &arg3,
			 const EventArg &arg4,
			 const EventArg &arg5,
			 const EventArg &arg6,
			 const EventArg &arg7,
			 const EventArg &arg8)
: m_args({arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8})
, m_id(id)
#ifndef NDEBUG
, m_reads(0)
#endif
{
}



const EventArg& Event::at(const std::size_t i) const
{
    if(i > MAX_ARGS)
    {
        assert(false); // out of bounds/not set.
    }
    
    return m_args.at(i);
}



EventArg& Event::at(const std::size_t i)
{
	return const_cast<EventArg&>(
		static_cast<const Event&>(*this).at(i)
	);
}



bool Event::is(const EventID id) const
{
#ifndef NDEBUG
    ++m_reads;
#endif
    
    return m_id == id;
}


} // namespace
} // namespace

