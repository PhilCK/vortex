
#include <Application/StateComponents/LevelController.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Math/Math.hpp>



// ** Engine Hooks ** //

void LevelController::onParams(const CaffUtil::Param &params)
{
    m_maxNumberOfEnemyShips = params["max_number_of_enemy_ships"].asUInt32(m_maxNumberOfEnemyShips);
    m_spawnRate             = params["spawn_rate"].asFloat(m_spawnRate);
}



void LevelController::onStart()
{
    getOwner().getInput().holdMouse(true);
}



void LevelController::onThink(const float dt)
{
    m_spawnTimer += dt;
    
    if(m_spawnTimer > m_spawnRate)
    {
        m_spawnTimer = 0;
        
        CaffEnt::Entity *ship = getOwner().getState()->pushNewEntity("EnemyBreederShip");
        
        if(ship)
        {
            VortexTransform *transform = ship->getComponent<VortexTransform>();
            assert(transform);
            
            transform->setAngle(CaffMath::RandFloatRange(0.f, CaffMath::Tau()));
            transform->setDepth(transform->getFarZone());
            
            ++m_numberOfEnemyShips;
        }
    }
}



bool LevelController::onEvent(const CaffSys::Event &data)
{
    return CaffSys::EventNotSwallowed;
}






COMPONENT_FACTORY_SOURCE(LevelController)
