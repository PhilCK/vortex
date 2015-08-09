

#include <Caffeine/Interface/ScriptState.hpp>


namespace Caffeine {
namespace Interface {


// TODO: Set name better.
ScriptState::ScriptState(ASHelper::ScriptObject &scriptObj)
: CaffSys::State("ScriptState")
, m_scriptObj(scriptObj)
, m_onStartFunc(m_scriptObj.getMethod("void onStart()"))
, m_onEnterFunc(m_scriptObj.getMethod("void onEnter()"))
, m_onExitFunc(m_scriptObj.getMethod("void onExit()"))
, m_onEndFunc(m_scriptObj.getMethod("void onEnd()"))
, m_onThinkFunc(m_scriptObj.getMethod("void onThink(const float dt)"))
{
	// Set the base impl.
	ASHelper::Function setBase = m_scriptObj.getMethod("void setBaseImpl(GameStateBase @setImpl)");
	setBase.addArg(this).call();
}
    
    
    
} // namespace
} // namespace