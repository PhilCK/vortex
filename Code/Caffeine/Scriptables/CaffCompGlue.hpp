
#ifndef CAFFEINE_COMPONENT_SCRIPT_GLUE_INCLUDED
#define CAFFEINE_COMPONENT_SCRIPT_GLUE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Includes/AngelScript/add_on/scriptarray/scriptarray.h>


namespace Caffeine {
namespace ScriptGlue {
namespace Comp {

    
    
#pragma mark - Register
    
void                RegisterTypes(asIScriptEngine *engine);
void                RegisterInteraction(asIScriptEngine *engine);


#pragma mark - Rigidbody

void				ApplyForce(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj);
void				ApplyLocalForce(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj);
void				ApplyTorque(const CaffMath::Vector3 &dir, CaffComp::PhysicsComponent *obj);
    

#pragma mark - Transform

void				SetWorldPosition(const CaffMath::Vector3 &pos, CaffComp::TransformComponent *obj);


#pragma mark - Colliders


void				ColliderCtor(void *mem);
void				ColliderCCtor(CaffComp::Collider &other, void *mem);
void				ColliderDtor(void *mem);

void				ColliderBoxCtor1(const CaffMath::Vector3 &vec3, void *mem);
void				ColliderBoxCtor2(void *mem, const float x = 0.f, const float y = 0.f, const float z = 0.f);
void				ColliderBoxCCtor(CaffComp::BoxCollider &other, void *mem);
void				ColliderBoxDtor(void *mem);

void				ColliderCubeCtor(void *mem, const float x = 0.f);
void				ColliderCubeCCtor(CaffComp::CubeCollider &other, void *mem);
void				ColliderCubeDtor(void *mem);

void				ColliderSPlaneCtor1(const CaffMath::Vector4 &vec4, void *mem);
void				ColliderSPlaneCtor2(void *mem, const float x = 0.f, const float y = 0.f, const float z = 0.f, const float w = 0.f);
void				ColliderSPlaneCCtor(CaffComp::StaticPlaneCollider &other, void *mem);
void				ColliderSPlaneDtor(void *mem);
	
void				ColliderCompoundCtor(void *mem);
void				ColliderCompoundCCtor(CaffComp::CompoundCollider &other, void *mem);
void				ColliderCompoundDtor(void *mem);

void				ColliderCompoundAddCollider(const CaffMath::Vector3 &pos, const CaffMath::Vector3 &rot, CaffComp::Collider &collider, CaffComp::CompoundCollider *obj);


#pragma mark - Camera Component

CaffMath::Matrix44	GetViewMat(CaffComp::CameraComponent *camera);
CaffMath::Matrix44	GetProjMat(CaffComp::CameraComponent *camera);
CaffMath::Matrix44	GetViewProjMat(CaffComp::CameraComponent *camera);
void				SetLookAt(const CaffMath::Vector3 &vec3, CaffComp::CameraComponent *camera);
void				SetPosition(const CaffMath::Vector3 &vec3, CaffComp::CameraComponent *camera);


#pragma mark - Entity
void				AddChild(CaffEnt::Entity *otherEntity, CaffEnt::Entity *self);


} // namespace
} // namespace
} // namespace

#endif // include guard
