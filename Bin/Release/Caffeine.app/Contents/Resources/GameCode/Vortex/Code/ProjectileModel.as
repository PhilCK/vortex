/*
	ProjectilModel
	--
	Projectile model will move a projectil up or down the tunnel.
*/


// ** Event Indexes ** //

enum ProjectilSpawnEvent 	{ OBJ_ID, POSITION, };
enum ProjectilMoveEvent 	{ OBJ_ID, POSITION, };



class ProjectileModel : GameComponent
{
	// ** Model Getters ** //

	void setDirection(const int dir)
	{
		m_projectileDirectionParam = dir > 0 ? 1 : -1;
		m_projectileVelocity = m_projectileSpeedParam * m_projectileDirectionParam;
	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_projectileSpeedParam 		= params["speed"].asFloat(m_projectileSpeedParam);
		m_projectileDirectionParam  = params["direction"].asInt(m_projectileDirectionParam);
	}



	private void onStart() override
	{
		// Calculate velocity
		{
			setDirection(m_projectileDirectionParam);
			m_projectileVelocity = m_projectileSpeedParam * m_projectileDirectionParam;
		}

		// Spawn Event
		{

		}
	}



	private void onThink(const float dt) override
	{
		// Update position
		{
			const float depth    = getVortexTransform().getDepth();
			const float newDepth = depth + (m_projectileVelocity * dt);

			getVortexTransform().setDepth(newDepth);

			// Move event
		}

		// Check out of bounds (if so delete!)
		{

		}
	}



	// ** Comp Helpers ** //

	private VortexTransform@ getVortexTransform()
	{
		if(m_vortexTransform is null)
		{
			@m_vortexTransform = @getOwner().getComponent("VortexTransform");

			if(m_vortexTransform is null)
			{
				Print("ShipPlayerController: Failed to find vortex-transform");
				return null;
			}
		}

		return m_vortexTransform;
	}



	// ** Member Vars ** //

	private float 				m_projectileSpeedParam 		= 1.0f;
	private int 				m_projectileDirectionParam 	= -1;

	private float 				m_projectileVelocity 		= 1;
	private VortexTransform 	@m_vortexTransform 			= null;

};
