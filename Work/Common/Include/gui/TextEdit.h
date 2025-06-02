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

class NATGUI_API TextEdit : public TextCtrl, public mu::ILineLoader
{
public:
    enum class ShortCut : unsigned char {Copy=0, Cut, Paste};
    
    enum class HorizontalScroll : unsigned char {No =0, Yes};
    enum class Events : unsigned char {DoNotSend = 0, Send};
    enum class Source : unsigned char {Intern=0, Keyboard, KeyboardNL, KeyboardDEL, KeyboardBackSpace, Paste};

    using TabStop = struct TS
    {
        int start;
        int nSteps;
        int delta;
    };
    
    using Selection = struct Sel
    {
        gui::Range selection;
        td::UINT4 lineNo;
        td::UINT4 lineStartPosInGlyphs;
        td::UINT4 getColNo() const
        {
            assert(lineStartPosInGlyphs <= selection.location);
            return selection.location - lineStartPosInGlyphs + 1; //1 based
        }
    };
    
protected:
    ILineNumbers* _pLineNumbers = nullptr;
    cnt::SafeFullVector<TabStop> _tabStops;
private:
    std::function<void(const Selection&)> _onChangedSelection;
    std::function<void()> _onModified;
protected:
//    td::UINT4 _modifiedUserEventID = 0;
    Source _source = Source::Intern;
    td::BYTE _codeEditing = 0;
    td::BYTE _sendLineAndColPos = 0;
    td::BYTE _useMarkupMap = 0;
private:
    td::BYTE _modified = 0;
private:
    friend class TextEditHelper;
    void sendSelChangeMessage(const Selection* pSel);
    void sendModifiedMessage();
    
    void setSource(Source source);
    TextEdit::Source getSource() const;
    
protected:
    static td::MutableString _mStrLine; //for obtaining lines from CodeEdit
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    void updateTabStops();
    bool onKeyPressed(const gui::Key& key) override;
    virtual bool onSelectionChange(const Selection& sel);
    virtual bool onChangedContent(Source source);
    virtual bool onNewLine(td::UINT2& nTabsToInsert);
//    virtual void onDeleteLines(const gui::Range& range, int nLinesToBeDeleted); //before deleting lines
    virtual void onDeletedLines(const gui::Range& range, td::UINT4 nDeletedLines); //after deeleting lines
    virtual void onCut();
    virtual void adjustPastingString(const char* pastingString, td::MutableString& mStrOut);
    virtual void onPaste();
    virtual void onUndo();
    virtual void onRedo();
    virtual void onFontChange();
    virtual bool onFindReplace();
    
    void enableDeleteLinesEvents(bool bEnable);
    td::UINT4 countLinesUpTo(td::UINT4 maxNo) const; //returns <= maxNo if (noLines <= maxNo) oherwise maxNo+1
    explicit TextEdit(td::BYTE codeEdit, td::BYTE useMarkupMap);
    
public:
    explicit TextEdit(TextEdit::HorizontalScroll useHorizontalScrolls = HorizontalScroll::No, TextEdit::Events sendEvents = TextEdit::Events::DoNotSend, bool readOnly = false);
    //~TextEdit();
    
    void enableAllClipBoardShortCuts(bool bEnable) const;
    
    void enableUndoRedo(bool bEnable) const;
    
    ILineNumbers* getILineNumbers();
    
    void setLineNumbers(ILineNumbers* pLN);
    
    void insertStringAtSelection(const char* str);
    void insertStringAtSelection(const td::String& str);
    void insertString(td::UINT4 position, const char* txt, bool tmpLock = false);
    
    void replaceString(const gui::Range& range, const char* str, bool tmpLock = false);
    void replaceString(const gui::Range& range, const td::String& str, bool tmpLock = false);

    void appendString(const char* str);
    void appendString(const td::String& str);
    
    void setHTML(const td::String& strHTML, Size* pSizeToFitInWidth = nullptr);
    void setText(const td::String& strText, Size* pSizeToFitInWidth = nullptr);
    
    td::String getText() const;
    td::String getHTML(bool addBodyTag) const;

    
    void setFontName(const char* fontName);
    void setFontName(const td::String& fontName);
    void setFontSize(float fontSize);
    float getFontSize() const;
    
    void getSelection(gui::Range& range) const;
    void setSelection(gui::Range& range, bool bMakeVisibleCurrentSelection);
    void setStyle(const gui::Range& range, gui::Font::Style style, bool bSet = true); //if bSet == false -> reset markup
//    void getStyle(const gui::Range& range, gui::Font::Style& style) const;
    
    void setMarkup(const gui::Range& range, gui::Font::Markup markup);
    void removeMarkup(const gui::Range& range, gui::Font::Markup markup);
    
    void getTextAttribs(td::UINT4 position, gui::TextAttribs& attribs) const;
    void getAllTextAttribs(cnt::PushBackVector<gui::TextAttribsRange>& allAttribs) const;
    
    void setColor(const gui::Range& range, td::ColorID colorID);
    void removeColor(const gui::Range& range);
//    td::ColorID getColor(const gui::Range& range) const;
    
    gui::ObjType getObjType() const override { return ObjType::TextEdit;}
    bool isEmpty() const;
    size_t getNumberOfGlyphs() const;

//    const std::function<void(const TextEdit::Selection&)>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void(const TextEdit::Selection&)>& fnToCall);
    
//    const std::function<void()>& getModifiedHandler() const;
    void onModified(const std::function<void()>& fnToCall);
    
    void setTabStops(const TabStop* tabStops, size_t nTabStopDefinitions = 1);
    const cnt::SafeFullVector<TabStop>& getTabStops() const;
    
    void useLineHighlightManager();
    
    td::UINT4 getTotalNumberOfLines() const;
    //CoordType getTotalHeigthInLogicalPixels() const;
    CoordType getAverageCharWidth() const;
    
    bool isCodeEdit() const;
    void sendLineAndColPos(bool bSend);
    bool isSendLineAndColPos() const;
    bool useMarkupMap() const;
    
    td::UINT4 getLinePos(td::UINT4 lineNo) const; //returns position in glyps
    td::UINT4 getLine(td::MutableString& strOut, td::UINT4 lineNo, td::UINT4 prevLinePos=0) const;
    
    //returns position of next line in glyphs (ultra fast for sequential read), 0 if we read last line
    td::UINT4 getLineContentAtPosition(td::MutableString& strOut, td::UINT4 lineHeadPos) const override final;
    td::UINT4 getNumberOfLeadingChars(td::UINT4 lineHeadPos, char ch) const;
    
    //returns height of the previous line (begin of the first visible line). 0->no line is visible
    //        first visible lineNo (1-based). If zero -> there is no any visible line
    //        output: startingPos of the next line. 0 -> there is no next line
    gui::CoordType getFirstVisibleLine(td::UINT4& lineNo, td::UINT4& startingPos) const;
    
    //returns line height of line which starts at startingPos
    //on return: linePos contains starting position of the next line. 0: there is no next line
    gui::CoordType getLineHeight(td::UINT4& startingPos) const;
    
    //this is minimum lineHeight (for empty line)
    CoordType getLineHeight() const;
    
    int find(const td::String& txtToFind, td::UINT4 startingPos) const; //-1 if not found, otherwise position
    
    int replace(const td::String& txtToFind, const td::String& textToReplace, td::UINT4 startingPos, bool replaceAll); //if replaceAll: returns number of replaced strings, otherwise -1 if nothing replaced or position of replaced string
    
    bool goToLine(td::UINT4 lineNo); //lineNo must be 1 - based
    
    void setModifiedFlag(td::BYTE modified);
    void resetModifiedFlag();
    bool isModified() const;
    
    //to speed up updating the content
    void beginUpdate();
    void endUpdate();
    void clean();
    
    //static methods
    static IFindReplace* getFindReplace();
    static void setFindReplaceInactiveMode(IFindReplace::InactiveMode mode);
    static bool loadMarkupMap(const td::String& fileName);
    static bool getMarkupReplacement(const char* pMarkup, td::String& replacement, int markupLen = -1);
};

} //namespace gui
