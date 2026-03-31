#pragma once
#include "GameObject.h"
#include <gui/Image.h>

enum class PowerUpType {
    MUSHROOM,
    FIRE_FLOWER,
    STAR
};

class PowerUp : public GameObject {
protected:
    PowerUpType powerUpType;
    bool isSpawning;
    double spawnTimer;
    double spawnDuration;
    double spawnStartY;
    double spawnEndY;
    bool movingRight;
    double moveSpeed;

    // Animation
    double animationTimer;
    td::INT4 animationFrame;

    // Sprites
    gui::Image* spriteMushroom;
    gui::Image* spriteFlower1;
    gui::Image* spriteFlower2;
    gui::Image* spriteStar;

public:
    PowerUp(double x, double y, PowerUpType type);

    void update(double deltaTime) override;
    void draw(const gui::Point& cameraOffset, const gui::Rect& viewPort) override;
    void onCollision(GameObject* other) override;

    PowerUpType getType() const { return powerUpType; }
    bool getIsSpawning() const { return isSpawning; }

    ~PowerUp();

private:
    void loadSprites();
    gui::Image* getCurrentSprite();
};
