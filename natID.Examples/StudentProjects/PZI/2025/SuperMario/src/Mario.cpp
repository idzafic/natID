#include "Mario.h"
#include <gui/Shape.h>

Mario::Mario()
    : GameObject(gui::Point(0, 0), gui::Size(50, 60), ObjectType::MARIO)
    , state(MarioState::IDLE)
    , powerUp(MarioPowerUp::SMALL)
    , _pSoundGrow(nullptr)
    , _pSoundCrash(nullptr)
    , isOnGround(false)
    , isFacingRight(true)
    , isInvincible(false)
    , jumpForce(-730.0)
    , walkSpeed(400.0)
    , runSpeed(600.0)
    , maxFallSpeed(1000.0)
    , invincibilityTimer(0.0)
    , animationFrame(0)
    , animationTimer(0.0)
    , spriteIdleRight(":mario_idle_right")
    , spriteWalk1Right(":mario_walk1_right")
    , spriteWalk2Right(":mario_walk2_right")
    , spriteJumpRight(":mario_jump_right")
    , spriteIdleLeft(":mario_idle_left")
    , spriteWalk1Left(":mario_walk1_left")
    , spriteWalk2Left(":mario_walk2_left")
    , spriteJumpLeft(":mario_jump_left")
{
}

void Mario::init(double x, double y, gui::Sound* pGrow, gui::Sound* pCrash)
{
    setPos({ x,y });
    state = MarioState::IDLE;
    powerUp = MarioPowerUp::SMALL;
    _pSoundGrow = pGrow;
    _pSoundCrash = pCrash;
    isOnGround = false;
    isFacingRight = true;
    isInvincible = false;
    jumpForce = -730.0;
    walkSpeed = 400.0;
    runSpeed = 600.0;
    maxFallSpeed = 1000.0;
    invincibilityTimer = 0.0;
    animationFrame = 0;
    animationTimer = 0.0;
}


gui::Image* Mario::getCurrentSprite()
{
    // Animacija hodanja: walk1 → idle → walk2 → idle → loop
    // Frame pattern: 0,1,2,3,0,1,2,3...
    // 0=walk1, 1=idle, 2=walk2, 3=idle

    bool isMoving = fabs(_velocity.x) > 10.0;
    bool isFalling = _velocity.y > 50.0;
    bool isJumping = _velocity.y < -50.0;

    if (isFacingRight)
    {
        // Ako skače ili pada
        if (isJumping || isFalling)
            return &spriteJumpRight;

        // Ako hoda
        if (isMoving)
        {
            int cycle = animationFrame % 4;
            if (cycle == 0) return &spriteWalk1Right;
            if (cycle == 1) return &spriteIdleRight;
            if (cycle == 2) return &spriteWalk2Right;
            if (cycle == 3) return &spriteIdleRight;
        }

        // Inače idle
        return &spriteIdleRight;
    }
    else  // Facing LEFT
    {
        if (isJumping || isFalling)
            return &spriteJumpLeft;

        if (isMoving)
        {
            int cycle = animationFrame % 4;
            if (cycle == 0) return &spriteWalk1Left;
            if (cycle == 1) return &spriteIdleLeft;
            if (cycle == 2) return &spriteWalk2Left;
            if (cycle == 3) return &spriteIdleLeft;
        }

        return &spriteIdleLeft;
    }
}

void Mario::update(double deltaTime)
{   

    // Update besmrtnost
    if (isInvincible && invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            isInvincible = false;
        }
    }

    // gravitacija ako je u zraku
    if (!isOnGround) {
        applyGravity(deltaTime);
    }

    // Limit fall speed
    if (_velocity.y > maxFallSpeed) {
        _velocity.y = maxFallSpeed;
    }

    // Update pozicije
    _pos.x += _velocity.x * deltaTime;
    _pos.y += _velocity.y * deltaTime;


    // Update stanje based po brzini
    if (!isOnGround) {  // Prvo provjera da li je u vazduhu
        if (_velocity.y > 50.0) {
            state = MarioState::FALLING;
        }
        else {
            state = MarioState::JUMPING;
        }
    }
    else if (fabs(_velocity.x) > 50.0) {  // Na tlu i kreće se
        state = MarioState::WALKING;
    }
    else {  // Na tlu i miruje
        state = MarioState::IDLE;
    }

    // Update animaciju
    updateAnimation(deltaTime);
}

void Mario::draw(const gui::Point& cameraOffset, const gui::Rect& viewPort)
{      

    if (isInvincible) {
        
        int blinkPhase = (int)(invincibilityTimer * 10) % 2;
        if (blinkPhase == 0)
            return;

    }


    gui::Rect drawRect(_pos.x - cameraOffset.x,
        _pos.y - cameraOffset.y,
        _pos.x - cameraOffset.x + _size.width,
        _pos.y - cameraOffset.y + _size.height);

    // Pokušaj nacrtati sprite
    gui::Image* currentSprite = getCurrentSprite();
    if (currentSprite)
    {
        currentSprite->draw(drawRect);
    }
    else
    {
        // Fallback - rect ako sprite ne postoji
        gui::Shape shape;
        shape.createRect(drawRect);
        td::ColorID color = isInvincible ? td::ColorID::Yellow : td::ColorID::Red;
        shape.drawFillAndWire(color, td::ColorID::Black);
    }
}

void Mario::onCollision(GameObject* other)
{
    int k = 0;
    ++k;
}

void Mario::applyGravity(double deltaTime)
{
    const double GRAVITY = 1200.0;
    _velocity.y += GRAVITY * deltaTime;
}

void Mario::moveLeft()
{
    _velocity.x = -walkSpeed;
    isFacingRight = false;
}

void Mario::moveRight()
{
    _velocity.x = walkSpeed;
    isFacingRight = true;
}

void Mario::jump()
{
    if (isOnGround)
    {
        _velocity.y = jumpForce;
        isOnGround = false;
    }
}

void Mario::stopMoving()
{
    _velocity.x = 0;
}

void Mario::setState(MarioState newState)
{
    state = newState;
}

MarioState Mario::getState() const
{
    return state;
}

void Mario::setPowerUp(MarioPowerUp newPowerUp)
{
    MarioPowerUp oldPowerUp = powerUp;
    powerUp = newPowerUp;

    double oldHeight = _size.height;

    // Promijeni veličinu
    if (powerUp == MarioPowerUp::SMALL)
    {
        _size.width = 50;
        _size.height = 60;
        if (oldPowerUp != powerUp)
            _pSoundCrash->play();  //play once
    }
    else  // BIG ili FIRE
    {
        _size.width = 60;
        _size.height = 80;
        if (oldPowerUp != powerUp)
            _pSoundCrash->play();   //play once
    }

    // Ako je Mario rastao, pomjeri ga gore
    // Ako se smanjio, ostavi ga gdje jeste (pao će na tlo prirodno)
    if (_size.height > oldHeight)
    {
        // Rastao je - pomjeri gore
        _pos.y -= (_size.height - oldHeight);
    }
}

MarioPowerUp Mario::getPowerUp() const
{
    return powerUp;
}

void Mario::takeDamage()
{
    if (isInvincible)
        return;

    if (powerUp == MarioPowerUp::FIRE) {
        setPowerUp(MarioPowerUp::BIG);
        makeInvincible(2.0);
    }
    else if (powerUp == MarioPowerUp::BIG) {
        setPowerUp(MarioPowerUp::SMALL);
        makeInvincible(2.0);
    }
    else {
        state = MarioState::DEAD;
        makeInvincible(2.0);
    }
}

void Mario::makeInvincible(double duration)
{
    isInvincible = true;
    invincibilityTimer = duration;
}

void Mario::setOnGround(bool onGround)
{
    isOnGround = onGround;
}

bool Mario::getIsOnGround() const
{
    return isOnGround;
}

void Mario::updateAnimation(double deltaTime)
{
    // Samo ažuriraj animaciju ako se kreće
    if (state == MarioState::WALKING || state == MarioState::RUNNING)
    {
        animationTimer += deltaTime;

        // Promijeni frame svakih 0.15 sekundi
        if (animationTimer > 0.15)
        {
            animationFrame++;
            animationTimer = 0;
        }
    }
    else if(state == MarioState::IDLE)
    {
        // Reset animacije kad ne hoda
        animationFrame = 0;
        animationTimer = 0;
    }
}

bool Mario::getIsFacingRight() const
{
    return isFacingRight;
}

bool Mario::getIsInvincible() const
{
    return isInvincible;
}

Mario::~Mario()
{
}
