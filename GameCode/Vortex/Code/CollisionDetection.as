
/*@
	## Description
	This will detect a collision and may destroy() the object if one is found.

	
	## Params
	- collide_with_tag - tag of entity to get a collision with.
	- destroy_on_collision - true/false
	

	## Events
	- OnCollisionEvent - Will only fire if destroy_on_collision if 'false'
	- OnDestroyEvent - Will only fire if destroy_on_collision is 'true'


	## Issues / Todo
	- Nice if this could take in an array of tags, but issue with exporting Params toTokens() from C++ side.
*/



// ** Event Indexes ** //

enum CollisionDetectionEvent { OBJ_ID, COLLIDED_WITH_ID, POSITION, };



// ** Component ** //

class CollisionDetection : GameComponent
{
	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_collideWithTagParam = params["collide_with_tag"].asString(m_collideWithTagParam);
		m_destroyOnCollision  = params["destroy_on_collision"].asBool(m_destroyOnCollision);
	}	



	private void onCollision(const CollidedWith &in collidedWith)
	{
		// If collided with the event
		if(collidedWith.entity.hasTag(m_collideWithTagParam))
		{
			if(m_destroyOnCollision)
			{
				// Fire death event.
				{
					
				}

				getOwner().destroy();
			}
			else
			{
				// Fire collision event.
				{

				}
			}
		}
	}



	// ** Member Vars ** //

	private string 							m_collideWithTagParam = "";
	private bool 							m_destroyOnCollision  = true;

};
