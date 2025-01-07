// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef MAINUTILS_EXPORTS
        #define MAINUTILS_API __declspec(dllexport)
		#define MAINUTILS_EXTERN 
	#else
        #define MAINUTILS_API __declspec(dllimport)
		#define MAINUTILS_EXTERN extern
	#endif
#else
	#ifdef MAINUTILS_EXPORTS
        #define MAINUTILS_API __attribute__((visibility("default")))
		#define MAINUTILS_EXTERN
	#else
        #define MAINUTILS_API
		#define MAINUTILS_EXTERN
	#endif
#endif
