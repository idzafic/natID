//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Timer.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>
#include <gui/Font.h>
#include <cmath>

const float FPS = 50.0f; //desired frame per second
const float dT = 1 / FPS;
const gui::CoordType c_w = 30;
const gui::CoordType c_spaceBetweenLetters = 5;
static float c_scaleLetter = 1;
const float c_mult = 1.2f;

#define SIMPLE2D

class MainView : public gui::Canvas
{
private:
protected:
    gui::Timer _timer;
    gui::Shape _E, _T, _F;
    gui::Point _rotPoint;
    gui::Size _size; //size of this view
    float _angle = 0;
    float _angleE = 0;
    float _angleT = 0;
    float _angleF = 0;
    float _scaleT = 1;

protected:
    
   const gui::Point* rotateAroundXAxis(gui::Point* pts, size_t nPoints, float angle)
    {
        for (int i = 0; i < nPoints; i++)
        {
            pts[i].y = pts[i].y * cos(angle);
        }
        return pts;
    }
    
    void createShapes()
    {
        // E
        gui::Point pointsE[] = { {-c_w, 50},  {-c_w, -50}, {c_w, -50}, {c_w, -40}, {-20, -40}, {-20, -5}, {c_w, -5}, {c_w, 5}, {-20, 5}, {-20, 40}, {c_w, 40}, {c_w, 50}, {-c_w, 50} };
        size_t nPointsE = sizeof(pointsE) / sizeof(gui::Point);
        _E.createPolygon(pointsE, nPointsE);
        // T
        gui::Point pointsT[] = { {-5, 50},  {-5, -40}, {-c_w, -40}, {-c_w, -50}, {c_w, -50}, {c_w, -40}, {5, -40}, {5, 50}};
        size_t nPointsT = sizeof(pointsT) / sizeof(gui::Point);
        _T.createPolygon(pointsT, nPointsT);
        // F
        gui::Point pointsF[] = { {-c_w, 50},  {-c_w, -50}, {c_w, -50}, {c_w, -40}, {-20, -40}, {-20, -5}, {c_w, -5}, {30, 5}, {-20, 5}, {-20, 50}};
        size_t nPointsF = sizeof(pointsF) / sizeof(gui::Point);
        _F.createPolygon(pointsF, nPointsF);
    }
    
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_timer.isRunning())
        {
            auto modifiers = inputDevice.getKeyboardModifiers();
            if (modifiers)
            {
                if (gui::isCtrlPressed(modifiers) || gui::isCmdPressed(modifiers))
                    c_scaleLetter *= c_mult;
            }
            else
                _timer.stop();
        }
        else
        {
            _timer.start();
            _rotPoint = inputDevice.getFramePoint();
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        c_scaleLetter /= c_mult;
    }
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
#ifdef SIMPLE2D
        {
            gui::Point pt(_size.width/2 - c_scaleLetter * 2.5 * c_w - c_spaceBetweenLetters, _size.height/2);
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(pt);
            tr.rotateRad(_angleE);
            tr.scale(c_scaleLetter);
//            if(_angleE < math::FLT_PI)
//                tr.scale((math::FLT_PI + _angleE) / math::FLT_PI);
//            else
//                tr.scale((3 * math::FLT_PI - _angleE) / math::FLT_PI);
            tr.appendToContext();
            _E.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }

        {
            gui::Point pt(_size.width/2, _size.height/2);
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(pt);
//            tr.rotateRad(_angleT);
            tr.scale(c_scaleLetter, c_scaleLetter * _scaleT);
            tr.appendToContext();
            _T.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
        
        {
            gui::Point pt(_size.width/2 + c_scaleLetter * 2.5 * c_w + c_spaceBetweenLetters, _size.height/2);
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(pt);
            tr.rotateRad(_angleF);
            tr.scale(c_scaleLetter);
            tr.appendToContext();
            _F.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
        
#else
        gui::Transformation zoomTr;
        double zoom = 1.; //experiment with this and see impact
        double scrollX = 0;
        double scrollY = 0;
        zoomTr.translate(scrollX, scrollY);
        zoomTr.scale(zoom);
        zoomTr.appendToContext(); //this is transformationfor all objects
        
        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(_rotPoint.x, _rotPoint.y);
            tr.rotateRad(_angle);
            tr.translate(200 - _rotPoint.x, 300 -_rotPoint.y);
            tr.rotateRad(_angleE);
            if(_angleE < math::FLT_PI)
                tr.scale((math::FLT_PI + _angleE) / math::FLT_PI);
            else
                tr.scale((3 * math::FLT_PI - _angleE) / math::FLT_PI);
            tr.appendToContext();
            _E.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }

        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(_rotPoint.x, _rotPoint.y);
            tr.rotateRad(_angle);
            tr.translate(275 - _rotPoint.x, 300 - _rotPoint.y);
            tr.rotateRad(_angleT);
            tr.appendToContext();
            _T.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
        
        {
            gui::Point pointsF[] = { {-30, 50},  {-30, -50}, {30, -50}, {30, -40}, {-20, -40}, {-20, -5}, {30, -5}, {30, 5}, {-20, 5}, {-20, 50} };
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(_rotPoint.x, _rotPoint.y);
            tr.rotateRad(_angle);
            tr.translate(350 - _rotPoint.x, 300 - _rotPoint.y);
            _F.updateNodes(rotateAroundXAxis(pointsF, sizeof(pointsF) / sizeof(gui::Point), _angleF), sizeof(pointsF) / sizeof(gui::Point));
            tr.appendToContext();
            _F.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
#endif
    }
    
    
    bool onTimer(gui::Timer* pTimer) override
    {
        float tNow = _timer.getInterval();
        _angle += 2 * math::FLT_PI * 20/60 * tNow;
        _angleE -= 2 * math::FLT_PI * 20 / 60 * tNow;
        if (_angleE < -2 * math::FLT_PI)
            _angleE += 2 * math::FLT_PI;
        _angleT += 2 * math::FLT_PI * 5 / 60 * dT;
        if (_angleT > 2 * math::FLT_PI)
            _angleT -= 2 * math::FLT_PI;
        _angleF += 2 * math::FLT_PI  * 20 / 60 * tNow;
        if (_angleF > 2 * math::FLT_PI)
            _angleF -= 2 * math::FLT_PI;
        _scaleT = std::cos(_angleT);
        reDraw();
        return true;
    }
    
public:
    MainView()
        : gui::Canvas({ gui::InputDevice::Event::PrimaryClicks,  gui::InputDevice::Event::SecondaryClicks})
        , _timer(this, dT, false)
    {
        enableResizeEvent(true);
        createShapes();
    }
    
   
};
