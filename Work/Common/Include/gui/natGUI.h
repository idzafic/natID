// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef NATGUI_EXPORTS
	#define NATGUI_API __declspec(dllexport)
	#else
	#define NATGUI_API __declspec(dllimport)
	#endif
#else
	#ifdef NATGUI_EXPORTS
    #define NATGUI_API __attribute__((visibility("default")))
	#else
    #define NATGUI_API
	#endif
#endif



