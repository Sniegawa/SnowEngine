#include "../../Utilities/PlatformUtils.h"
#include <Core/Application.h>
#include <cstdio>
#include <stdlib.h>


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



    return RunDialogueCommand(cmd);
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
//		SNOW_CORE_ERROR("Linux save file not implemented yet");
	  std::string cmd;
      
    if(system("which zenity > /dev/null 2>&1") == 0)
      cmd = "zenity --file-selection --save --confirm-overwrite 2>/dev/null";
    else 
    {
      SNOW_CORE_ERROR("Cannot find file dialogue for your system");
      return std::string();
    }

      return RunDialogueCommand(cmd);
	}
}
