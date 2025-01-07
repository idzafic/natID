// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <cnt/PushBackVector.h>

namespace gui
{
namespace gl
{

class NATGL_API OrthoView : public gui::gl::View
{
    glm::mat4 _orthoProjection; //transformation matrix
    gui::Point _modelRefPt; //for noninverted: bottom left corner; for inverted: top left point in model coordinates
    float _width; //width in logical pixels
    float _height; //height in logical pixels
    double _zoom = 1.0;
    double _minZoom = 0.01;
    double _maxZoom = 100;
    
    td::BYTE _dragging = 0;
    td::BYTE _invertY = 0;
    
    void adjustOrthoProjectionMatrix();
protected:
    OrthoView();
    void setZoom(double zoom);
    double getZoom() const override;
    void setZoomLimits(double minZoom, double maxZoom);
    
    const gui::Point& getOrigin() const;
    
    const glm::mat4& getProjection() const;
    gui::Point getModelPoint(const gui::Point& framePoint) const;
    void onResize(const gui::Size& newSize) override;
    bool onKeyPressed(const Key& key) override;
//    bool onKeyReleased(const Key& key) override;
    bool onZoom(const gui::InputDevice& inputDevice) override;
    bool onScroll(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    void createContext(bool invertYAxis = false);
//    bool triangulate(const cnt::PushBackVector<gui::gl::Point> vertices, cnt::PushBackVector<td::INT4>& indices) const;
};

} //namespace gl
} //namespace gui
