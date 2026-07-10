// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CodeEdit.h
    @brief A syntax-aware code editing control with color highlighting, variable tracking, and logging support. */
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
class CodeEditHelper;

/// @brief A text editor control extended with syntax highlighting, token parsing, variable tracking, and error logging.
class NATCODE_API CodeEdit : public TextEdit
{
    friend class CodeEditHelper;
public:

    /// @brief Describes a color marker applied to a range of text in the editor.
    using ColorMarker = struct CM
    {
        td::UINT4 location; ///< Character position where the color marker starts
        td::UINT4 length : 24; ///< Number of characters covered by this marker
        td::ColorID colorID; ///< Color identifier to apply in this range
    };

    /// @brief Represents a single parsed token in the code editor.
    class Token
    {
        Token() = delete;
    public:
        /// @brief Error type associated with this token.
        enum class Error : td::BYTE {None=0, CloseParenthesisMismatch};
        const td::String& strTok; ///< String content of the token
        gui::Range range; ///< Character range of the token in the document
        td::UINT4 lineNo; //1 - based ///< Line number (1-based) where the token appears
        td::UINT4 colNo; //1-based ///< Column number (1-based) where the token appears
        td::INT2 index; //index in case of function or reserved label ///< Index into function or reserved label lookup table
        sc::Token token; ///< Script engine token type
        Error error; ///< Error flag for this token

        /// @brief Constructs a Token with full context information.
        /// @param lbl The string content of the token.
        /// @param rangeIn The character range in the document.
        /// @param ln Line number (1-based).
        /// @param col Column number (1-based).
        /// @param ind Index in function/label lookup, or -1 if not applicable.
        /// @param scTokenIn The script engine token type.
        inline Token(const td::String& lbl, const gui::Range& rangeIn, td::UINT4 ln, td::UINT4 col, td::INT2 ind, sc::Token scTokenIn)
        : strTok(lbl)
        , range(rangeIn)
        , lineNo(ln)
        , colNo(col)
        , index(ind)
        , token(scTokenIn)
        {}
    };

    /// @brief Context information for a token at a specific line during parsing.
    class LineToken
    {
        LineToken() = delete;
    public:
        const Token* pCEToken = nullptr; ///< Pointer to the associated code editor token
        //td::UINT4 lineStartPos;
        td::UINT4 inAttrib = 0; ///< Attribute index at this line position
        sc::Token prevTokens[2] = {sc::Token::EndOfLine, sc::Token::EndOfLine}; ///< Previous two tokens for context
        signed char prevIndices[2] = {-1, -1}; ///< Indices corresponding to the previous two tokens
        sc::Location location; ///< Semantic location (model, submodel, etc.)
        td::BYTE generateErroLog; ///< Whether error logging is enabled for this token

//        LineToken(const CodeEdit::Token& tok, td::UINT4 currLineStartPos, sc::Location location, bool generateErr);
        /// @brief Constructs a LineToken with location and error logging flag.
        /// @param location The semantic location of this token.
        /// @param generateErr Whether to generate error log entries.
        LineToken(sc::Location location, bool generateErr);

        /// @brief Sets the current editor token for this line context.
        /// @param tok The token to associate with this line.
        void setToken(const CodeEdit::Token& tok);

        /// @brief Records the last observed token and its index.
        /// @param token The last token type.
        /// @param index The index of the token in the lookup.
        void setLastToken(sc::Token token, char index);
        /// @brief Returns true if the previous token is a base-level token.
        /// @return True if the previous context is a base token.
        bool isPrevBase() const;
    };
    //using VarAndParamMap = std::unordered_map<td::String, td::UINT4>;
    //using VarAndParamSet = std::unordered_set<td::String>;
    friend class mu::LineLoader;
protected:
    sc::Naming* _pNaming = nullptr; ///< Pointer to the naming/symbol registry
    const CodeEdit* _parentEditor = nullptr; ///< Pointer to the parent editor if this is a supplementary editor
    cnt::SafeFullVector<CodeEdit*> _supplemenaryEditors; ///< List of supplementary editor instances
    cnt::SafeFullVector<td::Variant> _headerAttribs;  //header attribs ///< Attribute values for header lines
    cnt::Array<cnt::PushBackVector<td::Variant>,2> _modelAttribs; //model and submodel attribs ///< Attribute values for model and submodel sections
    cnt::Array<Selection,2> _selections; ///< Saved selections for model and submodel
    cnt::Array<sc::Range,2> _locationOfVars; ///< Line ranges occupied by variable declarations
    cnt::Array<sc::Range,2> _locationOfParams; ///< Line ranges occupied by parameter declarations
    cnt::Array<sc::Range,2> _locationOfConsts; ///< Line ranges occupied by constant declarations
    cnt::Array<sc::Range,2> _locationOfDistribs; ///< Line ranges occupied by distribution declarations
    cnt::Array<sc::Range,2> _locationOfStats; ///< Line ranges occupied by state declarations
    cnt::Array<sc::Range,2> _locationOfDataSets; ///< Line ranges occupied by data set declarations
    cnt::Array<sc::Range,2> _locationOfSamplers; ///< Line ranges occupied by sampler declarations

    cnt::Array<sc::Range,2> _locationOfModels; ///< Line ranges for model sections
    cnt::Array<td::INT4,2> _indexOfDeltaT; //dummy ///< Reserved index for delta-T variable (unused)
    cnt::Array<td::INT4,2> _indexOfTime;   //dummy ///< Reserved index for time variable (unused)
    cnt::Array<td::UINT4,2> _numberOfLimits; ///< Number of limit constraints in model/submodel
    cnt::Array<sc::VarAndParamSet,8> _duplicatedVarsAndParams; ///< Sets of duplicated variable and parameter names

    CodeEditorStatusBar* _pStatusBar = nullptr; ///< Pointer to the associated status bar, if any

    sc::VarAndParamMap* _userLabels[(size_t)sc::UserLabel::Last]; ///< User-defined label maps for model scope
    sc::VarAndParamMap* _userLabelsSubModel[(size_t)sc::UserLabel::Last]; ///< User-defined label maps for submodel scope

    sc::ILog* _pLog = nullptr; ///< Pointer to the logging interface
    td::UINT4 _totalErrors = 0; ///< Total number of errors found during parsing
    td::UINT4 _glyphPos = 0;   ///< Current glyph (character) position in the document
    td::UINT4 _lastCountedLines = 1; ///< Last counted number of lines in the editor
    td::UINT2 _version = 1; ///< Document version number
    td::UINT2 _editorPositionInLogger = 0; ///< Index of this editor in the logger's editor list
    td::UINT2 _namingPosition = 0; //which naming position is used in an instance of this class (getSymbolPosition) ///< Position within the naming registry for symbol resolution
    td::INT2 _scopeLevel = 0; ///< Current scope nesting level during parsing

    sc::UserLabels _userLabelsType = sc::UserLabels::Intern; ///< Type of user label scope used

    sc::Type _type = sc::Type::ModelWithGroups; ///< Script model type
    sc::Group _currentGroup = sc::Group::Last; ///< Currently active group during parsing
    sc::Location _reColorSource = sc::Location::Model; ///< Source location that triggered the last re-coloring
    td::BYTE _complexDomain = 0; ///< Flag indicating complex-number domain
    td::BYTE _useLaplaceOperator = 0; ///< Flag indicating Laplace operator mode
    td::BYTE _needsReColor = 0; ///< Flag indicating that syntax coloring needs to be refreshed
    td::BYTE _timeDomainModel = 0;  ///< Flag indicating time-domain model mode

private:
    void checkAndReColorIfNecessary();
    bool checkAndReColorIfNecessary(const Selection& currSel);
    sc::ColorGroup checkForVarOrParam(const td::String& str, sc::Location loc) const;
    void clearDuplicated();
    bool isDuplicated(const td::String& name, sc::Location location) const;
    void addTimeDomainParams(td::MutableString& mStr);
protected:
    /// @brief Re-colors the current line based on syntax rules.
    void colorCurrentLine();
    /// @brief Re-colors the current and preceding line for context correctness.
    void colorTwoLines();
    /// @brief Parses a single line and applies syntax coloring.
    /// @param mStr The line content as a mutable string.
    /// @param lineNo The current line number (updated during parsing).
    /// @param prevGlyphPos Position of the first glyph on this line.
    /// @param generateErroLog Whether to generate error log entries.
    void parseLine(const td::MutableString& mStr, td::UINT4& lineNo, td::UINT4& prevGlyphPos, bool generateErroLog);
    /// @brief Finalizes loading by completing color resolution and optional initial selection.
    /// @param lastLineNo The last line number processed.
    /// @param generateErroLog Whether to generate error log entries.
    /// @param initialSelect Whether to apply an initial selection after loading.
    void finalizeLoading(td::UINT4 lastLineNo, bool generateErroLog, bool initialSelect);

    //overrides
    /// @brief Handles a key press event in the code editor.
    /// @param key The key event data.
    /// @return True if the event was handled.
    bool onKeyPressed(const gui::Key& key) override;
    /// @brief Called when the content of the editor changes.
    /// @param source The source of the change.
    /// @return True if the change was handled.
    bool onChangedContent(Source source) override;
    /// @brief Called when a new line is inserted by the user.
    /// @param nTabsToInsert Number of tabs to auto-insert on the new line.
    /// @return True if the event was handled.
    bool onNewLine(td::UINT2& nTabsToInsert) override;
    /// @brief Called when the text selection changes.
    /// @param sel The new selection.
    /// @return True if the event was handled.
    bool onSelectionChange(const Selection& sel) override;
    /// @brief Called after one or more lines are deleted.
    /// @param range The range of deleted text.
    /// @param nDeletedLines Number of lines deleted.
    void onDeletedLines(const gui::Range& range, td::UINT4 nDeletedLines) override; //after deeleting line(s)
    /// @brief Adjusts a string before it is pasted into the editor.
    /// @param pastingString The original string to paste.
    /// @param mStrOut The adjusted output string.
    void adjustPastingString(const char* pastingString, td::MutableString& mStrOut) override;
    /// @brief Called after a paste operation completes.
    void onPaste() override;
    /// @brief Called after a cut operation completes.
    void onCut() override;
    /// @brief Called after an undo operation completes.
    void onUndo() override;
    /// @brief Called after a redo operation completes.
    void onRedo() override;

    /// @brief Called when the editor font changes.
    void onFontChange() override;
protected:

    /// @brief Returns the user symbol map for a given label type in model scope.
    /// @param userSymbolType The label type to look up.
    /// @return Const pointer to the variable/parameter map.
    const sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType) const;
    /// @brief Returns the user symbol map for a given label type in submodel scope.
    /// @param userSymbolType The label type to look up.
    /// @return Const pointer to the variable/parameter map.
    const sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType) const;
    /// @brief Returns a mutable user symbol map for a given label type in model scope.
    /// @param userSymbolType The label type to look up.
    /// @return Pointer to the variable/parameter map.
    sc::VarAndParamMap* getUserSymbolMap(sc::UserLabel userSymbolType);
    /// @brief Returns a mutable user symbol map for a given label type in submodel scope.
    /// @param userSymbolType The label type to look up.
    /// @return Pointer to the variable/parameter map.
    sc::VarAndParamMap* getUserSymbolSubModelMap(sc::UserLabel userSymbolType);

    //-1: symbol is not available in "symbolType" list
    /// @brief Returns the position of a user label within a specific label type.
    /// @param strLabel The label string to search for.
    /// @param userLabelType The label type category to search in.
    /// @return The position index, or -1 if not found.
    int getUserLabelPosition(const td::String& strLabel, sc::UserLabel userLabelType) const;
    /// @brief Returns the position of a common label within a function set.
    /// @param strLabel The label string to search for.
    /// @param labelType The function set category to search in.
    /// @return The position index, or -1 if not found.
    int getCommonLabelPosition(const td::String& strLabel, sc::FunctionSet labelType) const; //common symbols (for c++, xml, javascript, modelMaker,...)

    /// @brief Returns a reference to the underlying mutable string buffer.
    /// @return Reference to the mutable string.
    td::MutableString& getMutableString() const;

//    virtual bool parseHeaderLine(const td::String& strLine, td::UINT4 headerLineNo, const std::initializer_list<const char*>& headerAttribNames, cnt::PushBackVector<td::Variant>* pAttribValues, const char* supAttName);

    /// @brief Parses variable and parameter declarations using a line loader.
    /// @param lineLoader The line loader providing lines.
    /// @param mStr Mutable string buffer for parsing.
    /// @return True if parsing succeeded.
    bool parseVarsAndParams(mu::LineLoader& lineLoader, td::MutableString& mStr);
    /// @brief Parses variable and parameter declarations from a string starting at a given position.
    /// @param strAll The full document string.
    /// @param mStr Mutable string buffer.
    /// @param fromPos Starting character position.
    /// @return True if parsing succeeded.
    virtual bool parseVarsAndParams(const td::String& strAll, td::MutableString& mStr, td::UINT4 fromPos);
    /// @brief Parses variable and parameter declarations from a file stream.
    /// @param f The input file stream.
    /// @param mStr Mutable string buffer.
    /// @param fromPos Starting character position.
    /// @return True if parsing succeeded.
    virtual bool parseVarsAndParams(std::ifstream& f, td::MutableString& mStr, td::UINT4 fromPos);
    /// @brief Parses variable and parameter declarations from the current text content.
    /// @param mStr Mutable string buffer.
    /// @return True if parsing succeeded.
    virtual bool parseVarsAndParams(td::MutableString& mStr); //from TextEdit

    /// @brief Determines the color group for a token at the current line context.
    /// @param lt The line token context.
    /// @return The color group to apply.
    virtual sc::ColorGroup getColorGroup(LineToken& lt);
//    td::ColorID getColor(ColorGroup) const;

    /// @brief Tokenizes a line and calls the callback for each token found.
    /// @param strLine The line content string.
    /// @param lineNo The line number (1-based).
    /// @param glyphPos Starting glyph position of the line.
    /// @param domain The scripting domain context.
    /// @param onToken Callback invoked for each parsed token.
    void tokenizeLine(const td::String& strLine, td::UINT4 lineNo, td::UINT4 glyphPos, sc::Domain domain, const std::function<void(const Token&)>& onToken);
    /// @brief Clears all currently applied color markers.
    void resetColorMarkers();
    /// @brief Appends a color marker to the pending list.
    /// @param cm The color marker to append.
    void appendColorMarker(ColorMarker& cm);
    /// @brief Applies all pending color markers up to the specified line position.
    /// @param lastLinePos The last line position to apply markers to.
    /// @param bLock Whether to lock the text storage during the operation.
    void applyColorMarkers(td::UINT4 lastLinePos, bool bLock = true);

    /// @brief Returns the semantic location for a given line number.
    /// @param lineNo The line number to query.
    /// @return The semantic location (model, submodel, vars, etc.).
    sc::Location getLocation(td::UINT4 lineNo) const;
    /// @brief Returns true if the given line is inside a model body.
    /// @param lineNo The line number to test.
    /// @return True if within a model body.
    bool isInModel(td::UINT4 lineNo) const;
    /// @brief Returns true if the given line is on a model declaration line.
    /// @param lineNo The line number to test.
    /// @return True if on a model declaration.
    bool isOnModel(td::UINT4 lineNo) const;
    /// @brief Returns true if the given line is inside a submodel body.
    /// @param lineNo The line number to test.
    /// @return True if within a submodel body.
    bool isInSubModel(td::UINT4 lineNo) const;
    /// @brief Returns true if the given location is within a submodel.
    /// @param location The semantic location to test.
    /// @return True if within a submodel.
    bool isInSubModel(sc::Location location) const;
    /// @brief Returns true if the given location is in a variables or parameters section.
    /// @param location The semantic location to test.
    /// @return True if within a vars/params section.
    bool isInVarsOrParams(sc::Location location) const;
    /// @brief Resets all tracked variable and parameter location ranges.
    void resetVarsAndParamsLocations();

    /// @brief Marks that re-coloring is needed starting from the given source location.
    /// @param sourceLoc The location that triggered the need for re-coloring.
    void needsReColor(sc::Location sourceLoc);
    /// @brief Clears the re-color indicator flag.
    void resetReColorIndicator();
    /// @brief Increments variable and parameter location ranges by a given value.
    /// @param location The semantic location section to adjust.
    /// @param incrementVal The amount to increment the ranges.
    void incrementVarsAndParamsLocations(sc::Location location, td::INT4 incrementVal);
    /// @brief Increments variable and parameter location ranges relative to a line number.
    /// @param currLineNo The current line number reference point.
    /// @param incrementVal The amount to increment the ranges.
    void incrementVarsAndParamsLocations(td::UINT4 currLineNo, td::INT4 incrementVal);

//    bool onSelectionChange(const Selection& sel) override;

//    td::UINT4 loadHeaders(std::ifstream& f, td::UINT4 numberOfHeaderLines, const std::initializer_list<const char*>& headerAttribNames, cnt::PushBackVector<td::Variant>* pAttribValues, const char* supAttName, td::MutableString* pSupplementaryText);

    /// @brief Loads content from a file stream starting at the given position.
    /// @param f The input file stream.
    /// @param pos Starting byte position in the file.
    /// @return True if loading succeeded.
    bool loadFromFile(std::ifstream& f, td::UINT4 pos);

    CodeEdit() = delete;
    CodeEdit(const CodeEdit&) = delete;
    CodeEdit& operator =(const CodeEdit&) = delete;
public:
    /// @brief Constructs a CodeEdit with specified type, supplementary editors, naming position, and label type.
    /// @param type The script model type.
    /// @param noOfSupplementaryEditors Number of supplementary editor slots.
    /// @param namingPosition Index into the naming registry for symbol resolution.
    /// @param userLabelsType The scope type for user label maps.
    CodeEdit(sc::Type type, td::UINT4 noOfSupplementaryEditors, td::UINT2 namingPosition, sc::UserLabels userLabelsType);

    /// @brief Attaches a naming registry to this editor.
    /// @param pNaming Pointer to the Naming registry.
    void setNaming(sc::Naming* pNaming);

    /// @brief Sets the document version number.
    /// @param version The version number to assign.
    void setVersionNumber(td::UINT2 version);
    /// @brief Returns the document version number.
    /// @return The current version number.
    td::UINT2 getVersionNumber() const;

    /// @brief Assigns a supplementary editor at the given index.
    /// @param pos The index position.
    /// @param supEditor Pointer to the supplementary CodeEdit.
    void setSupplementaryEditor(td::UINT4 pos, CodeEdit* supEditor);
    /// @brief Returns the supplementary editor at the given index.
    /// @param pos The index position.
    /// @return Pointer to the supplementary editor.
    CodeEdit* getSupplementaryEditor(td::UINT4 pos);
    /// @brief Sets the parent editor for this supplementary editor.
    /// @param parentEditor Pointer to the parent CodeEdit.
    void setParentEditor(const CodeEdit* parentEditor);
    /// @brief Returns the parent editor of this editor.
    /// @return Const pointer to the parent CodeEdit, or nullptr.
    const CodeEdit* getParentEditor() const;

    /// @brief Returns the set of duplicated variable and parameter names.
    /// @return Reference to the duplicated vars/params array.
    cnt::Array<sc::VarAndParamSet,8>&  getDuplicatedVarsAndParams();
    //Model attribs (if there is Model group)
    /// @brief Returns header attribute values.
    /// @return Reference to the header attribs vector.
    cnt::SafeFullVector<td::Variant>& getHeaderAttribs();
    /// @brief Returns header attribute values (const overload).
    /// @return Const reference to the header attribs vector.
    const cnt::SafeFullVector<td::Variant>& getHeaderAttribs() const;
    /// @brief Returns model attribute values.
    /// @return Const reference to the model attribs vector.
    const cnt::PushBackVector<td::Variant>& getModelAttribs();


    /// @brief Attaches a logger to this editor.
    /// @param pLogger Pointer to the ILog interface.
    void setLogger(sc::ILog* pLogger);
    /// @brief Returns the attached logger.
    /// @return Pointer to the ILog interface.
    sc::ILog* getLogger();

    /// @brief Returns whether the editor operates in the complex number domain.
    /// @return True if complex domain is active.
    bool isComplexDomain() const;

    /// @brief Returns the current scripting domain (real or complex).
    /// @return The current Domain enum value.
    sc::Domain getDomain() const;

    /// @brief Returns the user labels scope type.
    /// @return The UserLabels enum value.
    sc::UserLabels getUserLabelsType() const;

    /// @brief Returns the user label map for the given label type.
    /// @param lbl The label type to look up.
    /// @return Pointer to the VarAndParamMap.
    sc::VarAndParamMap* getUserLabels(sc::UserLabel lbl);
    /// @brief Sets the user label map for the given label type.
    /// @param lbl The label type.
    /// @param map Pointer to the VarAndParamMap to assign.
    void setUserLabels(sc::UserLabel lbl, sc::VarAndParamMap*);

    /// @brief Sets all user label maps for model scope.
    /// @param userLabels Array of pointers to VarAndParamMap for each label type.
    void setUserLabels(sc::VarAndParamMap* userLabels[(size_t)sc::UserLabel::Last]);
    /// @brief Sets all user label maps for submodel scope.
    /// @param userSubmodelLabels Array of pointers to VarAndParamMap for each label type.
    void setUserSubModelLabels(sc::VarAndParamMap* userSubmodelLabels[(size_t)sc::UserLabel::Last]);

    /// @brief Sets the starting glyph position for line tracking.
    /// @param glyphPos The glyph position to set.
    void setGlyphPos(td::UINT4 glyphPos);
    /// @brief Enables or disables complex domain mode.
    /// @param bComplex True to enable complex domain, false for real domain.
    void setComplexDomain(bool bComplex);

    /// @brief Loads the editor content from a string.
    /// @param strAll The full document string.
    /// @return True if loading succeeded.
    bool loadFromString(const td::String& strAll);

    /// @brief Loads the editor content from a file.
    /// @param fileName Path to the file to load.
    /// @return True if loading succeeded.
    bool loadFromFile(const td::String& fileName);
//    bool loadFromFile(const fo::fs::path& filePath, td::UINT4 numberOfHeaderLines=0, const std::span<const char* const>& headerAttribNames={}, const char* supAttName = nullptr, td::MutableString* pSupplementaryText = nullptr);

    /// @brief Writes the editor text content to an output file stream.
    /// @param f The output file stream.
    void saveToFile(std::ofstream& f) const; //saves only text to current position in ofstream

    /// @brief Returns the current line number in the editor.
    /// @return The current line number (0 if line tracking is inactive).
    td::UINT4 getCurrentLineNo() const; //0-if there is no line tracking

    /// @brief Returns the current text selection.
    /// @return Const reference to the current Selection.
    const Selection& getCurrentSelection() const;
    /// @brief Returns the previous text selection.
    /// @return Const reference to the previous Selection.
    const Selection& getPreviousSelection() const;

    /// @brief Attaches a status bar to this editor.
    /// @param pStatusBar Pointer to the CodeEditorStatusBar.
    void setStatusBar(CodeEditorStatusBar* pStatusBar);

    /// @brief Triggers a re-coloring pass on the entire document.
    /// @param bForce If true, forces a full re-color even if not needed.
    void reColor(bool bForce = false);

    /// @brief Adds a log entry using the attached logger.
    /// @param le The log entry data.
    /// @param type The type (error, warning, info) of the log entry.
    void addLogEntry(const sc::ILog::Entry& le, sc::ILog::Type type = sc::ILog::Type::Error);

    /// @brief Returns whether the editor is in time-domain mode.
    /// @return True if time-domain model is active.
    bool isTimeDomain() const;

    /// @brief Returns the attached naming registry.
    /// @return Pointer to the Naming registry.
    sc::Naming* getNaming();

    /// @brief Sets the editor's position index in the associated logger.
    /// @param posInLogger The logger position index.
    void setPositionInLogger(td::UINT2 posInLogger);

    //void detectTimeDomain();

    /// @brief Sets the color used for a given color group in light and dark modes.
    /// @param colorGroup The color group to configure.
    /// @param colorForLightDesktop The color ID to use in light mode.
    /// @param colorForDarkDesktop The color ID to use in dark mode.
    static void setColorForGroup(sc::ColorGroup colorGroup, td::ColorID colorForLightDesktop, td::ColorID colorForDarkDesktop);

    /// @brief Resets all color groups to their default values.
    static void setDefaultColorGroups();

    /// @brief Switches the color scheme between dark and light mode.
    /// @param bDarkMode True for dark mode, false for light mode.
    static void setColorMode(bool bDarkMode);
};

} //namespace gui
