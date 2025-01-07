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


const float dS = 50;
const float scaleAttached = 0.12f;
const float scaleFighter = 0.82f;

class ViewFighter : public gui::Canvas
{
protected:
    gui::Symbol _symbol;
    gui::Symbol _star;
    gui::SymbolDrawingAttribs _drawingAttribs[2];
    float _freq = 0;
    float _angle = math::FLT_PI;
    bool _useDrawingAttribs = false;
public:
    ViewFighter()
    : _symbol(":jet")
    , _star(":star")
    {
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
    }
    
    void initAngle(float initialAngle)
    {
        _angle = initialAngle;
    }
    
    void incrementAngle(float dTheta)
    {
        _angle += dTheta;
        
        if (_angle > 2 * math::FLT_PI)
            _angle -= 2*math::FLT_PI;
        reDraw();
    }
    
    void setVisualEffects(float freq, bool bUseCustomColor, td::ColorID jetClrID, td::ColorID etfColorID)
    {
        _freq = freq;
        _useDrawingAttribs = bUseCustomColor;
        _drawingAttribs[0].fillColor = jetClrID;
        _drawingAttribs[1].lineColor = etfColorID;
        _drawingAttribs[1].fillColor = etfColorID;
        reDraw();
    }

    float getFrequency() const
    {
        return _freq;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Size sz;
        getSize(sz);
        gui::Point cp(sz.width/2, sz.height/2);
        float radius = math::Min(cp.x, cp.y);
        radius -= dS;
        float angleJet = _angle + math::FLT_PI/2;
        
        cp.x += radius * std::cos(_angle);
        cp.y += radius * std::sin(_angle);
        
        if (_useDrawingAttribs)
            _symbol.draw(cp, angleJet, _drawingAttribs, scaleFighter);
        else
            _symbol.draw(cp, angleJet, nullptr, scaleFighter);
    }
};
