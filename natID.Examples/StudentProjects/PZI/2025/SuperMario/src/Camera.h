#pragma once
#include <gui/Types.h>
#include "GameObject.h"

class Camera {
protected:
    gui::Point _pos;
    gui::Size _viewportSize;
    gui::Size _worldSize;
    GameObject* _target;
    double _smoothSpeed;

public:
    Camera();
    void init(const gui::Size& viewportSize, const gui::Size& worldSize);
    void update(double deltaTime);
    void setTarget(GameObject* target) { _target = target; }
    void setWorldSize(const gui::Size& size) { _worldSize = size; }

    gui::Point getOffset() const { return _pos; }
    void setPosition(const gui::Point& pos) { _pos = pos; }
    bool isInView(const gui::Rect& rect) const;

private:
    void clampToWorld();
};