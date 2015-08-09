
enum ScaleObjectTestState { NONE, ALL, };

class ScaleObjectTest : GameComponent
{


	private void onParams(const Param &in params) override
	{
		const string scaleState = params["scale_in"].asString("ALL");

		if(scaleState == "NONE") 	{ m_stateParam = ScaleObjectTestState::NONE; 	}
		if(scaleState == "ALL") 	{ m_stateParam = ScaleObjectTestState::ALL; 	}
	}

	

	private void onThink(const float dt) override
	{
		m_scaleOffset += dt;
		const float scaleSize = (Math::Sin(m_scaleOffset) + 1.5f) * 0.5f;

		TransformComponent @transform = getOwner().getComponent("Transform");

		switch(m_stateParam)
		{
			case(ScaleObjectTestState::ALL):
			{
				Math::Vec3 scale = {scaleSize, scaleSize, scaleSize};
				transform.setScale(scale);

				break;
			}
		}
	}


	// ** Member Vars ** //
	private ScaleObjectTestState 		m_stateParam 	= ScaleObjectTestState::NONE;

	private float 						m_scaleOffset	= 0.0f;

};
