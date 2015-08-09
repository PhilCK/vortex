
#ifndef CAFFEINE_APPLICATION_SCRIPT_GLUE_INCLUDED
#define CAFFEINE_APPLICATION_SCRIPT_GLUE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Includes/AngelScript/add_on/scriptarray/scriptarray.h>
#include <vector>


namespace Caffeine {
namespace ScriptGlue {


namespace App {
    
#pragma mark - Register
    
void            RegisterTypes(asIScriptEngine *engine);
void            RegisterInteraction(asIScriptEngine *engine);
    

#pragma mark - Format Desc Glue
    
void			FormatDescCtor(void *mem);
void			FormatDescCCtor(const CaffApp::FormatDesc &other, void *mem);
void			FormatDescListCtor(const CaffApp::FormatDesc *self, void *mem);
void			FormatDescDotr(void *mem);


#pragma mark - Renderer Glue

std::size_t		AddVertexFormat(CScriptArray *array, CaffApp::Renderer *renderer);
std::size_t     AddVertexBuffer(CScriptArray *array, CaffApp::Renderer *renderer);
    
std::size_t     AddShader(const std::string &path, CaffApp::Renderer *renderer);
std::size_t     AddTexture(const std::string &path, CaffApp::Renderer *renderer);
    
void			SetShader1f(const std::string &constant, const float value, CaffApp::Renderer *renderer);
void			SetShader2f(const std::string &constant, const CaffMath::Vector2 &vec, CaffApp::Renderer *renderer);
void			SetShader3f(const std::string &constant, const CaffMath::Vector3 &vec, CaffApp::Renderer *renderer);
void			SetShader4f(const std::string &constant, const CaffMath::Vector4 &vec, CaffApp::Renderer *renderer);
void			SetMatrix44(const std::string &constant, const CaffMath::Matrix44 &mat, CaffApp::Renderer *renderer);

} // namespace


} // namespace
} // namespace


#endif // include guard
