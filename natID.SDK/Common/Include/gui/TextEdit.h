// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TextEdit.h
    @brief Multi-line rich-text editor control with find/replace and optional code-editing features. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "TextCtrl.h"
#include <td/ColorID.h>
#include <gui/TextAttribs.h>
#include <cnt/PushBackVector.h>
#include <td/MutableString.h>
#include <cnt/SafeFullVector.h>
#include <gui/IFindReplace.h>
#include <mu/LineLoader.h>

namespace gui
{

class TextEditHelper;
class ILineNumbers;

/// @brief A multi-line text editor with rich-text formatting, tab stops, and optional code-editing support.
class NATGUI_API TextEdit : public TextCtrl, public mu::ILineLoader
{
public:
    /// @brief Standard clipboard shortcut operations.
    enum class ShortCut : unsigned char
    {
        Copy=0, ///< Copy selection to clipboard.
        Cut,    ///< Cut selection to clipboard.
        Paste   ///< Paste from clipboard.
    };

    /// @brief Controls whether the editor shows a horizontal scroll bar.
    enum class HorizontalScroll : unsigned char
    {
        No =0, ///< No horizontal scrolling.
        Yes    ///< Horizontal scrolling enabled.
    };

    /// @brief Controls whether text-change events are sent to the parent.
    enum class Events : unsigned char
    {
        DoNotSend = 0, ///< Change events are suppressed.
        Send           ///< Change events are sent.
    };

    /// @brief Identifies the source of a text-content change.
    enum class Source : unsigned char
    {
        Intern=0,         ///< Change originated internally (programmatic).
        Keyboard,         ///< Change caused by regular keyboard input.
        KeyboardNL,       ///< Change caused by a newline key press.
        KeyboardDEL,      ///< Change caused by the Delete key.
        KeyboardBackSpace, ///< Change caused by the Backspace key.
        Paste             ///< Change caused by a paste operation.
    };

    /// @brief Defines a tab-stop rule with a starting position, step count, and step size.
    using TabStop = struct TS
    {
        int start;   ///< Starting character position of the tab stop.
        int nSteps;  ///< Number of steps before the pattern repeats.
        int delta;   ///< Size of each step in characters.
    };

    /// @brief Describes the current selection state including line and column numbers.
    using Selection = struct Sel
    {
        gui::Range selection;                ///< Glyph range of the current selection.
        td::UINT4 lineNo;                    ///< One-based line number of the selection start.
        td::UINT4 lineStartPosInGlyphs;      ///< Glyph offset of the start of the current line.

        /// @brief Computes the one-based column number of the selection start.
        /// @return Column number (1-based).
        td::UINT4 getColNo() const
        {
            assert(lineStartPosInGlyphs <= selection.location);
            return selection.location - lineStartPosInGlyphs + 1; //1 based
        }
    };

protected:
    ILineNumbers* _pLineNumbers = nullptr; ///< Optional line-number gutter companion.
    cnt::SafeFullVector<TabStop> _tabStops; ///< Active tab-stop rules.
private:
    std::function<void(const Selection&)> _onChangedSelection; ///< Callback for selection-change events.
    std::function<void()> _onModified; ///< Callback for content-modification events.
protected:
//    td::UINT4 _modifiedUserEventID = 0;
    Source _source = Source::Intern; ///< Source of the most recent content change.
    td::BYTE _codeEditing = 0; ///< Non-zero when code-editing mode is active.
    td::BYTE _sendLineAndColPos = 0; ///< Non-zero when line/column position events are enabled.
    td::BYTE _useMarkupMap = 0; ///< Non-zero when a markup replacement map is in use.
private:
    td::BYTE _modified = 0; ///< Non-zero when the content has been modified since last reset.
private:
    friend class TextEditHelper;
    /// @brief Sends a selection-change message to registered listeners.
    /// @param pSel Pointer to the selection descriptor, or nullptr.
    void sendSelChangeMessage(const Selection* pSel);

    /// @brief Sends a content-modified message to registered listeners.
    void sendModifiedMessage();

    /// @brief Sets the source of the next content change.
    /// @param source Source identifier.
    void setSource(Source source);

    /// @brief Returns the source of the most recent content change.
    /// @return Source identifier.
    TextEdit::Source getSource() const;

protected:
    static td::MutableString _mStrLine; //for obtaining lines from CodeEdit ///< Shared mutable buffer used when reading lines from a CodeEdit.

    /// @brief Measures the preferred size of this text editor.
    /// @param[out] cell Cell information to fill.
    void measure(CellInfo&) override;

    /// @brief Re-measures the editor after a layout change.
    /// @param[out] cell Cell information to update.
    void reMeasure(CellInfo&) override;

    /// @brief Applies the current tab-stop rules to the native control.
    void updateTabStops();

    /// @brief Handles key-press events.
    /// @param key Description of the pressed key.
    /// @return True if the key event was consumed.
    bool onKeyPressed(const gui::Key& key) override;

    /// @brief Called when the selection changes.
    /// @param sel New selection descriptor.
    /// @return True if the event was handled.
    virtual bool onSelectionChange(const Selection& sel);

    /// @brief Called when the text content changes.
    /// @param source Identifies the cause of the change.
    /// @return True if the event was handled.
    virtual bool onChangedContent(Source source);

    /// @brief Called when the user presses Enter to insert a new line.
    /// @param[out] nTabsToInsert Receives the number of leading tabs to insert on the new line.
    /// @return True if the event was handled.
    virtual bool onNewLine(td::UINT2& nTabsToInsert);

    /// @brief Called after one or more lines have been deleted.
    /// @param range Glyph range that was deleted.
    /// @param nDeletedLines Number of lines removed.
    virtual void onDeletedLines(const gui::Range& range, td::UINT4 nDeletedLines); //after deeleting lines

    /// @brief Called after a cut operation completes.
    virtual void onCut();

    /// @brief Allows subclasses to transform pasted text before insertion.
    /// @param pastingString The raw text being pasted.
    /// @param[out] mStrOut Receives the adjusted text to insert.
    virtual void adjustPastingString(const char* pastingString, td::MutableString& mStrOut);

    /// @brief Called after a paste operation completes.
    virtual void onPaste();

    /// @brief Called after an undo operation completes.
    virtual void onUndo();

    /// @brief Called after a redo operation completes.
    virtual void onRedo();

    /// @brief Called when the editor font changes.
    virtual void onFontChange();

    /// @brief Called when the find/replace panel is requested.
    /// @return True if the event was handled.
    virtual bool onFindReplace();

    /// @brief Enables or disables delete-lines events.
    /// @param bEnable True to enable, false to disable.
    void enableDeleteLinesEvents(bool bEnable);

    /// @brief Counts the number of lines up to the given maximum.
    /// @param maxNo Upper bound for the count.
    /// @return Number of lines, capped at maxNo+1 if the actual count exceeds maxNo.
    td::UINT4 countLinesUpTo(td::UINT4 maxNo) const; //returns <= maxNo if (noLines <= maxNo) oherwise maxNo+1

    /// @brief Internal constructor used by code-editing subclasses.
    /// @param codeEdit Non-zero to activate code-editing mode.
    /// @param useMarkupMap Non-zero to activate markup replacement.
    explicit TextEdit(td::BYTE codeEdit, td::BYTE useMarkupMap);
    TextEdit(const TextEdit&) = delete;
    TextEdit& operator =(const TextEdit&) = delete;
public:
    /// @brief Constructs a standard TextEdit control.
    /// @param useHorizontalScrolls Whether to enable horizontal scrolling.
    /// @param sendEvents Whether to send content-changed events.
    /// @param readOnly If true, the control starts in read-only mode.
    explicit TextEdit(TextEdit::HorizontalScroll useHorizontalScrolls = HorizontalScroll::No, TextEdit::Events sendEvents = TextEdit::Events::DoNotSend, bool readOnly = false);

    /// @brief Enables or disables all clipboard keyboard shortcuts.
    /// @param bEnable True to enable shortcuts.
    void enableAllClipBoardShortCuts(bool bEnable) const;

    /// @brief Enables or disables undo/redo functionality.
    /// @param bEnable True to enable undo/redo.
    void enableUndoRedo(bool bEnable) const;

    /// @brief Returns the associated line-number interface, if any.
    /// @return Pointer to ILineNumbers, or nullptr.
    ILineNumbers* getILineNumbers();

    /// @brief Sets the line-number companion for this editor.
    /// @param pLN Pointer to the ILineNumbers implementation.
    void setLineNumbers(ILineNumbers* pLN);

    /// @brief Inserts a C-string at the current selection position.
    /// @param str Null-terminated string to insert.
    void insertStringAtSelection(const char* str);

    /// @brief Inserts a String at the current selection position.
    /// @param str String to insert.
    void insertStringAtSelection(const td::String& str);

    /// @brief Inserts a C-string at the specified glyph position.
    /// @param position Zero-based glyph position at which to insert.
    /// @param txt Null-terminated string to insert.
    /// @param tmpLock If true, temporarily locks the document during insertion.
    void insertString(td::UINT4 position, const char* txt, bool tmpLock = false);

    /// @brief Replaces the text within the specified range with a C-string.
    /// @param range Glyph range to replace.
    /// @param str Replacement string.
    /// @param tmpLock If true, temporarily locks the document during replacement.
    void replaceString(const gui::Range& range, const char* str, bool tmpLock = false);

    /// @brief Replaces the text within the specified range with a String.
    /// @param range Glyph range to replace.
    /// @param str Replacement string.
    /// @param tmpLock If true, temporarily locks the document during replacement.
    void replaceString(const gui::Range& range, const td::String& str, bool tmpLock = false);

    /// @brief Appends a C-string to the end of the document.
    /// @param str Null-terminated string to append.
    void appendString(const char* str);

    /// @brief Appends a String to the end of the document.
    /// @param str String to append.
    void appendString(const td::String& str);

    /// @brief Replaces the entire content with HTML-formatted text.
    /// @param strHTML HTML string to load.
    /// @param pSizeToFitInWidth Optional size constraint for word-wrap calculation.
    void setHTML(const td::String& strHTML, Size* pSizeToFitInWidth = nullptr);

    /// @brief Replaces the entire content with plain text.
    /// @param strText Plain text to load.
    /// @param pSizeToFitInWidth Optional size constraint for word-wrap calculation.
    void setText(const td::String& strText, Size* pSizeToFitInWidth = nullptr);

    /// @brief Returns the entire content as a plain-text string.
    /// @return Plain-text string.
    td::String getText() const;

    /// @brief Returns the entire content as an HTML string.
    /// @param addBodyTag If true, wraps the output in a full HTML body tag.
    /// @return HTML string.
    td::String getHTML(bool addBodyTag) const;


    /// @brief Sets the font name for the entire document.
    /// @param fontName Null-terminated font name.
    void setFontName(const char* fontName);

    /// @brief Sets the font name for the entire document.
    /// @param fontName Font name string.
    void setFontName(const td::String& fontName);

    /// @brief Sets the font size in points.
    /// @param fontSize Desired font size.
    void setFontSize(float fontSize);

    /// @brief Returns the current font size in points.
    /// @return Font size.
    float getFontSize() const;

    /// @brief Returns the current selection as a glyph range.
    /// @param[out] range Receives the current selection range.
    void getSelection(gui::Range& range) const;

    /// @brief Sets the current selection to the specified glyph range.
    /// @param range Glyph range to select.
    /// @param bMakeVisibleCurrentSelection If true, scrolls the selection into view.
    void setSelection(gui::Range& range, bool bMakeVisibleCurrentSelection);

    /// @brief Applies or removes a font style to the specified range.
    /// @param range Glyph range to style.
    /// @param style Font style to apply.
    /// @param bSet True to apply, false to remove the style.
    void setStyle(const gui::Range& range, gui::Font::Style style, bool bSet = true); //if bSet == false -> reset markup
//    void getStyle(const gui::Range& range, gui::Font::Style& style) const;

    /// @brief Applies a markup to the specified glyph range.
    /// @param range Glyph range.
    /// @param markup Markup to apply.
    void setMarkup(const gui::Range& range, gui::Font::Markup markup);

    /// @brief Removes a markup from the specified glyph range.
    /// @param range Glyph range.
    /// @param markup Markup to remove.
    void removeMarkup(const gui::Range& range, gui::Font::Markup markup);

    /// @brief Returns text attributes at the specified glyph position.
    /// @param position Zero-based glyph position.
    /// @param[out] attribs Receives the attributes at that position.
    void getTextAttribs(td::UINT4 position, gui::TextAttribs& attribs) const;

    /// @brief Returns all styled ranges in the document.
    /// @param[out] allAttribs Vector that receives all styled ranges.
    void getAllTextAttribs(cnt::PushBackVector<gui::TextAttribsRange>& allAttribs) const;

    /// @brief Applies a color to the specified glyph range.
    /// @param range Glyph range.
    /// @param colorID Color to apply.
    void setColor(const gui::Range& range, td::ColorID colorID);

    /// @brief Removes color formatting from the specified glyph range.
    /// @param range Glyph range from which to remove the color.
    void removeColor(const gui::Range& range);
//    td::ColorID getColor(const gui::Range& range) const;

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::TextEdit.
    gui::ObjType getObjType() const override { return ObjType::TextEdit;}

    /// @brief Returns whether the document is empty.
    /// @return True if the document contains no glyphs.
    bool isEmpty() const;

    /// @brief Returns the total number of glyphs in the document.
    /// @return Glyph count.
    size_t getNumberOfGlyphs() const;

//    const std::function<void(const TextEdit::Selection&)>& getChangedSelectionHandler() const;
    /// @brief Registers a callback invoked when the selection changes.
    /// @param fnToCall Function receiving the new selection descriptor.
    void onChangedSelection(const std::function<void(const TextEdit::Selection&)>& fnToCall);

//    const std::function<void()>& getModifiedHandler() const;
    /// @brief Registers a callback invoked when the document content is modified.
    /// @param fnToCall Function to call on modification.
    void onModified(const std::function<void()>& fnToCall);

    /// @brief Sets tab-stop rules for the editor.
    /// @param tabStops Pointer to an array of TabStop definitions.
    /// @param nTabStopDefinitions Number of definitions in the array.
    void setTabStops(const TabStop* tabStops, size_t nTabStopDefinitions = 1);

    /// @brief Returns the current set of tab-stop definitions.
    /// @return Const reference to the tab-stop vector.
    const cnt::SafeFullVector<TabStop>& getTabStops() const;

    /// @brief Activates the line-highlight manager for syntax or focus highlighting.
    void useLineHighlightManager();

    /// @brief Returns the total number of lines in the document.
    /// @return Line count.
    td::UINT4 getTotalNumberOfLines() const;

    //CoordType getTotalHeigthInLogicalPixels() const;
    /// @brief Returns the average character width in logical pixels.
    /// @return Average character width.
    CoordType getAverageCharWidth() const;

    /// @brief Returns whether this editor is in code-editing mode.
    /// @return True if code-editing mode is active.
    bool isCodeEdit() const;

    /// @brief Enables or disables sending of line-and-column position events.
    /// @param bSend True to enable.
    void sendLineAndColPos(bool bSend);

    /// @brief Returns whether line-and-column position events are enabled.
    /// @return True if enabled.
    bool isSendLineAndColPos() const;

    /// @brief Returns whether the markup replacement map is active.
    /// @return True if a markup map is in use.
    bool useMarkupMap() const;

    /// @brief Returns the glyph position of the start of the specified line.
    /// @param lineNo One-based line number.
    /// @return Glyph position of the line start.
    td::UINT4 getLinePos(td::UINT4 lineNo) const; //returns position in glyps

    /// @brief Reads the content of the specified line into a mutable string.
    /// @param[out] strOut Receives the line content.
    /// @param lineNo One-based line number to read.
    /// @param prevLinePos Glyph position of the previous line start (0 for sequential access).
    /// @return Glyph position of the start of the next line, or 0 if this was the last line.
    td::UINT4 getLine(td::MutableString& strOut, td::UINT4 lineNo, td::UINT4 prevLinePos=0) const;

    //returns position of next line in glyphs (ultra fast for sequential read), 0 if we read last line
    /// @brief Reads the line starting at the given glyph position (fast sequential access).
    /// @param[out] strOut Receives the line content.
    /// @param lineHeadPos Glyph position of the line start.
    /// @return Glyph position of the start of the next line, or 0 if this was the last line.
    td::UINT4 getLineContentAtPosition(td::MutableString& strOut, td::UINT4 lineHeadPos) const override final;

    /// @brief Returns the number of leading occurrences of a character at the start of a line.
    /// @param lineHeadPos Glyph position of the line start.
    /// @param ch Character to count.
    /// @return Count of leading characters.
    td::UINT4 getNumberOfLeadingChars(td::UINT4 lineHeadPos, char ch) const;

    //returns height of the previous line (begin of the first visible line). 0->no line is visible
    //        first visible lineNo (1-based). If zero -> there is no any visible line
    //        output: startingPos of the next line. 0 -> there is no next line
    /// @brief Returns information about the first currently visible line.
    /// @param[out] lineNo One-based line number of the first visible line (0 if none visible).
    /// @param[out] startingPos Glyph position of the start of the next line after the first visible one.
    /// @return Height of the line preceding the first visible line (0 if no line is visible).
    gui::CoordType getFirstVisibleLine(td::UINT4& lineNo, td::UINT4& startingPos) const;

    //returns line height of line which starts at startingPos
    //on return: linePos contains starting position of the next line. 0: there is no next line
    /// @brief Returns the height of the line starting at the given glyph position.
    /// @param[in,out] startingPos On input, the glyph start of the line; on output, the start of the next line (0 if none).
    /// @return Height of the line in logical pixels.
    gui::CoordType getLineHeight(td::UINT4& startingPos) const;

    //this is minimum lineHeight (for empty line)
    /// @brief Returns the minimum line height (for an empty line).
    /// @return Minimum line height in logical pixels.
    CoordType getLineHeight() const;

    /// @brief Searches for the first occurrence of a string from the given position.
    /// @param txtToFind String to search for.
    /// @param startingPos Glyph position from which to start searching.
    /// @return Glyph position of the match, or -1 if not found.
    int find(const td::String& txtToFind, td::UINT4 startingPos) const; //-1 if not found, otherwise position

    /// @brief Replaces occurrences of a search string within the document.
    /// @param txtToFind String to search for.
    /// @param textToReplace Replacement string.
    /// @param startingPos Glyph position from which to start.
    /// @param replaceAll If true, replaces all occurrences; otherwise replaces only the first.
    /// @return Number of replacements made (replaceAll), or glyph position of the replaced string, or -1 if none replaced.
    int replace(const td::String& txtToFind, const td::String& textToReplace, td::UINT4 startingPos, bool replaceAll); //if replaceAll: returns number of replaced strings, otherwise -1 if nothing replaced or position of replaced string

    /// @brief Scrolls the editor to make the specified line visible.
    /// @param lineNo One-based line number to navigate to.
    /// @return True if the operation succeeded.
    bool goToLine(td::UINT4 lineNo); //lineNo must be 1 - based

    /// @brief Sets the modified flag to the given value.
    /// @param modified Non-zero value to mark the document as modified.
    void setModifiedFlag(td::BYTE modified);

    /// @brief Clears the modified flag.
    void resetModifiedFlag();

    /// @brief Returns whether the document has been modified since the last flag reset.
    /// @return True if modified.
    bool isModified() const;

    //to speed up updating the content
    /// @brief Begins a batch content-update, suppressing intermediate redraws.
    void beginUpdate();

    /// @brief Ends a batch content-update and triggers a redraw.
    void endUpdate();

    /// @brief Clears all content from the editor.
    void clean();

    //static methods
    /// @brief Returns the active find/replace interface, if any.
    /// @return Pointer to the IFindReplace interface.
    static IFindReplace* getFindReplace();

    /// @brief Sets the behaviour of the find/replace panel when it becomes inactive.
    /// @param mode Desired inactive mode.
    static void setFindReplaceInactiveMode(IFindReplace::InactiveMode mode);

    /// @brief Loads a markup replacement map from a file.
    /// @param fileName Path to the markup map file.
    /// @return True if the file was loaded successfully.
    static bool loadMarkupMap(const td::String& fileName);

    /// @brief Looks up a replacement string for the given markup key.
    /// @param pMarkup Pointer to the markup key string.
    /// @param[out] replacement Receives the replacement string.
    /// @param markupLen Length of the markup key (-1 to use null-termination).
    /// @return True if a replacement was found.
    static bool getMarkupReplacement(const char* pMarkup, td::String& replacement, int markupLen = -1);
};

} //namespace gui
