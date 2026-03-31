#pragma once
#include <td/Types.h>
#include <gui/Types.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gui/gl/Camera.h>

class MojaCamera : public gui::gl::Camera
{
private:
    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;
    static constexpr float SPEED = 0.02f;
    static constexpr float SENSITIVITY_X = 0.25f;
    static constexpr float SENSITIVITY_Y = 0.25f;

    static constexpr float INITIAL_ZOOM = 45.0f;
    static constexpr float ZOOM_MAX = 140.0f;
    static constexpr float ZOOM_MIN = 10.0f;
    static constexpr bool PITCH_CONSTRAINED = true;
    static constexpr float PITCH_MAX_ANGLE = 89.0f;
    static constexpr bool INVERT_YAXIS = false;

    static constexpr float offsetWindow = 6.0f;
    static constexpr bool USE_MOUSE_YAWPITCH = true;
    static constexpr float KB_YAWPITCH_SENS = 5.0f;

public:
    enum Movement : td::BYTE { FORWARD = 1, BACKWARD = 2, LEFT = 4, RIGHT = 8 };
    enum class Yaw : td::BYTE { NONE = 1, INC = 2, DEC = 3 };
    enum class Pitch : td::BYTE { NONE = 1, INC = 2, DEC = 3 };

private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;

    float _speed;
    float _sensitivity_X, _sensitivity_Y;
    float _fieldOfView;
    bool _pitchConstrained;
    bool _invertYAxis;

    gui::Size _viewportSize;
    float _aspectRatio;

    float _nearPlane;
    float _farPlane;

    gui::Point _lastMousePos;
    float _lastScrollPos;

    td::BYTE _movementKeysSinceLastUpdate;
    gui::Point _currentMousePos;
    float _currentScrollPos;

    Yaw _yawKB;
    Pitch _pitchKB;
    gui::Point _deltaMouseMove;

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);

        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }

public:
    MojaCamera(gui::Size viewport_size,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float near_plane = 0.1f,
        float far_plane = 100.0f,
        float speed = SPEED,
        float sensitivity_X = SENSITIVITY_X,
        float sensitivity_Y = SENSITIVITY_Y,
        float zoom = INITIAL_ZOOM,
        float yaw = YAW,
        float pitch = PITCH,
        bool pitchConstrained = PITCH_CONSTRAINED,
        bool invertY = INVERT_YAXIS)
        : _front(glm::vec3(0.0f, 0.0f, -1.0f))
        , _speed(speed)
        , _sensitivity_X(sensitivity_X)
        , _sensitivity_Y(sensitivity_Y)
        , _fieldOfView(zoom)
        , _pitchConstrained(pitchConstrained)
        , _position(position)
        , _worldUp(up)
        , _yaw(yaw)
        , _pitch(pitch)
        , _invertYAxis(invertY)
        , _viewportSize(viewport_size)
        , _aspectRatio(float(viewport_size.width) / viewport_size.height)
        , _nearPlane(near_plane)
        , _farPlane(far_plane)
        , _movementKeysSinceLastUpdate(0)
        , _lastMousePos(viewport_size.width / 2.f, viewport_size.height / 2.f)
        , _currentMousePos(viewport_size.width / 2.f, viewport_size.height / 2.f)
        , _currentScrollPos(0)
        , _lastScrollPos(0)
        , _yawKB(Yaw::NONE)
        , _pitchKB(Pitch::NONE)
        , _deltaMouseMove(0, 0)
    {
        updateCameraVectors();
    }

    // Expose base class public methods transparently:
    // ... add all needed base public methods here if needed, or just rely on inheritance

    void centerMousePos()
    {
        _lastMousePos.x = _viewportSize.width / 2.f;
        _lastMousePos.y = _viewportSize.height / 2.f;
        _currentMousePos = _lastMousePos;
    }

    void updateMouseToCenter()
    {
        _currentMousePos = _lastMousePos;
    }

    void setPosition(glm::vec3 position)
    {
        _position = position;
    }
    void setSpeed(float speed)
    {
        _speed = speed;
    }
    void setSensitivity(float sensX, float sensY)
    {
        _sensitivity_X = sensX;
        _sensitivity_Y = sensY;
    }
    void setFov(float fov)
    {
        _fieldOfView = fov;
    }
    void setViewportSize(const gui::Size& sz)
    {
        _viewportSize = sz;
        _aspectRatio = float(sz.width) / sz.height;
        centerMousePos();
    }

    glm::vec3 position() const
    {
        return _position;
    }
    glm::vec3 front() const
    {
        return _front;
    }
    glm::vec3 right() const
    {
        return _right;
    }
    glm::vec3 up() const
    {
        return _up;
    }

    float fov() const
    {
        return _fieldOfView;
    }
    float yaw() const
    {
        return _yaw;
    }
    float pitch() const
    {
        return _pitch;
    }
    gui::Size viewportSize() const
    {
        return _viewportSize;
    }
    glm::mat4 MojgetViewMatrix() const
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 MojgetPerspectiveMatrix() const
    {
        return glm::perspective(glm::radians(_fieldOfView), _aspectRatio, _nearPlane, _farPlane);
    }

    void move(Movement direction)
    {
        _movementKeysSinceLastUpdate |= direction;
    }

    void unMove(Movement direction)
    {
        _movementKeysSinceLastUpdate &= ~direction;
    }

    void yawKB(Yaw direction)
    {
        _yawKB = direction;
    }

    void pitchKB(Pitch direction)
    {
        _pitchKB = direction;
    }

    void yawPitchMouse(gui::Point deltaMouseMove)
    {
        _deltaMouseMove = deltaMouseMove;
    }

    void scroll(float currPos)
    {
        _currentScrollPos = currPos;
    }

    void processInputs(float delta_time, bool cameraControl)
    {
        if (!cameraControl)
            return;

        float scroll_y_offset = _currentScrollPos - _lastScrollPos;
        _lastScrollPos = _currentScrollPos;

        td::BYTE currentMovement = _movementKeysSinceLastUpdate;

        processKeyboardInput(currentMovement, delta_time);
        if (USE_MOUSE_YAWPITCH)
            processMouseYawPitch();
        else
            processKBYawPitch();

        processMouseScroll(scroll_y_offset);
    }

    void setYaw(float yaw)
    {
        _yaw = yaw;
        updateCameraVectors();
    }

    void setPitch(float pitch)
    {
        _pitch = pitch;
        updateCameraVectors();
    }

    void lookAt(glm::vec3 target)
    {
        _front = glm::normalize(target - _position);
        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }

private:
    void processKeyboardInput(td::BYTE movement_keys, float deltaTime)
    {
        float velocity = _speed * deltaTime;
        if (movement_keys & Movement::FORWARD)
            _position += _front * velocity;
        if (movement_keys & Movement::BACKWARD)
            _position -= _front * velocity;
        if (movement_keys & Movement::LEFT)
            _position -= _right * velocity;
        if (movement_keys & Movement::RIGHT)
            _position += _right * velocity;
    }

    void processKBYawPitch()
    {
        float xoffset = 0.0f;
        if (_yawKB == Yaw::DEC)
            xoffset = -KB_YAWPITCH_SENS;
        else if (_yawKB == Yaw::INC)
            xoffset = KB_YAWPITCH_SENS;

        float yoffset = 0.0f;
        if (_pitchKB == Pitch::DEC)
            yoffset = -KB_YAWPITCH_SENS;
        else if (_pitchKB == Pitch::INC)
            yoffset = KB_YAWPITCH_SENS;

        _yaw += xoffset;
        _pitch += yoffset;

        if (_pitchConstrained)
        {
            _pitch = std::clamp(_pitch, -PITCH_MAX_ANGLE, PITCH_MAX_ANGLE);
        }
        _yawKB = Yaw::NONE;
        _pitchKB = Pitch::NONE;

        updateCameraVectors();
    }

    void processMouseYawPitch()
    {
        float xoffset = _deltaMouseMove.x;
        float yoffset = _invertYAxis ? -_deltaMouseMove.y : _deltaMouseMove.y;

        _yaw += xoffset * _sensitivity_X;
        _pitch += yoffset * _sensitivity_Y;

        if (_pitchConstrained)
        {
            _pitch = std::clamp(_pitch, -PITCH_MAX_ANGLE, PITCH_MAX_ANGLE);
        }

        _deltaMouseMove.toZero();

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        _fieldOfView = std::clamp(_fieldOfView - yoffset, ZOOM_MIN, ZOOM_MAX);
    }
};