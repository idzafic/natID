// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CameraView.h
    @brief Declares CameraView, a perspective OpenGL view driven by a Camera instance. */
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

/// @brief An OpenGL view that renders with a perspective projection controlled by an attached Camera.
class NATGL_API CameraView : public View
{

protected:
    glm::mat4 _perspectiveMatrix;          ///< Current perspective projection matrix.
    gui::gl::Size _frameSize;              ///< Current frame (viewport) dimensions in logical pixels.
    Camera* _pCurrentCamera = nullptr;     ///< Pointer to the active Camera; may be nullptr if none is set.
    float _fiedOfView = 45.0f;             // Field of view in degrees
    float _nearClip = 0.01f;               // Near clipping plane
    float _farClip = 100.0f;               // Far clipping plane
protected:
    /// @brief Creates an OpenGL context with the given configuration, setting up the perspective projection.
    /// @param context The OpenGL context descriptor.
    void createContext(const gl::Context& context);
    /// @brief Forwards a key-press event to the attached Camera for movement processing.
    /// @param key The key that was pressed.
    /// @return True if the event was consumed.
    bool onKeyPressed(const Key& key) override;
    /// @brief Forwards a key-release event to the attached Camera.
    /// @param key The key that was released.
    /// @return True if the event was consumed.
    bool onKeyReleased(const Key& key) override;
    /// @brief Handles a zoom gesture by scaling the field of view.
    /// @param inputDevice Input device carrying the zoom data.
    /// @return True if the event was consumed.
    bool onZoom(const gui::InputDevice& inputDevice) override;
    /// @brief Handles a scroll gesture by forwarding it to the Camera.
    /// @param inputDevice Input device carrying the scroll data.
    /// @return True if the event was consumed.
    bool onScroll(const gui::InputDevice& inputDevice) override;
    /// @brief Forwards cursor-move events to the Camera for look-around processing.
    /// @param inputDevice Input device carrying the cursor position.
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
//    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
//    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    /// @brief Resets cursor tracking state when the cursor leaves the view.
    /// @param inputDevice Input device carrying the exit event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    /// @brief Scales the field of view by the given factor and rebuilds the projection matrix.
    /// @param scale Scaling factor to apply to the current field of view.
    virtual void scaleFieldOfView(float scale);
    /// @brief Rebuilds the perspective projection matrix when the view is resized.
    /// @param newSize New view size in logical pixels.
    void onResize(const gui::Size& newSize) override;

    /// @brief Deleted copy constructor – CameraView is non-copyable.
    CameraView(const CameraView&) = delete;
    /// @brief Deleted copy assignment – CameraView is non-copyable.
    CameraView& operator =(const CameraView&) = delete;
public:
    /// @brief Default constructor; initialises the view with default perspective parameters.
    CameraView();

    /// @brief Attaches a Camera to drive the view transformation.
    /// @param pCamera Pointer to the Camera to use; pass nullptr to detach.
    void setCamera(Camera* pCamera);
    /// @brief Returns a pointer to the currently attached Camera.
    /// @return Pointer to the active Camera, or nullptr if none is set.
    Camera* getCamera();
    /// @brief Returns the combined camera (view * projection) matrix.
    /// @return The combined 4x4 camera matrix.
    glm::mat4 getCameraMatrix() const;
    /// @brief Returns the current field-of-view scale factor.
    /// @return Field-of-view scale value.
    float getFieldOfViewScale() const;
};

} //namespace gl
} //namespace gui
