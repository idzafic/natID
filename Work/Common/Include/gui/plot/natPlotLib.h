// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

// ##################################################################################
// # native Interface Design (natID) ver. 3.2.7 (2024-12-16)
// # Licensed under the Academic Free License, version 3
// # Copyright (C) 2024 Prof. dr Izudin Dzafic 
// # 
// # You may use this code under the terms of the Academic Free License, version 3.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ###################################################################################

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
