#pragma once
#include "GameObject.h"
#include <gui/Image.h>
#include <gui/Sound.h>

enum class MarioState { IDLE, WALKING, RUNNING, JUMPING, FALLING, DEAD };
enum class MarioPowerUp { SMALL, BIG, FIRE };

class Mario : public GameObject {
protected:
    MarioState state;
    MarioPowerUp powerUp;
    gui::Sound* _pSoundGrow;
    gui::Sound* _pSoundCrash;

    bool isOnGround;
    bool isFacingRight;
    bool isInvincible;

    double jumpForce;
    double walkSpeed;
    double runSpeed;
    double maxFallSpeed;

    double invincibilityTimer;
    td::INT4 animationFrame;
    double animationTimer;

    // Sprite-ovi
    gui::Image spriteIdleRight;
    gui::Image spriteWalk1Right;
    gui::Image spriteWalk2Right;
    gui::Image spriteJumpRight;

    gui::Image spriteIdleLeft;
    gui::Image spriteWalk1Left;
    gui::Image spriteWalk2Left;
    gui::Image spriteJumpLeft;

public:
    Mario();

    void init(double x, double y, gui::Sound* pGrow, gui::Sound* pCrash);

    void update(double deltaTime) override;
    void draw(const gui::Point& cameraOffset, const gui::Rect& viewPort) override;
    void onCollision(GameObject* other) override;

    void moveLeft();
    void moveRight();
    void jump();
    void stopMoving();

    void setState(MarioState newState);
    MarioState getState() const;

    void setPowerUp(MarioPowerUp newPowerUp);
    MarioPowerUp getPowerUp() const;
    void takeDamage();
    void makeInvincible(double duration);

    void setOnGround(bool onGround);
    bool getIsOnGround() const;
    void applyGravity(double deltaTime);

    void updateAnimation(double deltaTime);

    bool getIsFacingRight() const;
    bool getIsInvincible() const;

    ~Mario();

private:
    gui::Image* getCurrentSprite();
};
