
// ** CharacterController ** //
/*
	Basic FPS 3D camera movement and collider.
*/

class CharacterController : GameComponent
{
	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_yawAngleParam 		= params["yaw"].asFloat(m_yawAngleParam);
		m_pitchAngleParam 		= params["pitch"].asFloat(m_pitchAngleParam);
		m_sprintMultiplierParam = params["sprint_multiplier"].asFloat(m_sprintMultiplierParam);
		m_mouseSensitivityParam = params["mouse_sensitivity"].asFloat(m_mouseSensitivityParam);
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
				Util::LogError("CharacterController: Transform not found.");
			}
		}
	}



	private void onThink(const float dt) override
	{
		TransformComponent @transform = getOwner().getComponent("Transform");
		Input @input 				  = getOwner().getInput();

		if(transform !is null && input !is null)
		{
			// Rotations
			{
				// Update character yaw
				const float deltaYaw   = (input.getMouseDeltaX() * m_mouseSensitivityParam);

				if(deltaYaw != 0.f)
				{
					m_yawAngleParam -= deltaYaw * dt;

					const Math::Vec3 axis = {0.f, 1.f, 0.f};
					const Math::Quat yawRot = Math::Quat(m_yawAngleParam, axis);
					transform.setRotation(yawRot.normalize());
				}

				// Update head(camera) pitch
				const float deltaPitch = (input.getMouseDeltaY() * m_mouseSensitivityParam);

				if(deltaPitch != 0.f)
				{
					m_pitchAngleParam -= deltaPitch * dt;

					const Math::Vec3 axis = {1.f, 0.f, 0.f};
					const Math::Quat pitchRot = Math::Quat(m_pitchAngleParam, axis);

					// Apply to head
				}
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
				{
					const float moveSpeed = (10.f * sprintMultiplier) * dt;

					Math::Vec3 move = { 0.f, 0.f, 0.f };

					if(input.isDown(Input::KB_W)) { move.z += moveSpeed; }
					if(input.isDown(Input::KB_S)) { move.z -= moveSpeed; }
					if(input.isDown(Input::KB_A)) {	move.x -= moveSpeed; }
					if(input.isDown(Input::KB_D)) { move.x += moveSpeed; }

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
				}

				// Jump
				if(input.isDownOnFrame(Input::KB_SPACE))
				{
					PhysicsComponent@ rigidbody = getOwner().getComponent("Rigidbody");

					const float jumpForce = 30000 * dt;
					const Math::Vec3 jumpForceDir = {0.f, jumpForce, 0.f};
					
					rigidbody.applyForce(jumpForceDir);
				}
			} //Position
		}
	} // onThink(...)



	// ** Member Vars ** //

	private float m_yawAngleParam 			= 0.f;
	private float m_pitchAngleParam			= 0.f;
	private float m_sprintMultiplierParam 	= 1.5f;
	private float m_mouseSensitivityParam   = 1;

};
