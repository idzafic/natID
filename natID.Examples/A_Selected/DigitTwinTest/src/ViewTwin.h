//
//  Created by Izudin Dzafic on 04/08/2023.
//  Copyright © 2023 IDz. All rights reserved.
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
#include <gui/DrawableString.h>
#include <thread/BoundedBuffer.h>
#include "SimulationParams.h"
#include <sc/SCLib.h>
#include <mu/Timer.h>
#include "SolutionBuffer.h"
#include <thread>
#include <sc/TokenizedModel.h>


const float scalePendulum = 1.0f;

const gui::CoordType c_pixelPerMeter = 600;
const gui::CoordType c_dy = 50;
const gui::CoordType c_dx = 5;
static char s_tmpRes[256];

class ViewTwin : public gui::Canvas
{
protected:
    sc::Naming _naming;
    gui::Symbol _pendulum;
    gui::SymbolDrawingAttribs _drawingAttribs[2];
    gui::DrawableString _x;
    gui::DrawableString _y;
    gui::DrawableString _res;
    SimulationParams* _pSimulationParams = nullptr;
    sc::ISolver<SOLVERTYPE>* _pSolver = nullptr;
    SolutionBuffer _solutionConsumer;
    std::thread _thread;
    gui::Point _ptRes;
    const char* _pStrRes;
    bool _useDrawingAttribs = false;
    
private:
    bool initModel()
    {
        if (_pSolver)
        {
            _pSolver->release();
            _pSolver = nullptr;
        }
        
        //init symbolic computation library
        sc::reserveNamings(1);
        sc::setDefaultDataForAllNamings();
        sc::setDefaultDataForNaming(0);
        
#ifdef USE_ODE_MODEL
        //ODE-modl
        td::String modlModelFileName = getResFileName("pendODEModl");
        _pSolver = sc::createDblODESolver(nullptr);
        
#else
        //DAE modl
        td::String modlModelFileName = getResFileName("pendDAEModl");
        _pSolver = sc::createDblDAESolver(nullptr, 50);
#endif
        
        if (_pSolver == nullptr)
            return false;
        
        cnt::SafeFullVector<td::Variant> headerAttribs; // = me.getHeaderAttribs();
        sc::ILog* pLog = nullptr;
        sc::TokenizedModel tm(pLog, headerAttribs, gui::tr, _naming, 0);
        
#ifdef USE_MEMORY_MODELS
        td::String strModelContent = fo::loadFileContent(modlModelFileName);
        if (!tm.loadFromString(strModelContent))
            return false;
#else
        if (!tm.loadFromFile(modlModelFileName))
            return false;
#endif
        
        if (!_pSolver->init(&_naming, &tm))
            return false;
        
        return true;
    }
    
    void worker(double dT, int nSkip)
    {
        double t0 = 0;
        double t1 = -1;
#ifdef USE_ODE_MODEL
        int iTheta = _solutionConsumer.getThetaIndex();
        double theta0 = std::asin(_pSimulationParams->x0 / _pSimulationParams->l);
        _pSolver->setVariableValue(iTheta, theta0);
        int iOmega = _solutionConsumer.getOmegaIndex();
        _pSolver->setVariableValue(iOmega, 0);        
#else
        int ix = _solutionConsumer.getXIndex();
        _pSolver->setVariableValue(ix, _pSimulationParams->x0);
//        int iy=_solutionConsumer.getYIndex();
//        _pSolver->setVariableValue(iy, _pSimulationParams->y0);
        _pSolver->setVariableValue("v_x", 0);
        _pSolver->setVariableValue("v_y", 0);
#endif
        _pSolver->setParamValue("m", _pSimulationParams->m);
        _pSolver->setParamValue("k", _pSimulationParams->k);
        _pSolver->setParamValue("g", _pSimulationParams->g);
        
        _pSolver->solve(t0, dT, t1, &_solutionConsumer, nSkip);
    }
    
public:
    bool start()
    {        
        double dT = 1e-4;
//        double T = 2*math::DBL_PI*std::sqrt(1/9.81);
        
        int nSkip = 20;
        
        int deltaInMs = int(nSkip * dT * 1000);
        
        _solutionConsumer.init(deltaInMs, _pSolver);
        
        _thread = std::thread(&ViewTwin::worker, this, dT, nSkip);
        
        return true;
    }
    
    void stop()
    {
        _pSolver->stop();
        _solutionConsumer.removeAllEntries(); //if simulation is blocked, wake up
        if (_thread.joinable())
        {
            _thread.join();
        }
        _solutionConsumer.removeAllEntries();
    }
    
protected:
    void drawCoordinates(const gui::Size& sz)
    {
        gui::CoordType arrH = 6;
        gui::CoordType arrW = 10;
        gui::CoordType holderLen = 40;
        
        //x coordinates
        gui::Point p1(c_dx, c_dy);
        gui::Point p2(sz.width - c_dx, c_dy);
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x = p2.x - arrW;
        p1.y = p2.y - arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.y = p2.y + arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x = p2.x - 2*arrW;
        _x.draw(p1, gui::Font::ID::SystemLargest, td::ColorID::SysText);
        
        //y coordinate
        p1.x = sz.width / 2;
        p1.y = c_dx;
        p2.x = p1.x;
        p2.y = sz.height - c_dx;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x = p2.x - arrH;
        p1.y = p2.y - arrW;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x = p2.x + arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x -= 3*arrW;
        p1.y = p2.y - 3.5*arrW;
        _y.draw(p1, gui::Font::ID::SystemLargest, td::ColorID::SysText);
        
        //holder
        p1.x = sz.width / 2 - holderLen;
        p1.y = c_dy;
        p2 = p1;
        p2.x += 2*holderLen;
        for (size_t i=0; i<12; ++i)
        {
            gui::Shape::drawLine(p1, p2, td::ColorID::Yellow, 4);
            p1.x+= holderLen/6;
            p2 = p1;
            p2.x += holderLen/4;
            p2.y -= holderLen/2;
        }
        
        p1.x = sz.width / 2 - 3;
        p1.y = c_dy-3;
        gui::Size rsz(6,6);
        gui::Rect rHoldPoint(p1, rsz);
        gui::Shape::drawRect(rHoldPoint, td::ColorID::DarkRed);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Size sz;
        getSize(sz);
        
        drawCoordinates(sz);
        
        //draw pendulum
        if (_pSimulationParams->animation)
        {
            td::DateTime currTime;
            currTime.now();
            struct SolEntry val;
            if (_solutionConsumer.get(currTime, val))
            {
                gui::Point pt1(sz.width/2, c_dy);
                gui::Point pt2(sz.width/2+ val.x*c_pixelPerMeter, c_dy + val.y*c_pixelPerMeter);
                gui::Shape::drawLine(pt1, pt2, _pSimulationParams->fontColor, 2);
                
                td::Date datum = val.dt.getDate();
                td::Time time = val.dt.getTime();
                SNPRINTF(s_tmpRes, 256, _TRUNCATE, "d=%d.%d.%d\nt=%02d:%02d:%02d.%03d\nx=%.2fm\ny=%.2fm\nv=%.2fm/s", datum.getDay(), datum.getMonth(), datum.getYear(), time.getHour(), time.getMinute(), time.getSecond(), time.getMilisecond(), val.x, val.y, val.v);
                _res = _pStrRes;
                _res.draw(_ptRes, gui::Font::ID::SystemNormal, td::ColorID::SysText);
                float angle = (float) std::asin(val.x/_pSimulationParams->l);
                
                if (_useDrawingAttribs)
                    _pendulum.draw(pt2, -angle, _drawingAttribs, scalePendulum);
                else
                    _pendulum.draw(pt2, -angle, nullptr, scalePendulum);
            }
            else
            {
                _pSolver->stop();
            }
        }
        else
        {
            gui::Point pt1(sz.width/2, c_dy);
            gui::Point pt2(sz.width/2+_pSimulationParams->x0*c_pixelPerMeter, c_dy+_pSimulationParams->y0*c_pixelPerMeter);
            gui::Shape::drawLine(pt1, pt2, _pSimulationParams->fontColor, 2);
            
            td::Date datum;
            datum.now();
            td::Time time;
            time.now();
            SNPRINTF(s_tmpRes, 256, _TRUNCATE, "d=%d.%d.%d\nt=%02d:%02d:%02d.%03d\nx=%.2fm\ny=%.2fm\nv=0.00m/s", datum.getDay(), datum.getMonth(), datum.getYear(), time.getHour(), time.getMinute(), time.getSecond(), time.getMilisecond(), _pSimulationParams->x0, _pSimulationParams->y0);
            _res = _pStrRes;
            _res.draw(_ptRes, gui::Font::ID::SystemNormal, td::ColorID::SysText);
            
            float angle = (float) std::asin(_pSimulationParams->x0/_pSimulationParams->l);
            
            if (_useDrawingAttribs)
                _pendulum.draw(pt2, -angle, _drawingAttribs, scalePendulum);
            else
                _pendulum.draw(pt2, -angle, nullptr, scalePendulum);
        }
    }
    
public:
    ViewTwin()
    : _pendulum(":pendulum")
    , _x("x")
    , _y("y")
    , _ptRes(c_dx, c_dy+5)
    {
        _pStrRes= &s_tmpRes[0];
       _res = _pStrRes;
        
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

        if (!initModel())
        {
            gui::Alert::show(tr("initErr"), tr("initErrDetails"));
        }
    }
    
    ~ViewTwin()
    {
        if (_pSolver)
            _pSolver->release();
    }
    
    void setSimulationParameters(SimulationParams* pSimulationParams)
    {
        _pSimulationParams = pSimulationParams;
    }
    
    void setVisualEffects()
    {
        assert(_pSimulationParams);
        _useDrawingAttribs = _pSimulationParams->useCustomColors;
        _drawingAttribs[0].fillColor = _pSimulationParams->fillColor;
        _drawingAttribs[1].lineColor = _pSimulationParams->fontColor;
        _drawingAttribs[1].fillColor = _pSimulationParams->fontColor;
        reDraw();
    }
    
    void update()
    {
        reDraw();
    }
};
