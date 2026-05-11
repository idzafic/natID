// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include "Canvas.h"

namespace gui
{

class NATGUI_API ViewportCanvas : public Canvas
{
public:

    struct ViewState
    {
        double originX = 0.0;
        double originY = 0.0;
        double zoom = 1.0;
    };

protected:

    ViewState _view;

    bool _panInProgress = false;
    gui::Point _lastMousePos;

    double _minZoom = 0.01;
    double _maxZoom = 1000.0;

public:

    ViewportCanvas();
    virtual ~ViewportCanvas();

    //
    // coordinate conversion
    //

    gui::Point modelToView(const gui::Point& pt) const;
    gui::Point viewToModel(const gui::Point& pt) const;

    //
    // viewport
    //

    void setOrigin(double x, double y);
    void getOrigin(double& x, double& y) const;

    void pan(double dxPixels, double dyPixels);

    void setZoom(double zoom);
    double getZoom() const;

    void zoomAtPoint(double factor,
                     const gui::Point& viewPoint);

    void zoomToFit(const gui::Rect& modelBounds,
                   double margin = 10.0);

    void centerAt(const gui::Point& modelPoint);

    //
    // visible area
    //

    void getVisibleModelRect(gui::Rect& r) const;

protected:

    //
    // drawing
    //

    virtual void drawModel() = 0;

    void onDraw(const gui::Rect& invalidRect) override;

    //
    // mouse interaction
    //

//    bool onMousePressed(const gui::MouseEvent& e) override;
//    bool onMouseReleased(const gui::MouseEvent& e) override;
//    bool onMouseMoved(const gui::MouseEvent& e) override;
//    bool onMouseWheel(const gui::MouseEvent& e) override;

protected:

    void clampZoom();
};

}
