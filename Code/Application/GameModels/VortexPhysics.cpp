
#include <Application/GameModels/VortexPhysics.hpp>
#include <Application/Common/EventDetails.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>



void VortexPhysics::onParams(const CaffUtil::Param &params)
{
	// Get collision tags
	{
		const std::string tags = params["collide_tags"].asStdString();
		std::vector<std::string> tagVec = CaffUtil::SplitStringBySpaces(tags);
		
		m_collideWithTags.swap(tagVec);
	}
}



void VortexPhysics::onStart()
{
}



void VortexPhysics::onThink(const float dt)
{
}



void VortexPhysics::onCollision(const CaffEnt::ColliderDetails &details)
{
	const std::vector<std::string> tags = details.collidedWith->getTags();

	if(CaffUtil::ContainsTokens(tags, m_collideWithTags))
	{
		CaffSys::Event collidedEvent(VortexCollision::GetID());
		collidedEvent.at(VortexCollision::OBJ_ID) = CaffSys::EventArg(getOwner().getID());
		
		getOwner().onEvent(collidedEvent);
	}
}



COMPONENT_FACTORY_SOURCE(VortexPhysics)
