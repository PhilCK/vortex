
#include <Caffeine/Scriptables/CaffSysGlue.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/TextRenderer.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <ASHelper.hpp>



namespace Caffeine {
namespace ScriptGlue {
namespace Systems {


    
void RegisterTypes(asIScriptEngine *engine)
{
	ASEngine asEngine(engine);
	
	asEngine
        .addValueType<CaffSys::EventArg>("EventArg",			asOBJ_APP_CLASS_CDK).end()
        .addValueType<CaffSys::Event>("Event",                  asOBJ_ASHANDLE).end()
        .addRefType("ModelData",								asOBJ_NOCOUNT).end()
		.addRefType("CameraManager",							asOBJ_NOCOUNT).end()
        .addRefType("CameraComponent",							asOBJ_NOCOUNT).end()
        //.addRefType("Event",									asOBJ_NOCOUNT).end()
        .addRefType("EventManager",								asOBJ_NOCOUNT).end()
        .addRefType("ModelManager",								asOBJ_NOCOUNT).end()
        .addRefType("State",                                    asOBJ_NOCOUNT).end()
        .addRefType("StateManager",                             asOBJ_NOCOUNT).end()
		.addRefType("TextRenderer",								asOBJ_NOCOUNT).end()
	.end();
}



namespace
{
    // ModelData
//    void ModelDataCtor(void *mem)									{ assert(mem); new(mem) CaffSys::ModelData();						 }
//    void ModelDataCCtor(const CaffSys::ModelData &other, void *mem) { assert(mem); new((void*)mem) CaffSys::ModelData(other);			 }
//    void ModelDataDtor(void *mem)									{ assert(mem); static_cast<CaffSys::ModelData*>(mem)->~ModelData();  }
	
    CaffApp::VertexID ModelDataGetVertexID(const unsigned int index, const CaffSys::ModelData *obj)
    {
		assert(obj);
        assert(index < obj->model->getNumberOfMeshes());
        return obj->vertBufferIDs[index];
    }
    
    CaffApp::VertexID ModelDataGetIndexID(const unsigned int index, const CaffSys::ModelData *obj)
    {
		assert(obj);
        assert(index < obj->model->getNumberOfMeshes());
        return obj->indexBufferIDs[index];
    }
	
	CaffApp::Model& ModelDataGetModel(const CaffSys::ModelData *obj)
	{
		assert(obj);
		return *obj->model;
	}
	
	
	
	// Event
    void                        EventCtorD(void *mem)                                       { assert(mem); new(mem) CaffSys::Event(1);                          }
    void                        EventCtor(const uint32_t id, void *mem)                     { assert(mem); new(mem) CaffSys::Event(id);                         }
    void                        EventCCtor(CaffSys::Event &other, void *mem)                { assert(mem); new((void*)mem) CaffSys::Event(other);               }
    void                        EventDtor(void *mem)                                        { assert(mem); static_cast<CaffSys::Event*>(mem)->~Event();         }
	void                        EventArgCtor(void *mem)										{ assert(mem); new(mem) CaffSys::EventArg(nullptr);					}
	//void                      EventArgString2Ctor(const std::string str, void *mem)		{ assert(mem); new(mem) CaffSys::EventArg(str);						}
	void                        EventArgStringCtor(const std::string &str, void *mem)		{ assert(mem); new(mem) CaffSys::EventArg(str);						}
	void                        EventArgFloatCtor(const float f, void *mem)					{ assert(mem); new(mem) CaffSys::EventArg(f);						}
	void                        EventArgUInt32Ctor(const uint32_t num, void *mem)			{ assert(mem); new(mem) CaffSys::EventArg(num);						}
	void                        EventArgInt32Ctor(const int32_t num, void *mem)				{ assert(mem); new(mem) CaffSys::EventArg(num);						}
	void                        EventArgVec2Ctor(const CaffMath::Vector2 &vec, void *mem)	{ assert(mem); new(mem) CaffSys::EventArg(CaffMath::Vector2(vec));	}
	void                        EventArgVec3Ctor(const CaffMath::Vector3 &vec, void *mem)	{ assert(mem); new(mem) CaffSys::EventArg(CaffMath::Vector3(vec));	}
	void                        EventArgVec4Ctor(const CaffMath::Vector4 &vec, void *mem)	{ assert(mem); new(mem) CaffSys::EventArg(CaffMath::Vector4(vec));	} // TODO: Do the vecs need copying?
	void                        EventArgCCtor(CaffSys::EventArg &other, void *mem)			{ assert(mem); new((void*)mem) CaffSys::EventArg(other);			}
	void                        EventArgDtor(void *mem)										{ assert(mem); static_cast<CaffSys::EventArg*>(mem)->~Variant();	}
	const std::string&			EventArgAsString(const CaffSys::EventArg *arg)              { assert(arg); return arg->get<std::string>();						}
	float                       EventArgAsFloat(const CaffSys::EventArg *arg)				{ assert(arg); return arg->get<float>();							}
	uint32_t                    EventArgAsUInt32(const CaffSys::EventArg *arg)				{ assert(arg); return arg->get<uint32_t>();							}
	int32_t                     EventArgAsInt32(const CaffSys::EventArg *arg)				{ assert(arg); return arg->get<int32_t>();							}
	const CaffMath::Vector2&	EventArgAsVec2(const CaffSys::EventArg *arg)                { assert(arg); return arg->get<CaffMath::Vector2>();				}
	const CaffMath::Vector3&	EventArgAsVec3(const CaffSys::EventArg *arg)                { assert(arg); return arg->get<CaffMath::Vector3>();				}
	const CaffMath::Vector4&	EventArgAsVec4(const CaffSys::EventArg *arg)                { assert(arg); return arg->get<CaffMath::Vector4>();				}
    
    void EventSetArg(const unsigned int index, CaffSys::EventArg &assign, CaffSys::Event *evt)
    {
        evt->at(index) = assign;
    }
	
	
	
	// EventManager
	void	EventManagerSendZeroArgs(const uint32_t id, CaffSys::EventManager *evtMgr)								{ assert(evtMgr); evtMgr->sendInstantEvent(id, nullptr); }
	void	EventManagerSendOneArgs(const uint32_t id, CaffSys::EventArg &arg1, CaffSys::EventManager *evtMgr)		{ assert(evtMgr); evtMgr->sendInstantEvent(id, arg1); }
	void	EventManagerSendTwoArgs(const uint32_t id, CaffSys::EventArg &arg1, CaffSys::EventArg &arg2, CaffSys::EventManager *evtMgr)								{ assert(evtMgr); evtMgr->sendInstantEvent(id, arg1, arg2); }
	void	EventManagerSendThreeArgs(const uint32_t id, CaffSys::EventArg &arg1, CaffSys::EventArg &arg2, CaffSys::EventArg &arg3, CaffSys::EventManager *evtMgr)	{ assert(evtMgr); evtMgr->sendInstantEvent(id, arg1, arg2, arg3); }
    
    void EventManagerSendEvent(const CaffSys::Event &evt, CaffSys::EventManager *evtMgr)
    {
        assert(evtMgr);
        evtMgr->sendInstantEvent(evt);
    }
}
    
	

void RegisterInteraction(asIScriptEngine *engine)
{
    ASEngine asEngine(engine);
    
    asEngine
    
        .addRefType("CameraManager", asOBJ_NOCOUNT)
			.addMethod("CameraComponent@ getCurrentCamera() const", asMETHOD(CaffSys::CameraManager, getCurrentCamera), asCALL_THISCALL)
        .end()
		
		
		.addRefType("TextRenderer", asOBJ_NOCOUNT)
			.addMethod("void render2DScreenText(const string &in, const Math::Vec2 &in, const string &in = \"\", const float = 1.f, const bool = false, const bool = false)", asMETHOD(CaffSys::TextRenderer, render2DScreenText), asCALL_THISCALL)
		.end()
    

        .addRefType("ModelData", asOBJ_NOCOUNT)
	//		.addCtor("void f()",									 asFUNCTION(ModelDataCtor),			asCALL_CDECL_OBJLAST)
	//		.addCtor("void f(const ModelData &in)",					 asFUNCTION(ModelDataCCtor),		asCALL_CDECL_OBJLAST)
	//		.addDtor("void f()",									 asFUNCTION(ModelDataDtor),			asCALL_CDECL_OBJLAST)
			.addProperty("Model@ model",							 asOFFSET(CaffSys::ModelData, model))
			.addMethod("Model& getModel() const",					 asFUNCTION(ModelDataGetModel),		asCALL_CDECL_OBJLAST)
			//.addMethod("ModelData &opAssign(const ModelData @)",	 asMETHODPR(CaffSys::ModelData, operator=, (const CaffSys::ModelData*), CaffSys::ModelData*), asCALL_THISCALL)
			.addMethod("uint get_vertexBufferID(const uint)",		 asFUNCTION(ModelDataGetVertexID),	asCALL_CDECL_OBJLAST)
			.addMethod("uint get_indexBufferID(const uint)",		 asFUNCTION(ModelDataGetIndexID),	asCALL_CDECL_OBJLAST)
        .end()
		

        .getRefType("ModelManager")
			.addMethod("ModelData & getModelData(const string &in)", asMETHOD(CaffSys::ModelManager, getModelData), asCALL_THISCALL)
        .end()
		

		.addValueType<CaffSys::EventArg>("EventArg", asOBJ_APP_CLASS_CDK)
			.addCtor("void f()",							asFUNCTION(EventArgCtor),			asCALL_CDECL_OBJLAST)
			//.addCtor("void f(const string)",				asFUNCTION(EventArgString2Ctor),	asCALL_CDECL_OBJLAST)
			.addCtor("void f(const string &in)",			asFUNCTION(EventArgStringCtor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const float)",					asFUNCTION(EventArgFloatCtor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const uint)",					asFUNCTION(EventArgUInt32Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const int)",					asFUNCTION(EventArgInt32Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Math::Vec2 &in)",		asFUNCTION(EventArgVec2Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Math::Vec3 &in)",		asFUNCTION(EventArgVec3Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Math::Vec4 &in)",		asFUNCTION(EventArgVec4Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const int)",					asFUNCTION(EventArgInt32Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const int)",					asFUNCTION(EventArgInt32Ctor),		asCALL_CDECL_OBJLAST)
			.addCtor("void f(const EventArg &in)",			asFUNCTION(EventArgCCtor),			asCALL_CDECL_OBJLAST)
			.addDtor("void f()",							asFUNCTION(EventArgDtor),			asCALL_CDECL_OBJLAST)
			.addMethod("int asInt() const",					asFUNCTION(EventArgAsInt32),		asCALL_CDECL_OBJLAST)
			.addMethod("uint asUInt() const",				asFUNCTION(EventArgAsUInt32),		asCALL_CDECL_OBJLAST)
			.addMethod("float asFloat() const",				asFUNCTION(EventArgAsFloat),		asCALL_CDECL_OBJLAST)
			.addMethod("string asString() const",			asFUNCTION(EventArgAsString),		asCALL_CDECL_OBJLAST)
			.addMethod("const Math::Vec2& asVec2() const",	asFUNCTION(EventArgAsVec2),			asCALL_CDECL_OBJLAST)
			.addMethod("const Math::Vec3& asVec3() const",	asFUNCTION(EventArgAsVec3),			asCALL_CDECL_OBJLAST)
			.addMethod("const Math::Vec4& asVec4() const",	asFUNCTION(EventArgAsVec4),			asCALL_CDECL_OBJLAST)
            //.addMethod("EventArg &opAssign(const EventArg @)",	 asFUNCTION(EventArgAssign),			asCALL_CDECL_OBJLAST)
		.end()
		

		.addValueType<CaffSys::Event>("Event", asOBJ_ASHANDLE)
			.addCtor("void f()",                                asFUNCTION(EventCtorD),                                                                 asCALL_CDECL_OBJLAST)
			.addCtor("void f(const uint)",                      asFUNCTION(EventCtor),                                                                  asCALL_CDECL_OBJLAST)
			.addCtor("void f(const Event &in)",                 asFUNCTION(EventCCtor),                                                                 asCALL_CDECL_OBJLAST)
			.addDtor("void f()",                                asFUNCTION(EventDtor),                                                                  asCALL_CDECL_OBJLAST)
			.addMethod("EventArg& at(const uint)",				asMETHODPR(CaffSys::Event, at, (const std::size_t), CaffSys::EventArg&),				asCALL_THISCALL)
			.addMethod("const EventArg& at(const uint) const",	asMETHODPR(CaffSys::Event, at, (const std::size_t) const, const CaffSys::EventArg&),	asCALL_THISCALL)
			.addMethod("uint getID() const",					asMETHOD(CaffSys::Event, getID),														asCALL_THISCALL)
            .addMethod("void setArg(const uint, EventArg &in)", asFUNCTION(EventSetArg), asCALL_CDECL_OBJLAST)
		.end()
		

		.addRefType("EventManager", asOBJ_NOCOUNT)
			.addMethod("void sendInstantEvent(const uint)",												asFUNCTION(EventManagerSendZeroArgs),		asCALL_CDECL_OBJLAST)
			.addMethod("void sendInstantEvent(const Event &in)",                                        asFUNCTION(EventManagerSendEvent),          asCALL_CDECL_OBJLAST)
			//.addMethod("void sendInstantEvent(const Event& in)")										asFUNCTION(EventManagerSendInstant),		asCALL_CDECL_OBJLAST)
			.addMethod("void sendInstantEvent(const uint, EventArg &in)",								asFUNCTION(EventManagerSendOneArgs),		asCALL_CDECL_OBJLAST)
			.addMethod("void sendInstantEvent(const uint, EventArg &in, EventArg &in)",					asFUNCTION(EventManagerSendTwoArgs),		asCALL_CDECL_OBJLAST)
			.addMethod("void sendInstantEvent(const uint, EventArg &in, EventArg &in, EventArg &in)",	asFUNCTION(EventManagerSendThreeArgs),		asCALL_CDECL_OBJLAST)
			
			//.addMethod("void sendInstantEvent(const uint, EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg(), EventArg = EventArg())", asMETHODPR(CaffSys::EventManager, sendInstantEvent, (const CaffSys::EventID id, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg, CaffSys::EventArg), void), asCALL_THISCALL)
			
			.addMethod("uint getEventID(const string &in)", asMETHOD(CaffSys::EventManager, getEventID), asCALL_THISCALL)
		.end()

        .addRefType("State", asOBJ_NOCOUNT)
			.addMethod("Entity@ pushNewEntity(const string &in)", asMETHODPR(CaffSys::State, pushNewEntity, (const std::string&), CaffEnt::Entity*), asCALL_THISCALL)
        .end()
    
        // StateManager
        .addRefType("StateManager", asOBJ_NOCOUNT)
        .end()
		
	.end();
}
 
	   
} // namespace
} // namespace
} // namespace
