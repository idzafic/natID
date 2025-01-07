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

namespace gui
{

class TextEditHelper;
class NATGUI_API TextEdit : public TextCtrl
{
private:
    std::function<void()> _onChangedSelection;
public:
    enum class HorizontalScroll : unsigned char {No =0, Yes};
    enum class Events : unsigned char {DoNotSend = 0, Send};

    struct TabStop {
        int start;
        int nSteps;
        int delta;
    };
    
private:
    
    friend class TextEditHelper;
    void sendSelChangeMessage();
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    bool onKeyPressed(const gui::Key& key) override;
    virtual void onPostKeyPress(const gui::Key& key);
    virtual void onBeforePaste();
    virtual void onPaste();
    virtual void onBeforeCut();
    virtual void onCut();
    virtual void onFontChange();
public:
    explicit TextEdit(TextEdit::HorizontalScroll useHorizontalScrolls = HorizontalScroll::No, TextEdit::Events sendEvents = TextEdit::Events::DoNotSend, bool readOnly = false);
    ~TextEdit();
    
    void insertStringAtSelection(const char* str);
    void insertStringAtSelection(const td::String& str);
    
    void replaceString(const gui::Range& range, const char* str);
    void replaceString(const gui::Range& range, const td::String& str);

    void appendString(const char* str);
    void appendString(const td::String& str);
    
    //if pSizeToFitInWidth is specified: on input it has to contain required width or zero an height or zero,
    //                       on output it will contain size required to fit all content and tomake it visible
    void setHTML(const td::String& strHTML, Size* pSizeToFitInWidth = nullptr);
    void setText(const td::String& strText, Size* pSizeToFitInWidth = nullptr);
    
    td::String getText() const;
    td::String getHTML(bool addBodyTag) const;
    
//    void setFont(gui::Font* pFont);
//    void setFont(gui::Font::ID fontID);
//    void getFont(gui::Font& font) const;
    
    void setFontName(const char* fontName);
    void setFontName(const td::String& fontName);
    void setFontSize(float fontSize);
    float getFontSize() const;
    
    void selectAll();
    
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

    const std::function<void()>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void()>& fnToCall);
    
    void setTabStops(struct TabStop* tabStops, size_t nTabStopDefinitions = 1);
};

}
