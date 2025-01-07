#pragma once

#ifdef MU_WINDOWS
    #ifdef MYMATH_SHLIB_EXPORTS
        #define MYMATH_SHLIB_API __declspec(dllexport)
    #else
        #define MYMATH_SHLIB_API __declspec(dllimport)
    #endif
#else
    #ifdef MYMATH_SHLIB_EXPORTS
        #define MYMATH_SHLIB_API __attribute__((visibility("default")))
    #else
        #define MYMATH_SHLIB_API
    #endif
#endif


