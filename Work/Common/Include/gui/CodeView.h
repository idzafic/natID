//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Canvas.h"
#include <td/ColorID.h>
#include <gui/TextAttribs.h>
#include <cnt/PushBackVector.h>

namespace gui
{

class CodeViewHelper;
class NATGUI_API CodeView : public Canvas
{
private:
    std::function<void()> _onChangedSelection;
public:
    enum class Events : unsigned char {DoNotSend = 0, Send};

    struct TabStop {
        int start;
        int nSteps;
        int delta;
    };
    
private:
    
    friend class CodeViewHelper;
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
    explicit CodeView(CodeView::Events sendEvents = CodeView::Events::DoNotSend, bool readOnly = false);
    ~CodeView();
    
    td::String getText() const;
    void setText(const td::String& strTxt);
    
    void insertStringAtSelection(const char* str);
    void insertStringAtSelection(const td::String& str);
    
    void replaceString(const gui::Range& range, const char* str);
    void replaceString(const gui::Range& range, const td::String& str);

    void appendString(const char* str);
    void appendString(const td::String& str);
    
    //if pSizeToFitInWidth is specified: on input it has to contain required width or zero an height or zero,
    
    void setFontName(const char* fontName);
    void setFontName(const td::String& fontName);
    void setFontSize(float fontSize);
    float getFontSize() const;
    
    void selectAll();
    
    void getSelection(gui::Range& range) const;
    void setSelection(gui::Range& range, bool bMakeVisibleCurrentSelection);

//    td::ColorID getColor(const gui::Range& range) const;
    
    gui::ObjType getObjType() const override { return ObjType::CodeView;}
    bool isEmpty() const;
    size_t getNumberOfGlyphs() const;

    const std::function<void()>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void()>& fnToCall);
    
    void setTabStops(struct TabStop* tabStops, size_t nTabStopDefinitions = 1);
};

}
