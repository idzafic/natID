#pragma once

#ifdef MU_WINDOWS
    #ifdef SHAPE_IO_EXPORTS
        #define SHAPE_IO_API __declspec(dllexport)
    #else
        #define SHAPE_IO_API __declspec(dllimport)
    #endif
#else
    #ifdef SHAPE_IO_EXPORTS
        #define SHAPE_IO_API __attribute__((visibility("default")))
    #else
        #define SHAPE_IO_API
    #endif
#endif


