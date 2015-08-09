
/*
	FreeCamera
	--
	
*/
class FreeCamera : GameComponent
{
	CameraComponent 	@camera;
	Math::Vec3 			position;

	void onStart()
	{
		position.x = 0.1f;
		position.y = 3.1f;
		position.z = 15.1f;

		if(getOwner() !is null)
		{
			getOwner().addTag("camera");
			getOwner().addTag("debug");

			// Add transform.
			//getOwner().addComponent("Transform");

			// Add camera component.
			//if(getOwner().addComponent("Camera"))
			{
				// Hold mouse input
				getOwner().getInput().setHoldMouse(true);

				@camera = cast<CameraComponent@>(getOwner().getComponent("Camera"));

				if(camera !is null)
				{
					camera.makeActiveCamera();

					camera.setPosition(position);
					
					Math::Vec3 lookat = {0.f, 0.f, 0.f};
					camera.setLookAt(lookat);
				}
			}
			//else
			//{
			//	Print("Failed to add camera!");
			//}
		}
		else
		{
			Print("FreeCamera component has no entity! EEEK!");
		}
	}


	void onEarlyThink(const float dt)
	{
		if(camera.isActive() && getOwner() !is null)
		{
			Input @input = getOwner().getInput();

			// WASD keys.
			// if(input.isDown(Key::KB_W))
			// {
			// 	position.x += (1 * dt);
			// 	camera.setPosition(position);
			// }
			// if(input.isDown(Key::KB_A))
			// {
			// 	position.y += (1 * dt);	
			// 	camera.setPosition(position);
			// }
			// if(input.isDown(Key::KB_S))
			// {
			// 	position.x -= (1 * dt);
			// 	camera.setPosition(position);
			// }
			// if(input.isDown(Key::KB_D))
			// {
			// 	position.y -= (1 * dt);
			// 	camera.setPosition(position);
			// }

			// Rotations
			// Do rots
		}
	}


	bool onEvent(int someData)
	{
		//if(someData is EVENT_ID("FreeCamMakeActive"))
		{
			if(camera !is null)
			{
				camera.makeActiveCamera();

				Input@ input = getOwner().getInput();

				if(input !is null)
				{
					input.setHoldMouse(true);
				}
			}
		}

		//return EVENT_NO_SWALLOW;
		return false;
	}

};
