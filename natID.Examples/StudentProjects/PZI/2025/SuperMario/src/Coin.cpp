#include "Coin.h"
#include <gui/Shape.h>

Coin::Coin(double x, double y)
    : GameObject(gui::Point(x, y), gui::Size(30, 30), ObjectType::ITEM)
    , isCollected(false)
    , animationTimer(0.0)
    , animationFrame(0)
    , sprite1(":coin1")
    , sprite2(":coin2")
    , sprite3(":coin3")
{
}

gui::Image* Coin::getCurrentSprite()
{
    // Animacija: rotate kroz 3 frame-a
    int cycle = animationFrame % 3;
    if (cycle == 0)
        return &sprite1;
    if (cycle == 1)
        return &sprite2;
    if (cycle == 2)
        return &sprite3;
    return &sprite1;
}

void Coin::update(double deltaTime)
{
    if (isCollected)
        return;

    // Animacija - rotate coin
    animationTimer += deltaTime;
    if (animationTimer > 0.15)
    {
        animationFrame++;
        animationTimer = 0;
    }
}

void Coin::draw(const gui::Point& cameraOffset, const gui::Rect& viewPort)
{
    if (isCollected)
        return;  // Dob't draw if already collected

    gui::Rect drawRect(_pos.x - cameraOffset.x,
        _pos.y - cameraOffset.y,
        _pos.x - cameraOffset.x + _size.width,
        _pos.y - cameraOffset.y + _size.height);

    if (drawRect.right < 0)
        return; //not visible
    if (drawRect.right > viewPort.width())
        return; //not visible
    
    // Draw the sprite
    gui::Image* currentSprite = getCurrentSprite();
    if (currentSprite)
        currentSprite->draw(drawRect);
    else
    {
        // Fallback - žuti krug
        gui::Circle coinCircle(
            drawRect.left + _size.width / 2.0,
            drawRect.top + _size.height / 2.0,
            _size.width / 2.0
        );

        gui::Shape shape;
        shape.createCircle(coinCircle);
        shape.drawFillAndWire(td::ColorID::Yellow, td::ColorID::Orange);
    }
}

void Coin::onCollision(GameObject* other)
{
    // Za sada prazno
    int k=0;
}

void Coin::collect()
{
    isCollected = true;
    _isAlive = false;  // Mark za uklanjanje
}
