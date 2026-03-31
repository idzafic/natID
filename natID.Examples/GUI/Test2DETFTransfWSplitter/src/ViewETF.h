#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Transformation.h>
#include <math/Constants.h>
#include <math/math.h>
#include <gui/InputDevice.h>
#include <functional>
#include <gui/Sound.h>
#include <cmath>
#include "common.h"

const float EPS = 0.0001;
const gui::CoordType c_w = 30;
const gui::CoordType c_h = 50;
const gui::CoordType c_spaceBetweenLetters = 10;
const float c_lineWidth = 2;
const float c_mult = 1.15;
const float c_timeRestore = 1;

const float FPS = 50.0f; //desired frame per second
const float dT = 1 / FPS;

class ViewETF : public gui::Canvas
{
protected:
    gui::Shape _E, _T, _F;
    float _angle, _angleE, _angleT, _angleF;
    gui::Point _viewCentralPoint;
    bool _bombActivated= false;
    float _bombScaleFactor = 1;
    td::INT4 _stepsFromExplosion = 0;
    td::ColorID _fillColor, _lineColor;
    gui::Sound _bomb;
    Direction _rotateDirection;
    float _frequency;
    double _normRotationRadius;
    std::function<bool(void)> _fcnIsRunning;

    const gui::Point* rotateAroundXAxis(gui::Point* pts, size_t nPoints, float angle)
    {
        for (int i = 0; i < nPoints; i++)
        {
            pts[i].y = pts[i].y * std::cos(angle);
        }
        return pts;
    }
    void createShapes()
    {
        // E
        gui::Point pointsE[] = { {-c_w, c_h},  {-c_w, -c_h}, {c_w, -c_h}, {c_w, -40}, {-20, -40}, {-20, -5}, {c_w, -5}, {c_w, 5}, {-20, 5}, {-20, 40}, {c_w, 40}, {c_w, c_h}, {-c_w, c_h} };
        size_t nPointsE = sizeof(pointsE) / sizeof(gui::Point);
        _E.createPolygon(pointsE, nPointsE);
        // T
        gui::Point pointsT[] = { {-5, c_h},  {-5, -40}, {-c_w, -40}, {-c_w, -c_h}, {c_w, -c_h}, {c_w, -40}, {5, -40}, {5, c_h} };
        size_t nPointsT = sizeof(pointsT) / sizeof(gui::Point);
        _T.createPolygon(pointsT, nPointsT);
        // F
        gui::Point pointsF[] = { {-c_w, c_h},  {-c_w, -c_h}, {c_w, -c_h}, {c_w, -40}, {-20, -40}, {-20, -5}, {c_w, -5}, {c_w, 5}, {-20, 5}, {-20, c_h} };
        size_t nPointsF = sizeof(pointsF) / sizeof(gui::Point);
        _F.createPolygon(pointsF, nPointsF);
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        //call base class
        if (gui::Canvas::onKeyPressed(key))
            return true;
        
        if (key.getType() == gui::Key::Type::ASCII)
        {
            char ch = key.getChar();
            if (ch == ' ')
            {
                if(_fcnIsRunning())   
                    explosion();
                else
                    showAlert(tr("closeNOK"), tr("cloeErr"));
                return true;
            }
        }
        return false;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Transformation zoomTr;
        double zoom = 1.; //experiment with this and see impact
        double scrollX = 0;
        double scrollY = 0;
        zoomTr.translate(scrollX, scrollY);
        zoomTr.scale(zoom);
        zoomTr.appendToContext(); //this is transformationfor all objects

        if (!_bombActivated)
        {
            // primary axis
            double primaryAxisLength;
            if (_viewCentralPoint.x > _viewCentralPoint.y)
                primaryAxisLength = _viewCentralPoint.y - 2 * 1.5 * c_h - c_w - c_spaceBetweenLetters;
            else
                primaryAxisLength = _viewCentralPoint.x - 2 * 2 * c_w - c_w - c_spaceBetweenLetters;

            {
                // scaling factor for E
                float scalingFactor;
                if (std::pow(-1, (int)_rotateDirection) * _angleE < math::FLT_PI)
                    scalingFactor = (math::FLT_PI + std::pow(-1, (int)_rotateDirection) * _angleE) / math::FLT_PI;
                else
                    scalingFactor = (3 * math::FLT_PI - std::pow(-1, (int)_rotateDirection) * _angleE) / math::FLT_PI;
                gui::Transformation::saveContext();
                gui::Transformation tr;
                tr.translate(_viewCentralPoint.x, _viewCentralPoint.y);
                tr.rotateRad(_angle);
                tr.translate(-2 * c_w * scalingFactor - primaryAxisLength * _normRotationRadius - c_spaceBetweenLetters, 0);
                tr.rotateRad(_angleE);
                tr.scale(scalingFactor);
                tr.appendToContext();
                _E.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::restoreContext();
            }

            {
                gui::Transformation::saveContext();
                gui::Transformation tr;
                tr.translate(_viewCentralPoint.x, _viewCentralPoint.y);
                tr.rotateRad(_angle);
                tr.translate(-primaryAxisLength * _normRotationRadius, 0);
                tr.rotateRad(_angleT);
                tr.appendToContext();
                _T.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::restoreContext();
            }

            {
                gui::Transformation::saveContext();
                gui::Transformation tr;
                tr.translate(_viewCentralPoint.x, _viewCentralPoint.y);
                tr.rotateRad(_angle);
                tr.translate(2 * c_w - primaryAxisLength * _normRotationRadius + c_spaceBetweenLetters, 0);
                tr.rotateRad(_angleF);
                tr.appendToContext();
                _F.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::restoreContext();
            }
        }
        else
        {
            _bombScaleFactor *= c_mult;
            if ( (_bombScaleFactor * 3 * c_w > 2 * _viewCentralPoint.x) || (_bombScaleFactor * 3 * c_h > 2 * _viewCentralPoint.y))
            {
                _bombActivated = false;
                _bombScaleFactor = 0;
                _bomb.play();
                _angle = 0;
                _angleE = 0;
                _angleT = 0;
                _angleF = 0;
                _stepsFromExplosion = 1;

            }
            {
                gui::Transformation::saveContext();
                gui::Transformation tr;
                tr.translate(-2 * _bombScaleFactor  *  c_w  - c_spaceBetweenLetters + _viewCentralPoint.x, _viewCentralPoint.y);
                tr.scale(_bombScaleFactor);
                tr.appendToContext();
                _E.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::restoreContext();
            }
            {
                gui::Transformation::saveContext();
                gui::Transformation tr;
                tr.translate(_viewCentralPoint.x, _viewCentralPoint.y);
                tr.scale(_bombScaleFactor);
                tr.appendToContext();
                _T.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::restoreContext();
            }
            {
                gui::Transformation tr;
                tr.translate(2 * _bombScaleFactor * c_w + c_spaceBetweenLetters + _viewCentralPoint.x, _viewCentralPoint.y);
                tr.scale(_bombScaleFactor);
                tr.appendToContext();
                _F.drawFillAndWire(_fillColor, _lineColor, c_lineWidth);
                gui::Transformation::saveContext();
            }
        }
    }
    
    void onResize(const gui::Size& newSize) override
    {
        _viewCentralPoint = { newSize.width / 2, newSize.height / 2 };
    }
    
public:
    ViewETF(std::function<bool()>& fcnIsRunning)
        : gui::Canvas( { gui::InputDevice::Event::Keyboard } )
        , _angleE(0)
        , _angleT(0)
        , _angleF(0)
        , _angle(0)
        , _fillColor(td::ColorID::DarkBlue)
        , _lineColor(td::ColorID::White)
        , _frequency(20)
        , _bomb(":bomb")
        , _rotateDirection(Direction::CLOCKWISE)
        , _normRotationRadius(0.5)
        , _fcnIsRunning(fcnIsRunning)
    {
        enableResizeEvent(true);
        createShapes();
        gui::Size newSize;
        getSize(newSize);
        _viewCentralPoint = { newSize.width / 2, newSize.height / 2 };
    }

    
    void changeSettings(td::ColorID fillColor, td::ColorID lineColor, float f, Direction direction, double normRotationRadius)
    {
        _frequency = f;
        _fillColor = fillColor;
        _lineColor = lineColor;
        _rotateDirection = direction;
        _normRotationRadius = normRotationRadius;
        setFocus();
        reDraw();
    }
    
    void explosion()
    {
        if(!_bombActivated && !_stepsFromExplosion)
            _bombActivated = true;
    }
    
    void reDrawScene(float tNow)
    {
        if (!_bombActivated && _stepsFromExplosion && _stepsFromExplosion * dT < c_timeRestore)
        {
            _stepsFromExplosion++;
        }
        else if (fabs(_stepsFromExplosion * dT - c_timeRestore) < EPS)
        {
            _stepsFromExplosion = 0;
            _bombScaleFactor = 1;
        }
        else
        {
            float dAngle = 2 * math::FLT_PI * _frequency / 60 * tNow;
            _angle += std::pow(-1, (int)_rotateDirection) * dAngle;
            _angleE += std::pow(-1, (int)_rotateDirection) * dAngle;;
            if (_angleE > 2 * math::FLT_PI)
                _angleE -= 2 * math::FLT_PI;
            if (_angleE < -2 * math::FLT_PI)
                _angleE += 2 * math::FLT_PI;
            _angleF -= 2 * std::pow(-1, (int)_rotateDirection) * dAngle;
            reDraw();
        }
    }
};
