
#include <Caffeine/Systems/ScriptManager.hpp>
#include <Caffeine/Scriptables/CaffAppGlue.hpp>
#include <Caffeine/Scriptables/CaffMathGlue.hpp>
#include <Caffeine/Scriptables/CaffCompGlue.hpp>
#include <Caffeine/Scriptables/CaffSysGlue.hpp>
#include <Caffeine/Scriptables/CaffUtilGlue.hpp>

#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>

#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/CameraManager.hpp>

#include <Caffeine/Interface/ScriptState.hpp>
#include <Caffeine/Interface/ScriptComponent.hpp>

#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/Param.hpp>

#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>
#include <regex>

// Objects for script access
#include <Caffeine/Systems/State.hpp>

namespace
{
	const std::string componentHeader = ""
			"class GameComponent"
			"{"
			"	GameComponentBase @impl;"
            " int foo;"
			""
			"   void    setBaseImpl(GameComponentBase @setImpl) { @impl = setImpl; if(setImpl is null) {Print(\"Null\"); } }"
			""
            "   void    setInt(int i) { if(i > 0) { Print(\"woot\"); } else { Print(\"Boo\");} } "
			"	void	onStart() 	{}"
			"	void	onEnd()		{}"
			"	void	onAttachToEntity()		{}"
			"	void	onDisattachFromEntity()	{}"
			"	void	onParams(const Param &in)	{}"
			""
			"	void	onAttach()		{}"
			"	void	onDisattach()	{}"
			""
			"	void	onEarlyThink(const float dt)	{}"
			"	void	onThink(const float dt) 		{}"
			"	void	onLateThink(const float dt)		{}"
			""
			"	void	onPreRender()	{}"
			"	void	onRender()		{}"
			"	void	onPostRender()	{}"
			""
			"	void	onCollision(const CollidedWith &in)	{}"
			"	void	onTrigger()		{}"
			""
			"	void	onStateChange()	{}"
			"	void	onTextStream(const string &in) {} "
			"	void	onKeyboardChange(const KeyState state, const Key key)	{}"
			""
			"	void	onApplicationExit()			{}"
			"	void	onApplicationLostFocus()	{}"
			"	void	onApplicationGainedFocus()	{}"
			""
			"	bool	onEvent(const Event &in) { return false; }"
			"   bool    registerForEvent(const uint id) { return impl.registerForEvent(id); }"
			"   bool    unRegisterForEvent(const uint id) { return impl.unRegisterForEvent(id); }"
			//"	void   setName(const string name) 				{ impl.setName(name);	  }"
			""
			//"   string name { get const { return impl.getName(); } set { impl.setName(value); } }"
			"	Entity@ getOwner() const { return impl.getOwner(); }"
			"   string getName() const { return impl.getName(); }"
			"};";
			
	void NativePrint(const std::string &str)
	{
		std::cout << str << std::endl;
	}
    
	void NativePrint2(const int i)
	{
		std::cout << i << std::endl;
	}
    
    void NativePrint3(const float f)
    {
        std::cout << f << std::endl;
    }
}


namespace Caffeine {
namespace Systems {


ScriptManager::ScriptManager(const std::string &dir, const std::string &ext)
: m_componentScripts()
, m_stateScripts()
, m_scriptDir(dir)
, m_scriptExt(ext)
, m_asContext()
{
//    int r(0);
//    
//    asIScriptEngine* engine = ASHelper::GetASDetails().asEngine;
//    assert(engine);
//
//    // Global Functions
//    {
//        r = engine->RegisterGlobalFunction("void Print(const string &in)",  asFUNCTION(NativePrint),  asCALL_CDECL); assert(r >= 0);
//        r = engine->RegisterGlobalFunction("void Print(const int64)",       asFUNCTION(NativePrint2), asCALL_CDECL); assert(r >= 0);
//        r = engine->RegisterGlobalFunction("void Print(const float)",       asFUNCTION(NativePrint3), asCALL_CDECL); assert(r >= 0);
//    }
//	
//	// Register Glue
//	{
//        CaffGlue::Math::RegisterTypes(engine);
//        CaffGlue::Utils::RegisterTypes(engine);
//        CaffGlue::App::RegisterTypes(engine);
//        CaffGlue::Comp::RegisterTypes(engine);
//        CaffGlue::Systems::RegisterTypes(engine);
//        
//        CaffGlue::Math::RegisterInteraction(engine);
//        CaffGlue::Utils::RegisterInteraction(engine);
//        CaffGlue::App::RegisterInteraction(engine);
//        CaffGlue::Comp::RegisterInteraction(engine);
//        CaffGlue::Systems::RegisterInteraction(engine);
//	}
//	
//	// Add Base Definitions
//	{
//		m_asContext
//			.addObject<CaffInter::ScriptState>("GameStateBase", true)
////			.addBehaviourFactory(&CaffInter::ScriptState::RefFactory)
////			.addBehaviourRefAdd(&CaffInter::ScriptState::Addref)
////			.addBehaviourRefRelease(&CaffInter::ScriptState::Release)
//			//.addMethod(&CaffInter::ScriptState::onStart,	"void onStart()")
//			//.addMethod(&CaffInter::ScriptState::onEnter,	"void onEnter()")
//			//.addMethod(&CaffInter::ScriptState::onExit,		"void onExit()")
//			//.addMethod(&CaffInter::ScriptState::onEnd,		"void onEnd()")
//			//.addMethod(&CaffInter::ScriptState::onThink,	"void onThink(const float dt)")
//			.addMethod(&CaffInter::ScriptState::getName,	"string getName() const");
//			
//		m_asContext
//			.addObject<CaffInter::ScriptComponent>("GameComponentBase", true)
//			//.addBehaviourFactory(&CaffInter::ScriptComponent::RefFactory)
//			//.addBehaviourRefAdd(&CaffInter::ScriptComponent::Addref)
//			//.addBehaviourRefRelease(&CaffInter::ScriptComponent::Release)
//			.addMethod(&CaffInter::ScriptComponent::onStart,					"void onStart()")
//			.addMethod(&CaffInter::ScriptComponent::onEnd,						"void onEnd()")
//			.addMethod(&CaffInter::ScriptComponent::onParams,					"void onParam(const Param &in)")
//			.addMethod(&CaffInter::ScriptComponent::onAttach,					"void onAttach()")
//			.addMethod(&CaffInter::ScriptComponent::onDisattach,				"void onDisattach()")
//			.addMethod(&CaffInter::ScriptComponent::onEarlyThink,				"void onEarlyThink(const float dt)")
//			.addMethod(&CaffInter::ScriptComponent::onThink,					"void onThink(const float dt)")
//			.addMethod(&CaffInter::ScriptComponent::onLateThink,				"void onLateThink(const float dt)")
//			.addMethod(&CaffInter::ScriptComponent::onPreRender,				"void onPreRender()")
//			.addMethod(&CaffInter::ScriptComponent::onRender,					"void onRender()")
//			.addMethod(&CaffInter::ScriptComponent::onPostRender,				"void onPostRender()")
//			.addMethod(&CaffInter::ScriptComponent::onCollision,				"void onCollision()")
//			.addMethod(&CaffInter::ScriptComponent::onTrigger,					"void onTrigger()")
//			.addMethod(&CaffInter::ScriptComponent::onStateChange,				"void onStateChange()")
//			.addMethod(&CaffInter::ScriptComponent::onKeyboardChange,			"void onKeyboardChange(const KeyState state, const Key id)")
//			.addMethod(&CaffInter::ScriptComponent::onApplicationExit,			"void onApplicationExit()")
//			.addMethod(&CaffInter::ScriptComponent::onApplicationLostFocus,		"void onApplicationLostFocus()")
//			.addMethod(&CaffInter::ScriptComponent::onApplicationGainedFocus,	"void onApplicationGainedFocus()")
//			.addMethod(&CaffInter::ScriptComponent::onEvent,					"bool onEvent(const Event &in)")
//			.addMethod(&CaffInter::ScriptComponent::registerForEvent,			"bool registerForEvent(const uint)")
//			.addMethod(&CaffInter::ScriptComponent::unRegisterForEvent,			"bool unRegisterForEvent(const uint)")
//			.addMethod(&CaffInter::ScriptComponent::getOwner,					"Entity@ getOwner() const")
//			.addMethod(&CaffInter::ScriptComponent::setName,					"void setName(const string &in)")
//			.addMethod(&CaffInter::ScriptComponent::getName,					"string getName() const");
//	}
}



ScriptManager::ScriptManager(const CaffUtil::Param &params)
: ScriptManager(
	CaffProp::GetApplicationDir() + params["directory"].asStdString("Code"),
	params["extension"].asStdString("as"))
{
}



void ScriptManager::reloadAllScripts()
{
	m_componentScripts.clear();
	m_stateScripts.clear();
	
	std::vector<ScriptDetails> scripts;
	const uint16_t VEC_HINT = 256;
	scripts.reserve(VEC_HINT);
	
	std::vector<std::string> files;
	CaffUtil::GetFileNamesOfTypeInDirectory(files, m_scriptExt, m_scriptDir);
	
	const std::string headerCode = componentHeader;
	m_asContext.addCode(headerCode, "headerCode");
	
	for(auto & f : files)
	{
		ScriptDetails script;

		script.fileName   = CaffUtil::GetFileNameFromPath(f);
		script.scriptName = script.fileName.substr(0, script.fileName.size() - (m_scriptExt.size() + 1));
		script.timestamp  = 0;
		
		// Check this is not a duplicate
		{
			const bool isDuplicate = (m_componentScripts.count(script.scriptName) > 0);
			
			if(isDuplicate)
			{
				WARNING_LOG("ScriptManager: Script " + script.scriptName + " is duplicated.")
				continue;
			}
		}
		
		// Add code to context
		{
			std::ifstream in(f, std::ios::in | std::ios::binary);
			
			if(!in)
			{
				WARNING_LOG("ScriptManager: Script file is empty.")
				continue;
			}
			
			// add code.
			
			script.code = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
            
            // Macro search and replace.
            {

//                const std::string macroName = "EVENT_ID";
//                std::size_t pos = script.code.find(macroName, 0);
//                
//                while(pos != std::string::npos)
//                {
//                    std::cout << "EVENT_FOUND" << std::endl;
//                    std::cout << script.code.substr(pos, 10) << std::endl;
//                    
//                    //std::regex rgx("(a)bc(\")");
//                    //std::regex rgx("(?:(\\W))([\\w\\d]+)(?=\\W)");
//                    std::regex rgx("\"[^\"]+\"");
//                    //std::regex_iterator<std::string::iterator> current(script.code.begin() + pos, script.code.end(), rgx);
//                    std::string test = "EVENT(\"foo\")";
//                    
//                    std::regex_iterator<std::string::iterator> current(test.begin(), script.code.end(), rgx);
//                    std::regex_iterator<std::string::iterator> end;
//                    
//                    test =  current->str().substr(1, current->str().size() - 2);
//                    
//                    std::cout << test << std::endl;
//                    
//                    //pos = script.code.find(macroName, pos+1);
//                }
            }
			
			const bool wasCodeAdded = m_asContext.addCode(script.code, script.scriptName);
			
			if(!wasCodeAdded)
			{
				ERROR_LOG("ScriptManager: Failed to add code to context.");
				continue;
			}
		}
		
		scripts.push_back(script);
		
	} // for(...)
	
	m_asContext.buildCode();
	
	// Now we need to check the type info for all the scripts that were added, and add to correct map.
	for(auto &script : scripts)
	{
		ASHelper::TypeInfo type = m_asContext.getTypeInfo(script.scriptName);
		
		enum { COMPONENT_SCRIPT = 0, STATE_SCRIPT, };
		const std::array<std::string, 2> fileTypes = {"GameComponent"};
		
		const bool isComponentType  = type.isOfType(fileTypes.at(COMPONENT_SCRIPT));
		
		if(!(isComponentType))
		{
			WARNING_LOG(script.scriptName + " is not a GameComponent");
		}
		
		// Insert into correct pool.
		if(isComponentType)
		{
			m_componentScripts[script.scriptName] = script;
		}
	}
}


void ScriptManager::reloadChangedScripts()
{
	reloadAllScripts();
}


std::unique_ptr<CaffEnt::Component> ScriptManager::getComponent(const std::string &name)
{
	if(m_componentScripts.count(name))
	{
		const ScriptDetails &details = m_componentScripts.at(name);
		
		//ASHelper::ScriptObject scriptObj = m_asContext.getScriptObject(details.scriptName);
	
		std::unique_ptr<CaffEnt::Component> newComp(new CaffInter::ScriptComponent(m_asContext, details.scriptName));
		
		return newComp;
	}
	
	return std::unique_ptr<CaffEnt::Component>();
}


std::unique_ptr<CaffSys::State> ScriptManager::getState(const std::string &name)
{
	if(m_stateScripts.count(name))
	{
		const ScriptDetails &details = m_stateScripts.at(name);
	
		ASHelper::ScriptObject scriptObj = m_asContext.getScriptObject(details.scriptName);
	
		std::unique_ptr<CaffSys::State> newState(new CaffInter::ScriptState(scriptObj));
		
		return newState;
	}
	
	return std::unique_ptr<CaffSys::State>();
}


} // namespace
} // namespace

