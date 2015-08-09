
#ifndef LEVEL_CONTROLLER_INCLUDED
#define LEVEL_CONTROLLER_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


class LevelController : public CaffEnt::Component
{

public:
    
    
	COMPONENT_RTTI(LevelController)
	
    
private:
    
    
    // ** Engine Hooks ** //
    
    void                onParams(const CaffUtil::Param &params) override;
    void                onStart() override;
    void                onThink(const float dt) override;
    bool                onEvent(const CaffSys::Event &data) override;
    
    
private:
    
    
    uint32_t            m_numberOfEnemyShips    = 0;
    uint32_t            m_maxNumberOfEnemyShips = 10;
    float               m_spawnRate             = 2.f;
    float               m_spawnTimer            = 0.f;
    
    
}; // class


COMPONENT_FACTORY_INTERFACE(LevelController)


#endif // included guard
