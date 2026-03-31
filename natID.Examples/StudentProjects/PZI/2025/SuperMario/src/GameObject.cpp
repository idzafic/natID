#include "GameObject.h"

GameObject::GameObject(const gui::Point& pos, const gui::Size& size, ObjectType type)
    : _pos(pos)
    , _size(size)
    , _velocity(0, 0)
    , _isAlive(true)
    , _type(type)
{
}

gui::Rect GameObject::getBounds() const
{
    return gui::Rect(_pos.x, _pos.y, _pos.x + _size.width, _pos.y + _size.height);
}

bool GameObject::checkCollision(const GameObject* other) const
{
    if (!other || !other->isAlive() || !_isAlive)
        return false;

    auto myBounds = getBounds();
    auto otherBounds = other->getBounds();

    return (myBounds.left < otherBounds.right &&
        myBounds.right > otherBounds.left &&
        myBounds.top < otherBounds.bottom &&
        myBounds.bottom > otherBounds.top);
}

bool GameObject::checkCollision(const gui::Rect& rect) const
{
    auto myBounds = getBounds();

    return (myBounds.left < rect.right &&
        myBounds.right > rect.left &&
        myBounds.top < rect.bottom &&
        myBounds.bottom > rect.top);
}