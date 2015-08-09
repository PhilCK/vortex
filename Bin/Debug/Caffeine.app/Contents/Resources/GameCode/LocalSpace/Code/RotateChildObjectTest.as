

class RotateChildObjectTest : GameComponent
{
	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_childToRotateParam = params["child_to_rotate_name"].asString(m_childToRotateParam);
	}



	private void onStart() override
	{
		Entity@ child = getOwner().findChild(m_childToRotateParam);

		if(child !is null)
		{
			@m_childTransform = child.getComponent("Transform");

			if(m_childTransform is null)
			{
				Util::LogError("RotateChildObjectTest - Couldn't find requeted child transform.");
			}
		}
		else
		{
			Util::LogError("RotateChildObjectTest - Couldn't find requested child.");
		}
	}



	private void onThink(const float dt) override
	{
		if(m_childTransform !is null)
		{
			m_rotate += dt;
			const Math::Vec3 axis = {0.f, 1.f, 0.f};
			Math::Quat rotate(m_rotate, axis);

			m_childTransform.setRotation(rotate);
		}
	}



	// ** Member Vars ** //

	private string 				m_childToRotateParam 	= "cube_01";
	private TransformComponent 	@m_childTransform 		= null;
	private float 				m_rotate 				= 0.f;
};
