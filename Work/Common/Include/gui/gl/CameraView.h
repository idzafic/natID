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
#include <gui/gl/Camera.h>

namespace gui
{
namespace gl
{

class NATGL_API CameraView : public View
{
    
protected:
    glm::mat4 _perspectiveMatrix;
    gui::gl::Size _frameSize;
    Camera* _pCurrentCamera = nullptr;
    float _fiedOfView = 45.0f; // Field of view in degrees
    float _nearClip = 0.01f; // Near clipping plane
    float _farClip = 100.0f; // Far clipping plane
protected:
    void createContext(const gl::Context& context);
    bool onKeyPressed(const Key& key) override;
    bool onKeyReleased(const Key& key) override;
    bool onZoom(const gui::InputDevice& inputDevice) override;
    bool onScroll(const gui::InputDevice& inputDevice) override;
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
//    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    virtual void scaleFieldOfView(float scale);
    void onResize(const gui::Size& newSize) override;
public:
    CameraView();
    
    void setCamera(Camera* pCamera);
    Camera* getCamera();
    glm::mat4 getCameraMatrix() const;
    float getFieldOfViewScale() const;
};

} //namespace gl
} //namespace gui
