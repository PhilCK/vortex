
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <fstream>

namespace
{
	uint32_t    logLevels  = 0;
	uint32_t    logOutputs = 0;

    std::vector<std::string> logBuffer;
    const uint32_t logCapacity = 64;
    
    const std::string logPath = CaffUtil::GetPathDir() + "Log.txt";
    std::ofstream logFile;

    
    
	void LogFile(const std::string &log)
	{
        if(logFile.is_open())
        {
            logFile << log << "\n";
        }
	}
    
    

	void LogConsole(const std::string &log)
	{
		std::cout << log << std::endl;
	}
    
    
    
    void LogBuffer(const std::string &log)
    {
        logBuffer.push_back(log);
        
        // erase the first item.
        if(logBuffer.size() == logCapacity)
        {
            logBuffer.erase(logBuffer.begin());
        }
    }
    
    

	void Log(const std::string &log)
	{
		if(logOutputs & CaffUtil::LogOutput::CONSOLE) { LogConsole(log);   }
		if(logOutputs & CaffUtil::LogOutput::FILE)    { LogFile(log);      }
        if(logOutputs & CaffUtil::LogOutput::BUFFER)  { LogBuffer(log);    }
	}
} // namespace


namespace Caffeine {
namespace Utilities {



void SetLogLevel(const uint32_t setLogLevelsFlags)
{
    logLevels = setLogLevelsFlags;
}
    

void SetOutputLogTargets(const uint32_t setLogOutputsFlags)
{
    logOutputs = setLogOutputsFlags;
    
    if(logOutputs & CaffUtil::LogOutput::BUFFER)
    {
        logBuffer.reserve(logCapacity);
    }
    
    if(logOutputs & CaffUtil::LogOutput::FILE)
    {
        logFile.open(logPath);
    }
    else
    {
        if(logFile.is_open())
        {
            logFile.close();
        }
    }
}


	
void LogInfo(const std::string &log)
{
	if (logLevels & CaffUtil::LogLevel::INFO_LOG)
	{
		Log("Info: " + log);
	}
}



void LogWarning(const std::string &log)
{
	if (logLevels & CaffUtil::LogLevel::WARNING_LOG)
	{
		Log("Warning: " + log);
	}
}



void LogError(const std::string &log)
{
	if (logLevels & CaffUtil::LogLevel::ERROR_LOG)
	{
		Log("Error: " + log);
	}
}
    
    
    
std::vector<std::string> GetBuffer()
{
    return logBuffer;
}



} // namespace
} // namespace
