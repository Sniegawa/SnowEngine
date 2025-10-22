#include "../../Utilities/PlatformUtils.h"
#include "Core/Logging/Log.h"
#include <Core/Application.h>
#include <cstdio>
#include <stdlib.h>
#include <future>
#include <chrono>
#include <GLFW/glfw3.h>

namespace Snow
{
  static std::string RunDialogueCommand(const std::string& command)
  {
     std::array<char,512> buffer{};
    std::string result;


    FILE* pipe = popen(command.c_str(),"r");
    if(!pipe)
      return std::string();
    
    while(fgets(buffer.data(), buffer.size(),pipe))
    {
      result += buffer.data();
    }

    pclose(pipe);

    if(!result.empty() && result.back() == '\n')
    {
      result.pop_back();
    }
    return result;
	
  }

	std::string FileDialogs::OpenFile(const char* filter)
	{
    //SNOW_CORE_ERROR("Linux open file not implemented yet");
    std::string cmd;
  
    if(system("which zenity > /dev/null 2>&1") == 0)
      cmd = "zenity --file-selection 2>/dev/null";
    else if(system("which kdialog > /dev/null 2>&1") == 0)
      cmd = "kdialog --getopenfilename 2>/dev/null";
    else
    { 
      SNOW_CORE_ERROR("No tool found to display file dialogue"); 
      return std::string();
    }

    std::future<std::string> future = std::async(std::launch::async,[cmd]
    {
        return RunDialogueCommand(cmd);
    });

    while(future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready)
      glfwPollEvents();

    return future.get();

	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
//		SNOW_CORE_ERROR("Linux save file not implemented yet");
	  std::string cmd;
      
    if(system("which zenity > /dev/null 2>&1") == 0)
      cmd = "zenity --file-selection --save --confirm-overwrite 2>/dev/null";
    else if(system("which kdialog > /dev/null 2>&1") == 0)
      cmd = "kdialog --getopenfilename 2>/dev/null";
    else
    {
      SNOW_CORE_ERROR("No tool found to display file dialogue");
      return std::string();
    }
    std::future<std::string> future = std::async(std::launch::async,[cmd]
    {
      return RunDialogueCommand(cmd);
    });
    
    while(future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready)
      glfwPollEvents();

    return future.get();
	}
}
