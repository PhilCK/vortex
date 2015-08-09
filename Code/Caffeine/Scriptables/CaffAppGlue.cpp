
#include <Caffeine/Scriptables/CaffAppGlue.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>
#include <ASHelper.hpp>

#include <iostream>

namespace Caffeine {
namespace ScriptGlue {


namespace App {


#pragma mark - Register
    
void RegisterTypes(asIScriptEngine *engine)
{

	ASEngine asEngine(engine);
	
	asEngine
        .addValueType<CaffApp::FormatDesc>("FormatDesc",	asOBJ_POD | asOBJ_APP_CLASS_CDK).end()
        .addValueType<CaffApp::Model>("Model",				asOBJ_APP_CLASS_CDK | asOBJ_ASHANDLE).end()
        .addValueType<CaffApp::Mesh>("Mesh",				asOBJ_APP_CLASS_CDK).end()
		.addRefType("Window",					asOBJ_NOCOUNT).end()
        .addRefType("Renderer",					asOBJ_NOCOUNT).end()
        .addRefType("Input",					asOBJ_NOCOUNT).end()
		.addEnumType("Key").end()		
		.addEnumType("KeyState").end()
		.addEnumType("Primitive").end()
		.addEnumType("AttrFormat").end()
	.end();
}



namespace
{
    // Model
    void ModelCtor(void *mem)                               { new(mem) CaffApp::Model;                      }
    void ModelCCtor(const CaffApp::Model &other, void *mem) { new((void*)mem) CaffApp::Model(other);        }
    void ModelDtor(void *mem)                               { static_cast<CaffApp::Model*>(mem)->~Model();  }
    
    // Mesh
    void MeshCtor(void *mem)                              { new(mem) CaffApp::Mesh;                     }
    void MeshCCtor(const CaffApp::Mesh &other, void *mem) { new((void*)mem) CaffApp::Mesh(other);       }
    void MeshDtor(void *mem)                              { static_cast<CaffApp::Mesh*>(mem)->~Mesh();  }
}



void RegisterInteraction(asIScriptEngine *engine)
{

	ASEngine asEngine(engine);
		
	asEngine

        .addValueType<CaffApp::Model>("Model", asOBJ_APP_CLASS_CDK | asOBJ_ASHANDLE)
			.addCtor("void f()",										asFUNCTION(ModelCtor),														asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Model &in)",							asFUNCTION(ModelCCtor),														asCALL_CDECL_OBJLAST)
			.addDtor("void f()",										asFUNCTION(ModelDtor),														asCALL_CDECL_OBJLAST)
			.addMethod("uint getNumberOfMeshes() const",				asMETHOD(CaffApp::Model, getNumberOfMeshes),								asCALL_THISCALL)
			.addMethod("bool isMesh(const string &in)",					asMETHOD(CaffApp::Model, isMesh),											asCALL_THISCALL)
			.addMethod("Mesh& getMesh(const uint)",						asMETHODPR(CaffApp::Model, getMesh, (const std::size_t), CaffApp::Mesh&),	asCALL_THISCALL)
        .end()
    

        .addValueType<CaffApp::Mesh>("Mesh", asOBJ_APP_CLASS_CDK)
			.addCtor("void f()",										asFUNCTION(MeshCtor),						asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Mesh &in)",							asFUNCTION(MeshCCtor),						asCALL_CDECL_OBJLAST)
			.addDtor("void f()",										asFUNCTION(MeshDtor),						asCALL_CDECL_OBJLAST)
			.addMethod("uint getGLFaces() const",						asMETHOD(CaffApp::Mesh, getGLFaces),		asCALL_THISCALL)
        .end()
    

		.getRefType("Window")
			.addMethod("bool setResolution(const uint, const uint)",    asMETHOD(CaffApp::Window, setResolution),   asCALL_THISCALL)
			.addMethod("uint getWidth() const",                         asMETHOD(CaffApp::Window, getWidth),        asCALL_THISCALL)
			.addMethod("uint getHeight() const",                        asMETHOD(CaffApp::Window, getHeight),       asCALL_THISCALL)
			.addMethod("bool setFullscreen(const bool)",                asMETHOD(CaffApp::Window, setFullscreen),   asCALL_THISCALL)
			.addMethod("bool isFullscreen() const",                     asMETHOD(CaffApp::Window, isFullscreen),    asCALL_THISCALL)
			.addMethod("bool setTitle(const string &in)",               asMETHOD(CaffApp::Window, setTitle),        asCALL_THISCALL)
			.addMethod("string getTitle() const",                       asMETHOD(CaffApp::Window, getTitle),        asCALL_THISCALL)
		.end()
		

		.getRefType("Input")
			.addMethod("void setHoldMouse(const bool)",					asMETHOD(CaffApp::Input, holdMouse),        asCALL_THISCALL)
			.addMethod("float getMouseDeltaX() const",					asMETHOD(CaffApp::Input, getMouseDeltaX),   asCALL_THISCALL)
			.addMethod("float getMouseDeltaY() const",					asMETHOD(CaffApp::Input, getMouseDeltaY),   asCALL_THISCALL)
			.addMethod("int getMouseX() const",							asMETHOD(CaffApp::Input, getMouseX),        asCALL_THISCALL)
			.addMethod("int getMouseY() const",							asMETHOD(CaffApp::Input, getMouseY),        asCALL_THISCALL)
			.addMethod("void setTextStream(const bool)",				asMETHOD(CaffApp::Input, setTextStream),	asCALL_THISCALL)
			.addMethod("string getKeyName(const Key key) const",		asMETHOD(CaffApp::Input, getKeyName),		asCALL_THISCALL)
			.addMethod("bool isDown(Key) const",						asMETHOD(CaffApp::Input, isDown),           asCALL_THISCALL)
			.addMethod("bool isDownOnFrame(Key) const",					asMETHOD(CaffApp::Input, isDownOnFrame),    asCALL_THISCALL)
		.end()


		.getRefType("Renderer")
			.addMethod("void reset()",																asMETHOD(CaffApp::Renderer, reset),				asCALL_THISCALL)
			.addMethod("void apply()",																asMETHOD(CaffApp::Renderer, apply),				asCALL_THISCALL)
			.addMethod("void setViewPort(const uint, const uint)",									asMETHOD(CaffApp::Renderer, setViewPort),		asCALL_THISCALL)
			.addMethod("void setClearColor(const float, const float, const float, const float)",	asMETHOD(CaffApp::Renderer, setClearColor),		asCALL_THISCALL)
			.addMethod("void clearDefaults(const bool, const bool) const",							asMETHOD(CaffApp::Renderer, clearDefault),		asCALL_THISCALL)
			.addMethod("uint addShader(const string &in)",											asFUNCTION(CaffGlue::App::AddShader),			asCALL_CDECL_OBJLAST)
			.addMethod("void setShader(const uint)",												asMETHOD(CaffApp::Renderer, setShader),			asCALL_THISCALL)
			.addMethod("uint getShader() const",													asMETHOD(CaffApp::Renderer, getShader),			asCALL_THISCALL)
			.addMethod("void setShader1f(const string &in, const float)",							asFUNCTION(CaffGlue::App::SetShader1f),			asCALL_CDECL_OBJLAST)
			.addMethod("void setShader2f(const string &in, const Math::Vec2 &in)",					asFUNCTION(CaffGlue::App::SetShader2f),			asCALL_CDECL_OBJLAST)
			.addMethod("void setShader3f(const string &in, const Math::Vec3 &in)",					asFUNCTION(CaffGlue::App::SetShader3f),			asCALL_CDECL_OBJLAST)
			.addMethod("void setShader4f(const string &in, const Math::Vec4 &in)",					asFUNCTION(CaffGlue::App::SetShader4f),			asCALL_CDECL_OBJLAST)
			.addMethod("void setShaderMatrix44f(const string &in, const Math::Mat4 &in)",			asFUNCTION(CaffGlue::App::SetMatrix44),			asCALL_CDECL_OBJLAST)
			.addMethod("uint addVertexFormat(const FormatDesc[] &in)",								asFUNCTION(CaffGlue::App::AddVertexFormat),		asCALL_CDECL_OBJLAST)
			.addMethod("void setVertexFormat(const uint)",											asMETHOD(CaffApp::Renderer, setVertexFormat),	asCALL_THISCALL)
			.addMethod("uint addVertexBuffer(const float[] &in)",									asFUNCTION(CaffGlue::App::AddVertexBuffer),		asCALL_CDECL_OBJLAST)
			.addMethod("void setVertexBuffer(const uint)",											asMETHOD(CaffApp::Renderer, setVertexBuffer),   asCALL_THISCALL)
			.addMethod("void setIndexBuffer(const uint)",											asMETHOD(CaffApp::Renderer, setIndexBuffer),	asCALL_THISCALL)
			.addMethod("uint addTexture(const string &in)",											asFUNCTION(CaffGlue::App::AddTexture),			asCALL_CDECL_OBJLAST)
			.addMethod("void setTexture(const string &in, const uint)",								asMETHOD(CaffApp::Renderer, setTexture),		asCALL_THISCALL)
			.addMethod("void drawArray(const Primitive prim, const int, const uint)",				asMETHOD(CaffApp::Renderer, drawArray),			asCALL_THISCALL)
		.end()
		
		
		.addEnumType("Key")
			.addEnumValue("KB_A",			(int)CaffApp::KeyID::KB_A)
			.addEnumValue("KB_B",			(int)CaffApp::KeyID::KB_B)
			.addEnumValue("KB_C",			(int)CaffApp::KeyID::KB_C)
			.addEnumValue("KB_D",			(int)CaffApp::KeyID::KB_D)
			.addEnumValue("KB_E",			(int)CaffApp::KeyID::KB_E)
			.addEnumValue("KB_F",			(int)CaffApp::KeyID::KB_F)
			.addEnumValue("KB_G",			(int)CaffApp::KeyID::KB_G)
			.addEnumValue("KB_H",			(int)CaffApp::KeyID::KB_H)
			.addEnumValue("KB_I",			(int)CaffApp::KeyID::KB_I)
			.addEnumValue("KB_J",			(int)CaffApp::KeyID::KB_J)
			.addEnumValue("KB_K",			(int)CaffApp::KeyID::KB_K)
			.addEnumValue("KB_L",			(int)CaffApp::KeyID::KB_L)
			.addEnumValue("KB_M",			(int)CaffApp::KeyID::KB_M)
			.addEnumValue("KB_N",			(int)CaffApp::KeyID::KB_N)
			.addEnumValue("KB_O",			(int)CaffApp::KeyID::KB_O)
			.addEnumValue("KB_P",			(int)CaffApp::KeyID::KB_P)
			.addEnumValue("KB_Q",			(int)CaffApp::KeyID::KB_Q)
			.addEnumValue("KB_R",			(int)CaffApp::KeyID::KB_R)
			.addEnumValue("KB_S",			(int)CaffApp::KeyID::KB_S)
			.addEnumValue("KB_T",			(int)CaffApp::KeyID::KB_T)
			.addEnumValue("KB_U",			(int)CaffApp::KeyID::KB_U)
			.addEnumValue("KB_V",			(int)CaffApp::KeyID::KB_V)
			.addEnumValue("KB_W",			(int)CaffApp::KeyID::KB_W)
			.addEnumValue("KB_X",			(int)CaffApp::KeyID::KB_X)
			.addEnumValue("KB_Y",			(int)CaffApp::KeyID::KB_Y)
			.addEnumValue("KB_Z",			(int)CaffApp::KeyID::KB_Z)
			
			.addEnumValue("KB_1",			(int)CaffApp::KeyID::KB_1)
			.addEnumValue("KB_2",			(int)CaffApp::KeyID::KB_2)
			.addEnumValue("KB_3",			(int)CaffApp::KeyID::KB_3)
			.addEnumValue("KB_4",			(int)CaffApp::KeyID::KB_4)
			.addEnumValue("KB_5",			(int)CaffApp::KeyID::KB_5)
			.addEnumValue("KB_6",			(int)CaffApp::KeyID::KB_6)
			.addEnumValue("KB_7",			(int)CaffApp::KeyID::KB_7)
			.addEnumValue("KB_8",			(int)CaffApp::KeyID::KB_8)
			.addEnumValue("KB_9",			(int)CaffApp::KeyID::KB_9)
			.addEnumValue("KB_0",			(int)CaffApp::KeyID::KB_0)
			
			.addEnumValue("KB_UP",			(int)CaffApp::KeyID::KB_UP)
			.addEnumValue("KB_DOWN",		(int)CaffApp::KeyID::KB_DOWN)
			.addEnumValue("KB_LEFT",		(int)CaffApp::KeyID::KB_LEFT)
			.addEnumValue("KB_RIGHT",		(int)CaffApp::KeyID::KB_RIGHT)
			
			.addEnumValue("KB_ESCAPE",		(int)CaffApp::KeyID::KB_ESCAPE)
			.addEnumValue("KB_RSHIFT",		(int)CaffApp::KeyID::KB_RSHIFT)
			.addEnumValue("KB_LSHIFT",		(int)CaffApp::KeyID::KB_LSHIFT)
			.addEnumValue("KB_SPACE",		(int)CaffApp::KeyID::KB_SPACE)
			.addEnumValue("KB_RCTRL",		(int)CaffApp::KeyID::KB_RCTRL)
			.addEnumValue("KB_LCTRL",		(int)CaffApp::KeyID::KB_LCTRL)
			.addEnumValue("KB_RALT",		(int)CaffApp::KeyID::KB_RALT)
			.addEnumValue("KB_LALT",		(int)CaffApp::KeyID::KB_LALT)
			.addEnumValue("KB_TAB",			(int)CaffApp::KeyID::KB_TAB)
			.addEnumValue("KB_CAPS",		(int)CaffApp::KeyID::KB_CAPS)
			.addEnumValue("KB_NUM",			(int)CaffApp::KeyID::KB_NUM)
			.addEnumValue("KB_ENTER",		(int)CaffApp::KeyID::KB_ENTER)
			.addEnumValue("KB_RETURN",		(int)CaffApp::KeyID::KB_RETURN)
			.addEnumValue("KB_BACKSPACE",	(int)CaffApp::KeyID::KB_BACKSPACE)
			.addEnumValue("KB_DELETE",		(int)CaffApp::KeyID::KB_DELETE)
			
			.addEnumValue("MS_LEFT",		(int)CaffApp::KeyID::MS_LEFT)
			.addEnumValue("MS_CENTER",		(int)CaffApp::KeyID::MS_CENTER)
			.addEnumValue("MS_RIGHT",		(int)CaffApp::KeyID::MS_RIGHT)
			.addEnumValue("MS_XAXIS",		(int)CaffApp::KeyID::MS_XAXIS)
			.addEnumValue("MS_YAXIS",		(int)CaffApp::KeyID::MS_YAXIS)
		.end()
				
				
		.addEnumType("KeyState")
			.addEnumValue("DOWN",			(int)CaffApp::KeyState::DOWN)
			.addEnumValue("UP",				(int)CaffApp::KeyState::UP)
		.end()
		
		
		.addEnumType("Primitive")
			.addEnumValue("TRIANGLES",      (int)CaffApp::Primitive::TRIANGLES)
			.addEnumValue("TRIANGLE_FAN",   (int)CaffApp::Primitive::TRIANGLE_FAN)
			.addEnumValue("TRIANGLE_STRIP", (int)CaffApp::Primitive::TRIANGLE_STRIP)
			.addEnumValue("QUADS",          (int)CaffApp::Primitive::QUADS)	
			.addEnumValue("LINES",          (int)CaffApp::Primitive::LINES)	
			.addEnumValue("LINE_STRIP",     (int)CaffApp::Primitive::LINE_STRIP)
			.addEnumValue("LINE_LOOP",      (int)CaffApp::Primitive::LINE_LOOP)
			.addEnumValue("POINTS",         (int)CaffApp::Primitive::POINTS)
		.end()
		
		
		.addEnumType("AttrFormat")
			.addEnumValue("FLOAT",			(int)CaffApp::AttrFormat::FLOAT)
			.addEnumValue("NIBBLE",			(int)CaffApp::AttrFormat::NIBBLE)
			.addEnumValue("UBYTE",			(int)CaffApp::AttrFormat::UBYTE)
		.end()
		
	.end();
		
    int r = 0;
	
    r = engine->RegisterObjectBehaviour("FormatDesc", asBEHAVE_CONSTRUCT,       "void f()",                                         asFUNCTION(CaffGlue::App::FormatDescCtor), asCALL_CDECL_OBJLAST);		assert(r >= 0);
    r = engine->RegisterObjectBehaviour("FormatDesc", asBEHAVE_LIST_CONSTRUCT,  "void f(const int &in) {string, AttrFormat, uint}", asFUNCTION(CaffGlue::App::FormatDescListCtor), asCALL_CDECL_OBJLAST);	assert(r >= 0);
    r = engine->RegisterObjectBehaviour("FormatDesc", asBEHAVE_CONSTRUCT,       "void f(const FormatDesc &in)",                     asFUNCTION(CaffGlue::App::FormatDescCCtor), asCALL_CDECL_OBJLAST);		assert(r >= 0);
    r = engine->RegisterObjectBehaviour("FormatDesc", asBEHAVE_DESTRUCT,        "void f()",                                         asFUNCTION(CaffGlue::App::FormatDescDotr), asCALL_CDECL_OBJLAST);		assert(r >= 0);
    r = engine->RegisterObjectMethod("FormatDesc", "FormatDesc &opAssign(const FormatDesc &in)", asMETHODPR(CaffApp::FormatDesc, operator=, (const CaffApp::FormatDesc&), CaffApp::FormatDesc&), asCALL_THISCALL); assert(r >= 0);
    r = engine->RegisterObjectProperty("FormatDesc", "string attrName", asOFFSET(CaffApp::FormatDesc, attrName));	assert(r >= 0);
    r = engine->RegisterObjectProperty("FormatDesc", "AttrFormat type", asOFFSET(CaffApp::FormatDesc, type));		assert(r >= 0);
    r = engine->RegisterObjectProperty("FormatDesc", "uint size",		asOFFSET(CaffApp::FormatDesc, size));		assert(r >= 0);

}
    

#pragma mark - Format Desc Glue

void FormatDescCtor(void *mem)
{
	new(mem) CaffApp::FormatDesc;
}

void FormatDescCCtor(const CaffApp::FormatDesc &other, void *mem)
{
	new((void*)mem) CaffApp::FormatDesc(other);
}

void FormatDescListCtor(const CaffApp::FormatDesc *self, void *mem)
{
	new((void*)mem) CaffApp::FormatDesc{self->attrName, self->type, self->size};
}

void FormatDescDotr(void *mem)
{
	static_cast<CaffApp::FormatDesc*>(mem)->~FormatDesc();
}


    
    
#pragma mark - Renderer Glue

std::size_t AddVertexFormat(CScriptArray *array, CaffApp::Renderer *renderer)
{
	assert(array && renderer);

	std::vector<CaffApp::FormatDesc> fmt;
	fmt.reserve(array->GetSize());
	
	for(asUINT i = 0; i < array->GetSize(); ++i)
	{
		fmt.push_back(*static_cast<CaffApp::FormatDesc*>(array->At(i)));
	}
	
	return renderer->addVertexFormat(&fmt[0], fmt.size());
}
    
  
    
std::size_t AddVertexBuffer(CScriptArray *array, CaffApp::Renderer *renderer)
{
    assert(array && renderer);
    
    std::vector<float> fmt;
    fmt.reserve(array->GetSize());
    
    for(asUINT i = 0; i < array->GetSize(); ++i)
    {
        fmt.push_back(*static_cast<float*>(array->At(i)));
    }
    
    return renderer->addVertexBuffer(&fmt[0], fmt.size() * sizeof(float), CaffApp::BufferAccess::STATIC);
}


std::size_t AddShader(const std::string &path, CaffApp::Renderer *renderer)
{
    return renderer->addShader(CaffUtil::GetPathDir() + "AppAssets/Shaders/" + path);
}
    

    
std::size_t AddTexture(const std::string &path, CaffApp::Renderer *renderer)
{
    return renderer->addTexture(CaffUtil::GetPathDir() + "AppAssets/Textures/" + path);
}
    

void SetShader1f(const std::string &constant, const float value, CaffApp::Renderer *renderer)
{
	renderer->setShader1f(constant, &value);
}
    


void SetShader2f(const std::string &constant, const CaffMath::Vector2 &vec, CaffApp::Renderer *renderer)
{
	renderer->setShader2f(constant, &vec.x);
}
    


void SetShader3f(const std::string &constant, const CaffMath::Vector3 &vec, CaffApp::Renderer *renderer)
{
	renderer->setShader3f(constant, &vec.x);
}

    

void SetShader4f(const std::string &constant, const CaffMath::Vector4 &vec, CaffApp::Renderer *renderer)
{
	renderer->setShader4f(constant, &vec.x);
}

    

void SetMatrix44(const std::string &constant, const CaffMath::Matrix44 &mat, CaffApp::Renderer *renderer)
{
	renderer->setShaderMatrix44f(constant, &mat._11);
}

} // namespace


} // namespace
} // namespace
