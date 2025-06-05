#pragma once


// Need this to export as dll, without declaring SNOW_API before
// every class/struct there would be symbol errors
#ifdef SNOW_WINDOWS
	#ifdef SNOW_BUILD_DLL
		#define SNOW_API __declspec(dllexport)
	#else
		#define SNOW_API __declspec(dllimport)
	#endif
#else
	#define SNOW_API
#endif

#define BIT(x) (1 << x)