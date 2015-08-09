
/*
	CameraGameModel
	--
	The general movement of game camera.
	It will 'lazily' move to target position.
*/

// ** Event Indexes ** //

enum CameraMoveEvent 		{ OBJ_ID, POSITION, };


class CameraGameModel : GameComponent
{
	// ** Model Getters ** //

	Math::Vec3 	getTargetPosition() const 	{ return m_targetPosition;  	}
	float 		getChangeRate() const 		{ return m_changeRateParam; 	}



	// ** Model Setters ** //

	void setTargetPosition(const Math::Vec3 &in target) { m_targetPosition = target; 		}
	void setChangeRate(const float changeRate)  		{ m_changeRateParam = changeRate; 	}



	// ** Engine Hooks ** //
	
	private void onStart() override
	{
		// Get/Set the camera component
		{
			// Set the camera up.
			if(getCameraComponent() !is null)
			{
				getCameraComponent().makeActiveCamera();

				const Math::Vec3 camOffset = {0.f, 0.f, m_cameraTrailDistance};
				getTransform().setWorldPosition(m_targetPosition.add(camOffset));

				//Math::Vec3 lookat = {0.f, 0.f, 0.f};
				//getCameraComponent().setLookAt(lookat);
			}
		}
	}



	private void onParams(const Param &in params) override
	{
		m_changeRateParam		= params["change_rate"].asFloat(m_changeRateParam);
		m_cameraTrailDistance 	= params["trail_distance"].asFloat(m_cameraTrailDistance);
	}



	private void onLateThink(const float dt) override
	{
		// Update current position ** Can be overriden by controller by setting changeRatepParam to zero.
		{
			const Math::Vec3 direction = m_targetPosition.subtract(m_currentPosition);

			if(direction.length() != 0)
			{
				const float distance 	= dt * direction.length();
				const float scale 		= dt * m_changeRateParam;
				const float minScale	= Math::Min(Math::Abs(distance), Math::Abs(scale)); // take smallest to avoid jitter.

				const Math::Vec3 normDirection 	= direction.normalize();
				const Math::Vec3 newDistance 	= normDirection.scale(-minScale);
				const Math::Vec3 newPosition 	= m_currentPosition.add(newDistance);
				const Math::Vec3 camOffset 		= {0.f, 0.f, m_cameraTrailDistance};

				getTransform().setWorldPosition(newPosition.add(camOffset));

				m_currentPosition = newPosition;
			}
		}
	}



	// ** Comp Helpers ** //

	private CameraComponent@ getCameraComponent()
	{
		if(m_cameraComponent is null)
		{
			@m_cameraComponent = @getOwner().getComponent("Camera");

			if(m_cameraComponent is null)
			{
				Print("CameraGameModel: Failed to find camera");
				return null;
			}
		}

		return m_cameraComponent;
	}


	private TransformComponent@ getTransform()
	{
		if(m_transform is null)
		{
			@m_transform = @getOwner().getComponent("Transform");

			if(m_transform is null)
			{
				Print("CameraGameModel: Failed to find transform");
				return null;
			}
		}

		return m_transform;		
	}



	// ** Member Vars ** //

	private float 						m_changeRateParam    = 25.0f;
	private float 						m_cameraTrailDistance = 11.f;

	private Math::Vec3 					m_targetPosition   = {0.f, 0.f, 25.f};
	private Math::Vec3 					m_currentPosition  = {0.f, 0.f, 25.f};
	private CameraComponent 			@m_cameraComponent = null;
	private TransformComponent 			@m_transform	   = null;
};
