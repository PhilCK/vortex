
/*@
	## Description
	Will spawn on the vortex. Randomly or at specified position. 

	
	## Params
	- spawn_at_bottom - true if bottom, false if top.
	- angle - angle in which to spawn.
	- spawn_random_angle - if true overrides the angle.
	

	## Events
	- OnSpawn when the entity spawns on the vortex.
*/



// ** Event Indexes ** //

enum VortexSpawnEvent { OBJ_ID, POSITION, };



// ** Model State ** //

enum VortexSpawnPositionState { NOTHING, SPAWN, SPAWNED, };



// ** Component ** //

class VortexSpawnPosition : GameComponent
{
	// ** Component Interaction ** //

	void setSpawned() { m_state = VortexSpawnPositionState::SPAWNED; 			}
	void makeSpawn()  { m_state = VortexSpawnPositionState::SPAWN;   			}
	bool hasSpawned() { return m_state == VortexSpawnPositionState::SPAWNED; 	}


	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_spawnAtBottomParam 	= params["spawn_at_bottom"].asBool(m_spawnAtBottomParam);
		m_randomAngleParam 		= params["spawn_random_angle"].asBool(m_randomAngleParam);
		m_angleParam 			= params["angle"].asFloat(m_angleParam);
	}



	private void onEarlyThink(const float dt) override
	{
		switch(m_state)
		{
			/*
				If spawning we want to set the angle on the vortex.
				and the depth on the vortex.
			*/
			case(VortexSpawnPositionState::SPAWN):
			{
				// Set angle.
				{
					if(m_randomAngleParam)
					{
						const float spawnAngle  = Math::RandFloatRange(0.f, Math::Tau());
						getVortexTransform().setAngle(spawnAngle);
					}
					else
					{
						getVortexTransform().setAngle(m_angleParam);
					}
				}

				// Set Depth
				{
					const float spawnDepth = m_spawnAtBottomParam ? getVortexTransform().getFarSpawnZone() : getVortexTransform().getNearSpawnZone();
					getVortexTransform().setDepth(spawnDepth);
				}

				// Spawned Event
				{
					// const string eventName = "VortexSpawnEvent";
					// const uint   eventID   = getOwner().getEventManager().getEventID(eventName);

					// Event event(eventID);
					// event.at(VortexSpawnEvent::OBJ_ID) 		= EventArg(getOwner().getID());
					// event.at(VortexSpawnEvent::POSITION) 	= EventArg(getVortexTransform().getPosition());

					// getOwner().getEventManager().sendInstantEvent(eventID, eventData);
				}

				m_state = VortexSpawnPositionState::SPAWNED;
			}
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
				Print("VortexSpawnPositionState: Failed to find vortex-transform.");
				return null;
			}
		}

		return m_vortexTransform;
	}



	// ** Member Vars ** //

	private bool 						m_spawnAtBottomParam 	= true;
	private bool 						m_randomAngleParam 		= true;
	private float 						m_angleParam 			= 0.f;

	private VortexTransform 			@m_vortexTransform 		= null;
	private VortexSpawnPositionState	m_state 				= VortexSpawnPositionState::NOTHING;
};
