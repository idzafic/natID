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
#include <math/Constants.h>
#include <math/math.h>
#include <gui/Sound.h>
#include <gui/DrawableString.h>
#include <gui/Font.h>
#include <td/Timer.h>

const double cInitialAngle = math::DBL_PI;
const float dS = 50;
const float scaleAttached = 0.12f;
const float scaleFighter = 0.82f;


class ViewFighter : public gui::Canvas
{
protected:
    gui::Symbol _symbol;
    gui::Symbol _star;
    gui::Sound _bomb;
    gui::Sound _machineGun;
    gui::Sound _flightSound;
    gui::Size _size;
    gui::SymbolDrawingAttribs _drawingAttribs[2];
    gui::Font _font;
    gui::DrawableString _strInfo;
    td::Timer<false> _timer; //measures time between start and current frame
    double _angle = cInitialAngle;
    double _prevAngle = cInitialAngle;
    gui::CoordType _infoHeight = 0;
    char _bufferFPF[16];
    int _nCharsFPS = 0;
    float _frequency = 0.1f;
    int _nFrames = 0;
    
    bool _useDrawingAttribs = false;
    bool _bMachineGun = false;
protected:
    bool onKeyPressed(const gui::Key& key) override
    {
        //call base class
         if (gui::Canvas::onKeyPressed(key))
            return true;
        auto keyType = key.getType();
        if (keyType == gui::Key::Type::ASCII)
        {
            char ch = key.getChar();
            if (ch == 'b' || ch == 'B')
            {
                bomb();
                return true;
            }
            else if (ch == 'm' || ch == 'M')
            {
                if (!_bMachineGun)
                {
                    _machineGun.play(true);
                    _bMachineGun = true;
                }
                    
                return true;
            }
        }
        return false;
    }
    
    bool onKeyReleased(const gui::Key& key) override
    {
        if (key.getType() == gui::Key::Type::ASCII)
        {
            char ch = key.getChar();
            if (ch == 'm' || ch == 'M')
            {
                if (_bMachineGun)
                {
                    _bMachineGun = false;
                    _machineGun.stop();
                }
                
                return true;
            }
        }
        return false;
    }
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        if (isAnimating())
        {
            double dT = _timer.getDurationInSeconds();
            _angle = _prevAngle + 2 * math::FLT_PI * _frequency * dT;
            if (_angle > 2 * math::FLT_PI)
                _angle -= 2*math::FLT_PI;
            if (dT > 0.5)
            {
                _prevAngle = _angle;
                _timer.start();
                double FPS = ++_nFrames / dT;
                _nCharsFPS = SNPRINTF(_bufferFPF, 16, _TRUNCATE, "FPS:%.2f", FPS);
                _nFrames = 0;
            }
            else
            {
                ++_nFrames;
            }
        }
       
        //gui::Size sz;
        //getSize(sz);
        gui::Point cp(_size.width/2, _size.height/2);
        float radius = math::Min(cp.x, cp.y);
        radius -= dS;
        float angleJet = _angle + math::FLT_PI/2;
        
        cp.x += radius * std::cos(_angle);
        cp.y += radius * std::sin(_angle);
        
        if (_useDrawingAttribs)
            _symbol.draw(cp, angleJet, _drawingAttribs, scaleFighter);
        else
            _symbol.draw(cp, angleJet, nullptr, scaleFighter);
        
        gui::CoordType verticalExtra=12; //extra vertical space for info

        gui::Rect rectForTextInfo(0, _size.height-_infoHeight-verticalExtra, _size.width, _size.height);
        float alpha = 0.3f;
        gui::Shape::drawRect(rectForTextInfo, alpha, td::ColorID::Blue);
        _strInfo.draw(rectForTextInfo, &_font, td::ColorID::Yellow, td::TextAlignment::Center, td::VAlignment::Center, td::TextEllipsize::End);
        
        if (_nCharsFPS > 0)
        {
            gui::Point pt(5,5);
            gui::DrawableString::draw(_bufferFPF, _nCharsFPS, pt, gui::Font::ID::SystemLargerBold, td::ColorID::SysText);
        }
    }
    
public:
    ViewFighter()
    : Canvas({gui::InputDevice::Event::Keyboard}) //enable keyboard events
    , _symbol(":jet")
    , _star(":star")
    , _bomb(":bomb")
    , _machineGun(":mGun")
    , _flightSound(":flight")
    , _strInfo(tr("info"))
    {
        setPreferredFrameRateRange(60, 60);
        enableResizeEvent(true);
        _symbol.attachSymbol(0, 0, _star, scaleAttached);
        _symbol.attachSymbol(0, 1, _star, scaleAttached);
        
        _drawingAttribs[0].displayGroup = 1;
        _drawingAttribs[0].overideLineWidth = 1;
        _drawingAttribs[0].overideLineColor = 1;
        _drawingAttribs[0].setLineWidth(2);
        _drawingAttribs[0].lineColor = td::ColorID::SysText;
        _drawingAttribs[0].overideFillColor = 1;
        _drawingAttribs[0].fillColor = td::ColorID::DarkBlue;
        
        _drawingAttribs[1].displayGroup = 1;
        _drawingAttribs[1].overideLineWidth = 1;
        _drawingAttribs[1].overideLineColor = 1;
        _drawingAttribs[1].setLineWidth(2);
        _drawingAttribs[1].lineColor = td::ColorID::Yellow;
        _drawingAttribs[1].overideFillColor = 1;
        _drawingAttribs[1].fillColor = td::ColorID::Yellow;
        
        _font.create("Arial", 14.0f, gui::Font::Style::BoldItalic, gui::Font::Unit::LogicalPixel);
        gui::Size szInfo;
        _strInfo.measure(&_font, szInfo);
        _infoHeight = szInfo.height;
    }
    
    void initAngle(float initialAngle)
    {
        _angle = initialAngle;
    }
    
    void setVisualEffects(float frequency, bool bUseCustomColor, td::ColorID jetClrID, td::ColorID etfColorID)
    {
        _frequency = frequency;
        _useDrawingAttribs = bUseCustomColor;
        _drawingAttribs[0].fillColor = jetClrID;
        _drawingAttribs[1].lineColor = etfColorID;
        _drawingAttribs[1].fillColor = etfColorID;
        reDraw();
    }
    
    void bomb()
    {
        _bomb.play();
    }
    
    void start(bool bRestartFromInitialPosition)
    {
        if (bRestartFromInitialPosition)
            _prevAngle = cInitialAngle;
        else
            _prevAngle = _angle;
            
        _flightSound.play(true);
        _timer.start();
        startAnimation();
    }
    
    void stop()
    {
        stopAnimation();
        _flightSound.stop();
        _bomb.stop(); //if bomb effect was started
    }
    
    bool isRunning() const
    {
        return isAnimating();
    }
};
