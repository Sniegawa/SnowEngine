#pragma once

#include <memory>

//#include "SnowEngine/Engine/SnowEngineAPI.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Snow
{
	class Log
	{
	public:
		static void Init(const std::string& appName);
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
};

//Core log macros
#define SNOW_CORE_TRACE(...)		::Snow::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SNOW_CORE_INFO(...)			::Snow::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SNOW_CORE_WARN(...)			::Snow::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SNOW_CORE_ERROR(...)		::Snow::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SNOW_CORE_CRITICAL(...)		::Snow::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define SNOW_CLIENT_TRACE(...)		::Snow::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SNOW_CLIENT_INFO(...)		::Snow::Log::GetClientLogger()->info(__VA_ARGS__)
#define SNOW_CLIENT_WARN(...)		::Snow::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SNOW_CLIENT_ERROR(...)		::Snow::Log::GetClientLogger()->error(__VA_ARGS__)
#define SNOW_CLIENT_CRITICAL(...)	::Snow::Log::GetClientLogger()->critical(__VA_ARGS__)
