
// ** CharacterController ** //
/*
	Basic FPS 3D camera movement and collider.
*/

enum CubeCreatedEvent { OBJ_ID, };

class ProgramableCubeController : GameComponent
{
	// ** Object Interaction ** //

	void throwCube(const Math::Vec3 direction, const Math::Vec3 position)
	{
		m_throwDirection = direction;
		m_throwPosition  = position;
	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
	}



	private void onStart() override
	{
		// Check if transform component is present.
		{
			TransformComponent @transform = getOwner().getComponent("Transform");

			if(transform is null)
			{
				Util::LogError("ProgramableCubeController - Transform not found.");
			}
		}

		// Check if rigidbody is present
		{
			PhysicsComponent @rigidbody = getOwner().getComponent("Rigidbody");

			if(rigidbody is null)
			{
				Util::LogError("ProgramableCubeController - Can't find rigidbody.");
			}		
		}

		// Set program
		{
			ChaiInterpeter @chai = getOwner().getComponent("ChaiInterpeter");

			if(chai !is null)
			{
				chai.setScript("scale_cube();");
			}
			else
			{
				Util::LogError("ProgramableCubeController - Can't find chai.");
			}
		}

		// Fire Created cube
		{
			const string eventName = "CubeCreated";
			const uint   eventID   = getOwner().getEventManager().getEventID(eventName);

			EventArg objID(getOwner().getID());

			Event event(eventID);
			event.setArg(CubeCreatedEvent::OBJ_ID, objID);

			getOwner().getEventManager().sendInstantEvent(event);
		}
	}



	private void onEarlyThink(const float dt) override
	{
		// If we have a throw to process
		if(m_throwDirection.length() > 0.f)
		{
			// Set position
			{
				TransformComponent @transform = getOwner().getComponent("Transform");

				if(transform !is null)
				{
					transform.setPosition(m_throwPosition);
					const Math::Vec3 resetPos = {0.f, 0.f, 0.f};
					m_throwPosition = resetPos;
				}
			}

			// Set force
			{
				PhysicsComponent @rigidbody = getOwner().getComponent("Rigidbody");

				if(rigidbody !is null)
				{
					const Math::Vec3 scaledDirection = m_throwDirection.scale(dt);
					rigidbody.applyForce(m_throwDirection);

					const Math::Vec3 resetDir = {0.f, 0.f, 0.f};
					m_throwDirection = resetDir;
				}
			}
		}
	}



	private bool onEvent(const Event &in data) override
	{
		
		return false;
	}




	// ** Member Vars ** //
	Math::Vec3  			m_throwDirection = {0.f, 0.f, 0.f};
	Math::Vec3 				m_throwPosition  = {0.f, 0.f, 0.f};

};
