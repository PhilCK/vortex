
#ifndef EVENT_DETAILS_INCLUDED
#define EVENT_DETAILS_INCLUDED


#include <Caffeine/Platform.hpp>


namespace EventDetail
{
	static uint32_t eventIDCounter = 0;
}


#ifndef CAFF_ON_MAC

#define EVENT(event, ...)										\
namespace event													\
{																\
	static uint32_t eventID = ++EventDetail::eventIDCounter;	\
	static uint32_t GetID() { return eventID; }					\
	enum ENUM : uint32_t { __VA_ARGS__ };						\
}																\

#else

#define EVENT(event, ...)                                                               \
namespace event                                                                         \
{                                                                                       \
__attribute__ ((unused)) static uint32_t eventID = ++EventDetail::eventIDCounter;       \
__attribute__ ((unused)) static uint32_t GetID() { return eventID; }					\
enum ENUM : uint32_t { __VA_ARGS__ };                                                   \
}                                                                                       \

#endif


// Object Movement
EVENT(ObjectMove,				OBJ_ID,     POSITION,       DIRECTION)
EVENT(ObjectJump,				OBJ_ID,     POSITION,       SPEED)

// Ship Health
EVENT(ObjectSpawned,			OBJ_ID,     POSITION)
EVENT(ObjectDamage,				OBJ_ID,     HEALTH_LOST,    HEALTH_LEFT)
EVENT(ObjectHeal,				OBJ_ID,		HEALTH_GAINED,	HEALTH_LEFT)
EVENT(ObjectDestroyed,			OBJ_ID,     POSITION)

// Vortex Transform
EVENT(VortexMove,				OBJ_ID,     POSITION)
EVENT(VortexOutOfBounds,		OBJ_ID,     POSITION)

// Vortex Physics
EVENT(VortexCollision,			OBJ_ID)

// Input
EVENT(InputNavUp,               OBJ_ID)
EVENT(InputNavDown,             OBJ_ID)
EVENT(InputNavLeft,             OBJ_ID)
EVENT(InputNavRight,            OBJ_ID)
EVENT(InputNavEnter,            OBJ_ID)
EVENT(InputNavBack,             OBJ_ID)

EVENT(InputGPadLeft,            OBJ_ID)
EVENT(InputGPadRight,           OBJ_ID)
EVENT(InputGPadUp,              OBJ_ID)
EVENT(InputGPadDown,            OBJ_ID)
EVENT(InputGPadShoot,           OBJ_ID)
EVENT(InputGPadJump,            OBJ_ID)
EVENT(InputGPadEscape,			OBJ_ID)

// Level Model
EVENT(LevelStart,               OBJ_ID)
EVENT(LevelUp,                  OBJ_ID)
EVENT(LevelGameOver,            OBJ_ID)



#endif // include guard
