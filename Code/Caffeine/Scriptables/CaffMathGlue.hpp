
#ifndef CAFFEINE_MATH_SCRIPT_GLUE_INCLUDED
#define CAFFEINE_MATH_SCRIPT_GLUE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Caffeine/Math/Quaternion.hpp>
#include <AngelScript/angelscript.h>
#include <Includes/AngelScript/add_on/scriptarray/scriptarray.h>
#include <vector>


namespace Caffeine {
namespace ScriptGlue {
namespace Math {
    
    
#pragma mark - Register
    
void            RegisterTypes(asIScriptEngine *engine);
void            RegisterInteraction(asIScriptEngine *engine);

    
    
#pragma mark - Glue Methods
    
void			Vec2ListCtor(const CaffMath::Vector2 *self, void *mem);
void			Vec3ListCtor(const CaffMath::Vector3 *self, void *mem);
void			Vec4ListCtor(const CaffMath::Vector4 *self, void *mem);
void            Mat3ListCtor(const CaffMath::Matrix33 *self, void *mem);
void            Mat4ListCtor(const CaffMath::Matrix44 *self, void *mem);


} // namespace
} // namespace
} // namespace


#endif // include guard
