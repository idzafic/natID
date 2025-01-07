#pragma once
#include <gui/View.h>
#include "Button.h"
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/TextEdit.h>
#include "BtnDelegate.h"
#include <gui/ColorPicker.h>
#include <gui/Color.h>
#include <gui/FileDialog.h>
#include <gui/Font.h>

class TextEditor : public gui::View
{
protected:
    enum class WndID : td::UINT4 {FileOpenDlg, FileSaveDlg};
    
    BtnDelegate _btnDelegate;
    Button _btnNormal;
    Button _btnBold;
    Button _btnItalic;
    Button _btnBoldItalic;
    Button _btnUnderline;
    Button _btnRemoveUnderline;
    Button _btnSubscript;
    Button _btnSuperscript;
    Button _btnRemoveSubAndSuperscript;
    
    gui::ColorPicker _color;
    Button _btnRemoveColor;
    Button _btnIncreaseFontSize;
    Button _btnDecreaseFontSize;
    gui::TextEdit _textEdit;
    gui::HorizontalLayout _hl;
    gui::VerticalLayout _vl;
//    gui::Font _fnt;
protected:
    inline bool getRange(gui::Range& range)
    {
        _textEdit.getSelection(range);
        if (range.length <= 0)
            return false;
        return true;
    }
    
    void updateColor()
    {
        gui::Range range;
        if (getRange(range))
        {
            td::ColorID clrID = _color.getValue();
            _textEdit.setColor(range, clrID);
        }
    }
    
    void handleSelectionChange()
    {
        gui::Range range;
        _textEdit.getSelection(range);
        gui::TextAttribs attribs;
        _textEdit.getTextAttribs(range.location, attribs);
        
        switch(attribs.style)
        {
            case gui::Font::Style::Normal:
                _btnNormal.setChecked(true);
                _btnBold.setChecked(false);
                _btnItalic.setChecked(false);
                _btnBoldItalic.setChecked(false);
                break;
            case gui::Font::Style::Bold:
                _btnNormal.setChecked(false);
                _btnBold.setChecked(true);
                _btnItalic.setChecked(false);
                _btnBoldItalic.setChecked(false);
                break;
            case gui::Font::Style::Italic:
                _btnNormal.setChecked(false);
                _btnBold.setChecked(false);
                _btnItalic.setChecked(true);
                _btnBoldItalic.setChecked(false);
                break;
            case gui::Font::Style::BoldItalic:
                _btnNormal.setChecked(false);
                _btnBold.setChecked(false);
                _btnItalic.setChecked(false);
                _btnBoldItalic.setChecked(true);
                break;
            default:
                break;
        }
        
        td::ColorID clrID = attribs.colorID;
        _color.setValue(clrID, false);
        
        //check markups
        bool bUnderline = false;
        bool bSubscript = false;
        bool bSuperscript = false;
        for (auto markup : attribs.markups)
        {
            if (markup == gui::Font::Markup::NA)
            {
                break;
            }
            
            if (markup == gui::Font::Markup::Underline)
                bUnderline = true;
            else if (markup == gui::Font::Markup::Subscript)
                bSubscript = true;
            else if (markup == gui::Font::Markup::Superscript)
                bSuperscript = true;
        }
        _btnUnderline.setChecked(bUnderline);
        _btnSubscript.setChecked(bSubscript);
        _btnSuperscript.setChecked(bSuperscript);
    }
    
public:
    TextEditor()
    : _btnNormal(&_btnDelegate, 10)
    , _btnBold(&_btnDelegate, 20)
    , _btnItalic(&_btnDelegate, 30)
    , _btnBoldItalic(&_btnDelegate, 40)
    , _btnUnderline(&_btnDelegate, 50)
    , _btnRemoveUnderline(&_btnDelegate, 51)
    , _btnSubscript(&_btnDelegate, 60)
    , _btnSuperscript(&_btnDelegate, 70)
    , _btnRemoveSubAndSuperscript(&_btnDelegate, 71)
    , _btnRemoveColor(&_btnDelegate, 80)
    , _btnIncreaseFontSize(&_btnDelegate, 100)
    , _btnDecreaseFontSize(&_btnDelegate, 110)
    , _textEdit(gui::TextEdit::HorizontalScroll::Yes, gui::TextEdit::Events::Send)
    , _hl(16)
    , _vl(2)
    {
        _color.setToolTip(tr("TxtColor"));
        //_textEdit.setText("Ovo je samo neki tekst da ne\n moram kucati. Na primjer ovo se mora postaviti na m2 a ovo da bude podvuceno. \nJos jedan red\n\n\tI jedan ispod nakon taba\n...");
        td::String strHTML = gui::getResText("textEditIntro");
        _textEdit.setHTML(strHTML);
        _textEdit.forwardMessagesTo(this);
        
        _hl << _btnNormal << _btnBold <<_btnItalic  << _btnBoldItalic << _btnUnderline << _btnRemoveUnderline;
        _hl.appendSpace(20);
        _hl << _btnSubscript << _btnSuperscript << _btnRemoveSubAndSuperscript;
        _hl.appendSpace(25);
        _hl << _color << _btnRemoveColor;
        _hl.appendSpacer(50);
        _hl << _btnIncreaseFontSize << _btnDecreaseFontSize;
        _vl << _hl << _textEdit;
       setLayout(&_vl);
        
        _textEdit.setFontName("Times New Roman");
        
        _btnIncreaseFontSize.onClick([this]()
        {
            float fontSize = _textEdit.getFontSize();
            fontSize += 2;
            _textEdit.setFontSize(fontSize);
        });
        
        _btnDecreaseFontSize.onClick([this]()
        {
            float fontSize = _textEdit.getFontSize();
            if (fontSize <= 8)
                return;
            fontSize -= 2;
            if (fontSize <= 8)
                fontSize = 8;
            _textEdit.setFontSize(fontSize);
        });
        
        _btnNormal.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setStyle(range, gui::Font::Style::Normal);
        });
        
        _btnBold.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setStyle(range, gui::Font::Style::Bold);
        });
        
        _btnItalic.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setStyle(range, gui::Font::Style::Italic);
        });
        
        _btnBoldItalic.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setStyle(range, gui::Font::Style::BoldItalic);
        });
        
        _btnUnderline.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setMarkup(range, gui::Font::Markup::Underline);
        });
        
        _btnRemoveUnderline.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.removeMarkup(range, gui::Font::Markup::Underline);
        });
        
 
        _btnSubscript.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setMarkup(range, gui::Font::Markup::Subscript);
        });
        
        _btnSuperscript.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.setMarkup(range, gui::Font::Markup::Superscript);
        });
        
        _btnRemoveSubAndSuperscript.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.removeMarkup(range, gui::Font::Markup::Superscript);
        });
        
        _color.onChangedValue([this]()
        {
            updateColor();
        });
        
        _btnRemoveColor.onClick([this]()
        {
            gui::Range range;
            if (getRange(range))
                _textEdit.removeColor(range);
        });
        
        _textEdit.onChangedSelection([this]()
        {
            handleSelectionChange();
        });
    }
    
    void loadHTML()
    {
        //open HTML
        td::UINT4 dlgID = 3333;
        gui::OpenFileDialog::show(this, tr("OpenF"), {{tr("HTMLDocs"), "*.html"}}, dlgID, [this](gui::FileDialog* pFileDlg)
        {
            auto status = pFileDlg->getStatus();
            if (status == gui::FileDialog::Status::OK)
            {
                td::String strFileName = pFileDlg->getFileName();
                td::String strContent;
                
                if (fo::loadFileContent(strFileName, strContent))
                {
                    _textEdit.setHTML(strContent);
                }
            }
        });
    }
    void saveHTML()
    {
        //save HTML
        td::UINT4 dlgID = 4444;
        gui::SaveFileDialog::show(this, tr("SaveHTML"), "*.html", dlgID, [this](gui::FileDialog* pFileDlg)
          {
              auto status = pFileDlg->getStatus();
              if (status == gui::FileDialog::Status::OK)
              {
                  td::String strHTML = _textEdit.getHTML(false);
                  if (strHTML.length() == 0)
                      return;
                  
                  td::String strFileName = pFileDlg->getFileName();
                  fo::OutFile f;
                  if (fo::createTextFile(f, strFileName, true))
                      f << strHTML;
                  f.close();
              }
          });
//        auto pFD = new gui::SaveFileDialog(this, tr("SaveHTML"), "*.html");
//        pFD->open([this](gui::FileDialog* pFileDlg)
//        {
//            auto status = pFileDlg->getStatus();
//            if (status == gui::FileDialog::Status::OK)
//            {
//                td::String strHTML = _textEdit.getHTML(false);
//                if (strHTML.length() == 0)
//                    return;
//                
//                td::String strFileName = pFileDlg->getFileName();
//                fo::OutFile f;
//                if (fo::createTextFile(f, strFileName, true))
//                    f << strHTML;
//                f.close();
//            }
//        });
    }
};
