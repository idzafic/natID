// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <sc/Naming.h>
#include "VarsAndParams.h"
#include <cnt/Vector.h>
#include <cnt/BlockVector.h>
#include <sc/ILog.h>
#include <span>
#include <mu/LineLoader.h>
#include <cnt/Array.h>
#include <td/LinePattern.h>

#include <tuple>
#include <td/ColorID.h>

namespace sc
{

using TokLine = cnt::Vector<sc::TokVal>;
using TokLineIterator = TokLine::iterator;

using LineInfo = struct _tl
{
    td::UINT4 lineNo;
    td::UINT4 linePos; 
    td::UINT4 expressionStartPos;
    td::UINT4 nExpressionTokens;
    td::UINT4 attribStartPos;
    td::UINT4 nAttribTokens;
};

using ExpressionTokens = cnt::BlockVector<TokVal, 64*1024/sizeof(sc::TokVal)>;

using AttribTokens = cnt::BlockVector<TokVal, 1024>;

using LinesInfo = cnt::BlockVector<LineInfo, 1024>;

using ModelInfo = struct _MI
{
    double eps;
    const td::StringExt* modelName;
    const td::StringExt* methodName;
    sc::ModelType modelType;
    sc::Domain domain;
};

class SYMBOLICOMP_API TokenizedModel
{
public:
    using CntGroupInfo = cnt::Array<CountersAndPointers, size_t(Group::Last)>;
protected:
    cnt::SafeFullVector<td::Variant>& _headerAttribs;
    
    TranslateFunction _translate;
    sc::Naming& _naming;
    sc::ExpressionTokens _modelTokens;
    sc::AttribTokens _attribTokens;
    sc::LinesInfo _mainModelLinesInfo;
    sc::LinesInfo _subModelLinesInfo;
    
    CntGroupInfo _subModelGroups;
    CntGroupInfo _mainModelGroups;
    
    cnt::Array<cnt::PushBackVector<td::Variant>,2> _modelAttribs; //model and submodel attribs
    cnt::Array<sc::Range,2> _locationOfVars;
    cnt::Array<sc::Range,2> _locationOfParams;
    cnt::Array<sc::Range,2> _locationOfDistribs;
    cnt::Array<sc::Range,2> _locationOfStats;
    cnt::Array<sc::Range,2> _locationOfModels;
    cnt::Array<td::INT4,2> _indexOfDeltaT;
    cnt::Array<td::INT4,2>_indexOfTime;
    cnt::Array<ModelInfo,2> _modelInfo;
    sc::ILog* _pLog;
    td::UINT4 _nInitialVars = 0;
    td::UINT4 _nInitialParams = 0;
    td::UINT4 _nOutVars = 0;
    td::UINT4 _nOutParams = 0;
    td::INT4 _copyParametersFromParentToSubModelRequired = 0;
    td::UINT4 _copyParametersFromParentToSubModelCopied = 0;
    td::UINT4 _initialNumberOfSubProblemParams = 0;
    
    td::UINT2 _namingPosition = 0;
    td::BYTE _complexDomain = 0;
    
    TokenizedModel() = delete;
protected:
    bool countExpressions(CntGroupInfo& gi, const sc::LinesInfo& li, sc::ModelLevel modelLevel);
    void addLogEntry(const sc::ILog::Entry& le, sc::ILog::Type type = sc::ILog::Type::Error);
    //load helpers
    void initModelLoader(void* modelLoader);
    bool loadContent(void* modelLoader, mu::LineLoader& ll, td::MutableString& mStr, td::INT4 nSuppLines);
    bool loadVarsAndParams(void* modelLoader, td::MutableString& mStr, td::INT4& nSuppLines);
public:
    explicit TokenizedModel(sc::ILog* pLog, cnt::SafeFullVector<td::Variant>& headerAttribs, TranslateFunction translate, sc::Naming& naming, td::UINT2 namingPosition);
    
    //load methods
    bool loadFromEditor(mu::ILineLoader* pLL, bool cleanNaming, sc::Domain domain);
    bool loadFromFile(const td::String& fileName);
    bool loadFromString(const td::String& strDigitModel);
    
    const ModelInfo& getModelInfo(sc::ModelLevel modelLevel) const;
    td::UINT4 getNumberOfExpressions(sc::Group groupID, sc::ModelLevel modelLevel) const;
    bool isComplex() const;
    const sc::ExpressionTokens& getExpressionTokens() const;
    const sc::AttribTokens& getAttribTokens() const;
    td::UINT4 countOutSymbols();
    td::UINT4 getNumberOfOutVars() const;
    td::UINT4 getNumberOfOutParams() const;
    void populateOutVars(td::INT4* indices) const;
    void populateOutParams(td::INT4* indices, td::INT4 offset) const;
    void populateOutSymbols(cnt::SafeFullVector<sc::SymbolInfo>& symbolInfo, td::UINT4 nVars) const;
    bool hasRepetitions() const;
    td::INT4 getIndexOfTimeParam(sc::ModelLevel modelLevel) const;
    td::INT4 getIndexOfDeltaTimeParam(sc::ModelLevel modelLevel) const;
    
    class const_iterator;

    class iterator
    {
        const sc::LinesInfo* _pLinesInfo;
        const sc::ExpressionTokens* _pModelTokens;
        const sc::AttribTokens* _pAttribTokens;
        
        td::UINT4 _pos;
        td::UINT4 _lastPos;
        
    protected:
        
        inline bool get(const td::StringExt& valIn, double& valOut) const
        {
            const char* pStr = valIn.c_str();
            if (!pStr)
                return false;
            valOut = std::atof(pStr);
            return true;
        }
        
        inline bool get(const td::StringExt* valIn, double& valOut) const
        {
            if (!valIn)
            {
//                valOut = 0;
                return false;
            }
            const char* pStr = valIn->c_str();
            if (!pStr)
                return false;
            valOut = std::atof(pStr);
            return true;
        }
        
        // Generic version with concept to exclude StringExt
        template <typename TVALIN, typename TVALOUT>
            requires (!std::is_same_v<std::remove_cv_t<TVALIN>, td::StringExt*>)
        inline bool get(const TVALIN& valIn, TVALOUT& valOut) const
        {
            valOut = TVALOUT(valIn);
            return true;
        }
        
        
        
    public:
        iterator()
        : _pLinesInfo(nullptr)
        , _pModelTokens(nullptr)
        , _pAttribTokens(nullptr)
        , _pos(0)
        , _lastPos(0)
        {
            
        }
        
        iterator(const LinesInfo* pLI, const ExpressionTokens* pModelTokens,
                 const sc::AttribTokens* pAttribTokens, td::UINT4 pos, td::UINT4 nElems)
        : _pLinesInfo(pLI)
        , _pModelTokens(pModelTokens)
        , _pAttribTokens(pAttribTokens)
        , _pos(pos)
        , _lastPos(nElems+pos)
        {
        }
        
        void operator = (const iterator& it)
        {
            _pLinesInfo = it._pLinesInfo;
            _pModelTokens = it._pModelTokens;
            _pAttribTokens = it._pAttribTokens;
            _pos = it._pos;
            _lastPos = it._lastPos;
        }
        
        bool operator == (const iterator& it) const
        {
            if (_pLinesInfo != it._pLinesInfo);
                return false;
            return (_pos == it._pos && _lastPos == it._lastPos);
        }
        
        const auto& operator* () const
        {
            return (*_pLinesInfo)[_pos];
        }

        const auto* operator ->() const
        {
            return &(*_pLinesInfo)[_pos];;
        }
        
        void operator ++ ()
        {
            //assert(_pos < _lastPos);
            ++_pos;
        }
        
        bool isOk() const
        {
            return (_pos < _lastPos);
        }
        
        const td::StringExt* getAttribValueStrPtr(sc::AttribID attID) const
        {
            if (!isOk())
                return nullptr;
            const auto& lin = (*_pLinesInfo)[_pos];
            if (lin.nAttribTokens == 0)
                return nullptr;
            
            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
            {
                const sc::TokVal& tv = (*_pAttribTokens)[pos];
                if (tv.attribID == attID)
                {
                    if ( (tv.token == sc::Token::ConstString) || (tv.token ==sc::Token::AttribEnumValue ) )
                        return tv.pStr;
                    else
                        return nullptr;
                }
            }
            return nullptr;
        }
        
        inline bool getAttribValueToken(sc::AttribID attID, sc::TokVal& tokAttValue) const
        {
            if (!isOk())
                return false;
            const auto& lin = (*_pLinesInfo)[_pos];
            if (lin.nAttribTokens == 0)
                return false;
            
            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
            {
                const sc::TokVal& tv = (*_pAttribTokens)[pos];
                if (tv.attribID == attID)
                {
                    tokAttValue = tv;
                    return true;
                }
            }
            return false;
        }
        
        bool getAttribValue(sc::AttribID attID, td::ColorID& val) const
        {
            assert(sc::isColor(attID));
            if (!sc::isColor(attID))
                return false;
            
            if (!isOk())
                return false;
            const auto& lin = (*_pLinesInfo)[_pos];
            if (lin.nAttribTokens == 0)
                return false;
            
            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
            {
                const sc::TokVal& tv = (*_pAttribTokens)[pos];
                if (tv.attribID == attID)
                {
                    val = tv.colorID;
                    return true;
                }
            }
            return false;
        }
        
        bool getAttribValue(sc::AttribID attID, td::LinePattern& val) const
        {
            assert(attID == sc::AttribID::Pattern);
            
            if (!isOk())
                return false;
            const auto& lin = (*_pLinesInfo)[_pos];
            if (lin.nAttribTokens == 0)
                return false;
            
            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
            {
                const sc::TokVal& tv = (*_pAttribTokens)[pos];
                if (tv.attribID == attID)
                {
                    if (tv.token == sc::Token::ConstLinePattern)
                        val = tv.linePattern;
                    else if (tv.token == sc::Token::ConstString)
                    {
                        if (tv.pStr)
                        {
                            //NEED FIX for this: LinePattern should be assigned earlier
                            td::LinePattern lp = td::toLinePattern(tv.pStr->c_str());
                            val = lp;
                        }
                        else
                            val = td::LinePattern::Solid;
                    }
                    else
                        val = td::LinePattern::Solid;
                    return true;
                }
            }
            return false;
        }
        
        bool getAttribValue(sc::AttribID attID, td::DotPattern& val) const
        {
            assert(false); //TODO
//            assert(attID == sc::AttribID::DotPattern);
//
//            if (!isOk())
//                return false;
//            const auto& lin = (*_pLinesInfo)[_pos];
//            if (lin.nAttribTokens == 0)
//                return false;
//            
//            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
//            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
//            {
//                const sc::TokVal& tv = (*_pAttribTokens)[pos];
//                if (tv.attribID == attID)
//                {
//                    val = tv.linePattern;
//                    return true;
//                }
//            }
//            return false;
        }
        
        template <typename T>
        bool getAttribValue(sc::AttribID attID, T& val) const
        {
            assert(!sc::isColor(attID));
            if (!isOk())
                return false;
            const auto& lin = (*_pLinesInfo)[_pos];
            if (lin.nAttribTokens == 0)
                return false;
            
            td::UINT4 nEnd = lin.nAttribTokens + lin.attribStartPos;
            for (td::UINT4 pos = lin.attribStartPos; pos < nEnd; ++pos)
            {
                const sc::TokVal& tv = (*_pAttribTokens)[pos];
                if (tv.attribID == attID)
                {
                    switch (tv.token)
                    {
                        case sc::Token::ConstInt:
                            return get(tv.i4Val, val);
                        case sc::Token::ConstFloat:
                            return get(tv.r8Val, val);
                        case sc::Token::ConstTrue:
                        {
                            bool bTrue = true;
                            return get(bTrue, val);
                        }
                        case sc::Token::ConstFalse:
                        {
                            bool bTrue = false;
                            return get(bTrue, val);
                        }
                        case sc::Token::ConstImag:
                            return get(tv.r8Val, val);
                        default:
                            return false;
                    }
                }
            }
            return false;
        }
    };
    
    iterator getGroup(Group exprGroup, ModelLevel modelLevel) const
    {
        if (modelLevel == ModelLevel::MainModel)
        {
            td::UINT4 pos = 0;
            td::UINT4 nElems = 0;
            pos = _mainModelGroups[size_t(exprGroup)].positionInList;
            nElems = _mainModelGroups[size_t(exprGroup)].nElems;
            if (pos > 0 && nElems > 0)
                ++nElems; //group definition with attrib
            iterator it(&_mainModelLinesInfo, &_modelTokens, &_attribTokens,  pos, nElems);
            return it;
        }
        
        td::UINT4 pos = _subModelGroups[size_t(exprGroup)].positionInList;
        td::UINT4 nElems = _subModelGroups[size_t(exprGroup)].nElems;
        if (pos > 0 && nElems > 0)
            ++nElems; //group definition with attribs
        iterator it(&_subModelLinesInfo, &_modelTokens, &_attribTokens, pos, nElems);
        return it;
    }
    
    td::UINT4 countChildren(const iterator& it, td::UINT2 namingPosition, sc::Group groupID, sc::ExpressionType et) const;
//    td::UINT4 countChildren(td::UINT2 namingPosition, sc::Group groupID, sc::ModelLevel modelLevel) const;
    sc::ExpressionType getExpressionType(const iterator& it, td::UINT2 namingPosition) const;
//    bool copyParentParentsToSubModel() const;
    
    td::UINT4 getCopyParamsFromParentToSubModelCopied() const
    {
        return _copyParametersFromParentToSubModelCopied;
    }
    
    td::UINT4 getNumberOfInitialSubProblemParams() const
    {
        return _initialNumberOfSubProblemParams;
    }
    
    bool getHeaderVariantValue(sc::HeaderParam headerParamID, td::Variant& val) const;
    
    //1 all ok, 0-attrib is not available, -1 - attrib is available but not double
    int getHeaderParamValue(sc::HeaderParam headerParamID, double& val) const;
    int getHeaderParamValue(sc::HeaderParam headerParamID, td::INT4& val) const;
    td::String getHeaderParamString(sc::HeaderParam headerParamID) const;
    
    Report getReportingType() const;
//    static std::span<const char* const> getModelAttribNames();
    
};

using TokenLinesIterator = TokenizedModel::iterator;


//inline std::span<const char* const> getAttribs()
//{
//    static const char* attribs[] = {sc::TokenizedModel::cVersion, sc::TokenizedModel::cStartTime, sc::TokenizedModel::cStepTime, sc::TokenizedModel::cEndTime, sc::TokenizedModel::cMaxIter, sc::TokenizedModel::cMaxReps, sc::TokenizedModel::cReport};
//    return attribs;
//}

//inline const std::initializer_list<const char*>& getModelAttribNames()
//{
//    static const std::initializer_list<const char*> = {sc::TokenizedModel::cVersion, sc::TokenizedModel::cStartTime, sc::TokenizedModel::cStepTime, sc::TokenizedModel::cEndTime, sc::TokenizedModel::cMaxIter, sc::TokenizedModel::cMaxReps, sc::TokenizedModel::cReport};
//}

} //namspace sc
