#pragma once

#ifdef BE_PLATFORM_WINDOWS
	#ifdef BE_BUILD_DLL
		#define BEATI_API __declspec(dllexport)
	#else
		#define BEATI_API __declspec(dllimport)
	#endif // BE_BUILD_DLL
#else
	#error Beati only supports Windows at the moment!
#endif // BE_PLATFORM_WINDOWS


#define BIT(x) (1 << x)


