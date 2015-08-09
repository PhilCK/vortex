
#ifndef CAFFEINE_INTERFACE_SCRIPT_STATE_INCLUDED
#define CAFFEINE_INTERFACE_SCRIPT_STATE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Systems/State.hpp>
#include <ASHelper.hpp>


namespace Caffeine {
namespace Interface {


class ScriptState final : public CaffSys::State//, public ASHelper::RefCount<ScriptState>
{
public:
    
    explicit                ScriptState(ASHelper::ScriptObject &scriptObj);
                            ~ScriptState() {}
    
private:
    
    ASHelper::ScriptObject  m_scriptObj;
    
    ASHelper::Function      m_onStartFunc;
    ASHelper::Function      m_onEnterFunc;
    ASHelper::Function      m_onExitFunc;
    ASHelper::Function      m_onEndFunc;
    ASHelper::Function      m_onThinkFunc;
    
}; // class
    
} // namespace
} // namespace


#endif // include guard
