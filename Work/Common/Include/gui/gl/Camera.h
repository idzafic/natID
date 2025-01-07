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
class NATGL_API Camera
{
    friend class CameraView;
    
    typedef union _directions
    {
        td::UINT4 val;
        td::BYTE movement[4];
    } Directions;
    
    static constexpr float DefaultYaw = -90.0f;
    static constexpr float DefaultPitch = 0.0f;
    static constexpr float DefaultSpeed = 0.25f;
    static constexpr float DefaultScrollSensitivity = 0.1f;
    static constexpr float DefaultMoveSensitivity = 0.01f;
//    static constexpr float DefaultZoom = 45.0f;
    
public:
    enum class Movement : td::BYTE {Forward=0, Backward, Left, Right};
private:
    gui::gl::ICameraLimiter* _pLimiter = nullptr;
    gui::gl::SkyBox* _pSkyBox = nullptr;
    
    glm::mat4 _lookAtMatrix;
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    gui::gl::Point2d _prevPos;
//    td::Timer _timer;
    //movement states
    Directions _directions;
    
    char _directionChars[4] = {'w', 's', 'a', 'd'}; //in Movement order
    
    // euler Angles
    float _minYaw = DefaultYaw - 90.0f;
    float _yaw = DefaultYaw;
    float _maxYaw = DefaultYaw + 90.0f;
    float _minPitch = DefaultPitch - 90;
    float _pitch = DefaultPitch;
    float _maxPitch = DefaultPitch + 90.0f;
    float _minZoomFactor = 0.5f; //fov = 90 deg
    float _zoomFactor = 1.0f;
    float _maxZoomFactor = 450; //fov = 0.1 deg
    // camera options
    float _movementSpeed = DefaultSpeed;
    float _mouseScrollSensitivity = DefaultScrollSensitivity;
    float _mouseMoveSensitivity = DefaultMoveSensitivity;
    
//    float _zoom = DefaultZoom;
    td::BYTE _updateMatrix = 1;
    td::BYTE _shiftPressed = 0;
    td::BYTE _fixed = 0;
private:
    td::BYTE getMovementPos(char ch) const;
    void updatePosition(bool bAlter);
protected:
    
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    bool onKeyPressed(const gui::Key& key);

    bool onKeyReleased(const gui::Key& key);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processScroll(const gui::gl::Point2d& delta, bool alter);
    
    void processMouseMove(const gui::gl::Point2d& pos, td::UINT4 modifiers);
    
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processZoom(float zoom);
    
public:
    // constructor with vectors
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = DefaultYaw, float pitch = DefaultPitch, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    
    Camera(const glm::vec3& position, const glm::vec3& target = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    const glm::mat4& getViewMatrix();
    const glm::mat4& getViewMatrix() const;
    
    void setDirectionChars(char directions[4]);
    const char* getDirectionChars() const;
    
    void setLimiter(gui::gl::ICameraLimiter* pCamLimiter);
    void setSkyBox(gui::gl::SkyBox* pSkyBox);
    
    float getSpeed() const;
    void setSpeed(float speed);
    
    float getMouseSensitivity() const;
    void getMouseSensitivity(float s);
    
    float getYaw() const;
    void setYaw(float yaw);
    
    float getPitch() const;
    void setPitch(float pitch);
    
    bool isFixed() const;
    void setFixed();
    
    void setZoomFactorLimits(float minZoomFactor, float maxZoomFactor);
    float getZoomFactor() const;
};


} //namespace gl
} //namespace gui
