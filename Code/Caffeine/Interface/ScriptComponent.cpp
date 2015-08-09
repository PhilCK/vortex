

#include <Caffeine/Interface/ScriptComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <iostream>

namespace Caffeine {
namespace Interface {


ScriptComponent::ScriptComponent(ASHelper::Context &context, const std::string &name)
: CaffEnt::Component()
, m_scriptObj(context.getScriptObject(name))
, m_onStartFunc(m_scriptObj.getMethod("void onStart()"))
, m_onEndFunc(m_scriptObj.getMethod("void onEnd()"))
, m_onAttachEntityFunc(m_scriptObj.getMethod("void onAttachToEntity()"))
, m_onDisattachEntityFunc(m_scriptObj.getMethod("void onDisattachFromEntity()"))
, m_onParamsFunc(m_scriptObj.getMethod("void onParams(const Param &in)"))
, m_onAttachFunc(m_scriptObj.getMethod("void onAttach()"))
, m_onDisattachFunc(m_scriptObj.getMethod("void onDisattach()"))
, m_onEarlyThinkFunc(m_scriptObj.getMethod("void onEarlyThink(const float dt)"))
, m_onThinkFunc(m_scriptObj.getMethod("void onThink(const float dt)"))
, m_onLateThinkFunc(m_scriptObj.getMethod("void onLateThink(const float dt)"))
, m_onPreRenderFunc(m_scriptObj.getMethod("void onPreRender()"))
, m_onRenderFunc(m_scriptObj.getMethod("void onRender()"))
, m_onPostRenderFunc(m_scriptObj.getMethod("void onPostRender()"))
, m_onCollisionFunc(m_scriptObj.getMethod("void onCollision(const CollidedWith &in)"))
, m_onTriggerFunc(m_scriptObj.getMethod("void onTrigger()"))
, m_onStateChangeFunc(m_scriptObj.getMethod("void onStateChange()"))
, m_onTextStreamFunc(m_scriptObj.getMethod("void onTextStream(const string &in)"))
, m_onKeyboardChangeFunc(m_scriptObj.getMethod("void onKeyboardChange(const KeyState state, const Key key)"))
, m_onAppExitFunc(m_scriptObj.getMethod("void onApplicationExit()"))
, m_onAppLostFunc(m_scriptObj.getMethod("void onApplicationLostFocus()"))
, m_onAppGainedFunc(m_scriptObj.getMethod("void onApplicationGainedFocus()"))
, m_onEventFunc(m_scriptObj.getMethod("bool onEvent(const Event &in)"))
{
	setName(name);

	// Set the base impl.
	ASHelper::Function setBase = m_scriptObj.getMethod("void setBaseImpl(GameComponentBase @setImpl)");
	setBase.addArg(this).call();
}



void ScriptComponent::onStart()		{ m_onStartFunc.call();		}
void ScriptComponent::onEnd()		{ m_onEndFunc.call();		}



void ScriptComponent::onAttachToEntity()		{ m_onAttachEntityFunc.call();		}
void ScriptComponent::onDisattachFromEntity()	{ m_onDisattachEntityFunc.call();	}



void ScriptComponent::onParams(const CaffUtil::Param &params)
{
	m_onParamsFunc.addArg((void*)&params);
	m_onParamsFunc.call();
}



void ScriptComponent::onAttach()	{ m_onAttachFunc.call(); }
void ScriptComponent::onDisattach() { m_onDisattachFunc.call(); }



void ScriptComponent::onEarlyThink(const float dt)
{
	m_onEarlyThinkFunc.addArg(dt);
	m_onEarlyThinkFunc.call();
}



void ScriptComponent::onThink(const float dt)
{
	m_onThinkFunc.addArg(dt);
	m_onThinkFunc.call();
}



void ScriptComponent::onLateThink(const float dt)
{
	m_onLateThinkFunc.addArg(dt);
	m_onLateThinkFunc.call();
}



void ScriptComponent::onPreRender()  { m_onPreRenderFunc.call();	}
void ScriptComponent::onRender()	 { m_onRenderFunc.call();		}
void ScriptComponent::onPostRender() { m_onPostRenderFunc.call();	}



void ScriptComponent::onCollision(const CaffEnt::ColliderDetails &details)
{
	m_onCollisionFunc.addArg((void*)&details);
	m_onCollisionFunc.call();
}



void ScriptComponent::onTrigger() { m_onTriggerFunc.call();		}



void ScriptComponent::onTextStream(const std::string &str)
{
	m_onTextStreamFunc.addArg((void*)&str);
	m_onTextStreamFunc.call();
}



void ScriptComponent::onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID id)
{
	m_onKeyboardChangeFunc.addArg((int)state);
	m_onKeyboardChangeFunc.addArg((int)id);
	m_onKeyboardChangeFunc.call();
	
}



void ScriptComponent::onStateChange() { m_onStateChangeFunc.call();	}



void ScriptComponent::onApplicationExit()		 { m_onAppExitFunc.call();	 }
void ScriptComponent::onApplicationLostFocus()	 { m_onAppLostFunc.call();	 }
void ScriptComponent::onApplicationGainedFocus() { m_onAppGainedFunc.call(); }



bool ScriptComponent::onEvent(const CaffSys::Event &data)
{
    CaffSys::ASMethodHelper method(m_onEventFunc.m_context->getCtx(), m_onEventFunc.m_func, m_onEventFunc.m_obj);
    
    method.getContext()->SetArgAddress(0, (void*)&data);
    
    if(method.exec())
    {
        return static_cast<bool>(method.getContext()->GetReturnByte());
    }
    
    return false;
//	int r = 0;
//	
//    r = m_onEventFunc.m_context->getCtx()->PushState();
//    
//	r = m_onEventFunc.m_context->getCtx()->Prepare(m_onEventFunc.m_func);
//	
//	r = m_onEventFunc.m_context->getCtx()->SetObject(m_onEventFunc.m_obj);
//	
//	r = m_onEventFunc.m_context->getCtx()->SetArgAddress(0, (void*)&data);
//	r = m_onEventFunc.m_context->getCtx()->Execute();
//	
//	if(r == asEXECUTION_FINISHED)
//	{
//		bool result = (bool)m_onEventFunc.m_context->getCtx()->GetReturnByte();
//        m_onEventFunc.m_context->getCtx()->PopState();
//        
//        return result;
//	}
//	else if(r == asEXECUTION_EXCEPTION)
//	{
//		std::string errStr = m_onEventFunc.m_context->getCtx()->GetExceptionString();
//		asIScriptFunction *func = m_onEventFunc.m_context->getCtx()->GetExceptionFunction();
//		int i = m_onEventFunc.m_context->getCtx()->GetExceptionLineNumber();
//	}
//    
//    m_onEventFunc.m_context->getCtx()->PopState();
//
//	return false;


//	m_onEventFunc.prepare();
//	m_onEventFunc.addArg((void*)&data);
//	m_onEventFunc.call();
//
//	return m_onEventFunc.getReturnAsBool();
}


bool ScriptComponent::onEventFromScript(const CaffSys::Event &data)
{
	int r = 0;
	
	r = m_onEventFunc.m_context->getCtx()->Prepare(m_onEventFunc.m_func); assert(r >= 0);
	r = m_onEventFunc.m_context->getCtx()->SetArgAddress(0, (void*)&data); assert(r >= 0);
	r = m_onEventFunc.m_context->getCtx()->Execute(); assert(r >= 0);
	
	if(r == asEXECUTION_FINISHED)
	{
		return (bool)m_onEventFunc.m_context->getCtx()->GetReturnByte();
	}
	else if(r == asEXECUTION_EXCEPTION)
	{
		//std::string errStr = m_onEventFunc.m_context->getCtx()->GetExceptionString();
		//asIScriptFunction *func = m_onEventFunc.m_context->getCtx()->GetExceptionFunction();
		//int i = m_onEventFunc.m_context->getCtx()->GetExceptionLineNumber();
	}

	return false;
}

  

void ScriptComponent::cast(void **outRef, int32_t typeID)
{
	// If we hold a null handle, then just return null
	if(m_scriptObj.m_type == 0)
	{
		*outRef = 0;
		return;
	}

	// It is expected that the outRef is always a handle
	assert(typeID & asTYPEID_OBJHANDLE);

	// Compare the type id of the actual script object
	typeID &= ~asTYPEID_OBJHANDLE;
	asIScriptEngine  *engine = m_scriptObj.m_type->GetEngine();
	asIObjectType    *type   = engine->GetObjectTypeById(typeID);

	*outRef = nullptr;

	if(type == m_scriptObj.m_type)
	{
		// Must increase the ref count as we're returning a new reference to the object
		engine->AddRefScriptObject(m_scriptObj.m_scriptObj, m_scriptObj.m_type);
		*outRef = m_scriptObj.m_scriptObj;
	}
}


} // namespace
} // namespace
