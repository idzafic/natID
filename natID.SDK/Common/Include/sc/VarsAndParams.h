// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <unordered_map>
#include <unordered_set>
#include <mem/StringMemoryManager.h>

namespace sc
{

using VarAndParamMap = std::unordered_map<td::StringExt, td::UINT4>;
using VarAndParamSet = std::unordered_set<td::StringExt>;

using VarsAndParams = struct _VP
{
    VarAndParamMap* pVars = nullptr;
    VarAndParamMap* pParams = nullptr;
    VarAndParamMap* pVarsSubModel = nullptr;
    VarAndParamMap* pParamSubModel = nullptr;
};

using StrMemManager = mem::StringMemoryManager<td::StringExt, 64*1024>;

} //namspace sc
