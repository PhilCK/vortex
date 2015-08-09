
#ifndef CAFFEINE_SYSTEMS_SCRIPT_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_SCRIPT_MANAGER_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Utilities/UtilitiesFwd.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <ASHelper.hpp>
#include <memory>
#include <string>
#include <map>


namespace Caffeine {
namespace Systems {


class ScriptManager : private CaffUtil::NonCopyable
{
public:

	explicit							ScriptManager(const std::string &dir, const std::string &ext = "as");
	explicit							ScriptManager(const CaffUtil::Param &params);
	
	void								reloadAllScripts();
	void								reloadChangedScripts();
	
	std::unique_ptr<CaffEnt::Component>	getComponent(const std::string &name);
	inline bool							hasComponent(const std::string &name) const { return m_componentScripts.count(name) > 0; }
	
	std::unique_ptr<CaffSys::State>		getState(const std::string &name);
	inline bool							hasState(const std::string &name) const { return m_stateScripts.count(name) > 0; }
	
private:

	struct ScriptDetails
	{
		std::string		code;
		std::string		fileName;
        std::string		scriptName;
		uint64_t		timestamp;
	}; // struct.

	std::map<std::string, ScriptDetails>		m_componentScripts;
	std::map<std::string, ScriptDetails>		m_stateScripts;
	
	const std::string					m_scriptDir;
	const std::string					m_scriptExt;
	
	ASHelper::Context m_asContext;

}; // ScriptManager


} // namespace
} // namespace


#endif // include guard
