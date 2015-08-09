/*
	PlayerModel.as
*/

//[RegisteredEvents ["PlayerMoved", "PlayerJumped", "PlayerDied", "PlayerSpawned"]]
class PlayerModel : GameComponent
{
	PlayerModel()
	{
		m_nextFrameMovement.x = 0;
		m_nextFrameMovement.y = 0;
		m_nextFrameMovement.z = 0;
		m_nextFrameRotation   = 0;
	}



	void onLateThink(const float dt) override
	{
		// Process movement requests
		if(getOwner() !is null)
		{
			PhysicsComponent @rigidbody = getOwner().getComponent("Rigidbody");

			// Move player's rigidbody
			if(rigidbody !is null)
			{
				m_nextFrameMovement.x *= dt;
				m_nextFrameMovement.y *= dt;
				m_nextFrameMovement.z *= dt;
				rigidbody.applyLocalForce(m_nextFrameMovement);

				Math::Vec3 rotate = {0.0f, m_nextFrameRotation * dt, 0.0f};
				rigidbody.applyTorque(rotate);
			}
			else
			{
				Print("PlayerModel: Has no attached rigidbody.");
			}
		}
		else
		{
			Print("PlayerModel: Has no attached entity.");
		}

		// Reset Frame movements
		{
			m_nextFrameMovement.x = 0;
			m_nextFrameMovement.y = 0;
			m_nextFrameMovement.z = 0;
			m_nextFrameRotation   = 0;
		}
	}



	// Actor controls.
	void moveForward(const float amount) 	{ m_nextFrameMovement.z += amount; 	}
	void moveRight(const float amount)		{ m_nextFrameMovement.x += amount;	}
	void jump(const float amount) 			{ m_nextFrameMovement.y += amount; 	}
	void rotate(const float amount)			{ m_nextFrameRotation += amount;	}



	// Members Vars //
	private Math::Vec3 		m_nextFrameMovement;
	private float 			m_nextFrameRotation;

}; // class
