// ** FreeCameraController **//
/*
	This is a debug style camera that lets you
	fly around 3D space.

	It uses the input() directly so no need to route though events.

	Yaw and Pitch params will override the transform's rotation.
*/

class FreeCameraController : GameComponent
{
	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_yawAngleParam 		= params["yaw"].asFloat(m_yawAngleParam);
		m_pitchAngleParam 		= params["pitch"].asFloat(m_pitchAngleParam);
		m_sprintMultiplierParam = params["sprint_multiplier"].asFloat(m_sprintMultiplierParam);
		m_mouseSensitivityParam = params["mouse_sensitivity"].asFloat(m_mouseSensitivityParam);
		m_activeOnHoldParam 	= params["hold_to_move"].asBool(m_activeOnHoldParam);
	}



	private void onStart() override
	{
		// Hold mouse
		getOwner().getInput().setHoldMouse(true);

		// Check if transform component is present.
		{
			TransformComponent @transform = getOwner().getComponent("Transform");

			if(transform is null)
			{
				Util::LogError("FreeCameraController - Transform not found.");
			}
		}
	}



	private void onThink(const float dt) override
	{
		TransformComponent @transform 	= getOwner().getComponent("Transform");
		Input @input 					= getOwner().getInput();

		if(transform !is null && input !is null)
		{
			// Can be active
			if(m_activeOnHoldParam && !input.isDown(Input::MS_RIGHT))
			{
				return;
			}

			// Rotation
			{
				// Update angles
				m_yawAngleParam   -= (input.getMouseDeltaX() * m_mouseSensitivityParam) * dt;
				m_pitchAngleParam -= (input.getMouseDeltaY() * m_mouseSensitivityParam) * dt;

				Math::Quat yawRot;
				{
					const Math::Vec3 axis = {0.f, 1.f, 0.f};
					yawRot = Math::Quat(m_yawAngleParam, axis);
				}

				// Rotation in x
				Math::Quat pitchRot;
				{
					const Math::Vec3 axis = {1.f, 0.f, 0.f};
					pitchRot = Math::Quat(m_pitchAngleParam, axis);
				}

				const Math::Quat newRot = yawRot.multiply(pitchRot);

				transform.setRotation(newRot.normalize());
			}

			//Position
			{
				// Sprinting
				float sprintMultiplier = 1.f;
				{
					if(input.isDown(Input::KB_LSHIFT) || input.isDown(Input::KB_RSHIFT))
					{
						sprintMultiplier = m_sprintMultiplierParam;
					}
				}

				// W,A,S,D movement
				Math::Vec3 move = { 0.f, 0.f, 0.f };
				{
					const float moveSpeed = (10.f * sprintMultiplier) * dt;

					if(input.isDown(Input::KB_W)) { move.z += moveSpeed; }
					if(input.isDown(Input::KB_S)) { move.z -= moveSpeed; }
					if(input.isDown(Input::KB_A)) {	move.x -= moveSpeed; }
					if(input.isDown(Input::KB_D)) { move.x += moveSpeed; }
				}

				// Calculate move position
				if(move.length() > 0.1f)
				{
					const Math::Vec3 forward		= transform.getForwardVec();
					const Math::Vec3 fwdMovement	= forward.scale(move.z);

					const Math::Vec3 right			= transform.getRightVec();
					const Math::Vec3 rightMovement	= right.scale(move.x);

					const Math::Vec3 position		= transform.getPosition();
					const Math::Vec3 movement		= fwdMovement.add(rightMovement);
					const Math::Vec3 moveTo			= position.add(movement);
					
					transform.setPosition(moveTo);
				}
			} //Position
		}
	} // onThink(...)



	// ** Member Vars ** //

	private float m_yawAngleParam 			= 0.f;
	private float m_pitchAngleParam			= 0.f;
	private float m_sprintMultiplierParam 	= 1.5f;
	private float m_mouseSensitivityParam   = 1;
	private bool  m_activeOnHoldParam	    = true;

};
