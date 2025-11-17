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
#include <gui/Texture.h>
#include <random>
#include <chrono>


constexpr gui::CoordType dCtrl = 60, cX = 380, cY = 220, rB = 24.;
//dCtrl - shift for bezier control points
//cX - center (x coordinate) of the shape
//cY - center (Y coordinate) of the shape
//rB - radius of the Bezier shape (when dCtrl = 0)

constexpr bool showBezierConstructingPoints = true;
constexpr bool doNotShowNonBezierPrimitives = true;

//#define HIDE_NON_BEZIER

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
    gui::Image _imgWatters;
    gui::Texture _texture;
    gui::Texture _watters;
    gui::Texture _merlin;
    gui::Texture _hetfield;
    
    gui::Texture _mnist1; //100th image
    gui::Texture _mnist2; //100th image
    
    gui::Symbol _symbol;
    gui::Symbol _symbolBez1;
    gui::Symbol _symbolBez2;
    gui::Point _translate;
    double _scaleX = 1.0;
    double _scaleY = 1.0;
    cnt::PushBackVector<td::Point<td::INT4>> _kernelPoints;
protected:
    
//    bool getModelSize(gui::Size& modelSize) const override
//    {
//        modelSize.width = 1400;
//        modelSize.height = 1000;
////        modelSize.width = 1800;
////        modelSize.height = 1400;
//        return true;
//    }
    
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
#ifndef HIDE_NON_BEZIER
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
#endif
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
        
        gui::Point textureTL(400, 500);
        _texture.draw(textureTL);
        gui::Rect rectWatters(800, 500, 900, 620);
        _imgWatters.draw(rectWatters);
        
        for (const auto& pt : _kernelPoints)
        {
            gui::Size sz;
            _watters.getSize(sz);
            gui::Point ptRect= pt+textureTL;
            gui::Rect r(ptRect, sz);
            gui::Shape::drawRect(r, td::ColorID::Yellow, 2);
        }
        textureTL.x = 100;
        textureTL.y = 600;
        _mnist1.draw(textureTL, 2);
        textureTL.x = 250;
        _mnist2.draw(textureTL, 2);
    }
    
    void fillTextureWithRandomFaces(const gui::Texture textures[2], size_t nTimes)
    {
        // Use a high-quality random number generator (Mersenne Twister)
        // Seed the generator once outside the loop for better randomness
        static std::mt19937 generator((unsigned int) std::chrono::high_resolution_clock::now().time_since_epoch().count());

        const int destWidth = _texture.getWidth();
        const int destHeight = _texture.getHeight();

        for (size_t i = 0; i < nTimes; ++i)
        {
            const gui::Texture& sourceTexture = (i % 2 == 0) ? textures[0] : textures[1];
            
            const int sourceWidth = sourceTexture.getWidth();
            const int sourceHeight = sourceTexture.getHeight();
            
            // 1. Calculate the valid range for the random position.
            // The top-left corner (pos.x, pos.y) must allow the texture to fit.
            // Max valid X = destination width - source width
            const int maxX = destWidth - sourceWidth;
            // Max valid Y = destination height - source height
            const int maxY = destHeight - sourceHeight;

            // If the source texture is larger than the destination, the valid range is negative.
            if (maxX < 0 || maxY < 0) {
                // Cannot place this texture fully. You might choose to skip,
                // or place it at (0, 0) and let 'replace' handle the clipping.
                // We'll skip for a true 'fit'.
                continue;
            }

            // 2. Define the distributions for X and Y coordinates
            // The range is inclusive: [0, maxX] and [0, maxY]
            std::uniform_int_distribution<int> distX(0, maxX);
            std::uniform_int_distribution<int> distY(0, maxY);

            // 3. Generate random coordinates
            td::INT4 randomX = distX(generator);
            td::INT4 randomY = distY(generator);
            
            // 4. Perform the replacement
            _texture.replace({randomX, randomY}, sourceTexture);
        }
    }
    
    void extractTopMatches(std::map<float, td::Point<td::INT4>>& matches)
    {
        if (matches.empty())
            return;

        // Copy matches sorted descending
        std::vector<std::pair<float, td::Point<td::INT4>>> sortedMatches;
        for (auto it = matches.rbegin(); it != matches.rend(); ++it)
            sortedMatches.push_back(*it);

//        std::vector<td::Point<td::INT4>> results;
        _kernelPoints.reserve(5);
        _kernelPoints.push_back(sortedMatches[0].second);

        float prevDelta = 0.f;
        float sumDelta = 0.f;
        size_t countDelta = 0;

        for (size_t i = 1; i < sortedMatches.size(); ++i)
        {
            float delta = sortedMatches[i-1].first - sortedMatches[i].first;

            // Compute average of previous deltas
            float avgDelta = countDelta > 0 ? sumDelta / countDelta : delta;

            // Detect jump: delta significantly bigger than previous deltas
            if (delta > 1.5f * avgDelta && i > 1)
                break;

            _kernelPoints.push_back(sortedMatches[i].second);

            sumDelta += delta;
            ++countDelta;
        }

        return;
    }

public:
    ViewCanvas()
    : gui::Canvas({gui::InputDevice::Event::Zoom}) //scrolling and zooming without scrollbars
    , _img(":TransCost")
    , _imgWatters(":watters")
    , _texture({400, 500}, {0,0,0,255})
    , _mnist1({28, 28}, {0,0,0,255}, 1)   //if alpha = 0 (transparent for black color)
    , _mnist2({28, 28}, {0,0,0,255}, 1)
    , _watters(":watters")
    , _merlin(":merlin")
    , _hetfield(":hetfield")
    , _symbol(":Xfmr4")
    , _symbolBez1(":Bez1")
    , _symbolBez2(":Bez2")
    {
        gui::Texture faces[2] = {_merlin, _hetfield};
        fillTextureWithRandomFaces(faces, 5000);
        _texture.replace({5,105}, _watters);
        _texture.replace({160, 170}, _watters);
        _texture.replace({280, 300}, _watters);
        _texture.addNoise({140, 140, 140, 0});
        _texture.adjustBrightness(-0.33f);
        setClipsToBounds();
        registerForScrollEvents(); //Event::Zoom must be enabled, otherwise no scroll events
        
        //MNIST Image test
        {
            fo::fs::path home;
            mu::getHomePath(home);
            fo::fs::path mnistTestImages = home / "other_bin/TestData/AI/MNIST/TrainIDX3";
            td::String fn = mnistTestImages.string();
            td::Size<td::INT4> imgSize;
            td::BYTE bytesPerPixel = 0;
            std::ifstream f;
            int nImages = gui::Texture::openMNISTImageCollection(fn, f, imgSize, bytesPerPixel);
            if (nImages == 60000)
            {
                _mnist1.loadMNISTImage(f, 100);
                _mnist2.loadMNISTImage(f, 200);
            }
        }

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
        _shapeLines.createLines(&points[0], 4, lw, td::LinePattern::DashDotDot);
        
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
        
        //test correlations
        gui::Size textureSize;
        auto nW = _texture.getWidth();
        auto nH = _texture.getHeight();
        std::map<float, td::Point<td::INT4>> matchesAboveTreshold;
        float treshold = 0.85f;
        float maxCorrellation = 0;
        td::Point<td::INT4> ptWithMaxCorr;
        for (td::INT4 iRow=0; iRow < nH -_watters.getHeight()+5; ++iRow)
        {
            for (td::INT4 iCol=0; iCol < nW-_watters.getWidth()+5; ++iCol)
            {
                td::Point pt(iCol, iRow);
                float f = _texture.calculateNormalizedCrossCorrelation(pt, _watters);
                if (f > maxCorrellation)
                {
                    maxCorrellation = f;
                    ptWithMaxCorr = pt;
                }
                if (f >= treshold)
                    matchesAboveTreshold[f] = pt;
            }
        }
        
        size_t nMatches = matchesAboveTreshold.size();
        if (nMatches > 3)
            extractTopMatches(matchesAboveTreshold);
        else
        {
            if (nMatches > 0)
            {
                _kernelPoints.reserve(nMatches);
                size_t i=0;
                for (auto it : matchesAboveTreshold)
                {
                    _kernelPoints.push_back(it.second);
                }
            }
            
        }
    }
};
