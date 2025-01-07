// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  netOpLib.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef NETOP_EXPORTS
		#define NETOP_API __declspec(dllexport)
		#define NETOP_TEMPLATE
	#else
		#define NETOP_API __declspec(dllimport)
		#define NETOP_TEMPLATE extern
	#endif
#else
	#ifdef NETOP_EXPORTS
		#define NETOP_API __attribute__((visibility("default")))
		#define NETOP_TEMPLATE
	#else
		#define NETOP_API
		#define NETOP_TEMPLATE
	#endif
#endif

