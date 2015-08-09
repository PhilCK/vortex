
#ifndef CAFFEINE_UTILS_SCRIPT_GLUE_INCLUDED
#define CAFFEINE_UTILS_SCRIPT_GLUE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <AngelScript/angelscript.h>
#include <vector>


namespace Caffeine {
namespace ScriptGlue {


namespace Utils {

    
#pragma mark - Register
    
void            RegisterTypes(asIScriptEngine *engine);
void            RegisterInteraction(asIScriptEngine *engine);


} // namespace
} // namespace
} // namespace


#endif // include guard
