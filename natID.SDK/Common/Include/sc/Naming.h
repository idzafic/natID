// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Naming.h
 *  @brief Symbol naming registry that maps variable and parameter names to their positions.
 */
#pragma once
#include <sc/SCLib.h>
#include <sc/Token.h>
#include "VarsAndParams.h"
#include <sc/ILog.h>
#include <mem/StringMemoryManager.h>
#include <cnt/SafeFullVector.h>

namespace sc
{
class TokenizedModel;
/// @brief Registry that owns string memory and maps symbolic names to model positions for both main and sub-models.
class SYMBOLICOMP_API Naming
{
    friend class TokenizedModel;
private:
    sc::StrMemManager _strMemManager;                                                   ///< String memory pool for all symbol names.
    sc::VarAndParamMap _userLabels[(size_t)sc::UserLabel::Last];                        ///< Maps of user-defined labels for the main model, indexed by UserLabel type.
    sc::VarAndParamMap _userLabelsSubModel[(size_t)sc::UserLabel::Last];                ///< Maps of user-defined labels for sub-models, indexed by UserLabel type.
    sc::VarAndParamSet _duplicatedVarsAndParams[4];                                     ///< Sets of symbol names that appear more than once (duplicate detection).

    cnt::SafeFullVector<const td::StringExt*> _userLabelsByPos[(size_t)sc::UserLabel::Last];          ///< Position-indexed arrays of main-model label pointers.
    cnt::SafeFullVector<const td::StringExt*> _userLabelsSubModelByPos[(size_t)sc::UserLabel::Last];  ///< Position-indexed arrays of sub-model label pointers.
private:
    /// @brief Rebuild the position-indexed name arrays from the hash maps.
    void buildNamePositions();
public:
    /// @brief Construct an empty Naming registry.
    Naming();
    /// @brief Return the mutable symbol map for a given user-label type in the main model.
    /// @param userSymbolType The category of user label to retrieve.
    /// @return Pointer to the corresponding VarAndParamMap.
    sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType);
    /// @brief Return the mutable symbol map for a given user-label type in the sub-model.
    /// @param userSymbolType The category of user label to retrieve.
    /// @return Pointer to the corresponding VarAndParamMap.
    sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType);
    /// @brief Return the mutable array of all user-label maps for the main model.
    /// @return Pointer to the first element of the UserLabel map array.
    sc::VarAndParamMap* getUserLabels();
    /// @brief Return the mutable array of all user-label maps for sub-models.
    /// @return Pointer to the first element of the sub-model UserLabel map array.
    sc::VarAndParamMap* getUserSubModelLabels();

    /// @brief Return the const symbol map for a given user-label type and model level.
    /// @param userSymbolType The category of user label to retrieve.
    /// @param modelLevel Whether to query the main model or sub-model.
    /// @return Const pointer to the corresponding VarAndParamMap.
    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType, sc::ModelLevel modelLevel) const;

    /// @brief Return the const array of all user-label maps for the main model.
    /// @return Const pointer to the first element of the UserLabel map array.
    const sc::VarAndParamMap* getUserLabels() const;
    /// @brief Return the const array of all user-label maps for sub-models.
    /// @return Const pointer to the first element of the sub-model UserLabel map array.
    const sc::VarAndParamMap* getUserSubModelLabels() const;
    /// @brief Return the const symbol map for a given user-label type in the main model.
    /// @param userSymbolType The category of user label to retrieve.
    /// @return Const pointer to the corresponding VarAndParamMap.
    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType) const;
    /// @brief Return the const symbol map for a given user-label type in the sub-model.
    /// @param userSymbolType The category of user label to retrieve.
    /// @return Const pointer to the corresponding VarAndParamMap.
    const sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType) const;
    /// @brief Return the string memory manager used to allocate symbol name storage.
    /// @return Pointer to the StrMemManager.
    sc::StrMemManager* getStringMemManager();
    /// @brief Return the array of duplicate-symbol detection sets.
    /// @return Pointer to the first VarAndParamSet in the array.
    sc::VarAndParamSet* getDuplicatedVarsAndParams();
    /// @brief Write all variable and parameter names to an output stream for debugging.
    /// @param s Output stream to write to.
    void showVarsAndParams(std::ostream& s) const;
    /// @brief Return the number of variables at the specified model level.
    /// @param modelLevel Main model or sub-model.
    /// @return Variable count.
    td::UINT4 getNumberOfVars(sc::ModelLevel modelLevel) const;
    /// @brief Return the number of parameters at the specified model level.
    /// @param modelLevel Main model or sub-model.
    /// @return Parameter count.
    td::UINT4 getNumberOfParams(sc::ModelLevel modelLevel) const;
    /// @brief Return the number of user labels of a given type at the specified model level.
    /// @param userSymbolType Category of user label.
    /// @param modelLevel Main model or sub-model.
    /// @return Label count.
    td::UINT4 getNumberOfUserLabels(sc::UserLabel userSymbolType, sc::ModelLevel modelLevel) const;

    /// @brief Return the variable name at a given position for the specified model level.
    /// @param pos Zero-based variable position.
    /// @param modelLevel Main model or sub-model.
    /// @return Pointer to the StringExt variable name.
    const td::StringExt* getVarName(td::UINT4 pos, sc::ModelLevel modelLevel) const;
    /// @brief Return the parameter name at a given position for the specified model level.
    /// @param pos Zero-based parameter position.
    /// @param modelLevel Main model or sub-model.
    /// @return Pointer to the StringExt parameter name.
    const td::StringExt* getParamName(td::UINT4 pos, sc::ModelLevel modelLevel) const;
    /// @brief Return the user-label name at a given position for the specified type and model level.
    /// @param userSymbolType Category of user label.
    /// @param pos Zero-based label position.
    /// @param modelLevel Main model or sub-model.
    /// @return Pointer to the StringExt label name.
    const td::StringExt* getUserLabelName(sc::UserLabel userSymbolType, td::UINT4 pos, sc::ModelLevel modelLevel) const;

    /// @brief Look up the position of a variable by name.
    /// @param pName Variable name to search for.
    /// @param modelLevel Main model or sub-model.
    /// @return Zero-based position, or -1 if not found.
    td::INT4 getVarPos(const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    /// @brief Look up the position of a parameter by name.
    /// @param pName Parameter name to search for.
    /// @param modelLevel Main model or sub-model.
    /// @return Zero-based position, or -1 if not found.
    td::INT4 getParamPos(const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    /// @brief Look up the position of a user label by name.
    /// @param userSymbolType Category of user label.
    /// @param pName Label name to search for.
    /// @param modelLevel Main model or sub-model.
    /// @return Zero-based position, or -1 if not found.
    td::INT4 getUserLabelPos(sc::UserLabel userSymbolType, const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    /// @brief Reset all symbol maps and position arrays to an empty state.
    void reset();
    /// @brief Clear all symbol maps without releasing the string memory.
    void clean();
};

} //namspace sc
