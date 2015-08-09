
/*
	VortexPhysics
	--
	We only use the physics engine for collision detection. There is no real physics in here.
*/
class VortexPhysics : GameComponent
{
	// ** Engine Hooks ** //

	private void onAttach() override
	{
		getOwner().addComponent("Rigidbody");
	}



	private void onParams(const Param &in params) override
	{
		m_showDebug = params["show_debug"].asBool(m_showDebug);
	}



	private void onStart() override
	{
		@m_rigidbody = getOwner().getComponent("Rigidbody");

		// Setup rigidbody.
		if(m_rigidbody !is null)
		{
			m_rigidbody.setTrigger(false);
			m_rigidbody.setMass(1.f);
			m_rigidbody.showDebug(m_showDebug);
		}
		else
		{
			Print("VortexPhysics: No rigidbody was found.");
		}
	}



	// ** Member Vars ** //

	private bool 					m_showDebug  = false;

	private PhysicsComponent 		@m_rigidbody = null;
};