#pragma once
#include <gui/Types.h>

enum class ObjectType { MARIO, ENEMY, ITEM, TILE, PROJECTILE };

class GameObject {
protected:
    gui::Point _pos;
    gui::Size _size;
    gui::Point _velocity;
    bool _isAlive;
    ObjectType _type;

public:
    GameObject(const gui::Point& pos, const gui::Size& size, ObjectType type);
    virtual ~GameObject() = default;

    virtual void update(double deltaTime) = 0;
    virtual void draw(const gui::Point& cameraOffset, const gui::Rect& viewPort) = 0;
    virtual void onCollision(GameObject* other) = 0;

    const gui::Point& getPos() const { return _pos; }
    const gui::Size& getSize() const { return _size; }
    const gui::Point& getVelocity() const { return _velocity; }
    bool isAlive() const { return _isAlive; }
    ObjectType getType() const { return _type; }
    gui::Rect getBounds() const;

    void setPos(const gui::Point& pos) { _pos = pos; }
    void setVelocity(const gui::Point& vel) { _velocity = vel; }
    void setAlive(bool alive) { _isAlive = alive; }

    bool checkCollision(const GameObject* other) const;
    bool checkCollision(const gui::Rect& rect) const;
};
