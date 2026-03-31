#include "PowerUp.h"
#include <gui/Shape.h>

PowerUp::PowerUp(double x, double y, PowerUpType type)
    : GameObject(gui::Point(x, y), gui::Size(40, 40), ObjectType::ITEM)
    , powerUpType(type)
    , isSpawning(true)
    , spawnTimer(0.0)
    , spawnDuration(0.5)  // 0.5 sekundi za spawn animaciju
    , spawnStartY(y)
    , spawnEndY(y - 40)  // Skoči 1 tile visoko
    , movingRight(true)
    , moveSpeed(100.0)
    , animationTimer(0.0)
    , animationFrame(0)
    , spriteMushroom(new gui::Image(":mushroom"))
    , spriteFlower1(new gui::Image(":flower1"))
    , spriteFlower2(new gui::Image(":flower2"))
    , spriteStar(nullptr)
{
   //loadSprites();
}

/*void PowerUp::loadSprites()
{
    try {
        spriteMushroom = new gui::Image(":mushroom");
        spriteFlower1 = new gui::Image(":flower1");
        spriteFlower2 = new gui::Image(":flower2");
    }
    catch (...) {
        // Fallback ako ne učita
    }
}*/

gui::Image* PowerUp::getCurrentSprite()
{
    if (powerUpType == PowerUpType::MUSHROOM)
        return spriteMushroom;

    if (powerUpType == PowerUpType::FIRE_FLOWER)
    {
        // Animacija između 2 frame-a
        int cycle = animationFrame % 2;
        return (cycle == 0) ? spriteFlower1 : spriteFlower2;
    }

    if (powerUpType == PowerUpType::STAR)
        return spriteStar;

    return nullptr;
}

void PowerUp::update(double deltaTime)
{
    if (!_isAlive)
        return;

    // SPAWN ANIMATION - Power-up iskače iz blocka
    if (isSpawning)
    {
        spawnTimer += deltaTime;

        // Lerp pozicija od startY do endY
        double t = spawnTimer / spawnDuration;
        if (t > 1.0) t = 1.0;

        _pos.y = spawnStartY + (spawnEndY - spawnStartY) * t;

        if (spawnTimer >= spawnDuration)
        {
            isSpawning = false;
            _pos.y = spawnEndY;
        }

        return;  // Ne pomjeraj se dok spawnaš
    }

    // MOVEMENT - Lijevo-desno kao Goomba
    if (movingRight)
        _velocity.x = moveSpeed;
    else
        _velocity.x = -moveSpeed;

    // GRAVITACIJA (samo za mushroom, flower lebdi)
    if (powerUpType == PowerUpType::MUSHROOM || powerUpType == PowerUpType::STAR)
    {
        const double GRAVITY = 1200.0;
        _velocity.y += GRAVITY * deltaTime;

        if (_velocity.y > 600.0)
            _velocity.y = 600.0;
    }
    else
    {
        // Fire Flower ne pada
        _velocity.y = 0;
    }

    // Update pozicija
    _pos.x += _velocity.x * deltaTime;
    _pos.y += _velocity.y * deltaTime;

    // Animacija (za fire flower)
    if (powerUpType == PowerUpType::FIRE_FLOWER)
    {
        animationTimer += deltaTime;
        if (animationTimer > 0.2)
        {
            animationFrame++;
            animationTimer = 0;
        }
    }
}

void PowerUp::draw(const gui::Point& cameraOffset, const gui::Rect& viewPort)
{
    gui::Rect drawRect(_pos.x - cameraOffset.x,
        _pos.y - cameraOffset.y,
        _pos.x - cameraOffset.x + _size.width,
        _pos.y - cameraOffset.y + _size.height);

    gui::Image* currentSprite = getCurrentSprite();
    if (currentSprite)
    {
        currentSprite->draw(drawRect);
    }
    else
    {
        // Fallback sa bojama
        gui::Shape shape;
        shape.createRect(drawRect);

        td::ColorID color;
        if (powerUpType == PowerUpType::MUSHROOM)
            color = td::ColorID::Red;
        else if (powerUpType == PowerUpType::FIRE_FLOWER)
            color = td::ColorID::Orange;
        else
            color = td::ColorID::Yellow;

        shape.drawFillAndWire(color, td::ColorID::Black);
    }
}

void PowerUp::onCollision(GameObject* other)
{
}

PowerUp::~PowerUp()
{
    if (spriteMushroom) delete spriteMushroom;
    if (spriteFlower1) delete spriteFlower1;
    if (spriteFlower2) delete spriteFlower2;
    if (spriteStar) delete spriteStar;
}
