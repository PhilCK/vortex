
#ifndef CAFFEINE_ENTITY_COMPONENT_INCLUDED
#define CAFFEINE_ENTITY_COMPONENT_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <ASHelper.hpp>
#include <string>
#include <queue>


namespace Caffeine {
namespace Entity {


typedef Component*					ComponentPtr;
typedef std::unique_ptr<Component> 	ComponentUniquePtr;


#define COMPONENT_RTTI(name)									\
		static std::string getTypeID()	{ return #name; }		\
		std::string getInstTypeID()		{ return #name; }		\


class Component : public ASHelper::RefCount<Component>
{
	friend class Entity;

public:

	explicit					Component();
	virtual						~Component();
	
	virtual	std::string			getInstTypeID()						{ return "none"; }
	
	inline std::string			getName() const						{ return m_name; }
	inline void					setName(const std::string &name)	{ m_name = name; }
	
	inline Entity&				getOwner() const					{ assert(m_owner); return *m_owner;	}
	inline void					setOwner(Entity *entity)			{ m_owner = entity; }
	
	inline bool					isActive() const					{ return m_isActive;		}
	inline void					setActive(const bool isActive)		{ isActive ? onAwake() : onSleep(); m_isActive = isActive; }
	
private:

	virtual void				onStart() {}
	virtual void				onEnd()	  {}
	virtual void				onParams(const CaffUtil::Param &params) {}
	
	virtual void				onAttachToEntity()		{}
	virtual void				onDisattachFromEntity()	{}
    
    virtual void                onAwake() {}
    virtual void                onSleep() {}
    
	virtual	void				onAttach()	  {}
	virtual void				onDisattach() {}
	
    virtual void                onSuperEarlyThink(const float dt) {} // Engine components only.
	virtual void				onEarlyThink(const float dt) {}
	virtual void				onThink(const float dt)		 {}
	virtual void				onLateThink(const float dt)  {}
    virtual void                onSuperLateThink(const float dt) {} // Engine components only.
	
	virtual void				onPreRender()  {}
	virtual void				onRender()	   {}
	virtual void				onPostRender() {}
	
	virtual void				onTextStream(const std::string &text) {};
	virtual void				onKeyboardChange(const CaffApp::KeyState state, const CaffApp::KeyID id) {};
	
	virtual void				onCollision(const CaffEnt::ColliderDetails &details) {}
	virtual void				onTrigger()	  {}
	
	virtual void				onStateChange() {}
	
	virtual void				onApplicationExit()		   {}
	virtual void				onApplicationLostFocus()   {}
	virtual void				onApplicationGainedFocus() {}
	
	virtual bool				onEvent(const CaffSys::Event &data) { return false; }
    
    virtual void                onEntityChange(const EntityChangeAction changeAction) {}
	
protected:

	// The engine interface
	CaffApp::Window&			getWindow() const;
	void						getRenderer();
	void						getStateManager();
    
	void						getTextRenderer();
	void						getCameraManager();
	void						getTextureManager();
	void						getShaderManager();
	void						getInputManager();
	void						getEventManager();

	bool						registerForEvent(const CaffSys::EventID event);
	bool						unRegisterForEvent(const CaffSys::EventID event);
	
private:


	enum class PendingHooks
	{
		ON_PARAMS,
		ON_START,
		ON_ATTACH,
		ON_DISATTACH,
	};
	
	std::queue<PendingHooks>	m_pendingHooks;

	
	Entity*						m_owner;
	CaffSys::EventCallback      m_eventCallback;
	std::string					m_name;
    
    // Components
    CaffApp::Window             *m_windowSystem;
    CaffApp::Renderer           *m_rendererSystem;
    CaffApp::Input              *m_inputSystem;
    
    CaffSys::TextRenderer       *m_textRendererSystem;
    CaffSys::CameraManager      *m_cameraManagerSystem;
	
	bool						m_isActive;
	
}; // class


} // namespace
} // namespace

#endif
