// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VarsAndParams.h
    @brief Defines map and set types for symbolic compiler variable and parameter management. */
#pragma once
#include <td/String.h>
#include <unordered_map>
#include <unordered_set>
#include <mem/StringMemoryManager.h>

namespace sc
{

using VarAndParamMap = std::unordered_map<td::StringExt, td::UINT4>;
using VarAndParamSet = std::unordered_set<td::StringExt>;

/// @brief Groups pointers to variable and parameter maps for both the main model and sub-model.
using VarsAndParams = struct _VP
{
    VarAndParamMap* pVars = nullptr; ///< Pointer to the main model variable map
    VarAndParamMap* pParams = nullptr; ///< Pointer to the main model parameter map
    VarAndParamMap* pVarsSubModel = nullptr; ///< Pointer to the sub-model variable map
    VarAndParamMap* pParamSubModel = nullptr; ///< Pointer to the sub-model parameter map
};

using StrMemManager = mem::StringMemoryManager<td::StringExt, 64*1024>;

} //namspace sc
