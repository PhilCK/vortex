/*
	Hooks
	--
	Documentation on the various engine hooks.
*/
class Hooks : GameComponent
{
	// ** Engine Hooks **//

	/*
		getOwner() is guaranteed at this point as these hooks are called
		from the owner.
	*/

	
	private void onAttach() override
	{
		// Called when the component is attached to an entity.
		// In construction or using addComponent().

		// Do not expect all components to be attached at this point.
	}



	private void onDisattach() override
	{
		// Call when removeComponent() is invoked.
	}

		

	private void onParams(const Param &in params) override
	{
		// Called when the entity is created.
		// Params are called when the component has been attached to the entity.
		
		// Do not expect all components to be attached at this point.
	}



	private void onStart() override
	{
		// Called when pushed to state, and has statemanager set.
		
		// Consider all components now attached, if the state was
		// created from the factory.
	}



	private void onEarlyThink(const float dt) override
	{
		// First update call.
	}



	private void onThink(const float dt) override
	{
		// Second update call.
	}



	private void onLateThink(const float dt) override
	{
		// Thrid update call.
	}



	private void onCollision(const CollidedWith &in collidedWith) override
	{
		// Collided with.
	}



	private bool onEvent(const Event &in data) override
	{
		// Called when a component has registered for event data.

		// Return true if you wish to swallow the event.
		return false;
	}



	private void onEnd() override
	{
		// Called just before the entity is removed from the state.
	}

};
