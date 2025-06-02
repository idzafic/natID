// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natCode.h"
#include "TextEdit.h"
#include <td/ColorID.h>
#include <td/MutableString.h>
#include <unordered_set>
#include <sc/Token.h>
#include <fo/FileOperations.h>
#include <sc/VarsAndParams.h>
#include <functional>
#include <sc/ILog.h>
#include <mu/LineLoader.h>
#include <sc/SCLib.h>
#include <sc/Naming.h>
#include <span>

namespace gui
{

class CodeEditorStatusBar;

class NATCODE_API CodeEdit : public TextEdit
{
public:
    
    using ColorMarker = struct CM
    {
        td::UINT4 location;
        td::UINT4 length : 24;
        td::ColorID colorID;
    };
    
    class Token
    {
        Token() = delete;
    public:
        enum class Error : td::BYTE {None=0, CloseParenthesisMismatch};
        const td::String& strTok;
        gui::Range range;
        td::UINT4 lineNo; //1 - based
        td::UINT4 colNo; //1-based
        td::INT2 index; //index in case of function or reserved label
        sc::Token token;
        Error error;
        
        inline Token(const td::String& lbl, const gui::Range& rangeIn, td::UINT4 ln, td::UINT4 col, td::INT2 ind, sc::Token scTokenIn)
        : strTok(lbl)
        , range(rangeIn)
        , lineNo(ln)
        , colNo(col)
        , index(ind)
        , token(scTokenIn)
        {}
    };
    
    class LineToken
    {
        LineToken() = delete;
    public:
        const Token* pCEToken = nullptr;
        //td::UINT4 lineStartPos;
        td::UINT4 inAttrib = 0;
        sc::Token prevTokens[2] = {sc::Token::EndOfLine, sc::Token::EndOfLine};
        char prevIndices[2] = {-1, -1};
        sc::Location location;
        td::BYTE generateErroLog;
        
//        LineToken(const CodeEdit::Token& tok, td::UINT4 currLineStartPos, sc::Location location, bool generateErr);
        LineToken(sc::Location location, bool generateErr);
        
        void setToken(const CodeEdit::Token& tok);

        void setLastToken(sc::Token token, char index);
        bool isPrevBase() const;
    };
    //using VarAndParamMap = std::unordered_map<td::String, td::UINT4>;
    //using VarAndParamSet = std::unordered_set<td::String>;
    friend class mu::LineLoader;
protected:
    sc::Naming* _pNaming = nullptr;
    const CodeEdit* _parentEditor = nullptr;
    cnt::SafeFullVector<CodeEdit*> _supplemenaryEditors;
    cnt::SafeFullVector<td::Variant> _headerAttribs;  //header attribs
    cnt::Array<cnt::PushBackVector<td::Variant>,2> _modelAttribs; //model and submodel attribs
    cnt::Array<Selection,2> _selections;
    cnt::Array<sc::Range,2> _locationOfVars;
    cnt::Array<sc::Range,2> _locationOfParams;
    cnt::Array<sc::Range,2> _locationOfDistribs;
    cnt::Array<sc::Range,2> _locationOfStats;
    cnt::Array<sc::Range,2> _locationOfModels;
    cnt::Array<td::INT4,2> _indexOfDeltaT; //dummy
    cnt::Array<td::INT4,2> _indexOfTime;   //dummy
    cnt::Array<sc::VarAndParamSet,8> _duplicatedVarsAndParams;
    
    CodeEditorStatusBar* _pStatusBar = nullptr;
    
    sc::VarAndParamMap* _userLabels[(size_t)sc::UserLabel::Last];
    sc::VarAndParamMap* _userLabelsSubModel[(size_t)sc::UserLabel::Last];
    
    sc::ILog* _pLog = nullptr;
    td::UINT4 _totalErrors = 0;
    td::UINT4 _glyphPos = 0;  
    td::UINT4 _lastCountedLines = 1;
    td::UINT2 _version = 1;
    td::UINT2 _editorPositionInLogger = 0;
    td::UINT2 _namingPosition = 0; //which naming position is used in an instance of this class (getSymbolPosition)
    td::INT2 _scopeLevel = 0;
    
    sc::UserLabels _userLabelsType = sc::UserLabels::Intern;

    sc::Type _type = sc::Type::ModelWithGroups;
    sc::Group _currentGroup = sc::Group::Last;
    sc::Location _reColorSource = sc::Location::Model;
    td::BYTE _complexDomain = 0;
    td::BYTE _useLaplaceOperator = 0;
    td::BYTE _needsReColor = 0;
    td::BYTE _timeDomainModel = 0; 

private:
    void checkAndReColorIfNecessary();
    bool checkAndReColorIfNecessary(const Selection& currSel);
    sc::ColorGroup checkForVarOrParam(const td::String& str, sc::Location loc) const;
    void clearDuplicated();
    bool isDuplicated(const td::String& name, sc::Location location) const;
    void addTimeDomainParams(td::MutableString& mStr);
protected:
    void colorCurrentLine();
    void colorTwoLines();
    void parseLine(const td::MutableString& mStr, td::UINT4& lineNo, td::UINT4& prevGlyphPos, bool generateErroLog);
    void finalizeLoading(td::UINT4 lastLineNo, bool generateErroLog, bool initialSelect);
    
    //overrides
    bool onKeyPressed(const gui::Key& key) override;
    bool onChangedContent(Source source) override;
    bool onNewLine(td::UINT2& nTabsToInsert) override;
    bool onSelectionChange(const Selection& sel) override;
    void onDeletedLines(const gui::Range& range, td::UINT4 nDeletedLines) override; //after deeleting line(s)
    void adjustPastingString(const char* pastingString, td::MutableString& mStrOut) override;
    void onPaste() override;
    void onCut() override;
    void onUndo() override;
    void onRedo() override;
    
    void onFontChange() override;
protected:

    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType) const;
    const sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType) const;
    sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType);
    sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType);
    
    //-1: symbol is not available in "symbolType" list
    int getUserLabelPosition(const td::String& strLabel, sc::UserLabel userLabelType) const;
    int getCommonLabelPosition(const td::String& strLabel, sc::FunctionSet labelType) const; //common symbols (for c++, xml, javascript, modelMaker,...)
    
    td::MutableString& getMutableString() const;
    
//    virtual bool parseHeaderLine(const td::String& strLine, td::UINT4 headerLineNo, const std::initializer_list<const char*>& headerAttribNames, cnt::PushBackVector<td::Variant>* pAttribValues, const char* supAttName);
    
    bool parseVarsAndParams(mu::LineLoader& lineLoader, td::MutableString& mStr);
    virtual bool parseVarsAndParams(const td::String& strAll, td::MutableString& mStr, td::UINT4 fromPos);
    virtual bool parseVarsAndParams(std::ifstream& f, td::MutableString& mStr, td::UINT4 fromPos);
    virtual bool parseVarsAndParams(td::MutableString& mStr); //from TextEdit
    
    virtual sc::ColorGroup getColorGroup(LineToken& lt);
//    td::ColorID getColor(ColorGroup) const;
    
    void tokenizeLine(const td::String& strLine, td::UINT4 lineNo, td::UINT4 glyphPos, sc::Domain domain, const std::function<void(const Token&)>& onToken);
    void resetColorMarkers();
    void appendColorMarker(ColorMarker& cm);
    void applyColorMarkers(td::UINT4 lastLinePos, bool bLock = true);
    
    sc::Location getLocation(td::UINT4 lineNo) const;
    bool isInModel(td::UINT4 lineNo) const;
    bool isOnModel(td::UINT4 lineNo) const;
    bool isInSubModel(td::UINT4 lineNo) const;
    bool isInSubModel(sc::Location location) const;
    bool isInVarsOrParams(sc::Location location) const;
    void resetVarsAndParamsLocations();

    void needsReColor(sc::Location sourceLoc);
    void resetReColorIndicator();
    void incrementVarsAndParamsLocations(sc::Location location, td::INT4 incrementVal);
    void incrementVarsAndParamsLocations(td::UINT4 currLineNo, td::INT4 incrementVal);

//    bool onSelectionChange(const Selection& sel) override;
    
//    td::UINT4 loadHeaders(std::ifstream& f, td::UINT4 numberOfHeaderLines, const std::initializer_list<const char*>& headerAttribNames, cnt::PushBackVector<td::Variant>* pAttribValues, const char* supAttName, td::MutableString* pSupplementaryText);
    
    bool loadFromFile(std::ifstream& f, td::UINT4 pos);
    
    CodeEdit() = delete;
public:
    CodeEdit(sc::Type type, td::UINT4 noOfSupplementaryEditors, td::UINT2 namingPosition, sc::UserLabels userLabelsType);
    
    void setNaming(sc::Naming* pNaming);
    
    void setVersionNumber(td::UINT2 version);
    td::UINT2 getVersionNumber() const;
    
    void setSupplementaryEditor(td::UINT4 pos, CodeEdit* supEditor);
    CodeEdit* getSupplementaryEditor(td::UINT4 pos);
    void setParentEditor(const CodeEdit* parentEditor);
    const CodeEdit* getParentEditor() const;
    
    cnt::Array<sc::VarAndParamSet,8>&  getDuplicatedVarsAndParams();
    //Model attribs (if there is Model group)
    cnt::SafeFullVector<td::Variant>& getHeaderAttribs();
    const cnt::SafeFullVector<td::Variant>& getHeaderAttribs() const;
    const cnt::PushBackVector<td::Variant>& getModelAttribs();
    
    
    void setLogger(sc::ILog* pLogger);
    sc::ILog* getLogger();
    
    bool isComplexDomain() const;
    
    sc::Domain getDomain() const;
    
    sc::UserLabels getUserLabelsType() const;
    
    sc::VarAndParamMap* getUserLabels(sc::UserLabel lbl);
    void setUserLabels(sc::UserLabel lbl, sc::VarAndParamMap*);
    
    void setUserLabels(sc::VarAndParamMap* userLabels[(size_t)sc::UserLabel::Last]);
    void setUserSubModelLabels(sc::VarAndParamMap* userSubmodelLabels[(size_t)sc::UserLabel::Last]);
    
    void setGlyphPos(td::UINT4 glyphPos);
    void setComplexDomain(bool bComplex);
    
    bool loadFromString(const td::String& strAll);
    
    bool loadFromFile(const td::String& fileName);
//    bool loadFromFile(const fo::fs::path& filePath, td::UINT4 numberOfHeaderLines=0, const std::span<const char* const>& headerAttribNames={}, const char* supAttName = nullptr, td::MutableString* pSupplementaryText = nullptr);
    
    void saveToFile(std::ofstream& f) const; //saves only text to current position in ofstream
    
    td::UINT4 getCurrentLineNo() const; //0-if there is no line tracking
    
    const Selection& getCurrentSelection() const;
    const Selection& getPreviousSelection() const;
    
    void setStatusBar(CodeEditorStatusBar* pStatusBar);
    
    void reColor(bool bForce = false);
    
    void addLogEntry(const sc::ILog::Entry& le, sc::ILog::Type type = sc::ILog::Type::Error);
    
    bool isTimeDomain() const;
    
    sc::Naming* getNaming();
    
    void setPositionInLogger(td::UINT2 posInLogger);
    
    static void setColorForGroup(sc::ColorGroup colorGroup, td::ColorID colorForLightDesktop, td::ColorID colorForDarkDesktop);
    
    static void setDefaultColorGroups();
    
    static void setColorMode(bool bDarkMode);
};

} //namespace gui
