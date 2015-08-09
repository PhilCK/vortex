

#ifndef CAFFEINE_INTERFACE_SCRIPT_COMPONENT_INCLUDED
#define CAFFEINE_INTERFACE_SCRIPT_COMPONENT_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <ASHelper.hpp>


namespace Caffeine {
namespace Interface {


class ScriptComponent final : public CaffEnt::Component
{
public:

	explicit				ScriptComponent(ASHelper::Context &context, const std::string &name);
							~ScriptComponent() {}
	
	void					onStart()	final;
	void					onEnd()		final;
	
	void					onAttachToEntity() final;
	void					onDisattachFromEntity() final;
	
	void					onParams(const CaffUtil::Param &params)  final;
	
	void					onAttach()	  final;
	void					onDisattach() final;
	
	void					onEarlyThink(const float dt) final;
	void					onThink(const float dt)		 final;
	void					onLateThink(const float dt)  final;
	
	void					onPreRender()   final;
	void					onRender()		final;
	void					onPostRender()  final;
	
	void					onTextStream(const std::string &str) final;
	void					onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID id) final;
	
	void					onCollision(const CaffEnt::ColliderDetails &details) final;
	void					onTrigger()	  final;
	
	void					onStateChange() final;
	
	void					onApplicationExit()		   final;
	void					onApplicationLostFocus()   final;
	void					onApplicationGainedFocus() final;
	
	bool					onEvent(const CaffSys::Event &data) final;
	bool					onEventFromScript(const CaffSys::Event &data);
	
	void					cast(void **outRef, int32_t typeID);
	
	
private:

	ASHelper::ScriptObject  m_scriptObj;

	ASHelper::Function      m_onStartFunc;
	ASHelper::Function      m_onEndFunc;
	ASHelper::Function      m_onParamsFunc;
	
	ASHelper::Function      m_onAttachEntityFunc;
	ASHelper::Function      m_onDisattachEntityFunc;
	
	ASHelper::Function      m_onAttachFunc;
	ASHelper::Function      m_onDisattachFunc;
	
	ASHelper::Function      m_onEarlyThinkFunc;
	ASHelper::Function      m_onThinkFunc;
	ASHelper::Function      m_onLateThinkFunc;
	
	ASHelper::Function      m_onPreRenderFunc;
	ASHelper::Function      m_onRenderFunc;
	ASHelper::Function      m_onPostRenderFunc;
	
	ASHelper::Function      m_onCollisionFunc;
	ASHelper::Function      m_onTriggerFunc;
	
	ASHelper::Function      m_onStateChangeFunc;
	ASHelper::Function      m_onTextStreamFunc;
	ASHelper::Function      m_onKeyboardChangeFunc;
	
	ASHelper::Function      m_onAppExitFunc;
	ASHelper::Function      m_onAppLostFunc;
	ASHelper::Function      m_onAppGainedFunc;
	
	ASHelper::Function      m_onEventFunc;
	
	//ASHelper::Function      m_onEventFunc;
	
}; // class


} // namespace
} // namespace


#endif // include guard
