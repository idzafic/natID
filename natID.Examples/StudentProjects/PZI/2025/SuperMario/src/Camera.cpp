#include "Camera.h"
#include <math/math.h>

Camera::Camera()
    : _pos(0, 0)
    , _viewportSize({0.0, 0.0})
    , _worldSize({0.0, 0.0})
    , _target(nullptr)
    , _smoothSpeed(10.0)
{
}

void Camera::init(const gui::Size& viewportSize, const gui::Size& worldSize)
{
    _pos = { 0.0, 0.0 };
    _viewportSize = viewportSize;
    _worldSize = worldSize;
    _target = nullptr;
    _smoothSpeed = 10.;
}

void Camera::update(double deltaTime)
{
    if (!_target)
        return;

    double targetX = _target->getPos().x - _viewportSize.width / 2.0;

    _pos.x = _target->getPos().x - _viewportSize.width / 2.0;

  
    clampToWorld();

   
}

void Camera::clampToWorld()
{
    double oldX = _pos.x;
    if (_pos.x < 0)
        _pos.x = 0;
    if (_pos.x + _viewportSize.width > _worldSize.width)
        _pos.x = _worldSize.width - _viewportSize.width;

    if (_pos.y < 0)
        _pos.y = 0;
    if (_pos.y + _viewportSize.height > _worldSize.height)
        _pos.y = _worldSize.height - _viewportSize.height;
}

bool Camera::isInView(const gui::Rect& rect) const
{
    gui::Rect viewRect(_pos.x, _pos.y,
        _pos.x + _viewportSize.width,
        _pos.y + _viewportSize.height);

    return (viewRect.left < rect.right &&
        viewRect.right > rect.left &&
        viewRect.top < rect.bottom &&
        viewRect.bottom > rect.top);
}