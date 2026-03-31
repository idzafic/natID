// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/muLib.h>
#include <td/String.h>

namespace syst
{
class MAINUTILS_API Computer
{
public:
    static td::String getName();
};

enum class CPUArch : td::BYTE { x64, arm64, Unknown};

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
