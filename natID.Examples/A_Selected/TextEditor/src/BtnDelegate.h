#pragma once
#include <gui/IDrawDelegate.h>
#include <gui/Shape.h>

class BtnDelegate : public gui::IDrawDelegate
{
protected:
//    gui::Shape _shNormal;
//    gui::Shape _shBold;
//    gui::Shape _shItalic;
    gui::Shape _shUnderline1;
    gui::Shape _shUnderline2;
    gui::Shape _shPlus;
    gui::Shape _shMinus;
    gui::CoordType _tx;
    gui::CoordType _ty;
public:
    BtnDelegate();
    void getMinSize(gui::Size& sz, const gui::Control* pCtrl) const override;
    void draw(const gui::Rect& rect, gui::Control* pCtrl) override;
};
