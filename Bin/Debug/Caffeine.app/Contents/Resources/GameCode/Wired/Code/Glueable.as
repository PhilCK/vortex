/*
	Head Hack Games
	--
	www.headhackgames.com
	info@headhackgames.com
	--
	All rights reserved 2014.
*/

/*
	Glueable
	--
	Displays the ground.
*/

class Glueable : GameComponent
{
	Glueable()
	{
		m_glueTag = "glueable";
	}



	void onParams(const Param &in params) override
	{
		m_glueTag = params["glue_tag"].asString(m_glueTag);
	}



	void onStart() override
	{
		if(getOwner() !is null)
		{
			// Check has a glueable tag, if not add it!
			{
				if(!getOwner().hasTag(m_glueTag))
				{
					getOwner().addTag(m_glueTag);
				}
			}
		}
		else
		{
			Print("Glueable: Has no attached entity!");
		}

		// Different start positions for testing
		TransformComponent @transform = getOwner().getComponent("Transform");

		Print(getOwner().getID());

		Math::Vec3 pos = {(getOwner().getID() * 2.0f) - 13.0f, 2.0f, 4.0f };
		transform.setWorldPosition(pos);
	}



	void onCollision(const CollidedWith &in collider) override
	{
		// If collided object is also glueable.
		if(collider.entity.hasTag(m_glueTag) && (getOwner() !is null))
		{
			Entity @other = collider.entity;

			// Glue if this entity has a bigger ID.
			if(other.getID() > getOwner().getID())
			{
				PhysicsComponent @rigidbody = other.getComponent("Rigidbody");
				rigidbody.setActive(false);

				getOwner().addChild(other);
			}

			return;
		}
	}



	private string 			m_glueTag;

};
