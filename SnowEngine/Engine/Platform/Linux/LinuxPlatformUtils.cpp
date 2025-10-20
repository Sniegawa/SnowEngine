#include "../../Utilities/PlatformUtils.h"
#include <Core/Application.h>

namespace Snow
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		SNOW_CORE_ERROR("Linux open file not implemented yet");
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		SNOW_CORE_ERROR("Linux save file not implemented yet");
		return std::string();
	}
}
