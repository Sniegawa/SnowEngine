#pragma once

#include "Core/Logging/Log.h"

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <filesystem>

// Need this to export as dll, without declaring SNOW_API before
// every public class/struct there would be symbol errors while using them in app
//#ifdef SNOW_WINDOWS
//	#ifdef SNOW_BUILD_DLL
//		#define SNOW_API __declspec(dllexport)
//	#else
//		#define SNOW_API __declspec(dllimport)
//	#endif
//#else
//	#define SNOW_API
//#endif
//
//For now I'll export as static lib for simplicity


#ifdef SNOW_DEBUG
	#define SNOW_ENABLE_ASSERTS
#endif



#ifdef SNOW_ENABLE_ASSERTS
	#define SNOW_ASSERT(x, ...) { if(!(x)) { SNOW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SNOW_CORE_ASSERT(x, ...) { if(!(x)) { SNOW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SNOW_ASSERT(x, ...)
	#define SNOW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SNOW_BIND_EVENT_FN(fn,args) std::bind(&fn,this,std::placeholders::_##args)

#define SNOW_FONT_BOLD 0
#define SNOW_FONT_DEFAULT 1


namespace Snow
{
	using Path = std::filesystem::path;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T>
	constexpr WeakRef<T> CreateWeakRef(const Ref<T>& ref)
	{
		return WeakRef<T>(ref);
	}

	namespace Utils
	{
		inline std::string ToUTF8String(const std::filesystem::path& path)
		{
#ifdef SNOW_WINDOWS
			auto u8 = path.u8string();
			return std::string(reinterpret_cast<const char*>(u8.c_str()), u8.size());  // convert from UTF-16 to UTF-8
#elif SNOW_LINUX
			return path.string();    // already UTF-8
#else
			SNOW_CORE_ASSERT(false, "No conversion for your system found");
#endif
		}

		inline std::filesystem::path FromUTF8String(const std::string& utf8)
		{
#ifdef SNOW_WINDOWS
			return std::filesystem::path(std::u8string(reinterpret_cast<const char8_t*>(utf8.c_str())));
#elif SNOW_LINUX
			return std::filesystem::path(utf8);
#else
			SNOW_CORE_ASSERT(false, "No conversion for your system found");
#endif
		}
	}
};
