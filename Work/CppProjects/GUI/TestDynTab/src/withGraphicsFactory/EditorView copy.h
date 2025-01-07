//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
//#include <gui/Image.h>
#include <gui/Context.h>
#include <gui/Symbol.h>
#include <gui/GraphicsShapeFactory.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>


class EditorView : public gui::Canvas
{
private:
protected:
    //testing symbols
    gui::Symbol _pointer;
    gui::Symbol _bus;
    gui::Symbol _resistor;
    gui::Symbol _capacitor;
    gui::Symbol _inductor;
    gui::Symbol _shunt;
    gui::Symbol _generator;
    gui::Symbol _asArrowTL;
    gui::Symbol _asGen;
    gui::Symbol _asMotor;
    gui::Symbol _asSC;
    gui::Symbol _load;
    gui::Symbol _breaker;
    gui::Symbol _disconnector;
    gui::Symbol _fuse;
    gui::Symbol _twoWindingTransf;
    gui::Symbol _threeWindingTransf;
    gui::Symbol _fourWindingTransf;
    gui::Symbol _testRoundedRect;
    gui::Symbol _testOval;
    gui::Symbol _testPie;
    gui::Symbol _testArc;
    gui::GraphicsShapeFactoryLocal _graphStore;
    gui::GraphicsShape _polygon;
    gui::GraphicsShape _fillPolygon;
    
    //text in rect with alignment
    gui::GraphicsText _textRectLeft;
    gui::GraphicsText _textRectCenter;
    gui::GraphicsText _textRectRight;
    gui::GraphicsShape _gsRectLeft;
    gui::GraphicsShape _gsRectCenter;
    gui::GraphicsShape _gsRectRight;
    
    //text on point
    gui::GraphicsText _text2;
    gui::GraphicsText _text3;
    
    //image demo
    gui::Image _etf;
    gui::Image _unsa;
    
    //image vertical Aspect ratio demo
    gui::Rect _rETF_VStretch;
    gui::Rect _rETF_VTop;
    gui::Rect _rETF_VCenter;
    gui::Rect _rETF_VBottom;
    gui::GraphicsShape _gsVStretch;
    gui::GraphicsShape _gsVTop;
    gui::GraphicsShape _gsVCenter;
    gui::GraphicsShape _gsVBottom;
    
    //image horizontal aspect ratio demo
    gui::Rect _rETF_HStretch;
    gui::Rect _rETF_HLeft;
    gui::Rect _rETF_HCenter;
    gui::Rect _rETF_HRight;
    gui::GraphicsShape _gsHStretch;
    gui::GraphicsShape _gsHLeft;
    gui::GraphicsShape _gsHCenter;
    gui::GraphicsShape _gsHRight;
    
    gui::GraphicsShape _modelBoundRect;
    
    gui::Shape _grLine;
    //double _scale = 1.0;
    gui::DrawableString _drawableString;
    
    
protected:
    void makePlus()
    {
        gui::Point pts[] = {{2,2}, {2,10}, {-2,10}, {-2,2}, {-10,2}, {-10,-2}, {-2,-2}, {-2,-10}, {2,-10}, {2,-2}, {10,-2}, {10,2}};
        _grLine.updateNodes(pts, sizeof(pts)/sizeof(gui::Point));
    }
    
    void makeSomethingElse()
    {
        gui::Point pts[] = {{2,2}, {2,10}, {-2,10}, {-2,2}, {-10,2}, {-10,-2}, {-2,-2}, {-2,-10}, {2,-10}, {2,-2}, {10,-2}, {10,2}};
        for (size_t i=0; i<12; ++i)
        {
            pts[i].x *= 10;
            pts[i].y *= 3;
        }
        _grLine.updateNodes(pts, sizeof(pts)/sizeof(gui::Point));
    }
    
    virtual bool getModelSize(gui::Size& modelSize) const
    {
        modelSize.width = 2010;
        modelSize.height = 2260;
        return true;
    }
    
public:
    EditorView()
    : _pointer(":pointer")
    , _bus(":bus")
    , _asArrowTL(":ASArrowTL")
    , _resistor(":resistor")
    , _capacitor(":capacitor")
    , _inductor(":inductor")
    , _generator(":generator")
    , _asGen(":ASGen")
    , _asMotor(":ASMotor")
    , _asSC(":ASSynchComp")
    , _load(":load")
    , _shunt(":shunt")
    , _breaker(":breaker")
    , _disconnector(":disconctr")
    , _fuse(":fuse")
    , _twoWindingTransf(":Xfmr2")
    , _threeWindingTransf(":Xfmr3")
    , _fourWindingTransf(":Xfmr4")
    , _testRoundedRect(":TestRoundRect")
    , _testOval(":TestOval")
    , _testPie(":TestPie")
    , _testArc(":TestArc")
    , _graphStore(1, 10, 1, 5)
    , _etf(":ETF")
    , _unsa(":UNSA")
    , _rETF_VStretch(10, 900, 150, 1450)
    , _rETF_VTop(10, 900, 150, 1450)
    , _rETF_VCenter(10, 900, 150, 1450)
    , _rETF_VBottom(10, 900, 150, 1450)
    , _rETF_HStretch(210, 1500, 800, 1650)
    , _rETF_HLeft(210, 1500, 800, 1650)
    , _rETF_HCenter(210, 1500, 800, 1650)
    , _rETF_HRight(210, 1500, 800, 1650)
    , _drawableString("Ovo se ispisuje iz klase DrawableString!")
    {
        //create model bound rect
        gui::Size modelSize;
        getModelSize(modelSize);
        modelSize.width -= 4;
        modelSize.height -= 4;
        gui::Point ptOrig(1,1);
        gui::Rect modelBound(ptOrig, modelSize);
        _modelBoundRect = _graphStore.createRect(modelBound, 2, td::LinePattern::DashDot);
        
        //create other elements
        _rETF_VTop.translate(200,0);
        _rETF_VCenter.translate(400,0);
        _rETF_VBottom.translate(600,0);
        
        _rETF_HLeft.translate(0, 200);
        _rETF_HCenter.translate(0, 400);
        _rETF_HRight.translate(0, 600);
        
        _gsVStretch = _graphStore.createRect(_rETF_VStretch, 2, td::LinePattern::Dot);
        _gsVTop = _graphStore.createRect(_rETF_VTop, 2, td::LinePattern::Dot);
        _gsVCenter = _graphStore.createRect(_rETF_VCenter, 2, td::LinePattern::Dot);
        _gsVBottom = _graphStore.createRect(_rETF_VBottom, 2, td::LinePattern::Dot);
        
        _gsHStretch = _graphStore.createRect(_rETF_HStretch, 2, td::LinePattern::Dot);
        _gsHLeft = _graphStore.createRect(_rETF_HLeft, 2, td::LinePattern::Dot);
        _gsHCenter = _graphStore.createRect(_rETF_HCenter, 2, td::LinePattern::Dot);
        
        _gsHRight = _graphStore.createRect(_rETF_HRight, 2, td::LinePattern::Dot);
        
        
        gui::Point points[] = {gui::Point(1000,1000), gui::Point(1200,1200), gui::Point(1500,1200), gui::Point(1500,1200), gui::Point(1500, 1000), gui::Point(800,800)};
        _polygon = _graphStore.createPolygon(points, 6, 4, td::LinePattern::DashDot);

        for (size_t i=0; i<6;++i)
        {
            points[i].x += 500;
            points[i].y += 500;
        }
        
        _fillPolygon = _graphStore.createPolygon(points, 6, 16, td::LinePattern::Solid);
        td::String str1("Ovaj string se upisuje u pravouganik sa visinom 100 lp i sirinom 100 lp! Ovaj dio teksta ispod se ne vidi...");
        td::String str2("Drugi string - na lokaciji tj na zadatu tacku\nkoja se ispisuje u dva reda");
        gui::Rect rTxt(400, 400, 500, 520);
        _textRectLeft = _graphStore.createText(str1, gui::Font::ID::SystemNormal, rTxt, td::TextAlignment::Left);
        _gsRectLeft = _graphStore.createRect(rTxt, 1, td::LinePattern::Dot);
        
        rTxt.translate(150, 0);
        _textRectCenter = _graphStore.createText(str1, gui::Font::ID::SystemNormal, rTxt, td::TextAlignment::Center);
        _gsRectCenter = _graphStore.createRect(rTxt, 1, td::LinePattern::Dot);
        
        rTxt.translate(150, 0);
        _textRectRight = _graphStore.createText(str1, gui::Font::ID::SystemNormal, rTxt, td::TextAlignment::Right);
        _gsRectRight = _graphStore.createRect(rTxt, 1, td::LinePattern::Dot);
        
        gui::Point ptTxt(900,900);
        _text2 = _graphStore.createText(str2, gui::Font::ID::SystemNormalBold, ptTxt);
        gui::Point ptTxt3(1300,1700);
        td::String str3("Nesto veci string (1300,1700)");
        _text3 = _graphStore.createText(str3, gui::Font::ID::ViewNormal, ptTxt3);
        
        gui::Point pts[] = {{2,2}, {2,10}, {-2,10}, {-2,2}, {-10,2}, {-10,-2}, {-2,-2}, {-2,-10}, {2,-10}, {2,-2}, {10,-2}, {10,2}};
//        pts[0] = {2,2};
//        pts[1] = {2,10};
//        pts[2] = {-2,10};
//       // pts[0].x=2; pts[0].y=2;
//        //pts[1].x=2; pts[1].y=10;
//        //pts[2].x=-2; pts[2].y=10;
//        pts[3].x=-2; pts[3].y=2;
//        pts[4].x=-10; pts[4].y=2;
//        pts[5].x=-10; pts[5].y=-2;
//        pts[6].x=-2; pts[6].y=-2;
//        pts[7].x=-2; pts[7].y=-10;
//        pts[8].x=2; pts[8].y=-10;
//        pts[9].x=2; pts[9].y=-2;
//        pts[10].x=10; pts[10].y=-2;
//        pts[11].x=10; pts[11].y=2;
        _grLine.createPolyLine(pts, sizeof(pts)/sizeof(gui::Point), true);
    }
    
    
    inline void drawSymbols(gui::CoordType x, gui::CoordType y, gui::SymbolDrawingAttribs* pGroupAttribs = nullptr)
    {
        //_capacitor.drawDeg(x+=35, y, 90, pGroupAttribs);
        //return;
        _pointer.drawDeg(x, y, 0, pGroupAttribs);
        _bus.draw(x+=30, y, 0, pGroupAttribs);
        _capacitor.drawDeg(x+=35, y, 90, pGroupAttribs);
        _capacitor.attachSymbol(0, 0, _asArrowTL);
        _capacitor.drawDeg(x+=30, y, 90, pGroupAttribs);
        _capacitor.removeAttachedSymbol(0, 0);
        
        _inductor.drawDeg(x+=50, y, 90, pGroupAttribs);
        _resistor.drawDeg(x+=50, y, 90, pGroupAttribs);
        _shunt.drawDeg(x+=50, y, 90, pGroupAttribs);
        _generator.attachSymbol(0, 0, _asGen);
        _generator.drawDeg(x+=50, y, 90, pGroupAttribs);
        _generator.attachSymbol(0, 0, _asSC);
        _generator.drawDeg(x+=50, y,  90, pGroupAttribs);
        _generator.attachSymbol(0, 0, _asMotor);
        _generator.drawDeg(x+=50, y,  90, pGroupAttribs);
        _load.drawDeg(x+=50, y, 90, pGroupAttribs);
        //_motor.drawDeg(50, y+=40, 90);
        _twoWindingTransf.draw(x+=40, y, 0, pGroupAttribs);
        _threeWindingTransf.draw(x+=60, y, 0, pGroupAttribs);
        _fourWindingTransf.draw(x+=60, y, 0, pGroupAttribs);
        _breaker.draw(x+=60, y, 0, pGroupAttribs);
        _disconnector.draw(x+=50, y, 0, pGroupAttribs);
        _fuse.draw(x+=50, y, 0, pGroupAttribs);
        _testRoundedRect.draw(x+=50,y, 0, pGroupAttribs);
        _testOval.draw(x+=50,y, 0, pGroupAttribs);
        _testPie.draw(x+=50,y, 0, pGroupAttribs);
        _testArc.draw(x+=50,y, 0, pGroupAttribs);
    }
    
    virtual void onDraw(const gui::Rect& rDraw)
    {
        
        _modelBoundRect.drawWire(td::ColorID::SysText);
        //gui::Size sz;
        //getSize(sz);
        //mu::dbgLog("Size of view=%.1fx%.1f", sz.width, sz.height);
        
        //draw vertically aligned images
        _etf.draw(_rETF_VStretch, gui::Image::AspectRatio::No);
        _etf.draw(_rETF_VTop, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Top);
        _etf.draw(_rETF_VCenter, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Center);
        _etf.draw(_rETF_VBottom, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Bottom);
        
        //draw bounding rects
        _gsVStretch.drawWire(td::ColorID::SysText);
        _gsVTop.drawWire(td::ColorID::SysText);
        _gsVCenter.drawWire(td::ColorID::SysText);
        _gsVBottom.drawWire(td::ColorID::SysText);
        
        //draw horizontally aligned images
        _etf.draw(_rETF_HStretch, gui::Image::AspectRatio::No);
        _etf.draw(_rETF_HLeft, gui::Image::AspectRatio::Keep, td::HAlignment::Left, td::VAlignment::Top);
        _etf.draw(_rETF_HCenter, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Top);
        _etf.draw(_rETF_HRight, gui::Image::AspectRatio::Keep, td::HAlignment::Right, td::VAlignment::Top);
        
        //draw bounding rects
        _gsHStretch.drawWire(td::ColorID::SysText);
        _gsHLeft.drawWire(td::ColorID::SysText);
        _gsHCenter.drawWire(td::ColorID::SysText);
        _gsHRight.drawWire(td::ColorID::SysText);

        drawSymbols(20, 40);
        
        
        gui::SymbolDrawingAttribs genAttribs[4];
        genAttribs[0].displayGroup = 1;
        //genAttribs[0].overideLinePattern = 0;
        genAttribs[0].overideLineWidth = 1;
        genAttribs[0].overideLineColor = 1;
        genAttribs[0].setLineWidth(2);
        genAttribs[0].lineColor = td::ColorID::Yellow;
        genAttribs[0].overideFillColor = 1;
        genAttribs[0].fillColor = td::ColorID::Yellow;
        
        genAttribs[1].displayGroup = 1;
        //genAttribs[1].overideLinePattern = 0;
        genAttribs[1].overideLineWidth = 1;
        genAttribs[1].overideLineColor = 1;
        genAttribs[1].setLineWidth(2);
        genAttribs[1].lineColor = td::ColorID::Blue;
        genAttribs[1].overideFillColor = 1;
        genAttribs[1].fillColor = td::ColorID::Yellow;
        
        genAttribs[2].displayGroup = 1;
        //genAttribs[2].overideLinePattern = 0;
        genAttribs[2].overideLineWidth = 1;
        genAttribs[2].overideLineColor = 1;
        genAttribs[2].setLineWidth(2);
        genAttribs[2].lineColor = td::ColorID::Green;
        genAttribs[2].overideFillColor = 1;
        genAttribs[2].fillColor = td::ColorID::Blue;
        
        genAttribs[3].displayGroup = 1;
        //genAttribs[3].overideLinePattern = 0;
        genAttribs[3].overideLineWidth = 1;
        genAttribs[3].overideLineColor = 1;
        genAttribs[3].setLineWidth(2);
        genAttribs[3].lineColor = td::ColorID::Red;
        genAttribs[3].overideFillColor = 1;
        genAttribs[3].fillColor = td::ColorID::DarkMagenta;
        
        drawSymbols(20, 100, genAttribs);
        //return;
        gui::SymbolDrawingAttribs groupAttribs;
        groupAttribs.displayGroup = 1;
        //groupAttribs.overideLinePattern = 0;
        groupAttribs.overideLineWidth = 1;
        groupAttribs.overideLineColor = 1;
        groupAttribs.overideFillColor = 1;
        groupAttribs.setLineWidth(2);
        groupAttribs.lineColor = td::ColorID::Yellow;
        _capacitor.drawDeg(300, 300, 45, &groupAttribs);
        
        //groupAttribs.overideLinePattern = 0;
        //groupAttribs.linePattern = td::LinePattern::Dot;
        groupAttribs.setLineWidth(2);
        _capacitor.drawDeg(610, 600, 0, &groupAttribs);
        groupAttribs.lineColor = td::ColorID::Red;
        _capacitor.drawDeg(600, 610, 90, &groupAttribs);
        groupAttribs.lineColor = td::ColorID::Green;
        _capacitor.drawDeg(590, 600, 180, &groupAttribs);
        
        groupAttribs.lineColor = td::ColorID::Blue;
        _capacitor.drawDeg(600, 590, 270, &groupAttribs);
        
        //_disconnector.draw(1000, 100);

        _twoWindingTransf.drawDeg(1000,300, 45);
        
        gui::SymbolDrawingAttribs trGrAttribs[2];
        trGrAttribs[0].displayGroup = 1;
        //trGrAttribs[0].overideLinePattern = 0;
        trGrAttribs[0].overideLineWidth = 1;
        trGrAttribs[0].overideLineColor = 1;
        trGrAttribs[0].overideFillColor = 1;
        trGrAttribs[0].setLineWidth(2);
        trGrAttribs[0].lineColor = td::ColorID::Yellow;
        
        trGrAttribs[1].displayGroup = 1;
        //trGrAttribs[1].overideLinePattern = 0;
        trGrAttribs[1].overideLineWidth = 1;
        trGrAttribs[1].overideLineColor = 1;
        trGrAttribs[1].overideFillColor = 1;
        trGrAttribs[1].setLineWidth(4);
        trGrAttribs[1].lineColor = td::ColorID::Green;
        
        _twoWindingTransf.drawDeg(1000,320, 45, trGrAttribs);
        
        _fillPolygon.drawFillAndWire(td::ColorID::YellowGreen, td::ColorID::RosyBrown);
        _polygon.drawWire(td::ColorID::Maroon);
        
        _textRectLeft.draw(td::ColorID::Yellow);
        _textRectCenter.draw(td::ColorID::Yellow);
        _textRectRight.draw(td::ColorID::Yellow);
        
        _gsRectLeft.drawWire(td::ColorID::SysText);
        _gsRectCenter.drawWire(td::ColorID::SysText);
        _gsRectRight.drawWire(td::ColorID::SysText);

        _text2.draw(td::ColorID::Green);
        _text3.draw(td::ColorID::Magenta);
        
        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(600, 250);
            tr.scale(2);
            tr.appendToContext();
            _grLine.drawFillAndWire(td::ColorID::Red, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
        
        {
            gui::Context ctx;
            gui::Transformation tr;
            tr.translate(650, 250);
            tr.rotateDeg(45);
            //tr.scale(0.8);
            tr.appendToContext();
            _grLine.drawFillAndWire(td::ColorID::Red, td::ColorID::SysText);
            //gui::Transformation::restoreContext();
        }
        
        //change _grLine to something else
        makeSomethingElse();
        {
            gui::Context ctx;
            //gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(800, 250);
            //tr.rotateDeg(45);
            //tr.scale(0.8);
            tr.appendToContext();
            _grLine.drawFillAndWire(td::ColorID::Red, td::ColorID::SysText);
            //gui::Transformation::restoreContext();
        }
        
        {
            gui::Context ctx;
            //gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(1100, 250);
            tr.rotateDeg(45);
            tr.scale(0.5);
            tr.appendToContext();
            _grLine.drawFillAndWire(td::ColorID::Red, td::ColorID::SysText);
            //gui::Transformation::restoreContext();
        }
        //change _grLine to plus sign
        makePlus();
        
        //draw using DrawableString
        gui::Rect rectForDrawableStr(500, 800, 600, 900);
        _drawableString.draw(rectForDrawableStr, gui::Font::ID::SystemNormal, td::ColorID::Orange);
        
        //draw rotated string
        {
            gui::Context ctx;
            gui::Transformation tr;
            tr.translate(rectForDrawableStr.left, rectForDrawableStr.top);
            tr.rotateDeg(-90);
            tr.appendToContext();
            gui::Rect rect2(0,0, 100, 100);
            _drawableString.draw(rect2, gui::Font::ID::SystemNormal, td::ColorID::Orange);
        }
    }
    
    virtual bool onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID, gui::ActionItem* pAI)
    {
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //increase window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 1.2f;
//                g.size.height *= 1.2f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
//                return;
            }
            else if (actionID == 20)
            {
                //decrease window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 0.8f;
//                g.size.height *= 0.8f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
//                return;
            }
        }
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
    virtual void onPrimaryButtonPressed(const gui::InputDevice& inputDevice)
    {
        auto& modelPoint = inputDevice.getModelPoint();
        td::UINT4 keyModifiers = inputDevice.getKeyboardModifiers();
        double newScale = getScale();
        newScale /= 0.8;
        
        if (gui::isAltPressed(keyModifiers))
        {
            scaleToPoint(newScale, modelPoint);
        }
        else
        {
            scale(newScale);
        }
                
    }
    
    virtual void onSecondaryButtonPressed(const gui::InputDevice& inputDevice)
    {
        auto& modelPoint = inputDevice.getModelPoint();
        td::UINT4 keyModifiers = inputDevice.getKeyboardModifiers();
        
        double newScale = getScale();
        newScale *= 0.8;
        
        if (gui::isAltPressed(keyModifiers))
        {
            scaleToPoint(newScale, modelPoint);
        }
        else if (gui::isCmdPressed(keyModifiers))
        {
            scale(1.0);
        }
        else
        {
            scale(newScale);
        }
    }
    
    virtual bool onZoom(const gui::InputDevice& inputDevice)
    {
        auto& modelPoint = inputDevice.getModelPoint();
        double oldScale = getScale();
        double newScale = oldScale * inputDevice.getScale();
        scaleToPoint(newScale, modelPoint);
        return true;        
    }
};
