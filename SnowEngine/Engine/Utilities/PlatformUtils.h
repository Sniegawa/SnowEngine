#pragma once

#include <string>

namespace Snow
{
	class FileDialogs
	{
	public:
		//Returns empty string on cancelation
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
};