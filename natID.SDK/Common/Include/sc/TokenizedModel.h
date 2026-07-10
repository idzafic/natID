// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TokenizedModel.h
    @brief Declares the TokenizedModel class that holds the tokenized representation of a symbolic model. */
#pragma once
#include <sc/SCLib.h>
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
#include <mu/Utils.h>
#include <mu/IProgressor.h>

namespace sc
{

using TokLine = cnt::Vector<sc::TokVal>;
using TokLineIterator = TokLine::iterator;

/// @brief Stores positional and size metadata for a single tokenized model line.
using LineInfo = struct _tl
{
    td::UINT4 lineNo; ///< Source line number (1-based)
    td::UINT4 linePos; ///< Byte offset of the line start in the token stream
    td::UINT4 expressionStartPos; ///< Start position of the expression tokens for this line
    td::UINT4 nExpressionTokens; ///< Number of expression tokens on this line
    td::UINT4 attribStartPos; ///< Start position of attribute tokens for this line
    td::UINT4 nAttribTokens; ///< Number of attribute tokens on this line
};

using ExpressionTokens = cnt::BlockVector<TokVal, 64*1024/sizeof(sc::TokVal)>;

using AttribTokens = cnt::BlockVector<TokVal, 1024>;

using LinesInfo = cnt::BlockVector<LineInfo, 1024>;

/// @brief Holds top-level metadata about a parsed model.
using ModelInfo = struct _MI
{
    double eps; ///< Solver tolerance (epsilon)
    const td::StringExt* modelName; ///< Pointer to the model name string
    const td::StringExt* methodName; ///< Pointer to the numerical method name string
    sc::ModelType modelType; ///< Type of the model (NL, WLS, ODE, DAE, etc.)
    sc::Domain domain; ///< Mathematical domain (real or complex)
};

/// @brief Holds the complete tokenized representation of a symbolic model, including both main and sub-model structures.
class SYMBOLICOMP_API TokenizedModel
{
public:
    /// @brief Array type mapping each group ID to its counter and position data.
    using CntGroupInfo = cnt::Array<CountersAndPointers, size_t(Group::Last)>;
protected:
    cnt::SafeFullVector<td::Variant>* _pHeaderAttribs; ///< Pointer to the header attribute vector
    mu::IProgressor* _pProgressor = nullptr; ///< Optional progress reporter

    TranslateFunction _translate; ///< Translation/localization callback function
    sc::Naming* _pNaming; ///< Pointer to the naming/symbol table
    sc::ExpressionTokens _modelTokens; ///< Flat storage for all expression tokens
    sc::AttribTokens _attribTokens; ///< Flat storage for all attribute tokens
    sc::LinesInfo _mainModelLinesInfo; ///< Line metadata for the main model
    sc::LinesInfo _subModelLinesInfo; ///< Line metadata for the sub-model

    CntGroupInfo _subModelGroups; ///< Group counters and positions for the sub-model
    CntGroupInfo _mainModelGroups; ///< Group counters and positions for the main model

    cnt::Array<cnt::PushBackVector<td::Variant>,2> _modelAttribs; ///< Attribute lists for main model and sub-model
    cnt::Array<sc::Range,2> _locationOfVars; ///< Token ranges for variable declarations (main + sub)
    cnt::Array<sc::Range,2> _locationOfParams; ///< Token ranges for parameter declarations (main + sub)
    cnt::Array<sc::Range,2> _locationOfConsts; ///< Token ranges for constant declarations (main + sub)
    cnt::Array<sc::Range,2> _locationOfDistribs; ///< Token ranges for distribution declarations (main + sub)
    cnt::Array<sc::Range,2> _locationOfStats; ///< Token ranges for statistics declarations (main + sub)

    cnt::Array<sc::Range,2> _locationOfDataSets; ///< Token ranges for data set declarations (main + sub)
    cnt::Array<sc::Range,2> _locationOfSamplers; ///< Token ranges for sampler declarations (main + sub)

    cnt::Array<sc::Range,2> _locationOfModels; ///< Token ranges for model declarations (main + sub)
    cnt::Array<td::INT4,2> _indexOfDeltaT; ///< Index of the delta-T parameter for each model level
    cnt::Array<td::INT4,2> _indexOfTime; ///< Index of the time parameter for each model level
    cnt::Array<td::UINT4,2> _numberOfLimits; ///< Number of limit expressions for each model level
    cnt::Array<ModelInfo,2> _modelInfo; ///< Model metadata for main model and sub-model
    sc::ILog* _pLog = nullptr; ///< Pointer to the diagnostic log interface
    td::String _fileName; ///< Source file name associated with this model
    size_t _nProgressSteps = 0; ///< Total number of progress steps for reporting
    td::UINT4 _nInitialVars = 0; ///< Number of initial variables in the model
    td::UINT4 _nInitialParams = 0; ///< Number of initial parameters in the model
    td::UINT4 _nOutVars = 0; ///< Number of output variables
    td::UINT4 _nOutParams = 0; ///< Number of output parameters
    td::INT4 _copyParametersFromParentToSubModelRequired = 0; ///< Flag indicating if parent-to-sub-model parameter copy is required
    td::UINT4 _copyParametersFromParentToSubModelCopied = 0; ///< Count of parameters already copied from parent to sub-model
    td::UINT4 _initialNumberOfSubProblemParams = 0; ///< Initial parameter count for sub-problem setup
    td::UINT4 _noOfHeaderLines = 0; ///< Number of header lines in the source
    td::UINT2 _namingPosition = 0; ///< Naming position index used for attribute lookup
    td::BYTE _complexDomain = 0; ///< Non-zero if the model operates in the complex domain
    td::BYTE _progressLocation = 0; ///< Identifier for the progress reporting location
protected:
    /// @brief Counts expressions within a set of lines for a given model level.
    /// @param gi Group info array to populate with counts.
    /// @param li Line info array to scan.
    /// @param modelLevel The model level (main or sub-model).
    /// @return True on success, false if an error was encountered.
    bool countExpressions(CntGroupInfo& gi, const sc::LinesInfo& li, sc::ModelLevel modelLevel);

    /// @brief Appends a diagnostic log entry.
    /// @param le The log entry to add.
    /// @param type The severity type of the entry (default: Error).
    void addLogEntry(const sc::ILog::Entry& le, sc::ILog::Type type = sc::ILog::Type::Error);

    /// @brief Initializes internal state from an opaque model loader object.
    /// @param modelLoader Pointer to the model loader implementation.
    void initModelLoader(void* modelLoader);

    /// @brief Loads tokenized content from a line loader into internal storage.
    /// @param modelLoader Pointer to the model loader implementation.
    /// @param ll Reference to the line loader providing source lines.
    /// @param mStr Mutable string buffer used during loading.
    /// @param nSuppLines Number of supplementary lines to process.
    /// @return True on success, false if parsing failed.
    bool loadContent(void* modelLoader, mu::LineLoader& ll, td::MutableString& mStr, td::INT4 nSuppLines);

    /// @brief Loads and tokenizes variable and parameter declarations.
    /// @param modelLoader Pointer to the model loader implementation.
    /// @param mStr Mutable string buffer used during loading.
    /// @param nSuppLines Output count of supplementary lines found.
    /// @return True on success, false if parsing failed.
    bool loadVarsAndParams(void* modelLoader, td::MutableString& mStr, td::INT4& nSuppLines);
public:
    /// @brief Constructs a TokenizedModel with all required dependencies.
    /// @param pLog Pointer to the diagnostic log interface.
    /// @param headerAttribs Reference to the header attribute vector.
    /// @param translate Translation/localization callback function.
    /// @param naming Reference to the naming/symbol table.
    /// @param namingPosition Naming position index for attribute lookup.
    explicit TokenizedModel(sc::ILog* pLog, cnt::SafeFullVector<td::Variant>& headerAttribs, TranslateFunction translate, sc::Naming& naming, td::UINT2 namingPosition);

    /// @brief Default constructor creating an uninitialized TokenizedModel.
    TokenizedModel();

    /// @brief Initializes this TokenizedModel with all required dependencies after default construction.
    /// @param pLog Pointer to the diagnostic log interface.
    /// @param headerAttribs Reference to the header attribute vector.
    /// @param translate Translation/localization callback function.
    /// @param naming Reference to the naming/symbol table.
    /// @param namingPosition Naming position index for attribute lookup.
    void init(sc::ILog* pLog, cnt::SafeFullVector<td::Variant>& headerAttribs, TranslateFunction translate, sc::Naming& naming, td::UINT2 namingPosition);

    /// @brief Loads and tokenizes model content from an editor line loader.
    /// @param pLL Pointer to the line loader providing source content.
    /// @param cleanNaming Whether to reset the naming table before loading.
    /// @param domain The mathematical domain (real or complex) to use.
    /// @return True on success, false if loading or parsing failed.
    bool loadFromEditor(mu::ILineLoader* pLL, bool cleanNaming, sc::Domain domain);

    /// @brief Loads and tokenizes model content from a file on disk.
    /// @param fileName Path to the model source file.
    /// @return True on success, false if the file could not be read or parsed.
    bool loadFromFile(const td::String& fileName);

    /// @brief Loads and tokenizes model content from an in-memory string.
    /// @param strDigitModel The model source text as a string.
    /// @return True on success, false if parsing failed.
    bool loadFromString(const td::String& strDigitModel);

    /// @brief Returns the model metadata for the specified model level.
    /// @param modelLevel The model level (main or sub-model).
    /// @return Const reference to the ModelInfo structure.
    const ModelInfo& getModelInfo(sc::ModelLevel modelLevel) const;

    /// @brief Returns the number of expressions in a specific group and model level.
    /// @param groupID The group whose expression count is requested.
    /// @param modelLevel The model level (main or sub-model).
    /// @return The number of expressions in the group.
    td::UINT4 getNumberOfExpressions(sc::Group groupID, sc::ModelLevel modelLevel) const;

    /// @brief Returns whether the model operates in the complex domain.
    /// @return True if the domain is complex, false if real.
    bool isComplex() const;

    /// @brief Returns a const reference to the flat expression token storage.
    /// @return Const reference to the ExpressionTokens container.
    const sc::ExpressionTokens& getExpressionTokens() const;

    /// @brief Returns a const reference to the flat attribute token storage.
    /// @return Const reference to the AttribTokens container.
    const sc::AttribTokens& getAttribTokens() const;

    /// @brief Counts and caches the number of output symbols in the model.
    /// @return Total number of output symbols (variables + parameters).
    td::UINT4 countOutSymbols();

    /// @brief Returns the cached number of output variables.
    /// @return Number of output variables.
    td::UINT4 getNumberOfOutVars() const;

    /// @brief Returns the cached number of output parameters.
    /// @return Number of output parameters.
    td::UINT4 getNumberOfOutParams() const;

    /// @brief Populates an array with the indices of output variables.
    /// @param indices Pointer to the pre-allocated output array.
    void populateOutVars(td::INT4* indices) const;

    /// @brief Populates an array with the indices of output parameters, offset by a given value.
    /// @param indices Pointer to the pre-allocated output array.
    /// @param offset Offset added to each parameter index.
    void populateOutParams(td::INT4* indices, td::INT4 offset) const;

    /// @brief Populates a symbol-info vector with information about all output symbols.
    /// @param symbolInfo Vector to fill with SymbolInfo entries.
    /// @param nVars Total number of variables (used to compute parameter index offsets).
    void populateOutSymbols(cnt::SafeFullVector<sc::SymbolInfo>& symbolInfo, td::UINT4 nVars) const;

    /// @brief Returns whether the model contains any repeat constructs.
    /// @return True if the model has repetitions, false otherwise.
    bool hasRepetitions() const;

    /// @brief Returns the symbol-table index of the time parameter for a given model level.
    /// @param modelLevel The model level (main or sub-model).
    /// @return The index, or -1 if no time parameter is present.
    td::INT4 getIndexOfTimeParam(sc::ModelLevel modelLevel) const;

    /// @brief Returns the symbol-table index of the delta-time parameter for a given model level.
    /// @param modelLevel The model level (main or sub-model).
    /// @return The index, or -1 if no delta-time parameter is present.
    td::INT4 getIndexOfDeltaTimeParam(sc::ModelLevel modelLevel) const;

    /// @brief Attaches a progress reporter for long-running load operations.
    /// @param progressor Pointer to the IProgressor implementation.
    /// @param nSteps Total number of progress steps.
    /// @param progressLocation Identifier tag for the reporting location.
    void setProgressor(mu::IProgressor* progressor, size_t nSteps, td::BYTE progressLocation);

    class const_iterator;

    /// @brief Forward iterator over the tokenized lines of a model group.
    class iterator
    {
        const sc::LinesInfo* _pLinesInfo; ///< Pointer to the lines-info collection being iterated
        const sc::ExpressionTokens* _pModelTokens; ///< Pointer to the expression token storage
        const sc::AttribTokens* _pAttribTokens; ///< Pointer to the attribute token storage

        td::UINT4 _pos; ///< Current iterator position index
        td::UINT4 _lastPos; ///< One-past-the-end position index

    protected:

        /// @brief Converts a StringExt reference to a double.
        /// @param valIn Input string reference.
        /// @param valOut Output double value.
        /// @return True if conversion succeeded, false otherwise.
        inline bool get(const td::StringExt& valIn, double& valOut) const
        {
            const char* pStr = valIn.c_str();
            if (!pStr)
                return false;
            valOut = mu::toDouble(pStr);
            return true;
        }

        /// @brief Converts a StringExt pointer to a double.
        /// @param valIn Pointer to the input string.
        /// @param valOut Output double value.
        /// @return True if conversion succeeded, false otherwise.
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
            valOut = mu::toDouble(pStr);
            return true;
        }

        // Generic version with concept to exclude StringExt
        /// @brief Generic conversion helper for non-StringExt value types.
        /// @tparam TVALIN Input value type (must not be td::StringExt*).
        /// @tparam TVALOUT Output value type.
        /// @param valIn The input value.
        /// @param valOut The output value.
        /// @return Always true.
        template <typename TVALIN, typename TVALOUT>
            requires (!std::is_same_v<std::remove_cv_t<TVALIN>, td::StringExt*>)
        inline bool get(const TVALIN& valIn, TVALOUT& valOut) const
        {
            valOut = TVALOUT(valIn);
            return true;
        }



    public:
        /// @brief Default constructor creating an invalid (empty) iterator.
        iterator()
        : _pLinesInfo(nullptr)
        , _pModelTokens(nullptr)
        , _pAttribTokens(nullptr)
        , _pos(0)
        , _lastPos(0)
        {

        }

        /// @brief Constructs an iterator positioned at a specific range within a lines-info collection.
        /// @param pLI Pointer to the lines-info collection.
        /// @param pModelTokens Pointer to the expression token storage.
        /// @param pAttribTokens Pointer to the attribute token storage.
        /// @param pos Starting position index.
        /// @param nElems Number of elements to iterate over.
        iterator(const LinesInfo* pLI, const ExpressionTokens* pModelTokens,
                 const sc::AttribTokens* pAttribTokens, td::UINT4 pos, td::UINT4 nElems)
        : _pLinesInfo(pLI)
        , _pModelTokens(pModelTokens)
        , _pAttribTokens(pAttribTokens)
        , _pos(pos)
        , _lastPos(nElems+pos)
        {
        }

        /// @brief Assigns another iterator's state to this iterator.
        /// @param it The source iterator.
        void operator = (const iterator& it)
        {
            _pLinesInfo = it._pLinesInfo;
            _pModelTokens = it._pModelTokens;
            _pAttribTokens = it._pAttribTokens;
            _pos = it._pos;
            _lastPos = it._lastPos;
        }

        /// @brief Compares two iterators for equality.
        /// @param it The iterator to compare against.
        /// @return True if both iterators point to the same position in the same collection.
        bool operator == (const iterator& it) const
        {
            if (_pLinesInfo != it._pLinesInfo)
                return false;
            return (_pos == it._pos && _lastPos == it._lastPos);
        }

        /// @brief Dereferences the iterator to obtain the current LineInfo.
        /// @return Const reference to the current LineInfo entry.
        const auto& operator* () const
        {
            return (*_pLinesInfo)[_pos];
        }

        /// @brief Returns a pointer to the current LineInfo entry.
        /// @return Const pointer to the current LineInfo entry.
        const auto* operator ->() const
        {
            return &(*_pLinesInfo)[_pos];;
        }

        /// @brief Advances the iterator to the next line.
        void operator ++ ()
        {
            //assert(_pos < _lastPos);
            ++_pos;
        }

        /// @brief Checks whether the iterator is within its valid range.
        /// @return True if the current position is before the end, false otherwise.
        bool isOk() const
        {
            return (_pos < _lastPos);
        }

        /// @brief Retrieves a string pointer for an attribute value on the current line.
        /// @param attID The attribute ID to look up.
        /// @return Pointer to the string value, or nullptr if the attribute is absent or not a string.
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

        /// @brief Retrieves the raw TokVal for an attribute on the current line.
        /// @param attID The attribute ID to look up.
        /// @param tokAttValue Output TokVal filled with the attribute token data.
        /// @return True if the attribute was found, false otherwise.
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

        /// @brief Retrieves a color attribute value from the current line.
        /// @param attID The color attribute ID (must satisfy isColor()).
        /// @param val Output ColorID value.
        /// @return True if the color attribute was found, false otherwise.
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

        /// @brief Retrieves a line-pattern attribute value from the current line.
        /// @param attID The attribute ID (must be AttribID::Pattern).
        /// @param val Output LinePattern value.
        /// @return True if the pattern attribute was found, false otherwise.
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

        /// @brief Retrieves a dot-pattern attribute value from the current line (not yet implemented).
        /// @param attID The attribute ID.
        /// @param val Output DotPattern value.
        /// @return Always returns without a value (asserts false; TODO).
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

        /// @brief Retrieves a typed attribute value from the current line.
        /// @tparam T The target value type to convert the attribute token into.
        /// @param attID The attribute ID to look up.
        /// @param val Output value of type T.
        /// @return True if the attribute was found and converted successfully, false otherwise.
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

    /// @brief Returns an iterator over the lines belonging to a specific group and model level.
    /// @param exprGroup The group whose lines are to be iterated.
    /// @param modelLevel The model level (main or sub-model).
    /// @return An iterator positioned at the start of the group's lines.
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

    /// @brief Counts the number of child expressions (e.g. cases or sub-groups) under a given iterator position.
    /// @param it The iterator pointing to the parent expression.
    /// @param namingPosition The naming position index used for token classification.
    /// @param groupID The group in which to search.
    /// @param et The expression type to count.
    /// @return The number of matching child expressions.
    td::UINT4 countChildren(const iterator& it, td::UINT2 namingPosition, sc::Group groupID, sc::ExpressionType et) const;

    /// @brief Counts switch-case clauses under the given iterator and checks for a default clause.
    /// @param it The iterator pointing to the switch expression.
    /// @param namingPosition The naming position index.
    /// @param groupID The group containing the switch statement.
    /// @param defaultFound Output flag set to true if a default clause is present.
    /// @return The number of case clauses found.
    td::UINT4 countSwitchCases(const iterator& it, td::UINT2 namingPosition, sc::Group groupID, bool& defaultFound) const;
    //static int findtCaseConditionSeparator(const sc::ExpressionTokens& modelTokens, const iterator& it);

//    td::UINT4 countChildren(td::UINT2 namingPosition, sc::Group groupID, sc::ModelLevel modelLevel) const;

    /// @brief Determines the expression type for the line currently pointed to by the iterator.
    /// @param it The iterator pointing to the line of interest.
    /// @param namingPosition The naming position index used for classification.
    /// @return The ExpressionType of the current line.
    sc::ExpressionType getExpressionType(const iterator& it, td::UINT2 namingPosition) const;
//    bool copyParentParentsToSubModel() const;

    /// @brief Returns the number of parameters actually copied from a parent model to the sub-model.
    /// @return Count of copied parameters.
    td::UINT4 getCopyParamsFromParentToSubModelCopied() const
    {
        return _copyParametersFromParentToSubModelCopied;
    }

    /// @brief Returns the initial parameter count for the sub-problem at the time of model loading.
    /// @return Initial sub-problem parameter count.
    td::UINT4 getNumberOfInitialSubProblemParams() const
    {
        return _initialNumberOfSubProblemParams;
    }

    /// @brief Returns the number of header lines found in the source file.
    /// @return Number of header lines.
    td::UINT4 getNumberOfHeaderLines() const
    {
        return _noOfHeaderLines;
    }

    /// @brief Retrieves a header attribute value as a Variant by its parameter ID.
    /// @param headerParamID The header parameter ID to look up.
    /// @param val Output Variant filled with the header attribute value.
    /// @return True if the attribute was found, false otherwise.
    bool getHeaderVariantValue(sc::HeaderParam headerParamID, td::Variant& val) const;

    /// @brief Retrieves a header parameter value as a double.
    /// @param headerParamID The header parameter ID to look up.
    /// @param val Output double value.
    /// @return 1 if found and converted, 0 if the attribute is absent, -1 if present but not convertible to double.
    int getHeaderParamValue(sc::HeaderParam headerParamID, double& val) const;

    /// @brief Retrieves a header parameter value as a 32-bit integer.
    /// @param headerParamID The header parameter ID to look up.
    /// @param val Output integer value.
    /// @return 1 if found and converted, 0 if the attribute is absent, -1 if present but not convertible to INT4.
    int getHeaderParamValue(sc::HeaderParam headerParamID, td::INT4& val) const;

    /// @brief Retrieves a header parameter value as a string.
    /// @param headerParamID The header parameter ID to look up.
    /// @return The string value, or an empty string if the attribute is absent.
    td::String getHeaderParamString(sc::HeaderParam headerParamID) const;

    /// @brief Returns the reporting type configured for this model.
    /// @return The Report enumeration value.
    Report getReportingType() const;

    /// @brief Returns the number of limit expressions for a given model level.
    /// @param modelLevel The model level (main or sub-model).
    /// @return The number of limit expressions.
    const td::UINT4 getNumberOfLimits(sc::ModelLevel modelLevel) const
    {
        return _numberOfLimits[(td::BYTE) modelLevel];
    }

    /// @brief Sets the source file name associated with this model.
    /// @param fileName The file path string to store.
    void setFileName(const td::String& fileName)
    {
        _fileName = fileName;
    }

    /// @brief Returns the source file name associated with this model.
    /// @return Const reference to the stored file name string.
    const td::String& getFileName() const
    {
        return _fileName;
    }
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
