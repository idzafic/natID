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
#include <gui/Transformation.h>

constexpr gui::CoordType dCtrl = 60, cX = 380, cY = 220, rB = 24.;
//dCtrl - shift for bezier control points
//cX - center (x coordinate) of the shape
//cY - center (Y coordinate) of the shape
//rB - radius of the Bezier shape (when dCtrl = 0)

constexpr bool showBezierConstructingPoints = true;
constexpr bool doNotShowNonBezierPrimitives = true;

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
    gui::Shape _shapeBezier;
    gui::Shape _lineBez1;
    gui::Shape _lineBez2;
    gui::Shape _lineBez3;
    
    gui::Image _img;
    gui::Symbol _symbol;
    gui::Symbol _symbolBez1;
    gui::Symbol _symbolBez2;
    gui::Point _translate;
    double _scaleX = 1.0;
    double _scaleY = 1.0;
protected:
    
    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1200;
        modelSize.height = 800;
        return true;
    }
    
    bool onZoom(const gui::InputDevice& inputDevice) override
    {
        double scale = inputDevice.getScale();
        _scaleX *= scale;
        _scaleY *= scale;
        reDraw();
        return true;
    }
    
    bool onScroll(const gui::InputDevice& inputDevice) override
    {
        const gui::Point& delta = inputDevice.getScrollDelta();
        _translate += delta;
        reDraw();
        return true;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Transformation tr;
        tr.translate(_translate.x, _translate.y);
        tr.scale(_scaleX, _scaleY);
        tr.appendToContext();
        
        if (doNotShowNonBezierPrimitives)
        {
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
        }
        
        _shapeBezier.drawFillAndWire(td::ColorID::Red, td::ColorID::SysText);
        
        gui::Rect smbRect(300,500, 375, 590);
        _symbolBez2.drawInRect(smbRect);
        gui::Shape::drawRect(smbRect, td::ColorID::SysText, 1, td::LinePattern::Dot);
        
        smbRect -= gui::Point(100, 0);
        smbRect.setSize(50, 50);
        _symbolBez1.drawInRect(smbRect);
        gui::Shape::drawRect(smbRect, td::ColorID::SysText, 1, td::LinePattern::Dot);
        
        _lineBez1.drawWire(td::ColorID::Red);
        _lineBez2.drawWire(td::ColorID::Green);
        _lineBez3.drawWire(td::ColorID::Blue);
        
        if (showBezierConstructingPoints)
        {
            gui::Size sz(3, 3);
            //moveTo in Green (P1)
            //.moveTo({rB+cX,cY})
            gui::Point p1(rB+cX-sz.width/2, cY-sz.height/2);
            gui::Rect rp(p1, sz);
            gui::Shape::drawRect(rp, td::ColorID::Green);
            
            //quadratic (P1 to P2)
            //.quadraticTo({cX,    rB+cY},  {rB+cX+dCtrl,  rB+cY+dCtrl})  //P2, P2d
            gui::Point p2(cX-sz.width/2, rB+cY-sz.height/2);
            rp.setOriginAndSize(p2, sz);
            gui::Shape::drawRect(rp, td::ColorID::Blue);
            
            //control point for quadratic (P1 to P2)
            gui::Point p2c(rB+cX+dCtrl-sz.width/2, rB+cY+dCtrl-sz.height/2);
            rp.setOriginAndSize(p2c, sz);
            gui::Shape::drawRect(rp, td::ColorID::Yellow);
            
            //quadratic (P2 to P3)
            //.quadraticTo({-rB+cX, cY},    {-rB+cX-dCtrl, rB+cY+dCtrl})  //P3, P3c
            gui::Point p3(-rB+cX-sz.width/2, cY-sz.height/2);
            rp.setOriginAndSize(p3, sz);
            gui::Shape::drawRect(rp, td::ColorID::Blue);
            
            //control point for quadratic (P2 to P3)
            gui::Point p3c(-rB+cX-dCtrl-sz.width/2, rB+cY+dCtrl-sz.height/2);
            rp.setOriginAndSize(p3c, sz);
            gui::Shape::drawRect(rp, td::ColorID::Yellow);
            
            //quadratic (P3 to P4)
            //.quadraticTo({cX,    -rB+cY}, {-rB+cX-dCtrl, -rB+cY-dCtrl}) //P4, P4c
            gui::Point p4(cX-sz.width/2, -rB+cY-sz.height/2);
            rp.setOriginAndSize(p4, sz);
            gui::Shape::drawRect(rp, td::ColorID::Blue);
            
            //control point for quadratic (P3 to P4)
            gui::Point p4c(-rB+cX-dCtrl-sz.width/2, -rB+cY-dCtrl-sz.height/2);
            rp.setOriginAndSize(p4c, sz);
            gui::Shape::drawRect(rp, td::ColorID::Yellow);
            
            //quadratic (P4 to P5/P1)
            //.quadraticTo({rB+cX,  cY},    {rB+cX+dCtrl,  -rB+cY-dCtrl}, true); //P1, P1c
//            gui::Point p5(rB+cX-sz.width/2, cY-sz.height/2);
//            rp.setOriginAndSize(p5, sz);
//            gui::Shape::drawRect(rp, td::ColorID::Blue);
            
            //control point for quadratic (P2 to P3)
            gui::Point p5c(rB+cX+dCtrl-sz.width/2, -rB+cY-dCtrl-sz.height/2);
            rp.setOriginAndSize(p5c, sz);
            gui::Shape::drawRect(rp, td::ColorID::Yellow);
        }
    }
public:
    ViewCanvas()
    : gui::Canvas({gui::InputDevice::Event::Zoom}) //scrolling and zooming without scrollbars
    , _img(":TransCost")
    , _symbol(":Xfmr4")
    , _symbolBez1(":Bez1")
    , _symbolBez2(":Bez2")
    {
        registerForScrollEvents(); //Event::Zoom must be enabled, otherwise no scroll events
        //_symbolBez2.save("/Volumes/RAMDisk/Bez.xml");
        gui::Rect r(50, 50, 200, 150);
        float lw = 5;
        _shapeRect.createRect(r, lw);
        gui::Circle c(300, 300, 70);
        lw = 10;
        _shapeCircle.createCircle(c, lw, td::LinePattern::Dash);
        
        r.translate(200, 0);
        _shapeRoundRect.createRoundedRect(r, 20);
        _shapeRoundRect.updateRoundRectNodes(r, 20);
        
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
        
        //Bezier tests
        {
            //4-wings star
            _shapeBezier.createBezier().moveTo({rB+cX,cY})   //P1
                .quadraticTo({cX,    rB+cY},  {rB+cX+dCtrl,  rB+cY+dCtrl})  //P2, P2d
                .quadraticTo({-rB+cX, cY},    {-rB+cX-dCtrl, rB+cY+dCtrl})  //P3, P3c
                .quadraticTo({cX,    -rB+cY}, {-rB+cX-dCtrl, -rB+cY-dCtrl}) //P4, P4c
                .quadraticTo({rB+cX,  cY},    {rB+cX+dCtrl,  -rB+cY-dCtrl}, true); //P1, P1c
            
            //simple lines
            _lineBez1.createBezier(1.0, td::LinePattern::DashDot).moveTo({100, 300}).cubicTo({500,300}, {200,100}, {400,100});
            _lineBez2.createBezier(1.0, td::LinePattern::DashDot).moveTo({500,300}).cubicTo({800,300}, {600,500}, {700,100});
            _lineBez3.createBezier(1.0, td::LinePattern::DashDot).moveTo({100,300}).cubicTo({800,300}, {50,500}, {750,500});

        }
    }
};
