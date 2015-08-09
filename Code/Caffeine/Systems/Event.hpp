

/*
 Deal with strings
 */


#ifndef CAFFEINE_SYSTEM_EVENT_DATA_INCLUDED
#define CAFFEINE_SYSTEM_EVENT_DATA_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/Variant.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <memory>
#include <array>
#include <vector>
#include <string>


namespace Caffeine {
namespace Systems {
    

		
typedef std::size_t EventID;
    typedef CaffUtil::Variant<std::nullptr_t, float, uint32_t, int32_t, uint64_t, int64_t, std::size_t, std::string, Math::Vector2, Math::Vector3, Math::Vector4> EventArg;

const bool EventSwallowed = true;
const bool EventNotSwallowed = false;

	
class Event
{
public:
    
    explicit 			Event(const EventID id,
							  const EventArg &arg1 = EventArg(nullptr),
							  const EventArg &arg2 = EventArg(nullptr),
							  const EventArg &arg3 = EventArg(nullptr),
							  const EventArg &arg4 = EventArg(nullptr),
							  const EventArg &arg5 = EventArg(nullptr),
							  const EventArg &arg6 = EventArg(nullptr),
							  const EventArg &arg7 = EventArg(nullptr),
							  const EventArg &arg8 = EventArg(nullptr));
    
    const EventArg&		at(const std::size_t i) const;
	EventArg&			at(const std::size_t i);
    bool 				is(const EventID id) const;
    inline EventID      getID() const { return m_id; }
    
#ifndef NDEBUG
    uint32_t 			numberOfReads();
#endif
    
private:
    
    enum { MAX_ARGS = 8 };
    
    std::array<EventArg, MAX_ARGS> m_args;
	
    const   EventID 		m_id;
    
#ifndef NDEBUG
    mutable uint32_t 				m_reads;
#endif
    
}; // class


    
} // namespace
} // namespace


#endif // include guard
