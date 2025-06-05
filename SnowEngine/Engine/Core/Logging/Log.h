#pragma once

#include <memory>

#include "SnowEngine/Engine/SnowEngineAPI.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace SnowEngine
{
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> SNOW_API s_CoreLogger;
		static std::shared_ptr<spdlog::logger> SNOW_API s_ClientLogger;
	};
}

//Core log macros
#define SNOW_CORE_TRACE(...)		::SnowEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SNOW_CORE_INFO(...)			::SnowEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SNOW_CORE_WARN(...)			::SnowEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SNOW_CORE_ERROR(...)		::SnowEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SNOW_CORE_CRITICAL(...)		::SnowEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define SNOW_CLIENT_TRACE(...)		::SnowEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SNOW_CLIENT_INFO(...)		::SnowEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define SNOW_CLIENT_WARN(...)		::SnowEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SNOW_CLIENT_ERROR(...)		::SnowEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define SNOW_CLIENT_CRITICAL(...)	::SnowEngine::Log::GetClientLogger()->critical(__VA_ARGS__)