#include "Goomba.h"
#include <gui/Shape.h>

Goomba::Goomba(double x, double y)
    : GameObject(gui::Point(x, y), gui::Size(40, 40), ObjectType::ENEMY)
    , walkSpeed(100.0)
    , movingRight(true)
    , animationFrame(0)
    , animationTimer(0.0)
    , spriteIdle(new gui::Image(":goomba_idle"))
    , spriteWalk1(new gui::Image(":goomba_walk1"))
    , spriteWalk2(new gui::Image(":goomba_walk2"))
    , spriteDead(new gui::Image(":goomba_dead"))
    , deadTimer(0.0)
    , directionChangeTimer(0.0)
    , directionChangeCooldown(0.5)
{
    //loadSprites();
}

/*void Goomba::loadSprites()
{

    try {
        spriteIdle = new gui::Image(":goomba_idle");
        spriteWalk1 = new gui::Image(":goomba_walk1");
        spriteWalk2 = new gui::Image(":goomba_walk2");
        spriteDead = new gui::Image(":goomba_dead");
    }
    catch (...) {
        // Fallback ako ne učita
    }
}*/

gui::Image* Goomba::getCurrentSprite()
{
    if (!_isAlive) {
        return spriteDead;
    }

    // Animacija: idle → walk1 → walk2 → loop
    int cycle = animationFrame % 3;
    if (cycle == 0) return spriteIdle;
    if (cycle == 1) return spriteWalk1;
    if (cycle == 2) return spriteWalk2;

    return spriteIdle;
}

void Goomba::updateAnimation(double deltaTime)
{
    animationTimer += deltaTime;

    // Promijeni frame svakih 0.2 sekundi
    if (animationTimer > 0.2)
    {
        animationFrame++;
        animationTimer = 0;
    }
}

void Goomba::update(double deltaTime)
{
    if (directionChangeTimer > 0)
        directionChangeTimer -= deltaTime;

    // Kretanje lijevo-desno
    if (movingRight)
        _velocity.x = walkSpeed;
    else
        _velocity.x = -walkSpeed;

    // Gravitacija
    const double GRAVITY = 1200.0;
    _velocity.y += GRAVITY * deltaTime;

    // Limit fall speed
    if (_velocity.y > 600.0)
        _velocity.y = 600.0;

    // Update pozicija
    _pos.x += _velocity.x * deltaTime;
    _pos.y += _velocity.y * deltaTime;

    // Update animacija
    updateAnimation(deltaTime);
}

void Goomba::draw(const gui::Point& cameraOffset, const gui::Rect& viewPort)
{
    gui::Rect drawRect(_pos.x - cameraOffset.x,
        _pos.y - cameraOffset.y,
        _pos.x - cameraOffset.x + _size.width,
        _pos.y - cameraOffset.y + _size.height);

    if (drawRect.right < 0)
        return; //not visible
    if (drawRect.right > viewPort.width())
        return; //not visible
    
    // draw the sprite
    gui::Image* currentSprite = getCurrentSprite();
    if (currentSprite)
        currentSprite->draw(drawRect);
    else
    {
        // Fallback - braon rect
        gui::Shape shape;
        shape.createRect(drawRect);
        shape.drawFillAndWire(td::ColorID::Brown, td::ColorID::Black);
    }
}

void Goomba::onCollision(GameObject* other)
{
}

void Goomba::changeDirection()
{
    if (directionChangeTimer <= 0)
    {
        movingRight = !movingRight;
        directionChangeTimer = directionChangeCooldown;  // Reset timer
    }
}
bool Goomba::canChangeDirection() const
{
    return directionChangeTimer <= 0;
}

Goomba::~Goomba()
{
    if (spriteIdle) delete spriteIdle;
    if (spriteWalk1) delete spriteWalk1;
    if (spriteWalk2) delete spriteWalk2;
    if (spriteDead) delete spriteDead;
}
