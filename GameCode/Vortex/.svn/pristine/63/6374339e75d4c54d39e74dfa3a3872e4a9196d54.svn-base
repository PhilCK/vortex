
enum GameModelState { START, SPAWN_PLAYER, PLAYING, LIVE_LOST, GAME_OVER, }

class GameModel : GameComponent
{
	// ** Getters and Setters ** //

	void playerDied() 								{ m_gameModelState = GameModelState::LIVE_LOST; 		}
	void setSpawnTimer(const float timeTillSpawn) 	{ m_timeTillSpawn = timeTillSpawn; 	}



	// ** Engine Hooks ** //

	private void onThink(const float dt) override
	{
		switch(m_gameModelState)
		{
			case(GameModelState::START):
			case(GameModelState::SPAWN_PLAYER):
			{
				m_startTimer += dt;
				m_startTimer = 101.f;

				if(m_startTimer > m_timeTillSpawn)
				{
					getOwner().getState().pushNewEntity("Player");

					m_gameModelState = GameModelState::PLAYING;

					m_startTimer = 0.f;
					m_spawnTimer = 101.f;
				}

				break;
			}

			case(GameModelState::PLAYING):
			{
				m_spawnTimer += dt;

				if(m_spawnTimer > m_timeTillSpawn)
				{
					getOwner().getState().pushNewEntity("Splitter");

					m_spawnTimer = 0.f;
				}

				break;
			}

			case(GameModelState::LIVE_LOST):
			{
				m_gameModelState = GameModelState::SPAWN_PLAYER;

				// Send self destruct msg live-lost ....

				break;
			}

			case(GameModelState::GAME_OVER):
			{
				// Game Over State.

				break;
			}
		}
	}



	// ** Member Vars ** //

	private GameModelState 				m_gameModelState 	= GameModelState::START;
	private float 						m_spawnTimer 		= 0.f; // Graduate to param.
	private float 						m_startTimer 		= 0.f; // also grad
	private float 						m_timeTillSpawn		= 100.0f;

};
