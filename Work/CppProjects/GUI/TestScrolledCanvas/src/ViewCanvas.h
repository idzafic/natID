//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/DrawableString.h>
#include <gui/Context.h>

class ViewCanvas : public gui::Canvas
{
protected:
    gui::Shape _shapeRect;
    gui::Shape _shapeCircle;
    gui::Shape _shapeRoundRect;
    gui::Shape _shapePolygon;
    gui::Shape _shapePolyLine;
    gui::Shape _shapePie;
    gui::Shape _shapeArc;
    gui::Shape _shapeOval;
    gui::Shape _shapeLines;
    gui::Image _img;
    gui::Symbol _symbol;
    gui::DrawableString _drawableString;
public:
    ViewCanvas()
    : gui::Canvas({gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::Zoom})
    , _img(":TransCost")
    , _symbol(":Xfmr4")
    , _drawableString("This is text rendered by DrawableString! The text is a bit longer (for demonstration purposes)")
    {
        gui::Rect r(50, 50, 200, 150);
        float lw = 5;
        _shapeRect.createRect(r, lw);
        gui::Circle c(300, 300, 70);
        lw = 10;
        _shapeCircle.createCircle(c, lw, td::LinePattern::Dash);
        r.translate(200, 0);
        _shapeRoundRect.createRoundedRect(r, 20);
        
        gui::Point points[] = {{0,0}, {100, 50}, {30,150}, {-10, 50}};
        math::translate(&points[0], 4, 100.0, 300.0);
        lw = 4;
        _shapePolyLine.createPolyLine(&points[0], 4, lw, td::LinePattern::DashDot);
        
        math::translate(&points[0], 4, 300.0, 0.0);
        lw = 7;
        _shapePolygon.createPolygon(&points[0], 4, lw);
        
        c.translate(0, 100);
        float fromAngle = 0;
        float toAngle = 90;
        lw = 4;
        _shapePie.createPie(c, fromAngle, toAngle, lw);
        
        c.translate(-100, 0);
        _shapeArc.createArc(c, fromAngle, toAngle, lw);
        
        r.translate(200, 150);
        lw = 5;
        _shapeOval.createOval(r, lw);
        points[0] = {200, 10};
        points[1] = {600, 10};
        points[2] = {200, 20};
        points[3] = {600, 20};
        
        lw = 4;
        _shapeLines.createLines(&points[0], 4, lw, td::LinePattern::DashDot);
    }
    
    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1300;
        modelSize.height = 550;
        return true;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        double scale = 2;
        double oldScale = getScale();
        double newScale = oldScale * scale;
        //gui::Point modelPoint(300,300);
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        scaleToPoint(newScale, modelPoint);
        
//        const gui::Point& modelPoint = inputDevice.getModelPoint();
//        double scale = 2;
//        double oldScale = getScale();
//        double newScale = oldScale * scale;
//        scaleToPoint(newScale, modelPoint);
//        return true;
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        double scale = 2;
        double oldScale = getScale();
        double newScale = oldScale / scale;
        //gui::Point modelPoint(300,300);
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        scaleToPoint(newScale, modelPoint);
    }
    
    bool onZoom(const gui::InputDevice& inputDevice) override
    {
        auto& modelPoint = inputDevice.getModelPoint();
        double scale = inputDevice.getScale();
        //scale = 2;
        double oldScale = getScale();
        double newScale = oldScale * scale;
        scaleToPoint(newScale, modelPoint);
        return true;
    }
    
//    virtual void onClick(const gui::Point& modelPoint, td::UINT4 keyModifiers)
//    {
//        double scale = getScale();
//        scale *= 1.2;
//        setScale(scale);
//    }
//
//    virtual void onRightMouseDown(const gui::Point& modelPoint, td::UINT4 keyModifiers)
//    {
//        double scale = getScale();
//        scale /= 1.2;
//        setScale(scale);
//    }
    
    void onDraw(const gui::Rect& rect) override
    {
        mu::dbgLog("onDraw(%.1f, %1.f), (%1.f, %.1f)", rect.left, rect.top, rect.width(), rect.height());
        
        
        _shapeRect.drawFillAndWire(td::ColorID::Blue, td::ColorID::Yellow);
        _shapeCircle.drawWire(td::ColorID::DarkBlue);
        _shapeRoundRect.drawFill(td::ColorID::DarkGreen);
        _shapePolyLine.drawWire(td::ColorID::DarkRed);
        _shapePolygon.drawFillAndWire(td::ColorID::Yellow, td::ColorID::DarkOliveGreen);
        _shapePie.drawFillAndWire(td::ColorID::DarkCyan, td::ColorID::DarkGray);
        _shapeArc.drawFillAndWire(td::ColorID::DarkCyan, td::ColorID::DarkGray);
        _shapeOval.drawFillAndWire(td::ColorID::Pink, td::ColorID::DarkMagenta);
        _shapeLines.drawWire(td::ColorID::DarkOrange);
        
        
        
        //rect is wider
        {
            gui::Rect imgRect(450, 50, 650, 150);
            _img.draw(imgRect, gui::Image::AspectRatio::No); //no
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(210, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(210, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(210, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Right);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
        }
        
        //rect is higher
        {
            gui::Rect imgRect(650, 160, 750, 400);
            _img.draw(imgRect, gui::Image::AspectRatio::No); //no
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(110, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Top);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(110, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Center);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
            imgRect.translate(110, 0);
            _img.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Bottom);
            gui::Shape::drawRect(imgRect, td::ColorID::Blue, 2, td::LinePattern::Dot);
            
        }
        
        _symbol.draw(50, 520);
        
        gui::Rect symbolRect(100, 150, 200, 300);
        _symbol.drawInRect(symbolRect);
        
        symbolRect.translate(70, 0);
        _symbol.drawInRect(symbolRect, 0, gui::Symbol::getDefaultSymbolAttribs());
        
        {
            gui::Point p1(0, 100);
            gui::Point p2(2000, 100);
            float lw = 1;
            gui::Point dP(0,100);
            gui::Shape::drawLine(p1, p2, td::ColorID::Red, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Green, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Blue, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Yellow, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Orange, lw, td::LinePattern::Dot);
        }
        
        {
            gui::Point p1(100, 0);
            gui::Point p2(100, 2000);
            float lw = 1;
            gui::Shape::drawLine(p1, p2, td::ColorID::Red, lw, td::LinePattern::Dot);
            gui::Point dP(100,0);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Green, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Blue, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Yellow, lw, td::LinePattern::Dot);
            p1.translate(dP);
            p2.translate(dP);
            gui::Shape::drawLine(p1, p2, td::ColorID::Orange, lw, td::LinePattern::Dot);
        }

        gui::Rect rectForDrawableStr(500, 420, 600, 470);
        gui::Shape shRectTxt;
        shRectTxt.createRect(rectForDrawableStr, 1, td::LinePattern::Dot);

        shRectTxt.drawWire(td::ColorID::Yellow);
        _drawableString.draw(rectForDrawableStr, gui::Font::ID::SystemNormal, td::ColorID::Orange, td::TextAlignment::Left);

        //draw rotated string
        {
            gui::Context ctx;
            gui::Transformation tr;
            
            tr.translate(rectForDrawableStr.left, rectForDrawableStr.top);
            tr.rotateDeg(-90);
            //tr.translate(0, -100);

            //tr.appendToContext();
            tr.setToContext();
            gui::Rect rect2(0, 0, 200, 200);
            _drawableString.draw(rect2, gui::Font::ID::SystemNormal, td::ColorID::Orange);
        }

        gui::Point ptTransl(120, 0);
        rectForDrawableStr.translate(ptTransl);
        //        gui::Shape shRectTxt2;
        //        shRectTxt2.createRect(rectForDrawableStr, 1, td::LinePattern::Dot);
        shRectTxt.translateRectNodes(ptTransl);
        shRectTxt.drawWire(td::ColorID::Yellow);
        _drawableString.draw(rectForDrawableStr, gui::Font::ID::SystemNormal, td::ColorID::Blue, td::TextAlignment::Center);

        rectForDrawableStr.translate(ptTransl);
        //        gui::Shape shRectTxt3;
        //        shRectTxt3.createRect(rectForDrawableStr, 1, td::LinePattern::Dot);
        shRectTxt.translateRectNodes(ptTransl);
        shRectTxt.drawWire(td::ColorID::Yellow);
        _drawableString.draw(rectForDrawableStr, gui::Font::ID::SystemNormal, td::ColorID::Magenta, td::TextAlignment::Right);

        rectForDrawableStr.translate(ptTransl);
        //        gui::Shape shRectTxt3;
        //        shRectTxt3.createRect(rectForDrawableStr, 1, td::LinePattern::Dot);
        shRectTxt.translateRectNodes(ptTransl);
        shRectTxt.drawWire(td::ColorID::Yellow);
        _drawableString.draw(rectForDrawableStr, gui::Font::ID::SystemNormal, td::ColorID::Green, td::TextAlignment::Justified);
        
    }
    
    
    void toolbarZoomIn()
    {
        double scale = getScale();
        scale *= 2;
        setScale(scale);
    }
    
    void toolbarZoomOut()
    {
        double scale = getScale();
        scale /= 2;
        setScale(scale);
    }
};
