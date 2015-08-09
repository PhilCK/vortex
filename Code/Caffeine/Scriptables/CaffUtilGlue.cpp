
#include <Caffeine/Scriptables/CaffUtilGlue.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <ASHelper.hpp>
#include <iostream>


namespace Caffeine {
namespace ScriptGlue {


namespace Utils {
    
    
#pragma mark - Register
    
void RegisterTypes(asIScriptEngine *engine)
{
	ASEngine asEngine(engine);
		
	asEngine
		.addValueType<CaffUtil::Param>("Param", asOBJ_APP_CLASS_CDK | asOBJ_ASHANDLE).end()
    .end();
}
    

namespace
{
    void ParamCtor(void *mem)							{ assert(mem); new(mem) CaffUtil::Param();                      }
    void ParamCCtor(CaffUtil::Param &other, void *mem)	{ assert(mem); new((void*)mem) CaffUtil::Param(other);          }
    void ParamDtor(void *mem)							{ assert(mem); static_cast<CaffUtil::Param*>(mem)->~Param();    }
    
    Math::Vector3 ParamAsVec3(const Math::Vector3 &vec, const CaffUtil::Param *obj)
    {
        assert(obj);
        
        const std::array<float, 3> defaultVals = {{vec.x, vec.y, vec.z}};
        const std::array<float, 3> float3 = obj->asFloat3(defaultVals);
     
        enum { X = 0, Y, Z};
        const Math::Vector3 returnVec = {float3.at(X), float3.at(Y), float3.at(Z)};
        
        return returnVec;
    }
	
}
    
    
void RegisterInteraction(asIScriptEngine *engine)
{
	ASEngine asEngine(engine);
		
	asEngine
        // Global functions.
        .pushNamespace("Util")
            .addGlobalFunction("void LogInfo(const string &in)",    asFUNCTION(CaffUtil::LogInfo),      asCALL_CDECL)
            .addGlobalFunction("void LogWarning(const string &in)", asFUNCTION(CaffUtil::LogWarning),   asCALL_CDECL)
            .addGlobalFunction("void LogError(const string &in)",   asFUNCTION(CaffUtil::LogError),     asCALL_CDECL)
        .popNamespace()
    
        // Param - TODO: Missing functions, math and non-const operator[], int64
		.addValueType<CaffUtil::Param>("Param", asOBJ_APP_CLASS_CDK | asOBJ_ASHANDLE)
		.addCtor("void f()",			asFUNCTION(CaffGlue::Utils::ParamCtor),		asCALL_CDECL_OBJLAST)
		.addCtor("void f(Param &in)",	asFUNCTION(CaffGlue::Utils::ParamCCtor),	asCALL_CDECL_OBJLAST)
		.addDtor("void f()",			asFUNCTION(CaffGlue::Utils::ParamDtor),		asCALL_CDECL_OBJLAST)
    
		.addMethod("string getName() const",								asMETHOD(CaffUtil::Param, getName),					asCALL_THISCALL)
		.addMethod("bool isRoot() const",									asMETHOD(CaffUtil::Param, isRoot),					asCALL_THISCALL)
		.addMethod("bool doesMemberExist(const string &in) const",			asMETHOD(CaffUtil::Param, doesMemberExist),			asCALL_THISCALL)
		.addMethod("uint numberOfMembersOfName(const string &in) const",	asMETHOD(CaffUtil::Param, numberOfMembersOfName),	asCALL_THISCALL)
		.addMethod("uint numberOfMembers() const",							asMETHOD(CaffUtil::Param, numberOfMembers),			asCALL_THISCALL)
		.addMethod("string getDataAsTree() const",							asMETHOD(CaffUtil::Param, getDataAsTree),			asCALL_THISCALL)
    
        .addMethod("const Param& opIndex(const string &in) const",	asMETHODPR(CaffUtil::Param, operator[], (const std::string&), CaffUtil::Param&),  asCALL_THISCALL)
        .addMethod("const Param& opIndex(const uint) const",		asMETHODPR(CaffUtil::Param, operator[], (const std::size_t), CaffUtil::Param&),   asCALL_THISCALL)
    
        .addMethod("Param& push(const string &in)",                 asMETHOD(CaffUtil::Param, push),		asCALL_THISCALL)
        .addMethod("Param& addNode(const Param &in)",               asMETHOD(CaffUtil::Param, addNode),		asCALL_THISCALL)
    
        .addMethod("void opAssign(const float)",                    asMETHODPR(CaffUtil::Param, operator=, (const float), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const double)",                   asMETHODPR(CaffUtil::Param, operator=, (const double), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const uint)",                     asMETHODPR(CaffUtil::Param, operator=, (const uint32_t), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const int)",                      asMETHODPR(CaffUtil::Param, operator=, (const int32_t), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const uint64)",                   asMETHODPR(CaffUtil::Param, operator=, (const uint64_t), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const int64)",                    asMETHODPR(CaffUtil::Param, operator=, (const int64_t), void),		asCALL_THISCALL)
        .addMethod("void opAssign(const string &in)",               asMETHODPR(CaffUtil::Param, operator=, (const std::string&), void), asCALL_THISCALL)
    
        .addMethod("float asFloat(const float = 0.f) const",        asMETHOD(CaffUtil::Param, asFloat),         asCALL_THISCALL)
        .addMethod("uint asInt(const int = 0) const",               asMETHOD(CaffUtil::Param, asInt32),         asCALL_THISCALL)
        .addMethod("uint asUInt(const uint = 0) const",             asMETHOD(CaffUtil::Param, asUInt32),        asCALL_THISCALL)
        .addMethod("bool asBool(const bool = false) const",         asMETHOD(CaffUtil::Param, asBool),          asCALL_THISCALL)
        .addMethod("string asString(const string &in = "") const",  asMETHOD(CaffUtil::Param, asStdString),     asCALL_THISCALL)
        .addMethod("Math::Vec3 asVec3(const Math::Vec3 &in = {0.f, 0.f, 0.f}) const",			asFUNCTION(ParamAsVec3),	asCALL_CDECL_OBJLAST)
    
		.end()
		
		//
		
    .end();
}

    
} // namespace
} // namespace
} // namespace
