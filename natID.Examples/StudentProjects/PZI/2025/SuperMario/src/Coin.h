#pragma once
#include "GameObject.h"
#include <gui/Image.h>

class Coin : public GameObject {
protected:
    bool isCollected;
    double animationTimer;
    td::INT4 animationFrame;

    gui::Image sprite1;
    gui::Image sprite2;
    gui::Image sprite3;

public:
    Coin(double x, double y);

    void update(double deltaTime) override;
    void draw(const gui::Point& cameraOffset, const gui::Rect& viewPort) override;
    void onCollision(GameObject* other) override;

    void collect();
    bool getIsCollected() const { return isCollected; }

private:
    gui::Image* getCurrentSprite();
};
