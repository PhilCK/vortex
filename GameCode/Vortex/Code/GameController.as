
class GameController : GameComponent
{
	// ** Engine Hooks ** //

	private void onStart() override
	{
		// Reg for events
		{
			registerForEvent(getOwner().getEventManager().getEventID("PlayerShipDead"));
		}

		// Check required comps exist.
		{
			getModel();
		}
	}



	private void onThink(const float dt) override
	{
	}



	private bool onEvent(const Event &in data) override
	{
		// player died
		const uint playerDiedID = getOwner().getEventManager().getEventID("PlayerShipDead");

		if(data.getID() == playerDiedID)
		{
			getModel().playerDied();
		}

		// enemy died
		if(false)
		{
			
		}

		return false;
	}



	// ** Comp Helpers ** //

	private GameModel@ getModel()
	{
		if(m_model is null)
		{
			@m_model = getOwner().getComponent("GameModel");

			if(m_model is null)
			{
				Print("GameController: Can't find GameModel.");
			}
		}

		return m_model;
	}



	// ** Member Vars ** //

	private GameModel	 			@m_model = null;

};