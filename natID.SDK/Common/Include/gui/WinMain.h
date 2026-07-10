// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WinMain.h
    @brief Windows-specific entry-point helpers and RAII memory-leak detection utility. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#ifdef MU_WINDOWS
#ifdef MU_DEBUG
#ifdef MU_SHOW_VS_MEM_LEAKS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//#else
//#define DBG_NEW new
#endif
#endif

//#include <compiler/Definitions.h>

#pragma comment(linker, "/entry:mainCRTStartup")
#endif

/// @brief RAII helper that enables the CRT memory-leak detector on Windows debug builds.
///
/// Construct one instance at the very start of main(); the destructor dumps any
/// unreleased allocations to the debug output when the program exits.
class MemLeaks
{
public:
	/// @brief Constructor; activates the CRT heap-allocation tracking flags on Windows debug builds.
	MemLeaks()
	{
#ifdef MU_WINDOWS
#ifdef MU_DEBUG
#ifdef MU_SHOW_VS_MEM_LEAKS
		//InitMemLeakDetector();
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
#endif
	}

	/// @brief Destructor; dumps all unreleased heap blocks to the debug output on Windows debug builds.
	~MemLeaks()
	{
#ifdef MU_WINDOWS
#ifdef MU_DEBUG
#ifdef MU_SHOW_VS_MEM_LEAKS
		//_CrtCheckMemory();
		_CrtDumpMemoryLeaks();
#endif
#endif
#endif
	}
};
