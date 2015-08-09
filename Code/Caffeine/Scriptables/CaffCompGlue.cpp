
#include <Caffeine/Scriptables/CaffCompGlue.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/ColliderComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Interface/ScriptComponent.hpp>



namespace Caffeine {
namespace ScriptGlue {
namespace Comp {


    
#pragma mark - Register
    
void RegisterTypes(asIScriptEngine *engine)
{
    ASEngine asEngine(engine);
	
	asEngine
        .addValueType<CaffComp::BoxCollider>("BoxCollider",                 asGetTypeTraits<CaffComp::BoxCollider>()).end()
        .addValueType<CaffComp::CubeCollider>("CubeCollider",               asOBJ_APP_CLASS_CDK).end()
        .addValueType<CaffComp::StaticPlaneCollider>("StaticPlaneCollider", asOBJ_APP_CLASS_CDK).end()
        .addValueType<CaffComp::CompoundCollider>("CompoundCollider",       asOBJ_APP_CLASS_CDK).end()
        .addValueType<CaffEnt::ColliderDetails>("CollidedWith",             asOBJ_POD).end()
		
        .addRefType("Component",			asOBJ_NOCOUNT).end()
        .addRefType("CameraComponent",		asOBJ_NOCOUNT).end()
        .addRefType("PhysicsComponent",		asOBJ_NOCOUNT).end()
        .addRefType("TransformComponent",	asOBJ_NOCOUNT).end()
        .addRefType("Entity",				asOBJ_NOCOUNT).end()
		
    .end();
}

namespace
{
	// TODO: Some of these can go because we don't need dummy objects now.
	Math::Matrix44 TransformGetWorldMatrix(CaffComp::TransformComponent *obj)
	{
		const CaffMath::Matrix44 worldRet = obj->getWorldMatrix();
		
		return worldRet;
	}
	
	CaffMath::Vector3 TransformGetForward(CaffComp::TransformComponent *obj)
	{
		assert(obj);
		
		const CaffMath::Vector3 fwdRet = obj->getForwardVec();
		
		return fwdRet;
	}
	
	
	CaffMath::Vector3 TransformGetUp(CaffComp::TransformComponent *obj)
	{
		assert(obj);
		
		const CaffMath::Vector3 rightRet = obj->getUpVec();
		
		return rightRet;
	}
	
	
	CaffMath::Vector3 TransformGetRight(CaffComp::TransformComponent *obj)
	{
		assert(obj);
		
		const CaffMath::Vector3 rightRet = obj->getRightVec();
		
		return rightRet;
	}
	
	void TransformSetEulerRotation(Math::Vector3 &rot, CaffComp::TransformComponent *obj)
	{
		assert(obj);
		
		obj->setRotationWithEuler(rot.x, rot.y, rot.z);
	}

	CaffMath::Vector3 TransformGetRotation(CaffComp::TransformComponent *obj)
	{
		assert(obj);
		
		const CaffMath::Vector3 retVec = obj->getRotationAsEuler();

		return retVec;
	}
    
    CaffMath::Vector3 TransformGetPosition(CaffComp::TransformComponent *obj)
    {
        assert(obj);
        
        const CaffMath::Vector3 retVec = obj->getPosition();
        
        return retVec;
    }
    
    void TransformSetScale(Math::Vector3 &scale, CaffComp::TransformComponent *obj)
	{
		obj->setScale(scale);
	}
	
	
	void TransformSetPosition(const CaffMath::Vector3 &pos, CaffComp::TransformComponent *obj)
	{
		assert(obj);

		obj->setPosition(pos, true);
	}
    
    
    void TransformSetRotation(const CaffMath::Quaternion &quat, CaffComp::TransformComponent *obj)
    {
        assert(obj);
        
        obj->setRotation(quat, true);
    }
	

	bool AddComponent(const std::string &str, CaffEnt::Entity *obj)
	{
		assert(obj);

		return obj->addComponent(str);
	}
	
	
	CaffEnt::Entity * EntityGetChild(const std::string &str, CaffEnt::Entity *obj)
	{
		assert(obj);
		
		return obj->findChild(str);
	}
}

void RegisterInteraction(asIScriptEngine *engine)
{
    ASEngine asEngine(engine);
	
	asEngine
	
		// Application component.

        .getRefType("Component")
			.addImplicitCast("void f(?&out)",           asMETHODPR(CaffInter::ScriptComponent, cast, (void **, int), void),                 asCALL_THISCALL)
			//.addImplicitCast("void f(?&out)",           asFUNCTION(ComponentCast),															asCALL_CDECL_OBJLAST)
			.addImplicitCast("CameraComponent@ f()",    asFUNCTION((ASHelper::refCast<CaffEnt::Component, CaffComp::CameraComponent>)),     asCALL_CDECL_OBJLAST)
			.addImplicitCast("PhysicsComponent@ f()",   asFUNCTION((ASHelper::refCast<CaffEnt::Component, CaffComp::PhysicsComponent>)),    asCALL_CDECL_OBJLAST)
			.addImplicitCast("TransformComponent@ f()", asFUNCTION((ASHelper::refCast<CaffEnt::Component, CaffComp::TransformComponent>)),  asCALL_CDECL_OBJLAST)
			.addMethod("bool isActive() const",         asMETHOD(CaffEnt::Component, isActive),                                             asCALL_THISCALL)
			.addMethod("void setActive(const bool)",    asMETHOD(CaffEnt::Component, setActive),                                            asCALL_THISCALL)
        .end()
    
	
        .getRefType("CameraComponent")
			.addMethod("void makeActiveCamera()",						asMETHOD(CaffComp::CameraComponent, makeActiveCamera),              asCALL_THISCALL)
			.addMethod("bool isCameraActive() const",					asMETHOD(CaffComp::CameraComponent, isCameraActive),                asCALL_THISCALL)
			.addMethod("void setFOV(const uint)",						asMETHOD(CaffComp::CameraComponent, setFOV),                        asCALL_THISCALL)
			.addMethod("uint getFOV() const",							asMETHOD(CaffComp::CameraComponent, getFOV),                        asCALL_THISCALL)
			.addMethod("void setViewPort(const uint, const uint)",		asMETHOD(CaffComp::CameraComponent, setViewPort),                   asCALL_THISCALL)
			.addMethod("void setViewPortToWindowResolution()",			asMETHOD(CaffComp::CameraComponent, setViewPortToWindowResolution), asCALL_THISCALL)
			.addMethod("uint getViewPortWidth() const",					asMETHOD(CaffComp::CameraComponent, getViewPortWidth),              asCALL_THISCALL)
			.addMethod("uint getViewPortHeight() const",				asMETHOD(CaffComp::CameraComponent, getViewPortHeight),             asCALL_THISCALL)
			.addMethod("void setNearPlane(const float)",				asMETHOD(CaffComp::CameraComponent, setNearPlane),                  asCALL_THISCALL)
			.addMethod("void setFarPlane(const float)",					asMETHOD(CaffComp::CameraComponent, setFarPlane),                   asCALL_THISCALL)
			.addMethod("float getNearPlane() const",					asMETHOD(CaffComp::CameraComponent, getNearPlane),                  asCALL_THISCALL)
			.addMethod("float getFarPlane() const",						asMETHOD(CaffComp::CameraComponent, getFarPlane),                   asCALL_THISCALL)
			.addMethod("Math::Mat4 getViewMatrix() const",				asFUNCTION(CaffGlue::Comp::GetViewMat),								asCALL_CDECL_OBJLAST)
			.addMethod("Math::Mat4 getProjMatrix() const",				asFUNCTION(CaffGlue::Comp::GetProjMat),								asCALL_CDECL_OBJLAST)
			.addMethod("Math::Mat4 getViewProjMatrix() const",			asFUNCTION(CaffGlue::Comp::GetViewProjMat),							asCALL_CDECL_OBJLAST)
			.addMethod("bool isActive() const",							asMETHOD(CaffComp::CameraComponent, isActive),                      asCALL_THISCALL)
			.addMethod("void setActive(const bool)",					asMETHOD(CaffComp::CameraComponent, setActive),                     asCALL_THISCALL)
        .end()
    
        // Box Collider
        .addValueType<CaffComp::BoxCollider>("BoxCollider", asOBJ_APP_CLASS_CDK) // TODO: Make getValueType!
			.addCtor("void f(const Math::Vec3 &in)",                  asFUNCTION(CaffGlue::Comp::ColliderBoxCtor1),   asCALL_CDECL_OBJLAST)
			.addCtor("void f(const float, const float, const float)", asFUNCTION(CaffGlue::Comp::ColliderBoxCtor2),   asCALL_CDECL_OBJFIRST)
			.addCtor("void f()",                                      asFUNCTION(CaffGlue::Comp::ColliderBoxCtor2),   asCALL_CDECL_OBJFIRST)
			.addCtor("void f(const BoxCollider &in)",                 asFUNCTION(CaffGlue::Comp::ColliderBoxCCtor),   asCALL_CDECL_OBJLAST)
			.addDtor("void f()",                                      asFUNCTION(CaffGlue::Comp::ColliderBoxDtor),    asCALL_CDECL_OBJLAST)
        .end()
    
        // Cube Collider
        .addValueType<CaffComp::CubeCollider>("CubeCollider", asOBJ_APP_CLASS_CDK) // TODO: Make getValueType!
			.addCtor("void f()",                         asFUNCTION(CaffGlue::Comp::ColliderCubeCtor),   asCALL_CDECL_OBJFIRST)
			.addCtor("void f(const float)",              asFUNCTION(CaffGlue::Comp::ColliderCubeCtor),   asCALL_CDECL_OBJFIRST)
			.addCtor("void f(const CubeCollider &in)",   asFUNCTION(CaffGlue::Comp::ColliderCubeCCtor),  asCALL_CDECL_OBJLAST)
			.addDtor("void f()",                         asFUNCTION(CaffGlue::Comp::ColliderCubeDtor),   asCALL_CDECL_OBJLAST)
        .end()
    
        // Static Plane Collider
        .addValueType<CaffComp::StaticPlaneCollider>("StaticPlaneCollider", asOBJ_APP_CLASS_CDK)  // TODO: Make getValueType!
			.addCtor("void f(const Math::Vec4 &in)",                                        asFUNCTION(CaffGlue::Comp::ColliderSPlaneCtor1),    asCALL_CDECL_OBJLAST)
			.addCtor("void f()",                                                            asFUNCTION(CaffGlue::Comp::ColliderSPlaneCtor2),    asCALL_CDECL_OBJFIRST)
			.addCtor("void f(const float x, const float y, const float z, const float w)",  asFUNCTION(CaffGlue::Comp::ColliderSPlaneCtor2),    asCALL_CDECL_OBJFIRST)
			.addCtor("void f(const StaticPlaneCollider &in)",                               asFUNCTION(CaffGlue::Comp::ColliderSPlaneCCtor),    asCALL_CDECL_OBJLAST)
			.addDtor("void f()",                                                            asFUNCTION(CaffGlue::Comp::ColliderSPlaneDtor),     asCALL_CDECL_OBJLAST)
        .end()

        // Compound Collider
        .addValueType<CaffComp::CompoundCollider>("CompoundCollider", asOBJ_APP_CLASS_CDK) // TODO: Make getValueType!
			.addCtor("void f()",                                 asFUNCTION(CaffGlue::Comp::ColliderCompoundCtor),   asCALL_CDECL_OBJLAST)
			.addCtor("void f(const CompoundCollider &in)",       asFUNCTION(CaffGlue::Comp::ColliderCompoundCCtor),  asCALL_CDECL_OBJLAST)
			.addDtor("void f()",                                 asFUNCTION(CaffGlue::Comp::ColliderCompoundDtor),   asCALL_CDECL_OBJLAST)
			.addMethod("uint numberOfColliders() const",														  asMETHOD(CaffComp::CompoundCollider, numberOfColliders),    asCALL_THISCALL)
			.addMethod("void addCollider(const Math::Vec3 &in, const Math::Vec3 &in, BoxCollider &in)",           asFUNCTION(CaffGlue::Comp::ColliderCompoundAddCollider),      asCALL_CDECL_OBJLAST)
			.addMethod("void addCollider(const Math::Vec3 &in, const Math::Vec3 &in, CubeCollider &in)",          asFUNCTION(CaffGlue::Comp::ColliderCompoundAddCollider),      asCALL_CDECL_OBJLAST)
			.addMethod("void addCollider(const Math::Vec3 &in, const Math::Vec3 &in, StaticPlaneCollider &in)",   asFUNCTION(CaffGlue::Comp::ColliderCompoundAddCollider),      asCALL_CDECL_OBJLAST)
        .end();
    
    
    
    int r = 0;
    
    // Physics
    //r = engine->RegisterObjectBehaviour("Component", asBEHAVE_REF_CAST, "PhysicsComponent@ f()", asFUNCTION((ASHelper::refCast<CaffEnt::Component, CaffComp::PhysicsComponent>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
    r = engine->RegisterObjectMethod("PhysicsComponent", "void setMass(const float)",				asMETHOD(CaffComp::PhysicsComponent, setMass),						asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("PhysicsComponent", "float getMass() const",					asMETHOD(CaffComp::PhysicsComponent, getMass),						asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("PhysicsComponent", "void setTrigger(const bool)",				asMETHOD(CaffComp::PhysicsComponent, setTrigger),					asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("PhysicsComponent", "bool isTrigger() const",					asMETHOD(CaffComp::PhysicsComponent, isTrigger),					asCALL_THISCALL); assert(r >= 0);
    //r = engine->RegisterObjectMethod("PhysicsComponent", "void setColliderShape(StaticPlaneCollider &in) const", asMETHOD(CaffComp::PhysicsComponent, setColliderShape),asCALL_THISCALL); assert(r >= 0);
    //r = engine->RegisterObjectMethod("PhysicsComponent", "void setColliderShape(BoxCollider &in) const", asMETHOD(CaffComp::PhysicsComponent, setColliderShape),asCALL_THISCALL); assert(r >= 0);
    //r = engine->RegisterObjectMethod("PhysicsComponent", "void setColliderShape(CompoundCollider &in) const", asMETHOD(CaffComp::PhysicsComponent, setColliderShape),asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("PhysicsComponent", "void applyForce(const Math::Vec3 &in)", asFUNCTION(CaffGlue::Comp::ApplyForce), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("PhysicsComponent", "void applyTorque(const Math::Vec3 &in)", asFUNCTION(CaffGlue::Comp::ApplyTorque), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("PhysicsComponent", "void applyLocalForce(const Math::Vec3 &in)", asFUNCTION(CaffGlue::Comp::ApplyLocalForce), asCALL_CDECL_OBJLAST); assert(r >= 0);
	
	r = engine->RegisterObjectMethod("PhysicsComponent", "bool isActive() const",					asMETHOD(CaffEnt::Component, isActive), asCALL_THISCALL);  assert(r >= 0);
	r = engine->RegisterObjectMethod("PhysicsComponent", "void setActive(const bool)",				asMETHOD(CaffEnt::Component, setActive), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("PhysicsComponent", "void showDebug(const bool)",				asMETHOD(CaffComp::PhysicsComponent, showDebug), asCALL_THISCALL); assert(r >= 0);
    
    //r = engine->RegisterObjectBehaviour("Component", asBEHAVE_REF_CAST, "TransformComponent@ f()", asFUNCTION((ASHelper::refCast<CaffEnt::Component, CaffComp::TransformComponent>)), asCALL_CDECL_OBJLAST); assert(r >= 0);
    
    asEngine.addRefType("TransformComponent", asOBJ_NOCOUNT)
        .addMethod("void setPosition(const Math::Vec3 &in)", 	asFUNCTION(TransformSetPosition),				asCALL_CDECL_OBJLAST)
		.addMethod("Math::Vec3 getForwardVec() const",			asFUNCTION(TransformGetForward),				asCALL_CDECL_OBJLAST)
		.addMethod("Math::Vec3 getUpVec() const",				asFUNCTION(TransformGetUp),						asCALL_CDECL_OBJLAST)
		.addMethod("Math::Vec3 getRightVec() const",			asFUNCTION(TransformGetRight),					asCALL_CDECL_OBJLAST)
        .addMethod("Math::Vec3 getPosition() const",            asFUNCTION(TransformGetPosition),               asCALL_CDECL_OBJLAST)
        .addMethod("Math::Mat4 getWorldMatrix() const", 		asFUNCTION(TransformGetWorldMatrix), 			asCALL_CDECL_OBJLAST)
        .addMethod("void setRotationEuler(Math::Vec3 &in)", 	asFUNCTION(TransformSetEulerRotation), 			asCALL_CDECL_OBJLAST)
        .addMethod("Math::Vec3 getRotationEuler() const", 		asFUNCTION(TransformGetRotation), 				asCALL_CDECL_OBJLAST)
        .addMethod("void setScale(Math::Vec3 &in)", 			asFUNCTION(TransformSetScale),					asCALL_CDECL_OBJLAST)
    
        .addMethod("Math::Quat getRotation() const",			asMETHOD(CaffComp::TransformComponent, getRotation), asCALL_THISCALL)
        //.addMethod("void setRotation(const Math::Quat &in)",    asMETHOD(CaffComp::TransformComponent, setRotation), asCALL_THISCALL)
        .addMethod("void setRotation(const Math::Quat &in)",    asFUNCTION(TransformSetRotation),               asCALL_CDECL_OBJLAST)
    .end();
    
//    r = engine->RegisterObjectMethod("TransformComponent", "void setPosition(const Math::Vec3 &in)", asFUNCTION(CaffGlue::Comp::SetWorldPosition), asCALL_CDECL_OBJLAST); assert(r >= 0);
//	r = engine->RegisterObjectMethod("TransformComponent", "Math::Mat4 getWorldMatrix() const", asFUNCTION(TransformGetWorldMatrix), asCALL_CDECL_OBJLAST); assert(r >= 0);
//	r = engine->RegisterObjectMethod("TransformComponent", "void setRotation(Math::Vec3 &in)", asFUNCTION(TransformSetRotation), asCALL_CDECL_OBJLAST); assert(r >= 0);
//	r = engine->RegisterObjectMethod("TransformComponent", "Math::Vec3 getRotation() const", asFUNCTION(TransformGetRotation), asCALL_CDECL_OBJLAST); assert(r >= 0);
//    r = engine->RegisterObjectMethod("TransformComponent", "void setScale(Math::Vec3 &in)", asFUNCTION(TransformSetScale), asCALL_CDECL_OBJLAST); assert(r >= 0);
    
    // Entity
    r = engine->RegisterObjectMethod("Entity", "string getName() const",                    asMETHOD(CaffEnt::Entity, getName),             asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "string setName(const string &in)",          asMETHOD(CaffEnt::Entity, setName),             asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "uint getID() const",                        asMETHOD(CaffEnt::Entity, getID),               asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "void setID(const uint)",                    asMETHOD(CaffEnt::Entity, setID),               asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "void addTag(const string &in)",             asMETHOD(CaffEnt::Entity, addTag),              asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "bool hasTag(const string &in) const",       asMETHOD(CaffEnt::Entity, hasTag),              asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "void clearTags()",                          asMETHOD(CaffEnt::Entity, clearTags),           asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "bool isActive() const",                     asMETHOD(CaffEnt::Entity, isActive),            asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "void setActive(const bool)",                asMETHOD(CaffEnt::Entity, setActive),           asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "void destroy()",                            asMETHOD(CaffEnt::Entity, destroy),             asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "bool isDestroyed() const",                  asMETHOD(CaffEnt::Entity, isDestroyed),         asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Entity@ getParent() const",                 asMETHOD(CaffEnt::Entity, getParent),           asCALL_THISCALL); assert(r >= 0);
    //r = engine->RegisterObjectMethod("Entity", "void addChild(Entity @)",                 asMETHOD(CaffEnt::Entity, addChild),            asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("Entity", "void addChild(Entity @)",					asFUNCTION(CaffGlue::Comp::AddChild),			asCALL_CDECL_OBJLAST); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Entity@ findChild(const string &in) const", asFUNCTION(EntityGetChild),						asCALL_CDECL_OBJLAST); assert(r >= 0);
	//asMETHODPR(CaffInter::ScriptComponent, cast, (void **, int), void)
    r = engine->RegisterObjectMethod("Entity", "uint numberOfChidren() const",              asMETHOD(CaffEnt::Entity, numberOfChildren),    asCALL_THISCALL); assert(r >= 0);
    //r = engine->RegisterObjectMethod("Entity", "bool addComponent(const string &in)",       asMETHODPR(CaffEnt::Entity, addComponent, (const std::string&, const CaffUtil::Param&), bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("Entity", "bool addComponent(const string &in)", asFUNCTION(AddComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Component@ getComponent(const string &in) const", asMETHOD(CaffEnt::Entity, getComponentByName), asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "uint numberOfComponents() const",           asMETHOD(CaffEnt::Entity, numberOfComponents),  asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Window@ getWindow() const",                 asMETHOD(CaffEnt::Entity, getWindow),           asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Input@ getInput() const",                   asMETHOD(CaffEnt::Entity, getInput),            asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "Renderer@ getRenderer() const",             asMETHOD(CaffEnt::Entity, getRenderer),         asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "CameraManager@ getCameraManager() const",	asMETHOD(CaffEnt::Entity, getCameraManager),	asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "EventManager@ getEventManager() const",		asMETHOD(CaffEnt::Entity, getEventManager),		asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("Entity", "ModelManager@ getModelManager() const",		asMETHOD(CaffEnt::Entity, getModelManager),		asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "StateManager@ getStateManager() const",     asMETHOD(CaffEnt::Entity, getStateManager),     asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("Entity", "TextRenderer@ getTextRenderer() const",		asMETHOD(CaffEnt::Entity, getTextRenderer),		asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "State@ getState() const",                   asMETHOD(CaffEnt::Entity, getState),            asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectMethod("Entity", "bool onEvent(const Event &in data)",        asMETHOD(CaffEnt::Entity, onEvent),             asCALL_THISCALL); assert(r >= 0);
	
	// Collider
	r = engine->RegisterObjectProperty("CollidedWith", "Entity @entity", asOFFSET(CaffEnt::ColliderDetails, collidedWith)); assert(r >= 0);
	r = engine->RegisterObjectProperty("CollidedWith", "int test", asOFFSET(CaffEnt::ColliderDetails, test)); assert(r >= 0);
}


#pragma mark - Rigidbody

void ApplyForce(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj)					{ assert(obj); obj->applyForce(&dir.x); }
void ApplyLocalForce(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj)				{ assert(obj); obj->applyLocalForce(&dir.x); }
void ApplyTorque(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj)					{ assert(obj); obj->applyTorque(&dir.x); }


#pragma mark - Colliders

//void ColliderCtor(void *mem)																	{ new(mem) CaffComp::Collider();											}
//void ColliderCCtor(CaffComp::Collider &other, void *mem)										{ new(mem) CaffComp::Collider(other);										}
//void ColliderDtor(void *mem)																	{ static_cast<CaffComp::Collider*>(mem)->~Collider();						}

void ColliderBoxCtor1(const CaffMath::Vector3 &vec3, void *mem)									{ new(mem) CaffComp::BoxCollider(vec3.x, vec3.y, vec3.z);					}
void ColliderBoxCtor2(void *mem, const float x, const float y, const float z)					{ new(mem) CaffComp::BoxCollider(x, y, z);									}
void ColliderBoxCCtor(CaffComp::BoxCollider &other, void *mem)									{ new(mem) CaffComp::BoxCollider(other);									}
void ColliderBoxDtor(void *mem)																	{ static_cast<CaffComp::BoxCollider*>(mem)->~BoxCollider();					}

void ColliderCubeCtor(void *mem, const float x)													{ new(mem) CaffComp::CubeCollider(x);										}
void ColliderCubeCCtor(CaffComp::CubeCollider &other, void *mem)								{ new(mem) CaffComp::CubeCollider(other);									}
void ColliderCubeDtor(void *mem)																{ static_cast<CaffComp::CubeCollider*>(mem)->~CubeCollider();				}

void ColliderSPlaneCtor1(const CaffMath::Vector4 &vec4, void *mem)								{ new(mem) CaffComp::StaticPlaneCollider(vec4.x, vec4.y, vec4.z, vec4.w);	}
void ColliderSPlaneCtor2(void *mem, const float x, const float y, const float z, const float w)	{ new(mem) CaffComp::StaticPlaneCollider(x, y, z, w);						}
void ColliderSPlaneCCtor(CaffComp::StaticPlaneCollider &other, void *mem)						{ new(mem) CaffComp::StaticPlaneCollider(other);							}
void ColliderSPlaneDtor(void *mem)																{ static_cast<CaffComp::StaticPlaneCollider*>(mem)->~StaticPlaneCollider(); }

void ColliderCompoundCtor(void *mem)															{ new(mem) CaffComp::CompoundCollider();									}
void ColliderCompoundCCtor(CaffComp::CompoundCollider &other, void *mem)						{ new(mem) CaffComp::CompoundCollider(other);								}
void ColliderCompoundDtor(void *mem)															{ static_cast<CaffComp::CompoundCollider*>(mem)->~CompoundCollider();		}

void ColliderCompoundAddCollider(const CaffMath::Vector3 &pos, const CaffMath::Vector3 &rot, CaffComp::Collider &collider, CaffComp::CompoundCollider *obj)
{
	const glm::vec3 position(pos.x, pos.y, pos.z);
	const glm::vec3 rotation(rot.x, rot.y, rot.z);
	
	obj->addCollider(position, rotation, collider);
}



#pragma mark - Camera Component

CaffMath::Matrix44 GetViewMat(CaffComp::CameraComponent *camera)
{
	assert(camera);
	CaffMath::Matrix44 mat4;
	glm::mat4 glmMat = camera->getViewMatrix();
	
	mat4._11 = glmMat[0][0];
	mat4._12 = glmMat[0][1];
	mat4._13 = glmMat[0][2];
	mat4._14 = glmMat[0][3];

	mat4._21 = glmMat[1][0];
	mat4._22 = glmMat[1][1];
	mat4._23 = glmMat[1][2];
	mat4._24 = glmMat[1][3];
	
	mat4._31 = glmMat[2][0];
	mat4._32 = glmMat[2][1];
	mat4._33 = glmMat[2][2];
	mat4._34 = glmMat[2][3];
	
	mat4._41 = glmMat[3][0];
	mat4._42 = glmMat[3][1];
	mat4._43 = glmMat[3][2];
	mat4._44 = glmMat[3][3];
	
	//memcpy(&mat4, &camera->getViewMatrix()[0][0], 16 * sizeof(float));
	
	return mat4;
}


CaffMath::Matrix44 GetProjMat(CaffComp::CameraComponent *camera)
{
	assert(camera);
	CaffMath::Matrix44 mat4;
	glm::mat4 glmMat = camera->getProjMatrix();
	
	mat4._11 = glmMat[0][0];
	mat4._12 = glmMat[0][1];
	mat4._13 = glmMat[0][2];
	mat4._14 = glmMat[0][3];

	mat4._21 = glmMat[1][0];
	mat4._22 = glmMat[1][1];
	mat4._23 = glmMat[1][2];
	mat4._24 = glmMat[1][3];
	
	mat4._31 = glmMat[2][0];
	mat4._32 = glmMat[2][1];
	mat4._33 = glmMat[2][2];
	mat4._34 = glmMat[2][3];
	
	mat4._41 = glmMat[3][0];
	mat4._42 = glmMat[3][1];
	mat4._43 = glmMat[3][2];
	mat4._44 = glmMat[3][3];
	
	//memcpy(&mat4, &camera->getViewMatrix()[0][0], 16 * sizeof(float));
	
	return mat4;
}


CaffMath::Matrix44 GetViewProjMat(CaffComp::CameraComponent *camera)
{
	assert(camera);
	CaffMath::Matrix44 mat4;
	glm::mat4 glmMat = camera->getViewProjMatrix();
	
	mat4._11 = glmMat[0][0];
	mat4._12 = glmMat[0][1];
	mat4._13 = glmMat[0][2];
	mat4._14 = glmMat[0][3];

	mat4._21 = glmMat[1][0];
	mat4._22 = glmMat[1][1];
	mat4._23 = glmMat[1][2];
	mat4._24 = glmMat[1][3];
	
	mat4._31 = glmMat[2][0];
	mat4._32 = glmMat[2][1];
	mat4._33 = glmMat[2][2];
	mat4._34 = glmMat[2][3];
	
	mat4._41 = glmMat[3][0];
	mat4._42 = glmMat[3][1];
	mat4._43 = glmMat[3][2];
	mat4._44 = glmMat[3][3];
	
	//memcpy(&mat4, &camera->getViewMatrix()[0][0], 16 * sizeof(float));
	
	return mat4;
}



#pragma mark - Entity

void AddChild(CaffEnt::Entity *otherEntity, CaffEnt::Entity *self)
{
	assert(otherEntity != self);
	
	std::unique_ptr<CaffEnt::Entity> removeEntFromState = self->getState()->removeEntityByID(otherEntity->getID());
	
	if(removeEntFromState)
	{
		self->addChild(removeEntFromState);
	}
}

    
} // namespace
} // namespace
} // namespace
