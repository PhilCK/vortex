
class PlayerView : GameComponent
{
	const PlayerModel@ m_model;

	void onStart()
	{
		// Find Model
		@m_model = getOwner().getComponent("PlayerModel");	

		if(m_model is null)
		{
			Print("PlayerView: PlayerModel not found.");
		}
	}

};
