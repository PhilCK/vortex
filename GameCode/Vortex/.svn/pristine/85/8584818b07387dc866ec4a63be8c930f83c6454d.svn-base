
/*@
	## Description
	This is basically a countdown and spawner.
	It will try and create the entity with the given name,
	and not allow you to create another entity until the cool down has been reached.

	## Params
	

	## Events


	## Todo / Issues

*/

// ** Event Indexes ** //

enum WeaponFiredEvent		{ OBJ_ID, PROJECTILE_OBJ_ID, POSITION, };


class WeaponSystem : GameComponent
{
	// ** Setters ** //

	bool fireWeapon(const Math::Vec3 &in position, const bool up = false)
	{
		if(m_reloadTimer > 0)
		{
			Print("Reloading");
			return false;
		}

		// Create a projectile
		{
			Entity @entity = @getOwner().getState().pushNewEntity(m_projectileEntityNameParam);

			if(entity !is null)
			{
				VortexTransform @vortexTransform = @entity.getComponent("VortexTransform");

				if(vortexTransform !is null)
				{
					vortexTransform.setPosition(position);

					if(up)
					{
						// CONTROLLER!!!
						ProjectileModel @model = @entity.getComponent("ProjectileModel");
						model.setDirection(1);
					}

					// Fire bullet
					{
						const string eventName 	= "WeaponFired";
						const uint   eventID  	= getOwner().getEventManager().getEventID(eventName);

						EventArg argObjID(getOwner().getID());
						EventArg argPos(position);
					
						getOwner().getEventManager().sendInstantEvent(eventID, argObjID, argPos);
					}
				}
				else
				{
					Print("WeaponSystem: Couldn't find VortexTransform on projectile.");
				}
			}
		}


		// Fire weapon.
		// TODO: create entity.

		m_reloadTimer = m_reloadSpeedParam;
		m_hasReloaded = false;

		return true;
	}

	float getTimeTillReload() const 	{ return m_reloadTimer; 		}
	float getReloadTime() const 		{ return m_reloadSpeedParam; 	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_reloadSpeedParam 			= params["reload_speed"].asFloat(m_reloadSpeedParam);
		m_projectileEntityNameParam = params["projectile_name"].asString(m_projectileEntityNameParam);
	}



	private void onThink(const float dt) override
	{
		m_reloadTimer = Math::Max(0, m_reloadTimer - dt);

		// Reloaded event
		if(m_reloadTimer == 0 && !m_hasReloaded)
		{
			const string eventName 	= "WeaponCooledDown";
			const uint   eventID  	= getOwner().getEventManager().getEventID(eventName);

			EventArg argObjID(getOwner().getID());
			
			getOwner().getEventManager().sendInstantEvent(eventID, argObjID);

			m_hasReloaded = true;;
		}
	}	



	// ** Member Vars ** //

	private float 				m_reloadSpeedParam 				= 0.15f;
	private string 				m_projectileEntityNameParam 	= "Bullet";

	private float 				m_reloadTimer					= 0.f;
	private bool 				m_hasReloaded 					= true;

};
