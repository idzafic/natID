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

class ViewportCanvasHelper;

class NATGUI_API ViewportCanvas : public Canvas
{
    friend class ViewportCanvasHelper;
    
protected:
    gui::Size _size;
    gui::Point _modelRefPt;
    double _minZoom = 0.001;
    double _maxZoom = 1000.;
    td::BYTE _invertY = 0;
    td::BYTE _dragging = 0;
public:
    ViewportCanvas();
    ~ViewportCanvas();
    
    virtual void onZoomChanged();
    virtual void onOriginChanged();
    
    void setZoom(double zoom);
    double getZoom() const;
    void setZoomLimits(double minZoom, double maxZoom);
    
    const gui::Point& getOrigin() const; //in model coordinates
    void setOrigin(const gui::Point& tl);   //in model coordinates
    
    Canvas::Placement getPlacement() const override final;
    gui::Point getModelPoint(const gui::Point& framePoint) const;
    void onResize(const gui::Size& newSize) override;
    bool onKeyPressed(const Key& key) override;
    //bool onKeyReleased(const Key& key) override;
    bool onZoom(const gui::InputDevice& inputDevice) override;
    bool onScroll(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    void setModelRefPoint(const gui::Point& refPt);
};

}
