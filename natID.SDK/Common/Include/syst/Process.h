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
#include <td/Types.h>
#include <td/String.h>
#include <cnt/SafeFullVector.h>

namespace syst
{

class MAINUTILS_API Process
{
public:
    static size_t getID();
    static int execute(const char* pathToExecutable, char* const* argv);
    static int execute(const td::String& pathToExecutable, const cnt::SafeFullVector<td::String>& argv, bool bWait = true);
};

} //namespace syst
