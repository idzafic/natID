// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Camera.h
    @brief Declares the Camera class for processing 3D input and computing view matrices in OpenGL. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGL.h"
#include "Context.h"
#include <gui/DrawableView.h>
#include <gui/gl/GL.h>
#include <glm/glm.hpp>
#include <vector>
//#include <td/Timer.h>
#include <gui/Key.h>
#include <gui/gl/ICameraLimiter.h>

namespace gui
{
namespace gl
{

class SkyBox;
class CameraView;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
/// @brief Processes keyboard/mouse input and computes Euler-angle-based view matrices for use in OpenGL rendering.
class NATGL_API Camera
{
    friend class CameraView;

    /// @brief Union that packs four directional movement byte flags into a single 32-bit integer.
    typedef union _directions
    {
        td::UINT4 val;         ///< All four movement flags packed as a single value.
        td::BYTE movement[4];  ///< Individual directional movement flags indexed by Movement enum.
    } Directions;

    static constexpr float DefaultYaw = -90.0f;               ///< Default yaw angle in degrees.
    static constexpr float DefaultPitch = 0.0f;               ///< Default pitch angle in degrees.
    static constexpr float DefaultSpeed = 0.25f;              ///< Default camera movement speed.
    static constexpr float DefaultScrollSensitivity = 0.1f;   ///< Default mouse scroll sensitivity.
    static constexpr float DefaultMoveSensitivity = 0.01f;    ///< Default mouse move sensitivity.
//    static constexpr float DefaultZoom = 45.0f;

public:
    /// @brief Enumeration of supported camera movement directions.
    enum class Movement : td::BYTE {Forward=0, Backward, Left, Right};
private:
    gui::gl::ICameraLimiter* _pLimiter = nullptr; ///< Optional camera movement limiter interface.
    gui::gl::SkyBox* _pSkyBox = nullptr;           ///< Optional associated sky box to rotate with the camera.

    glm::mat4 _lookAtMatrix;    ///< Cached view (look-at) matrix.
    glm::vec3 _position;        ///< Current camera world-space position.
    glm::vec3 _front;           ///< Unit vector pointing in the camera's look direction.
    glm::vec3 _up;              ///< Camera up vector (re-computed from world up and front).
    glm::vec3 _right;           ///< Camera right vector (cross product of front and up).
    glm::vec3 _worldUp;         ///< World-space up vector (typically 0,1,0).
    gui::gl::Point2d _prevPos;  ///< Previous mouse cursor position used for delta computation.
//    td::Timer _timer;
    //movement states
    Directions _directions;     ///< Packed directional key-press state flags.

    char _directionChars[4] = {'w', 's', 'a', 'd'}; //in Movement order

    // euler Angles
    float _minYaw = DefaultYaw - 90.0f;   ///< Minimum allowed yaw angle.
    float _yaw = DefaultYaw;              ///< Current yaw angle in degrees.
    float _maxYaw = DefaultYaw + 90.0f;   ///< Maximum allowed yaw angle.
    float _minPitch = DefaultPitch - 90;  ///< Minimum allowed pitch angle.
    float _pitch = DefaultPitch;          ///< Current pitch angle in degrees.
    float _maxPitch = DefaultPitch + 90.0f; ///< Maximum allowed pitch angle.
    float _minZoomFactor = 0.5f;          //fov = 90 deg
    float _zoomFactor = 1.0f;             ///< Current zoom factor (1.0 = no zoom).
    float _maxZoomFactor = 450;           //fov = 0.1 deg
    // camera options
    float _movementSpeed = DefaultSpeed;               ///< Current camera movement speed.
    float _mouseScrollSensitivity = DefaultScrollSensitivity; ///< Sensitivity for scroll-wheel zoom.
    float _mouseMoveSensitivity = DefaultMoveSensitivity;     ///< Sensitivity for mouse-drag look.

//    float _zoom = DefaultZoom;
    td::BYTE _updateMatrix = 1;  ///< Dirty flag; non-zero means the view matrix must be recomputed.
    td::BYTE _shiftPressed = 0;  ///< Non-zero when the Shift modifier key is held.
    td::BYTE _fixed = 0;         ///< Non-zero when the camera is locked and cannot be moved.
private:
    /// @brief Returns the index of the movement array slot corresponding to the given character.
    /// @param ch Character to look up.
    /// @return Byte index into the movement array, or 0xFF if not found.
    td::BYTE getMovementPos(char ch) const;
    /// @brief Updates the camera world position based on the current direction flags.
    /// @param bAlter True if the Shift modifier is active, altering movement behaviour.
    void updatePosition(bool bAlter);
protected:

    // calculates the front vector from the Camera's (updated) Euler Angles
    /// @brief Recomputes the front, right, and up vectors from the current yaw and pitch angles.
    void updateCameraVectors();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    /// @brief Processes a key-press event and updates directional movement state.
    /// @param key The key that was pressed.
    /// @return True if the event was consumed by the camera.
    bool onKeyPressed(const gui::Key& key);

    /// @brief Processes a key-release event and clears the corresponding directional movement flag.
    /// @param key The key that was released.
    /// @return True if the event was consumed by the camera.
    bool onKeyReleased(const gui::Key& key);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    /// @brief Handles a scroll/pan gesture, adjusting camera position.
    /// @param delta Movement delta in the X and Y axes.
    /// @param alter True when alternate (e.g. Shift-modified) behaviour is requested.
    void processScroll(const gui::gl::Point2d& delta, bool alter);

    /// @brief Handles mouse-move events, updating the camera look direction.
    /// @param pos Current cursor position in the view.
    /// @param modifiers Active modifier key flags.
    void processMouseMove(const gui::gl::Point2d& pos, td::UINT4 modifiers);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    /// @brief Adjusts the field-of-view zoom factor from a scroll-wheel event.
    /// @param zoom Scroll wheel delta value.
    void processZoom(float zoom);

public:
    // constructor with vectors
    /// @brief Constructs the camera from a world position, yaw/pitch angles, and an up vector.
    /// @param position Initial camera world position.
    /// @param yaw Initial yaw angle in degrees.
    /// @param pitch Initial pitch angle in degrees.
    /// @param up World-space up vector.
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = DefaultYaw, float pitch = DefaultPitch, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    /// @brief Constructs the camera from a world position, a look-at target, and an up vector.
    /// @param position Initial camera world position.
    /// @param target World-space point the camera should look at.
    /// @param up World-space up vector.
    Camera(const glm::vec3& position, const glm::vec3& target = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    // constructor with scalar values
    /// @brief Constructs the camera from individual scalar position, up, yaw, and pitch values.
    /// @param posX Camera X position.
    /// @param posY Camera Y position.
    /// @param posZ Camera Z position.
    /// @param upX World up X component.
    /// @param upY World up Y component.
    /// @param upZ World up Z component.
    /// @param yaw Initial yaw angle in degrees.
    /// @param pitch Initial pitch angle in degrees.
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    /// @brief Returns the view matrix, recomputing it if the camera state is dirty.
    /// @return Const reference to the 4x4 view matrix.
    const glm::mat4& getViewMatrix();
    /// @brief Returns the cached view matrix (const overload, does not recompute).
    /// @return Const reference to the 4x4 view matrix.
    const glm::mat4& getViewMatrix() const;

    /// @brief Sets the four directional key characters (forward, backward, left, right).
    /// @param directions Array of exactly four characters in Movement enum order.
    void setDirectionChars(char directions[4]);
    /// @brief Returns the current directional key character array.
    /// @return Pointer to the internal array of four direction characters.
    const char* getDirectionChars() const;

    /// @brief Attaches a camera movement limiter to constrain positional updates.
    /// @param pCamLimiter Pointer to the ICameraLimiter implementation.
    void setLimiter(gui::gl::ICameraLimiter* pCamLimiter);
    /// @brief Associates a SkyBox that will rotate together with the camera.
    /// @param pSkyBox Pointer to the SkyBox instance.
    void setSkyBox(gui::gl::SkyBox* pSkyBox);

    /// @brief Returns the current camera movement speed.
    /// @return Movement speed value.
    float getSpeed() const;
    /// @brief Sets the camera movement speed.
    /// @param speed New movement speed.
    void setSpeed(float speed);

    /// @brief Returns the current mouse-move sensitivity.
    /// @return Mouse sensitivity value.
    float getMouseSensitivity() const;
    /// @brief Sets the mouse-move sensitivity.
    /// @param s New sensitivity value.
    void getMouseSensitivity(float s);

    /// @brief Returns the current yaw angle in degrees.
    /// @return Yaw angle.
    float getYaw() const;
    /// @brief Sets the yaw angle, clamping to the configured limits.
    /// @param yaw New yaw angle in degrees.
    void setYaw(float yaw);

    /// @brief Returns the current pitch angle in degrees.
    /// @return Pitch angle.
    float getPitch() const;
    /// @brief Sets the pitch angle, clamping to the configured limits.
    /// @param pitch New pitch angle in degrees.
    void setPitch(float pitch);

    /// @brief Returns true if the camera is locked and cannot be moved.
    /// @return True when fixed, false otherwise.
    bool isFixed() const;
    /// @brief Locks the camera so that movement and look input are ignored.
    void setFixed();

    /// @brief Sets the minimum and maximum allowed zoom factor values.
    /// @param minZoomFactor Minimum zoom factor (must be > 0).
    /// @param maxZoomFactor Maximum zoom factor.
    void setZoomFactorLimits(float minZoomFactor, float maxZoomFactor);
    /// @brief Returns the current zoom factor.
    /// @return Zoom factor value.
    float getZoomFactor() const;
};


} //namespace gl
} //namespace gui
