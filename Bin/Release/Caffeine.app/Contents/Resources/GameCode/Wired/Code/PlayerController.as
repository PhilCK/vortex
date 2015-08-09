/*
	PlayerController
	--
	Logic and control for the player.
*/

class PlayerController : GameComponent
{
	PlayerModel @m_model;

	void onStart() override
	{
		// Find Model
		@m_model = getOwner().getComponent("PlayerModel");

		if(m_model is null)
		{
			Print("PlayerController: PlayerModel not found.");
		}
	}



	// void onParams(const Param @params) override
	// {

	// }



	void onEarlyThink(const float dt) override
	{
		if(getOwner() !is null)
		{
			Input @input = getOwner().getInput();

			if(input !is null)
			{
				// Controls
				{
					// Jump
					if(input.isDown(Key::KB_SPACE))
					{
						m_model.jump(10000.0f);
					}

					// WASD Movement
					if(input.isDown(Key::KB_W))
					{
						m_model.moveForward(1000.0f);
					}
					if(input.isDown(Key::KB_S))
					{
						m_model.moveForward(-1000.0f);
					}
					if(input.isDown(Key::KB_A))
					{
						m_model.moveRight(-1000.0f);
					}
					if(input.isDown(Key::KB_D))
					{
						m_model.moveRight(1000.0f);
					}

					// Rotate
					const float rot = (input.getMouseDeltaX() * 10.0f);

					if(rot != 0.0f)
					{
						m_model.rotate(rot);
					}	
				}
			}
			else
			{
				Print("PlayerController: No Input found, this is an issue.");
			}
		}
		else
		{
			Print("PlayerController: Has no attached entity.");
		}
	}
};
