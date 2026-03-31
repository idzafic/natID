#pragma once
#include <gui/Canvas.h>
#include <gui/Timer.h>
#include <gui/Font.h>
#include <gui/Shape.h>
#include <gui/Sound.h>
#include <vector>
#include "GameObject.h"
#include "Tile.h"
#include "Camera.h"
#include "Mario.h"
#include "Goomba.h" 
#include "GameManager.h"
#include "Coin.h"
#include "PowerUp.h"
#include "Level.h"
#include <td/Timer.h>

class ViewMario : public gui::Canvas
{
protected:

    Camera _camera;
    Mario _mario;

    gui::Size _canvasSize;
    gui::Sound _background;
    gui::Sound _jump;
    gui::Sound _coin;
    gui::Sound _checkPoint;
    gui::Sound _grow;
    gui::Sound _crash;
    td::Timer<true> _timer; //measures time between two frames
    
    double _worldWidth;

    GameManager _gameManager;

    bool _showVictoryScreen;
    Level* _currentLevel = nullptr;
    
    // konstante
    const double GRAVITY = 300.0;
    const double JUMP_FORCE = -750.0;
    const double MOVE_SPEED = 400.0;
    const double TILE_SIZE = 50.0;

public:
    ViewMario()
    : gui::Canvas({ gui::InputDevice::Event::Keyboard })
    , _background(":background")
    , _jump(":jump")
    , _coin(":coin")
    , _checkPoint(":checkPoint")
    , _grow(":grow")
    , _crash(":crash")
    , _currentLevel(nullptr)
    , _showVictoryScreen(false)
    {
        startAnimation();
        enableResizeEvent(true);
        _background.play(true); //continuous background sound
    }
    
    ~ViewMario()
    {
        _background.stop();
        cleanup();
    }

    void init()
    {
        cleanup();

        _gameManager.init();
        _gameManager.startGame();  //postavljanje state na PLAYING

        _currentLevel = new Level(1, 1, 12, 50, TILE_SIZE);  // World 1-1
        _currentLevel->loadLevel();

        // World width iz levela
        _worldWidth = _currentLevel->getLevelWidth();

        // Kreiraj kameru
        gui::Size worldSize(_worldWidth, _canvasSize.height);
        _camera.init(_canvasSize, worldSize);

        // Kreiraj Maria na spawn poziciji iz levela
        _mario.init(_currentLevel->getSpawnX(), _currentLevel->getSpawnY(), &_grow, &_crash);

        // Postavi camera target
        _camera.setTarget(&_mario);
        
        _timer.start();
    }

    //PUBLIC da MainView može pozvati
    void updatePhysics(double deltaTime)
    {
        if (!_currentLevel)
            return;

        _mario.update(deltaTime);

        _currentLevel->update(deltaTime);

        checkTileCollisions();
        checkGoombaCollisions();
        checkCoinCollisions();
        checkPowerUpCollisions();
        checkPowerUpTileCollisions();

        // izbaci mrtve objekte
        _currentLevel->removeDeadObjects();

        // Camera update
        gui::Point marioPos = _mario.getPos();
        double camX = marioPos.x - (_canvasSize.width / 2.0);
        if (camX < 0) camX = 0;
        if (camX + _canvasSize.width > _worldWidth)
            camX = _worldWidth - _canvasSize.width;
        _camera.setPosition(gui::Point(camX, 0));

        //provjera pobjede
        if (_currentLevel->checkVictoryCondition(&_mario))
        {
            //_checkPoint.play(); //play once
            _gameManager.victoryCondition();
            _currentLevel->setCompleted(true);
            _showVictoryScreen = true;
            // TODO: ucitaj iduci level
            // Za sada resetuj
            // init();
        }
    }

protected:


    void cleanup()
    {
        if (_currentLevel)
        {
            delete _currentLevel;
            _currentLevel = nullptr;
        }

    }

    void onResize(const gui::Size& newSize) override
    {
        _canvasSize = newSize;

        // Reinicijalizuj level sa novom veličinom
        if (_currentLevel)
            init();
    }

    void checkTileCollisions()
    {
        if (!_currentLevel)
            return;

        gui::Rect marioRect = _mario.getBounds();
        gui::Point marioPos = _mario.getPos();
        gui::Point marioVel = _mario.getVelocity();
        gui::Size marioSize = _mario.getSize();

        bool foundGroundCollision = false;

        std::vector<Tile*> tiles = _currentLevel->getTiles();

        for (auto tile : tiles)
        {
            if (!tile->isSolid())
                continue;

            const gui::Rect& tileRect = tile->getRect();

            bool intersects = (marioRect.left < tileRect.right &&
                marioRect.right > tileRect.left &&
                marioRect.top < tileRect.bottom &&
                marioRect.bottom > tileRect.top);

            if (intersects)
            {
                double overlapLeft = marioRect.right - tileRect.left;
                double overlapRight = tileRect.right - marioRect.left;
                double overlapTop = marioRect.bottom - tileRect.top;
                double overlapBottom = tileRect.bottom - marioRect.top;

                double minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
                double minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

                if (minOverlapX < minOverlapY)
                {
                    // Horizontalna kolizija
                    if (overlapLeft < overlapRight)
                        marioPos.x = tileRect.left - marioSize.width;
                    else
                        marioPos.x = tileRect.right;

                    marioVel.x = 0;
                    _mario.setPos(marioPos);
                    _mario.setVelocity(marioVel);
                }
                else
                {
                    // Vertikalna kolizija
                    if (overlapTop < overlapBottom)
                    {
                        // Landing
                        marioPos.y = tileRect.top - marioSize.height;
                        marioVel.y = 0;
                        foundGroundCollision = true;
                        _mario.setPos(marioPos);
                        _mario.setVelocity(marioVel);
                    }
                    else
                    {
                        // Udario glavu
                        marioPos.y = tileRect.bottom;
                        marioVel.y = 0;
                        _mario.setPos(marioPos);
                        _mario.setVelocity(marioVel);

                        // Question block
                        if (tile->getType() == TileType::QUESTION_BLOCK && !tile->isHit())
                        {
                            tile->onHit();

                            ItemInBlock itemType = tile->getItemType();

                            if (itemType == ItemInBlock::COIN)
                            {
                                _gameManager.addCoin();
                                _gameManager.addScore(10);
                            }
                            else if (itemType == ItemInBlock::MUSHROOM)
                            {
                                double powerUpX = tileRect.left;
                                double powerUpY = tileRect.top;
                                _currentLevel->addItem(new PowerUp(powerUpX, powerUpY, PowerUpType::MUSHROOM));
                            }
                            else if (itemType == ItemInBlock::FIRE_FLOWER)
                            {
                                double powerUpX = tileRect.left;
                                double powerUpY = tileRect.top - 40;
                                _currentLevel->addItem(new PowerUp(powerUpX, powerUpY, PowerUpType::FIRE_FLOWER));
                            }
                        }
                    }
                }
            }
        }
        _mario.setOnGround(foundGroundCollision);
    }

    void onDraw(const gui::Rect& rect) override
    {
        double dT = _timer.getDurationInSeconds();
//        mu::dbgLog("dT=%f", dT);
        updatePhysics(dT);
        
        // background
        gui::Shape skyShape;
        gui::Rect skyRect(0, 0, _canvasSize.width, _canvasSize.height);
        skyShape.createRect(skyRect);
        skyShape.drawFill(td::ColorID::SkyBlue);

        if (!_currentLevel)
            return;

        // render levela
        _currentLevel->render(&_camera, rect);

        // cilj
        drawFlagpole(rect);
       
        gui::Point cameraOffset = _camera.getOffset();
        _mario.draw(cameraOffset, rect);

        // ui
        drawUI(rect);

        if (_showVictoryScreen)
            drawVictoryScreen();
    }

    void drawFlagpole(const gui::Rect& viewPort)
    {
        if (!_currentLevel)
            return;

        gui::Point cameraOffset = _camera.getOffset();

        double flagX = 115 * TILE_SIZE;
        if (flagX-20 > cameraOffset.x + viewPort.width())
            return; //do not render it if it's not visible
        
        double flagY = _canvasSize.height - TILE_SIZE;
        
        //mu::dbgLog("X");

        // stub
        gui::Rect poleRect(flagX + 20 - cameraOffset.x,
            flagY - 10 * TILE_SIZE - cameraOffset.y,
            flagX + 25 - cameraOffset.x,
            flagY - cameraOffset.y);
        gui::Shape pole;
        pole.createRect(poleRect);
        pole.drawFill(td::ColorID::White);

        // zastavica na vrhu
        gui::Rect flagRect(flagX + 25 - cameraOffset.x,
            flagY - 10 * TILE_SIZE - cameraOffset.y,
            flagX + 50 - cameraOffset.x,
            flagY - 9 * TILE_SIZE - cameraOffset.y);
        gui::Shape flag;
        flag.createRect(flagRect);
        flag.drawFillAndWire(td::ColorID::Red, td::ColorID::Black);
    }

    void drawUI( const gui::Rect& viewPort)
    {
        // pozadina
        gui::Shape uiBackground;
        gui::Rect bgRect(10, 10, 350, 110);
        uiBackground.createRect(bgRect);
        uiBackground.drawFillAndWire(td::ColorID::Cyan, td::ColorID::White, 2);

        //SCORE - yellow bar

        // Label (mali žuti kvadrat)
        gui::Rect scoreLabelRect(20, 20, 35, 35);
        gui::Shape scoreLabel;
        scoreLabel.createRect(scoreLabelRect);
        scoreLabel.drawFill(td::ColorID::Yellow);

        // Score bar 
        double scoreWidth = (_gameManager.getScore() / 50.0);  // Scale down
        if (scoreWidth > 250) scoreWidth = 250;
        if (scoreWidth < 5) scoreWidth = 5;

        gui::Rect scoreBar(45, 20, 45 + scoreWidth, 35);
        gui::Shape scoreShape;
        scoreShape.createRect(scoreBar);
        scoreShape.drawFillAndWire(td::ColorID::Yellow, td::ColorID::Orange, 1);


        // coins

        // Coin ikonica (žuti krug)
        gui::Circle coinIcon(30, 55, 10);
        gui::Shape coinShape;
        coinShape.createCircle(coinIcon);
        coinShape.drawFillAndWire(td::ColorID::Yellow, td::ColorID::Orange, 2);

        // Coins: small blicniing rects (each coin = 1 rect)
        gui::Rect currentView(viewPort);
        gui::Point cameraOffset = _camera.getOffset();
        currentView.translate(cameraOffset.x, 0);
        td::INT4 coins = _gameManager.getCoins();
        for (td::INT4 i = 0; i < coins; ++i)
        {
            gui::Rect coinRect(55 + i * 8, 50, 60 + i * 8, 60);
            if (!coinRect.intersects(currentView))
                continue;
            
            gui::Shape coinSquare;
            coinSquare.createRect(coinRect);
            coinSquare.drawFill(td::ColorID::Yellow);
        }
        
        // hearts

        // Label (crveni kvadrat)
        gui::Rect livesLabelRect(20, 75, 35, 90);
        gui::Shape livesLabel;
        livesLabel.createRect(livesLabelRect);
        livesLabel.drawFill(td::ColorID::Red);

        // Srca
        td::INT4 lives = _gameManager.getLives();
        for (td::INT4 i = 0; i < lives && i < 5; ++i)
        {
            double x =  55 + i * 30;
            if (x < cameraOffset.x || x>cameraOffset.x+viewPort.width()+15)
                continue;
            drawHeart(x, 75, td::ColorID::Red);
        }
    }

    void drawHeart(double x, double y, td::ColorID color)
    {
        // Lijevi krug
        gui::Circle leftCircle(x + 5, y + 3, 5);
        gui::Shape leftShape;
        leftShape.createCircle(leftCircle);
        leftShape.drawFill(color);

        // Desni krug
        gui::Circle rightCircle(x + 13, y + 3, 5);
        gui::Shape rightShape;
        rightShape.createCircle(rightCircle);
        rightShape.drawFill(color);

        // Donji dio
        gui::Rect bottom1(x + 3, y + 5, x + 15, y + 10);
        gui::Shape bottomShape1;
        bottomShape1.createRect(bottom1);
        bottomShape1.drawFill(color);

        gui::Rect bottom2(x + 5, y + 10, x + 13, y + 15);
        gui::Shape bottomShape2;
        bottomShape2.createRect(bottom2);
        bottomShape2.drawFill(color);

        gui::Rect bottom3(x + 7, y + 15, x + 11, y + 18);
        gui::Shape bottomShape3;
        bottomShape3.createRect(bottom3);
        bottomShape3.drawFill(color);
    }
    
    void drawVictoryScreen()
    {
        // Pozadina sa zvjezdicama
        gui::Shape overlay;
        gui::Rect overlayRect(0, 0, _canvasSize.width, _canvasSize.height);
        overlay.createRect(overlayRect);
        overlay.drawFill(td::ColorID::Blue);

        //random zvjezdice
        for (int i = 0; i < 50; i++) {
            double x = rand() % (int)_canvasSize.width;
            double y = rand() % (int)_canvasSize.height;
            gui::Rect star(x, y, x + 3, y + 3);
            gui::Shape starShape;
            starShape.createRect(star);
            starShape.drawFill(td::ColorID::Yellow);
        }

        // Mario glava kao trofej
        // Kapica
        gui::Rect cap(_canvasSize.width / 2.0 - 60,
            _canvasSize.height / 2.0 - 80,
            _canvasSize.width / 2.0 + 60,
            _canvasSize.height / 2.0 - 40);
        gui::Shape capShape;
        capShape.createRect(cap);
        capShape.drawFill(td::ColorID::Red);

        // Glava
        gui::Rect head(_canvasSize.width / 2.0 - 50,
            _canvasSize.height / 2.0 - 40,
            _canvasSize.width / 2.0 + 50,
            _canvasSize.height / 2.0 + 40);
        gui::Shape faceShape;
        faceShape.createRect(head);
        faceShape.drawFill(td::ColorID::Gold);

        //Lice
        gui::Rect leye(_canvasSize.width / 2.0 - 40,
            _canvasSize.height / 2.0-20,
            _canvasSize.width / 2.0 - 20,
            _canvasSize.height / 2.0 - 30);
        gui::Shape lefteye;
        lefteye.createRect(leye);
        lefteye.drawFill(td::ColorID::Black);

        gui::Rect reye(_canvasSize.width / 2.0 + 20,
            _canvasSize.height / 2.0 - 20,
            _canvasSize.width / 2.0 + 40,
            _canvasSize.height / 2.0 - 30);
        gui::Shape righteye;
        righteye.createRect(reye);
        righteye.drawFill(td::ColorID::Black);
        
        
        // Score bar kao kovanice
        td::INT4 finalScore = _gameManager.getScore();
        int coinCount = finalScore / 10;
        if (coinCount > 20) coinCount = 20;
        for (int i = 0; i < coinCount; i++) {
            gui::Rect coin(_canvasSize.width / 2.0 - 200 + i * 15,
                _canvasSize.height / 2.0 + 100,
                _canvasSize.width / 2.0 - 190 + i * 15,
                _canvasSize.height / 2.0 + 115);
            gui::Shape coinShape;
            coinShape.createRect(coin);
            coinShape.drawFillAndWire(td::ColorID::Yellow, td::ColorID::Orange, 1);
        }
    }


    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isVirtual())
        {
            gui::Key::Virtual vKey = key.getVirtual();

            if (vKey == gui::Key::Virtual::Left)
            {
                _mario.moveLeft();
                return true;
            }
            else if (vKey == gui::Key::Virtual::Right)
            {
                _mario.moveRight();
                return true;
            }
            else if (vKey == gui::Key::Virtual::Up)
            {
                if (_mario.getIsOnGround())
                {
                    _jump.play(); //plax once
                    _mario.jump();
                }
                return true;
            }
        }

        return false;
    }

    bool onKeyReleased(const gui::Key& key) override
    {
        if (key.isVirtual())
        {
            gui::Key::Virtual vKey = key.getVirtual();

            if (vKey == gui::Key::Virtual::Left || vKey == gui::Key::Virtual::Right)
            {
                _mario.stopMoving();
                return true;
            }
        }

        return false;
    }
    protected:
        void checkGoombaCollisions()
        {
            if (!_currentLevel)
                return;

            
            std::vector<GameObject*> enemies = _currentLevel->getEnemies();
            std::vector<Tile*> tiles = _currentLevel->getTiles();

            //Goomba i Tiles
            for (auto enemy : enemies)
            {
                if (!enemy->isAlive())
                    continue;

                gui::Rect goombaRect = enemy->getBounds();
                gui::Point goombaPos = enemy->getPos();
                gui::Point goombaVel = enemy->getVelocity();
                gui::Size goombaSize = enemy->getSize();

                for (auto tile : tiles)
                {
                    if (!tile->isSolid())
                        continue;

                    const gui::Rect& tileRect = tile->getRect();

                    bool intersects = (goombaRect.left < tileRect.right &&
                        goombaRect.right > tileRect.left &&
                        goombaRect.top < tileRect.bottom &&
                        goombaRect.bottom > tileRect.top);

                    if (intersects)
                    {
                        double overlapLeft = goombaRect.right - tileRect.left;
                        double overlapRight = tileRect.right - goombaRect.left;
                        double overlapTop = goombaRect.bottom - tileRect.top;
                        double overlapBottom = tileRect.bottom - goombaRect.top;

                        double minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
                        double minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

                        if (minOverlapX < minOverlapY)
                        {
                            // Horizontalna kolizija
                            Goomba* goomba = dynamic_cast<Goomba*>(enemy);
                            if (goomba)
                                if (goomba->canChangeDirection())
                                    goomba->changeDirection();

                            if (overlapLeft < overlapRight)
                                goombaPos.x = tileRect.left - goombaSize.width;
                            else
                                goombaPos.x = tileRect.right;

                            enemy->setPos(goombaPos);
                        }
                        else
                        {
                            // Vertikalna kolizija
                            if (overlapTop < overlapBottom)
                            {
                                goombaPos.y = tileRect.top - goombaSize.height;
                                goombaVel.y = 0;
                                enemy->setPos(goombaPos);
                                enemy->setVelocity(goombaVel);
                            }
                        }
                    }
                }
            }

            //Mario i Goomba
            gui::Rect marioRect = _mario.getBounds();
            gui::Point marioVel = _mario.getVelocity();

            for (auto enemy : enemies)
            {
                if (!enemy->isAlive())
                    continue;

                gui::Rect goombaRect = enemy->getBounds();

                bool intersects = (marioRect.left < goombaRect.right &&
                    marioRect.right > goombaRect.left &&
                    marioRect.top < goombaRect.bottom &&
                    marioRect.bottom > goombaRect.top);

                if (intersects)
                {
                    double verticalOverlap = marioRect.bottom - goombaRect.top;

                    if (marioVel.y > 0 && verticalOverlap < 20)
                    {
                        // zgnjecenje
                        enemy->setAlive(false);
                        _gameManager.addScore(100);

                        marioVel.y = -400.0;
                        _mario.setVelocity(marioVel);
                    }
                    else
                    {
                        // udarac
                        if (!_mario.getIsInvincible())
                        {
                            _mario.takeDamage();

                            if (_mario.getState() == MarioState::DEAD)
                            {
                                _gameManager.loseLife();

                                if (_gameManager.getLives() <= 0)
                                {
                                    init();
                                    return;
                                }
                            }

                            gui::Point marioPos = _mario.getPos();
                            if (marioRect.left < goombaRect.left)
                                marioPos.x = goombaRect.left - _mario.getSize().width - 5;
                            else
                                marioPos.x = goombaRect.right + 5;

                            _mario.setPos(marioPos);
                        }
                    }
                }
            }
        }
    
        void checkCoinCollisions()
        {
            if (!_currentLevel)
                return;

            gui::Rect marioRect = _mario.getBounds();

            
            std::vector<GameObject*> items = _currentLevel->getItems();

            for (auto item : items)
            {
                // Check da li je Coin
                Coin* coin = dynamic_cast<Coin*>(item);
                if (!coin || coin->getIsCollected())
                    continue;

                gui::Rect coinRect = coin->getBounds();

                bool intersects = (marioRect.left < coinRect.right &&
                    marioRect.right > coinRect.left &&
                    marioRect.top < coinRect.bottom &&
                    marioRect.bottom > coinRect.top);

                if (intersects)
                {
                    coin->collect();
                    _coin.play(); //play once for each coin
                    _gameManager.addCoin();
                    _gameManager.addScore(10);
                }
            }
        }
        void checkPowerUpCollisions()
        {
            if (!_currentLevel)
                return;

            gui::Rect marioRect = _mario.getBounds();

            
            std::vector<GameObject*> items = _currentLevel->getItems();

            for (auto item : items)
            {
                // Check da li je PowerUp
                PowerUp* powerUp = dynamic_cast<PowerUp*>(item);
                if (!powerUp || !powerUp->isAlive() || powerUp->getIsSpawning())
                    continue;

                gui::Rect powerUpRect = powerUp->getBounds();

                bool intersects = (marioRect.left < powerUpRect.right &&
                    marioRect.right > powerUpRect.left &&
                    marioRect.top < powerUpRect.bottom &&
                    marioRect.bottom > powerUpRect.top);

                if (intersects)
                {
                    powerUp->setAlive(false);

                    PowerUpType type = powerUp->getType();

                    if (type == PowerUpType::MUSHROOM)
                    {
                        if (_mario.getPowerUp() == MarioPowerUp::SMALL)
                        {
                            _mario.setPowerUp(MarioPowerUp::BIG);
                            _gameManager.addScore(1000);
                        }
                    }
                    else if (type == PowerUpType::FIRE_FLOWER)
                    {
                        _mario.setPowerUp(MarioPowerUp::FIRE);
                        _gameManager.addScore(1000);
                    }
                    else if (type == PowerUpType::STAR)
                    {
                        _mario.makeInvincible(10.0);
                        _gameManager.addScore(1000);
                    }
                }
            }
        }
        void checkPowerUpTileCollisions()
        {
            if (!_currentLevel)
                return;

            std::vector<GameObject*> items = _currentLevel->getItems();
            std::vector<Tile*> tiles = _currentLevel->getTiles();

            for (auto item : items)
            {
                PowerUp* powerUp = dynamic_cast<PowerUp*>(item);
                if (!powerUp || !powerUp->isAlive() || powerUp->getIsSpawning())
                    continue;

                if (powerUp->getType() == PowerUpType::FIRE_FLOWER)
                    continue;

                gui::Rect powerUpRect = powerUp->getBounds();
                gui::Point powerUpPos = powerUp->getPos();
                gui::Point powerUpVel = powerUp->getVelocity();
                gui::Size powerUpSize = powerUp->getSize();

                for (auto tile : tiles)
                {
                    if (!tile->isSolid())
                        continue;

                    const gui::Rect& tileRect = tile->getRect();

                    bool intersects = (powerUpRect.left < tileRect.right &&
                        powerUpRect.right > tileRect.left &&
                        powerUpRect.top < tileRect.bottom &&
                        powerUpRect.bottom > tileRect.top);

                    if (intersects)
                    {
                        double overlapLeft = powerUpRect.right - tileRect.left;
                        double overlapRight = tileRect.right - powerUpRect.left;
                        double overlapTop = powerUpRect.bottom - tileRect.top;
                        double overlapBottom = tileRect.bottom - powerUpRect.top;

                        double minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
                        double minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

                        if (minOverlapX < minOverlapY)
                        {
                            // Horizontalna kolizija
                            powerUpVel.x = -powerUpVel.x;
                            powerUp->setVelocity(powerUpVel);

                            if (overlapLeft < overlapRight)
                                powerUpPos.x = tileRect.left - powerUpSize.width;
                            else
                                powerUpPos.x = tileRect.right;

                            powerUp->setPos(powerUpPos);
                        }
                        else
                        {
                            // Vertikalna kolizija
                            if (overlapTop < overlapBottom)
                            {
                                powerUpPos.y = tileRect.top - powerUpSize.height;
                                powerUpVel.y = 0;
                                powerUp->setPos(powerUpPos);
                                powerUp->setVelocity(powerUpVel);
                            }
                        }
                    }
                }
            }
        }
};
