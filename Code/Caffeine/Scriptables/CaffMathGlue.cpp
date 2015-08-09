
#include <Caffeine/Scriptables/CaffMathGlue.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <ASHelper.hpp>


namespace Caffeine {
namespace ScriptGlue {


namespace Math {
    
    
#pragma mark - Register
    
void RegisterTypes(asIScriptEngine *engine)
{
	ASEngine asEngine(engine);
	
	asEngine
		.clearNamespaces()
        .pushNamespace("Math")
        
            .addValueType<CaffMath::Vector2>("Vec2", asOBJ_POD  | asOBJ_APP_CLASS_ALLFLOATS).end()
            .addValueType<CaffMath::Vector3>("Vec3", asOBJ_POD  | asOBJ_APP_CLASS_ALLFLOATS).end()
            .addValueType<CaffMath::Vector4>("Vec4", asOBJ_POD  | asOBJ_APP_CLASS_ALLFLOATS).end()
            .addValueType<CaffMath::Matrix33>("Mat3", asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS).end()
            .addValueType<CaffMath::Matrix44>("Mat4", asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS).end()
            .addValueType<CaffMath::Quaternion>("Quat", asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS).end()
		
        .popNamespace()
    .end();
}


namespace
{
    // Vec3
    CaffMath::Vector3 Vec3Add(const CaffMath::Vector3 &other, CaffMath::Vector3 *obj)
    {
        CaffMath::Vector3 ret = CaffMath::Vector3ComponentAdd(other, *obj);
        return ret;
    }

    CaffMath::Vector3 Vec3Subtract(const CaffMath::Vector3 &other, CaffMath::Vector3 *obj)
    {
        CaffMath::Vector3 ret = CaffMath::Vector3ComponentSubtract(other, *obj);
        return ret;
    }
    
    CaffMath::Vector3 Vec3Scale(const float scale, CaffMath::Vector3 *obj)
    {
        CaffMath::Vector3 ret = CaffMath::Vector3Scale(*obj, scale);
        return ret;
    }
    
    CaffMath::Vector3 Vec3Normalize(CaffMath::Vector3 *obj)
    {
        CaffMath::Vector3 ret = CaffMath::Vector3Normalize(*obj);
        return ret;
    }
    
    float Vec3Length(CaffMath::Vector3 *obj)
    {
        return CaffMath::Vector3Magnitude(*obj);
    }
    
    
    // Util functions
    std::string Vec3ToString(const CaffMath::Vector3 &vec)
    {
        return std::string(CaffUtil::ToString(vec.x) + ", " + CaffUtil::ToString(vec.y) + ", " + CaffUtil::ToString(vec.z));
    }
    
    
    // Quat
    void QuatListCtor(const CaffMath::Quaternion *self, void *mem)
    {
        new((void*)mem) CaffMath::Quaternion;
        static_cast<CaffMath::Quaternion*>(mem)->w = self->w;
        static_cast<CaffMath::Quaternion*>(mem)->x = self->x;
        static_cast<CaffMath::Quaternion*>(mem)->y = self->y;
        static_cast<CaffMath::Quaternion*>(mem)->z = self->z;
    }
    
    void QuatAngleAxisCtor(const float angle, const CaffMath::Vector3 &axis, CaffMath::Quaternion *self)
    {
        CaffMath::Quaternion quat = CaffMath::QuaternionInitFromAngleAxis(angle, axis);
        
        static_cast<CaffMath::Quaternion*>(self)->w = quat.w;
        static_cast<CaffMath::Quaternion*>(self)->x = quat.x;
        static_cast<CaffMath::Quaternion*>(self)->y = quat.y;
        static_cast<CaffMath::Quaternion*>(self)->z = quat.z;
    }
    
    std::string QuatToString(const CaffMath::Quaternion &quat)
    {
        return std::string(CaffUtil::ToString(quat.w) + ", " + CaffUtil::ToString(quat.x) + ", " + CaffUtil::ToString(quat.y) + ", " + CaffUtil::ToString(quat.z));
    }
    
    CaffMath::Quaternion QuatMult(const CaffMath::Quaternion &quat, CaffMath::Quaternion *self)
    {
        const CaffMath::Quaternion returnQuat = CaffMath::QuaternionMultiply(*self, quat);
        
        return returnQuat;
    }
	
	CaffMath::Quaternion QuatNorm( CaffMath::Quaternion *self)
	{
		assert(self);
		return CaffMath::QuaternionNormalize(*self);
	}
	
	
	// Utils
	std::string FloatToString(const float x) { return CaffUtil::ToString(x); }
	std::string IntToString(const int x) { return CaffUtil::ToString(x); }
	std::string UIntToString(const uint32_t x) { return CaffUtil::ToString(x); }
}
    
    
void RegisterInteraction(asIScriptEngine *engine)
{
    int r = 0;
	
	ASEngine asEngine(engine);
	
	asEngine
		.clearNamespaces()
    
        .pushNamespace("Util")
    
            .addGlobalFunction("string ToString(const Math::Vec3 &in)", asFUNCTION(Vec3ToString),	asCALL_CDECL)
            .addGlobalFunction("string ToString(const Math::Quat &in)", asFUNCTION(QuatToString),	asCALL_CDECL)
			.addGlobalFunction("string ToString(const float)",			asFUNCTION(FloatToString),	asCALL_CDECL)
			.addGlobalFunction("string ToString(const int)",			asFUNCTION(IntToString),	asCALL_CDECL)
			.addGlobalFunction("string ToString(const uint)",			asFUNCTION(UIntToString),	asCALL_CDECL)
    
        .popNamespace()
		.pushNamespace("Math")

            // Quaternion
            .addValueType<CaffMath::Quaternion>("Quat", asOBJ_POD | asOBJ_APP_CLASS_C)
                .addListCtor("void f(const int &in) {float, float, float, float}",  asFUNCTION(QuatListCtor),       asCALL_CDECL_OBJLAST)
                .addCtor("void f(const float, Vec3 &in)",                           asFUNCTION(QuatAngleAxisCtor),  asCALL_CDECL_OBJLAST)
                .addMethod("Math::Quat multiply(const Math::Quat &in) const",       asFUNCTION(QuatMult),           asCALL_CDECL_OBJLAST)
				.addMethod("Math::Quat normalize() const",							asFUNCTION(QuatNorm),			asCALL_CDECL_OBJLAST)
            .end()
    
            // Vec2
            .addValueType<CaffMath::Vector2>("Vec2", asOBJ_POD | asOBJ_APP_CLASS_C)
				.addListCtor("void f(const int &in) {float, float}", asFUNCTION(CaffGlue::Math::Vec2ListCtor), asCALL_CDECL_OBJLAST)
				.addProperty("float x", asOFFSET(CaffMath::Vector2, x))
				.addProperty("float y", asOFFSET(CaffMath::Vector2, y))
            .end()
    
            // Vec3
            .addValueType<CaffMath::Vector3>("Vec3", asOBJ_POD)
				.addListCtor("void f(const int &in) {float, float, float}", asFUNCTION(CaffGlue::Math::Vec3ListCtor), asCALL_CDECL_OBJLAST)
				.addProperty("float x", asOFFSET(CaffMath::Vector3, x))
				.addProperty("float y", asOFFSET(CaffMath::Vector3, y))
				.addProperty("float z", asOFFSET(CaffMath::Vector3, z))
				.addMethod("Vec3 add(const Vec3 &in) const",        asFUNCTION(Vec3Add),        asCALL_CDECL_OBJLAST)
				.addMethod("Vec3 subtract(const Vec3 &in) const",   asFUNCTION(Vec3Subtract),   asCALL_CDECL_OBJLAST)
				.addMethod("Vec3 normalize() const",                asFUNCTION(Vec3Normalize),  asCALL_CDECL_OBJLAST)
				.addMethod("Vec3 scale(const float) const",         asFUNCTION(Vec3Scale),      asCALL_CDECL_OBJLAST)
				.addMethod("float length() const",                  asFUNCTION(Vec3Length),     asCALL_CDECL_OBJLAST)
            .end()
    
            // Vec4
            .addValueType<CaffMath::Vector4>("Vec4", asOBJ_POD | asOBJ_APP_CLASS_C)
				.addListCtor("void f(const int &in) {float, float, float, float}", asFUNCTION(CaffGlue::Math::Vec4ListCtor), asCALL_CDECL_OBJLAST)
				.addProperty("float x", asOFFSET(CaffMath::Vector4, x))
				.addProperty("float y", asOFFSET(CaffMath::Vector4, y))
				.addProperty("float z", asOFFSET(CaffMath::Vector4, z))
				.addProperty("float w", asOFFSET(CaffMath::Vector4, w))
            .end()
            
            // Math Constants
            .addGlobalFunction("float Pi()",		asFUNCTION(CaffMath::Pi),		asCALL_CDECL)
            .addGlobalFunction("float TwoPi()",		asFUNCTION(CaffMath::TwoPi),	asCALL_CDECL)
            .addGlobalFunction("float HalfPi()",	asFUNCTION(CaffMath::HalfPi),	asCALL_CDECL)
            .addGlobalFunction("float Tau()",		asFUNCTION(CaffMath::Tau),		asCALL_CDECL)
            .addGlobalFunction("float HalfTau()",	asFUNCTION(CaffMath::HalfTau),	asCALL_CDECL)
            .addGlobalFunction("float QuartTau()",	asFUNCTION(CaffMath::QuartTau), asCALL_CDECL)
            .addGlobalFunction("float E()",			asFUNCTION(CaffMath::E),		asCALL_CDECL)
            .addGlobalFunction("float GRatio()",	asFUNCTION(CaffMath::GRatio),	asCALL_CDECL)
            .addGlobalFunction("float Root2()",		asFUNCTION(CaffMath::Root2),	asCALL_CDECL)
            .addGlobalFunction("float Root3()",		asFUNCTION(CaffMath::Root3),	asCALL_CDECL)
			
			// General
			.addGlobalFunction("float Min(const float, const float)", asFUNCTION((CaffMath::Min<float>)), asCALL_CDECL)
			.addGlobalFunction("float Min(const int, const int)", asFUNCTION((CaffMath::Min<int>)), asCALL_CDECL)
			.addGlobalFunction("float Min(const uint, const uint)", asFUNCTION((CaffMath::Min<unsigned int>)), asCALL_CDECL)
			
			.addGlobalFunction("float Max(const float, const float)", asFUNCTION((CaffMath::Max<float>)), asCALL_CDECL)
			
			.addGlobalFunction("float Abs(const float)", asFUNCTION((CaffMath::Abs<float>)), asCALL_CDECL)
			
			.addGlobalFunction("float Clamp(const float, const float, const float)", asFUNCTION((CaffMath::Clamp<float>)), asCALL_CDECL)
		
            // Trig
            .addGlobalFunction("float Sin(const float x)",	asFUNCTION(CaffMath::Sin),	asCALL_CDECL)
            .addGlobalFunction("float ASin(const float x)", asFUNCTION(CaffMath::ASin), asCALL_CDECL)
            .addGlobalFunction("float Cos(const float x)",	asFUNCTION(CaffMath::Cos),	asCALL_CDECL)
            .addGlobalFunction("float ACos(const float x)", asFUNCTION(CaffMath::ACos), asCALL_CDECL)
            .addGlobalFunction("float Tan(const float x)",	asFUNCTION(CaffMath::Tan),	asCALL_CDECL)
            .addGlobalFunction("float ATan(const float x)", asFUNCTION(CaffMath::ATan), asCALL_CDECL)
            .addGlobalFunction("float ATan2(const float x, const float y)", asFUNCTION(CaffMath::ATan2), asCALL_CDECL)
			
			// Rand
			.addGlobalFunction("float RandFloatRange(const float x, const float y)",	asFUNCTION(CaffMath::RandFloatRange),	asCALL_CDECL)
		
		//.popNamespace() // Math
		.end();
    
    
    // Mat3
    // List
    r = engine->RegisterObjectProperty("Mat3", "float _11", asOFFSET(CaffMath::Matrix33, _11)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _12", asOFFSET(CaffMath::Matrix33, _12)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _13", asOFFSET(CaffMath::Matrix33, _13)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _21", asOFFSET(CaffMath::Matrix33, _21)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _22", asOFFSET(CaffMath::Matrix33, _22)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _23", asOFFSET(CaffMath::Matrix33, _23)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _31", asOFFSET(CaffMath::Matrix33, _31)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _32", asOFFSET(CaffMath::Matrix33, _32)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat3", "float _33", asOFFSET(CaffMath::Matrix33, _33)); assert(r >= 0);
    
    // Mat4
    r = engine->RegisterObjectBehaviour("Mat4", asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float}", asFUNCTION(CaffGlue::Math::Mat4ListCtor), asCALL_CDECL_OBJLAST); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _11", asOFFSET(CaffMath::Matrix44, _11)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _12", asOFFSET(CaffMath::Matrix44, _12)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _13", asOFFSET(CaffMath::Matrix44, _13)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _14", asOFFSET(CaffMath::Matrix44, _14)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _21", asOFFSET(CaffMath::Matrix44, _21)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _22", asOFFSET(CaffMath::Matrix44, _22)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _23", asOFFSET(CaffMath::Matrix44, _23)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _24", asOFFSET(CaffMath::Matrix44, _24)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _31", asOFFSET(CaffMath::Matrix44, _31)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _32", asOFFSET(CaffMath::Matrix44, _32)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _33", asOFFSET(CaffMath::Matrix44, _33)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _34", asOFFSET(CaffMath::Matrix44, _34)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _41", asOFFSET(CaffMath::Matrix44, _41)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _42", asOFFSET(CaffMath::Matrix44, _42)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _43", asOFFSET(CaffMath::Matrix44, _43)); assert(r >= 0);
    r = engine->RegisterObjectProperty("Mat4", "float _44", asOFFSET(CaffMath::Matrix44, _44)); assert(r >= 0);
	
	asEngine.clearNamespaces();
}

    
    
#pragma mark - Glue Methods


void Vec2ListCtor(const CaffMath::Vector2 *self, void *mem)
{
	new((void*)mem)  CaffMath::Vector2{self->x, self->y};
}

void Vec3ListCtor(const CaffMath::Vector3 *self, void *mem)
{
	new((void*)mem)  CaffMath::Vector3{self->x, self->y, self->z};
}

void Vec4ListCtor(const CaffMath::Vector4 *self, void *mem)
{
	new((void*)mem)  CaffMath::Vector4{self->x, self->y, self->z, self->w};
}
    
void Mat3ListCtor(const CaffMath::Matrix33 *self, void *mem)
{

}

void Mat4ListCtor(const CaffMath::Matrix44 *self, void *mem)
{
    new((void*)mem)  CaffMath::Matrix44{self->_11, self->_12, self->_13, self->_14,self->_21, self->_22, self->_23, self->_24,self->_31, self->_32, self->_33, self->_34,self->_41, self->_42, self->_43, self->_44};
}

    
} // namespace


} // namespace
} // namespace
