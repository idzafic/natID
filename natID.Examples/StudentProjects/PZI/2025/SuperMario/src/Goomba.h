#pragma once
#include "GameObject.h"
#include <gui/Image.h>

class Goomba : public GameObject {
protected:
    double deadTimer;
    double walkSpeed;
    bool movingRight;

    double directionChangeTimer;
    double directionChangeCooldown;

    // Animacija
    td::INT4 animationFrame;
    double animationTimer;

    // Spritovi
    gui::Image* spriteIdle;
    gui::Image* spriteWalk1;
    gui::Image* spriteWalk2;
    gui::Image* spriteDead;

public:
    Goomba(double x, double y);

    void update(double deltaTime) override;
    void draw(const gui::Point& cameraOffset, const gui::Rect& viewPort) override;
    void onCollision(GameObject* other) override;

    void changeDirection();
    bool isMovingRight() const { return movingRight; }

    bool canChangeDirection() const;

    ~Goomba();

private:
    void loadSprites();
    gui::Image* getCurrentSprite();
    void updateAnimation(double deltaTime);
};
