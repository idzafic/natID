// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#ifdef NATPLOT_EXPORTS
    #ifdef MU_WINDOWS
        #define NATPLOT_API __declspec(dllexport)
    #else
        #define NATPLOT_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef MU_WINDOWS
        #define NATPLOT_API __declspec(dllimport)
    #else
        #define NATPLOT_API
    #endif
#endif
