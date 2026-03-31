// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 25/10/2020.
//  Copyright © 2020-2021 IDz. All rights reserved.
//


#pragma once
#ifndef MU_WINDOWS
#ifdef _WINDOWS
#define MU_WINDOWS
#endif
#endif

#ifndef MU_WINDOWS
#ifdef WIN32
#define MU_WINDOWS
#endif
#ifdef _WIN64
#define MU_WINDOWS
#endif
#endif

#ifdef _MSC_VER
#define MU_VS_COMPILER
#ifndef MU_WINDOWS
//#define _WINDOWS
#define MU_WINDOWS
#endif
#endif

#ifndef MU_WINDOWS
#ifdef WIN64
#define MU_WINDOWS
#endif
#endif

#ifndef MU_X64
#if defined(_WIN64) || defined(WIN64)
#define MU_X64
#endif
#endif

// Check GCC
#ifndef MU_WINDOWS
#if (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))

#define MU_UNIX
#if defined(__APPLE_CC__)
	#define MU_MACOS
	#if defined(__arm64__) || defined(__aarch64__)
		#define MU_ARM64
	#elif defined(__x86_64__)
    	#define MU_X64
	#endif
#else
	#ifndef MU_LINUX
		#define MU_LINUX
	#endif
	#define MU_X64
//alwas with 64 bits ??
#endif
#endif
#endif

#if defined(__APPLE_CC__)
    #if defined(__arm64__) || defined(__aarch64__)
        #define MU_ARM64
    #elif defined(__x86_64__)
        #define MU_X64
    #endif
#endif


#ifndef MU_DEBUG
#if defined(_DEBUG) || defined(DEBUG)
#define MU_DEBUG
#endif
#endif

#ifndef MU_WINDOWS
#if defined(__APPLE_CC__)
    #define MU_UNIX
#ifndef MU_MACOS
    #define MU_MACOS

    #ifndef MU_DEBUG
        #if defined(DEBUG) || defined(DEBUG_MODE)
            #define MU_DEBUG
        #endif
        #endif
    #endif
#endif
#endif
