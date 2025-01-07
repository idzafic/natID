#include "BtnDelegate.h"
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/DrawableButtonWithDelegate.h>
#include <gui/Application.h>

BtnDelegate::BtnDelegate()
{
}

void BtnDelegate::getMinSize(gui::Size& sz, const gui::Control* pCtrl) const
{
    gui::CoordType minSize = 24;
    if (sz.width < minSize)
        sz.width = minSize;
    if (sz.height < minSize)
        sz.height = minSize;
}

void BtnDelegate::draw(const gui::Rect& r, gui::Control* pCtrl)
{
    gui::Size sz;
    pCtrl->getSize(sz);
    if (!_shUnderline1.isInitialized())
    {
        double sx = 0.9;
        double sy = -0.75;
        _tx = sz.width/2;
        _ty = sz.height/2;
        gui::Point ptsUnderline1[] = {{-3,7},{-3,-4},{-2,-5},{2,-5},{3,-4},{3,7}};
        math::scale(ptsUnderline1, 6, sx, sy);
        math::translate(ptsUnderline1, 6, _tx, _ty);
        _shUnderline1.createPolyLine(ptsUnderline1, 6, 1.5f);
        gui::Point ptsUnderline2[] = {{-4,-8},{4,-8}};
        math::scale(ptsUnderline2, 2, sx, sy);
        math::translate(ptsUnderline2, 2, _tx, _ty);
        _shUnderline2.createPolyLine(ptsUnderline2, 2, 1.5f);

    //    gui::Shape _shSuperscript;
        
        gui::Point ptsPlus[] = {{-8,0},{8,0}, {0,-8},{0,8}};
        math::scale(ptsPlus, 4, sx, sy);
        math::translate(ptsPlus, 4, _tx, _ty);
        _shPlus.createLines(ptsPlus, 4, 3.0f);
        
        gui::Point ptsMinus[] = {{-6,0},{6,0}};
        math::scale(ptsMinus, 2, sx, sy);
        math::translate(ptsMinus, 2, _tx, _ty);
        _shMinus.createLines(ptsMinus, 2, 2.0f);
    }
    
    gui::Rect rect(0,0,sz.width, sz.height);
    gui::DrawableButtonWithDelegate* pBtn = (gui::DrawableButtonWithDelegate*) pCtrl;
    if (pBtn->isChecked())
        gui::Shape::drawRect(rect, td::ColorID::SysSelectedTextBack, td::ColorID::SysSelectedText, 1.0f);
    
    auto visualID = pCtrl->getVisualID();
    switch(visualID)
    {
        case 10:
        {
            const char* txt = "N";
            gui::DrawableString::draw(txt, 1, rect, gui::Font::ID::SystemLarger, td::ColorID::SysText, td::TextAlignment::Center, td::VAlignment::Center);
//            _shNormal.drawWire(td::ColorID::SysText);
        }
            break;
        case 20:
        {
            const char* txt = "B";
            gui::DrawableString::draw(txt, 1, rect, gui::Font::ID::SystemLargerBold, td::ColorID::SysText, td::TextAlignment::Center, td::VAlignment::Center);
//            _shBold.drawWire(td::ColorID::SysText);
        }

            break;
        case 30:
        {
            //            _shItalic.drawWire(td::ColorID::SysText);
            const char* txt = "I";
            gui::DrawableString::draw(txt, 1, rect, gui::Font::ID::SystemLargerItalic, td::ColorID::SysText, td::TextAlignment::Center, td::VAlignment::Center);
        }
            break;
        case 40:
        {
            //_shItalic.drawWire(td::ColorID::SysText, 3.0f);
            const char* txt = "BI";
            gui::DrawableString::draw(txt, 2, rect, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::SysText, td::TextAlignment::Center, td::VAlignment::Center);
        }
            
            break;
        case 50:
            //Unserline
            _shUnderline1.drawWire(td::ColorID::SysText);
            _shUnderline2.drawWire(td::ColorID::SysText);
            break;
        case 51:
        {
            //Cancel underline
            _shUnderline1.drawWire(td::ColorID::SysText);
            _shUnderline2.drawWire(td::ColorID::SysText);
            gui::Point p1(-7, 7);
            gui::Point p2(7, -7);
            p1.translate(_tx, _ty);
            p2.translate(_tx, _ty);
            gui::Shape::drawLine(p1, p2, td::ColorID::Red, 1.0f);
            gui::Point p3(-7, -7);
            gui::Point p4(7, 7);
            p3.translate(_tx, _ty);
            p4.translate(_tx, _ty);
            gui::Shape::drawLine(p3, p4, td::ColorID::Red, 1.0f);
        }
            break;
        case 60:
        {
            //subscript
            gui::Rect r1(4-8, 3-8, 7-8, 13-8);
            r1.translate(_tx, _ty);
            gui::Shape::drawRect(r1, td::ColorID::SysText, 1.0);
            gui::Rect r2(10-8, 12-8, 16-8, 16-8);
            r2.translate(_tx, _ty);
            gui::Shape::drawRect(r2, td::ColorID::SysText);
        }
            break;
        case 70:
        {
            //superscript
            gui::Rect r1(4-8, 3-8, 7-8, 13-8);
            r1.translate(_tx, _ty);
            gui::Shape::drawRect(r1, td::ColorID::SysText, 1.0);
            gui::Rect r2(10-8, 2-8, 16-8, 6-8);
            r2.translate(_tx, _ty);
            gui::Shape::drawRect(r2, td::ColorID::SysText);
        }
            break;
        case 71:
        {
            //Cancel subsript or superscript
            gui::Rect r1(4-8, 3-8, 7-8, 13-8);
            r1.translate(_tx, _ty);
            gui::Shape::drawRect(r1, td::ColorID::SysText, 1.0);
            gui::Rect r2(10-8, 0-8, 16-8, 4-8);
            r2.translate(_tx, _ty);
            gui::Shape::drawRect(r2, td::ColorID::SysText);
            gui::Rect r3(10-8, 12-8, 16-8, 16-8);
            r3.translate(_tx, _ty);
            gui::Shape::drawRect(r3, td::ColorID::SysText);
            gui::Point p1(-7, 7);
            gui::Point p2(7, -7);
            p1.translate(_tx, _ty);
            p2.translate(_tx, _ty);
            gui::Shape::drawLine(p1, p2, td::ColorID::Red, 1.0f);
            gui::Point p3(-7, -7);
            gui::Point p4(7, 7);
            p3.translate(_tx, _ty);
            p4.translate(_tx, _ty);
            gui::Shape::drawLine(p3, p4, td::ColorID::Red, 1.0f);
        }
            break;
        case 80:
        {
            //Cancel color
            const char* txt = "C";
            gui::Application* pApp = gui::getApplication();
            if (pApp->isDarkMode())
                gui::DrawableString::draw(txt, 1, rect, gui::Font::ID::SystemLarger, td::ColorID::LightBlue, td::TextAlignment::Center, td::VAlignment::Center);
            else
                gui::DrawableString::draw(txt, 1, rect, gui::Font::ID::SystemLarger, td::ColorID::DarkBlue, td::TextAlignment::Center, td::VAlignment::Center);
            gui::Point p1(-7, 7);
            gui::Point p2(7, -7);
            p1.translate(_tx, _ty);
            p2.translate(_tx, _ty);
            gui::Shape::drawLine(p1, p2, td::ColorID::Red, 1.0f);
            gui::Point p3(-7, -7);
            gui::Point p4(7, 7);
            p3.translate(_tx, _ty);
            p4.translate(_tx, _ty);
            gui::Shape::drawLine(p3, p4, td::ColorID::Red, 1.0f);
        }
            break;
        case 100:
            //plus
            _shPlus.drawWire(td::ColorID::SysText);
            break;
        case 110:
            //minus
            _shMinus.drawWire(td::ColorID::SysText);
            break;
        default:
            assert(false);
            
    }
}
