#pragma once


// Need this to export as dll, without declaring SNOW_API before
// every public class/struct there would be symbol errors while using them in app
#ifdef SNOW_WINDOWS
	#ifdef SNOW_BUILD_DLL
		#define SNOW_API __declspec(dllexport)
	#else
		#define SNOW_API __declspec(dllimport)
	#endif
#else
	#define SNOW_API
#endif

#ifdef SNOW_ENABLE_ASSERTS
	#define SNOW_ASSERT(x, ...) { if(!(x)) { SNOW_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SNOW_CORE_ASSERT(x, ...) { if(!(x)) { SNOW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SNOW_ASSERT(x, ...)
	#define SNOW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)