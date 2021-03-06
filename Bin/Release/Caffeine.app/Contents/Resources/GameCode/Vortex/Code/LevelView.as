
class LevelView : GameComponent
{
	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		if(params.doesMemberExist("outter_one"))
		{
			const string modelName = params["outter_one"]["model_name"].asString("");
			const float  turnSpeed = params["outter_one"]["turn_speed"].asFloat(0.1f);

			if(modelName != "")
			{
				ModelData @modelData = getOwner().getModelManager().getModelData(modelName);
				m_models.insertLast(modelData);

				m_turnSpeedParams.insertLast(turnSpeed);
			}
		}
	}


	private void onThink(const float dt) override
	{
		// Temp while level isn't complete
		{
			TransformComponent @transform = getOwner().getComponent("Transform");

			m_rotLevel = m_rotLevel + (m_turnSpeedParams[0] * dt * (0.5f));

			//const Math::Vec3 rot = {Math::QuartTau(), 0.f, 0.f};
			const Math::Vec3 rot = {0, 0.f, m_rotLevel};

			transform.setRotation(rot);
		}
	}



	// ** Member Vars ** //

	private uint 				m_shaderID 			= 0;
	private uint 				m_vertexFmtID 		= 0;
	private uint 				m_textureID 		= 0;
	private float 				m_rotLevel 			= 0.f; // temp to make level rotate.

	private array<ModelData@>	m_models;
	private string 				m_textureParam 		= "dev_grid_orange.png";
	private array<float>		m_turnSpeedParams;
};
