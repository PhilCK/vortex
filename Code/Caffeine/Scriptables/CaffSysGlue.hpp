
#ifndef CAFFEINE_SYSTEMS_SCRIPT_GLUE_INCLUDED
#define CAFFEINE_SYSTEMS_SCRIPT_GLUE_INCLUDED



#include <Caffeine/Platform.hpp>
#include <AngelScript/angelscript.h>



namespace Caffeine {
namespace ScriptGlue {
namespace Systems {



void RegisterTypes(asIScriptEngine *engine);
void RegisterInteraction(asIScriptEngine *engine);



} // namespace
} // namespace
} // namespace


#endif // include guard
