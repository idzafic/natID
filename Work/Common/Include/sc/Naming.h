// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
class SYMBOLICOMP_API Naming
{
    friend class TokenizedModel;    
private:
    sc::StrMemManager _strMemManager;
    sc::VarAndParamMap _userLabels[(size_t)sc::UserLabel::Last];
    sc::VarAndParamMap _userLabelsSubModel[(size_t)sc::UserLabel::Last];
    sc::VarAndParamSet _duplicatedVarsAndParams[4];
    
    cnt::SafeFullVector<const td::StringExt*> _userLabelsByPos[(size_t)sc::UserLabel::Last];
    cnt::SafeFullVector<const td::StringExt*> _userLabelsSubModelByPos[(size_t)sc::UserLabel::Last];
private:
    void buildNamePositions();
public:
    Naming();
    sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType);
    sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType);
    sc::VarAndParamMap* getUserLabels();
    sc::VarAndParamMap* getUserSubModelLabels();
    
    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType, sc::ModelLevel modelLevel) const;
    
    const sc::VarAndParamMap* getUserLabels() const;
    const sc::VarAndParamMap* getUserSubModelLabels() const;
    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType) const;
    const sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType) const;
    sc::StrMemManager* getStringMemManager();
    sc::VarAndParamSet* getDuplicatedVarsAndParams();
    void showVarsAndParams(std::ostream& s) const;
    td::UINT4 getNumberOfVars(sc::ModelLevel modelLevel) const;
    td::UINT4 getNumberOfParams(sc::ModelLevel modelLevel) const;
    td::UINT4 getNumberOfUserLabels(sc::UserLabel userSymbolType, sc::ModelLevel modelLevel) const;
    
    const td::StringExt* getVarName(td::UINT4 pos, sc::ModelLevel modelLevel) const;
    const td::StringExt* getParamName(td::UINT4 pos, sc::ModelLevel modelLevel) const;
    const td::StringExt* getUserLabelName(sc::UserLabel userSymbolType, td::UINT4 pos, sc::ModelLevel modelLevel) const;
    
    td::INT4 getVarPos(const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    td::INT4 getParamPos(const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    td::INT4 getUserLabelPos(sc::UserLabel userSymbolType, const td::StringExt* pName, sc::ModelLevel modelLevel) const;
    void reset();
    void clean();
};

} //namspace sc
