
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <string>



namespace
{
	// Settings
	const std::string appsDir = "GameCode/";
	
	// Settings Param
	std::string activeAppName;
	
	CaffUtil::Param applicationParams;
	
} // namespace



namespace Caffeine {
namespace Systems {
namespace Properties {



std::string GetApplicationName()
{
	if(activeAppName.empty())
	{
		GetApplicationParams();
	}

	return activeAppName;
}



const CaffUtil::Param & GetApplicationParams()
{
	if(activeAppName.empty())
	{
		CaffUtil::Param appParams(CaffUtil::GetPathDir() + appsDir + "Application.xml", CaffUtil::Param::FileType::XML);
		
		activeAppName = appParams["application"]["startup_app"].asStdString(activeAppName);
		
		applicationParams = CaffUtil::Param(GetApplicationDir() + appParams["application"]["settings"].asStdString(), CaffUtil::Param::FileType::XML);
	}
	
	return applicationParams;
}



std::string GetApplicationDir()
{
	if(GetApplicationName().empty())
	{
		GetApplicationParams();
	}
	
	return CaffUtil::GetPathDir() + appsDir + activeAppName + "/";
}


} // namespace
} // namespace
} // namespace
