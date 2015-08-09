
#ifndef PHYSICS_LAYERS_INCLUDED
#define PHYSICS_LAYERS_INCLUDED


#include <Caffeine/Platform.hpp>

/*
	What collides with what.
*/
namespace PhysicsLayer {


enum Collision
{
	PLAYER_SHIP		= 1 << 0,
	PLAYER_BULLET	= 1 << 1,
	POWER_UP		= 1 << 2,
	ENEMY_SHIP		= 1 << 3,
	ENEMY_BULLET	= 1 << 4,
}


constexpr uint32_t playerCollidesWith	= POWER_UP | ENEMY_BULLET | ENEMY_SHIP;
constexpr uint32_t enemyCollidesWith	= PLAYER_BULLET;


} // namespace


#endif // include guard
