// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Computer.h
    @brief Provides computer name and CPU architecture detection utilities. */
#pragma once
#include <mu/muLib.h>
#include <td/String.h>

namespace syst
{
/// @brief Provides static utilities to query information about the local computer.
class MAINUTILS_API Computer
{
public:
    /// @brief Returns the name of the local computer.
    /// @return A string containing the host name of the computer.
    static td::String getName();
};

/// @brief Enumerates the supported CPU architecture types.
enum class CPUArch : td::BYTE {
    x64,    ///< 64-bit x86 (AMD64/Intel 64) architecture.
    arm64,  ///< 64-bit ARM (AArch64) architecture.
    Unknown ///< Architecture could not be determined.
};

/// @brief Returns the CPU architecture of the current build target.
/// @return The CPUArch value corresponding to the compile-time target architecture.
inline CPUArch getCPUArchitecture()
{
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    return CPUArch::x64;

#elif defined(_M_ARM64) || defined(__aarch64__)
    return CPUArch::arm64;

#else
    return CPUArch::Unknown;
#endif

}

} //namespace syst
