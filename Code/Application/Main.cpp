
#include <Application/StateComponents/LevelController.hpp>
#include <Application/StateComponents/LevelModel.hpp>

#include <Application/GameModels/VortexCamera.hpp>
#include <Application/GameModels/VortexTransform.hpp>
#include <Application/GameModels/VortexPhysics.hpp>
#include <Application/GameModels/Movement.hpp>
#include <Application/GameModels/Health.hpp>

#include <Application/GameControllers/StarshipController.hpp>
#include <Application/GameControllers/BulletController.hpp>
#include <Application/GameControllers/EnemyBasicController.hpp>
#include <Application/GameControllers/EnemyBreederController.hpp>

#include <Caffeine/Caffeine.hpp>
#include <Caffeine/Systems/CaffeineMain.hpp>



#define REG_COMPONENT(comp) caffApp.registerComponent(#comp, new comp##Factory());



int main(int argc, char * arg[])
{
	CaffSys::CaffeineMain caffApp;
    
    REG_COMPONENT(LevelController)
	REG_COMPONENT(LevelModel)
    
	REG_COMPONENT(VortexCamera)
	REG_COMPONENT(VortexTransform)
	REG_COMPONENT(VortexPhysics)
	REG_COMPONENT(Movement)
	REG_COMPONENT(Health)
	
	REG_COMPONENT(StarShipController)
	REG_COMPONENT(BulletController)
	REG_COMPONENT(EnemyBasicController)
	REG_COMPONENT(EnemyBreederController)
	
    //caffApp.registerComponent("LevelController", new LevelControllerFactory());
	
	caffApp.start();
	
    return 0;
}
