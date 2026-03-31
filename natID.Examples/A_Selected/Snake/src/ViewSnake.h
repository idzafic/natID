#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Transformation.h>
#include <math/Constants.h>
#include <math/math.h>
#include <gui/DrawableString.h>
#include <gui/Sound.h>
#include <random>
#include <cnt/FullQueue.h>
#include <vector>
#include "Difficulty.h"


class ViewSnake : public gui::Canvas
{
public:
protected:
    const gui::CoordType _difficultySpeed[3] = { 2.5, 5., 10. };
    enum class Direction { NONE=0, LEFT, RIGHT, UP, DOWN };
    size_t _maxSnakeSize = 512;
protected:

    std::function<void()>* _pUpdateMenuAndTB;
    
    std::function<void(td::INT1, int, int)>* _pUpdateScrollBar;

    gui::Symbol _snake;
    gui::Symbol _snakeOM;
    gui::Symbol _snakeBodyElem;
    gui::Point _snakePos;
    float _snakeAngle = 0.f;
    size_t _currentSnakeSize = 0;
    size_t _startSnakeSize = 0; //koliko elemenata ima zmija na pocetku igre
    size_t _gracePeriod = 5; //koliko vocki zmija moze pojesti prije nego se ukljuci detekcija kolizije za repom
    size_t _growthFactor = 1; //za koliko elemenata naraste rep zmije kada pojede vocku
    cnt::Ring<cnt::SafeFullVector<std::pair<gui::Point, float>>, true, false> _headPosHistory;
    //cnt::FullQueue<std::pair<gui::Point, float>> _headPosHistory;
    
    //std::deque<std::pair<gui::Point, float>> _headPosHistory;
    size_t _headPosHistorySize = _maxSnakeSize;
    size_t _stepsInElemWidth = 1;
    gui::Image _food[3] = { (":apple"), (":pear"), (":banana") };

    gui::Size _size;

    bool _gameInProgress = false;
    Direction _direction = Direction::NONE;
    gui::CoordType _stepSize = _difficultySpeed[static_cast<int>(Difficulty::Hard)];

    gui::Shape _bounds;
    gui::CoordType _boundsSize = 4.; //debljina kaveza oko arene
    gui::Rect _boundsRect;

    gui::CoordType _foodSize = 30.; //velicina vocke
    gui::Rect _foodRect;
    gui::CoordType _foodMinimumDistanceFromSnakeHead = 100.; //nova vocka koja se stvori ne moze biti blize zmijinoj glavi od ove udaljenosti
    gui::CoordType _foodMaximumDistanceFromSnakeHead = 500.; //nova vocka koja se stvori ne moze biti dalje od zmijine glave od ove udaljenosti

    bool _gameOver = false;
    gui::Image _gameOverImg;
    
    int _fruitScore[3] = { 0, 0, 0 };
    td::String _scoreString;

    std::random_device _randomDevice;
    std::mt19937 _rng;
    std::uniform_int_distribution<std::mt19937::result_type> _randomDistributionW, _randomDistributionH;
    std::uniform_int_distribution<std::mt19937::result_type> _randomFruit;
    td::INT1 _currentFruit;
    gui::Sound _soundBackground;
    gui::Sound _soundEating;
    gui::Sound _soundGameOver;
    td::INT4 _score = 0;
    td::INT4 _highScore = 0;
    td::WORD _mouthState = 0;
    td::BYTE _openMouth = 0;

public:
    ViewSnake(std::function<void()>* pUpdateMenuAndTB, std::function<void(td::INT1, int, int)>* pUpdateScrollBar)
    : gui::Canvas({ gui::InputDevice::Event::Keyboard })
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    , _pUpdateScrollBar(pUpdateScrollBar)
    , _snake(":snake")
    , _snakeOM(":snakeOM")
    , _snakeBodyElem(":snakeBody")
    , _gameOverImg(":gameover")
    , _rng(_randomDevice())
    , _randomFruit(0, 2)
    , _soundBackground(":background")
    , _soundEating(":eatFood")
    , _soundGameOver(":gameOver")
    {
        _highScore = getApplication()->getProperties()->getValue("snakeHS", 0);
        setHistoryBufferSize();
        enableResizeEvent(true);
        auto appProperties = getAppProperties();
        _highScore = appProperties->getValue("highScore", (td::INT4) 0);
    }

    bool isGameInProgress() const
    {
        return _gameInProgress;
    }

    bool setDifficulty(Difficulty d)
    {
        if (_gameInProgress)
            return false;

        _stepSize = _difficultySpeed[static_cast<int>(d)];
        _headPosHistory.reset();
        setHistoryBufferSize();
        return true;
    }

    void newGame()
    {
        _snakePos = { _size.width / 2, _size.height / 2 };
        _gameInProgress = true;
        _gameOver = false;
        _score = 0;
        _fruitScore[0] = 0;
        _fruitScore[1] = 0;
        _fruitScore[2] = 0;

        _currentSnakeSize = math::Min(_startSnakeSize, _maxSnakeSize);
        _direction = Direction::NONE;
        _snakeAngle = 0.f;
        _headPosHistory.reset();
        newFood();

        reDraw();
    }
    void timeStep()
    {
        ++_mouthState;
        if (_mouthState > 10)
        {
            if (_openMouth == 1)
                _openMouth = 0;
            else
                _openMouth = 1;

            _mouthState = 0;
        }
            
        handleSnakeMovement();
        reDraw();
    }
    
    void setGameOver()
    {
        _soundGameOver.play();

        _gameInProgress = false;
        _direction = Direction::NONE;
        _gameOver = true;
        _soundBackground.stop();
        prepareGameOverScreen();
        (*_pUpdateMenuAndTB)();
        reDraw();
    }
protected:
    void setHistoryBufferSize() 
    {
        auto bodyElemW = _snakeBodyElem.getBounds().width();
        assert(bodyElemW >= _stepSize);
        _stepsInElemWidth = math::Max<size_t>(std::floor(bodyElemW / _stepSize), 1);
        _headPosHistorySize = _stepsInElemWidth * _maxSnakeSize;
        _headPosHistory.resize(_headPosHistorySize);
    }
    gui::CoordType distance(gui::Point a, gui::Point b)
    {
        auto dx = a.x - b.x;
        auto dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    gui::CoordType randomFoodPointX()
    {
        return _randomDistributionW(_rng);
    }

    gui::CoordType randomFoodPointY()
    {
        return _randomDistributionH(_rng);
    }

    void newFood()
    {
        _foodRect = { randomFoodPointX(), randomFoodPointY(), _foodSize };

        for (auto f = distance(_foodRect.center(), _snakePos); f < _foodMinimumDistanceFromSnakeHead || f > _foodMaximumDistanceFromSnakeHead; f = distance(_foodRect.center(), _snakePos))
            _foodRect = { randomFoodPointX(), randomFoodPointY(), _foodSize };

        _currentFruit = _randomFruit(_rng);
    }


    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _snakePos = { _size.width / 2, _size.height / 2 };
        _randomDistributionW = std::uniform_int_distribution<std::mt19937::result_type>(_foodSize, (std::mt19937::result_type)_size.width - _foodSize);
        _randomDistributionH = std::uniform_int_distribution<std::mt19937::result_type>(_foodSize, (std::mt19937::result_type)_size.height - _foodSize);
       
        auto offset = _boundsSize / 2.;
        gui::Point tl{ offset, offset }, br{ _size.width - offset, _size.height - offset };
        _boundsRect = { tl, br };
        _bounds.createRect(_boundsRect, _boundsSize);
    }

 
    void prepareGameOverScreen()
    {
        cnt::StringBuilderSmall sb;

        if (_score > _highScore)
        {
            _highScore = _score;
            sb << tr("GZBeatHS:") << ' ' << _score;
            auto appPropertis = getAppProperties();
            appPropertis->setValue("highScore", _highScore);
        }
        else
        {
            sb << tr("Score:") << ' ' << _score;
            sb << '\n' << tr("Highscore:") << ' ' << _highScore;
        }

        _scoreString = sb.toString();
    }

    void drawGameOver(const gui::Rect& rect)
    {
        _gameOverImg.draw(rect);
        gui::DrawableString scoreShow(_scoreString);
        gui::Size sz;
        scoreShow.measure(gui::Font::ID::SystemLargestBold, sz);
        gui::Point sp = { 0.5 * (_size.width - sz.width)  , 0.6 * _size.height };

        scoreShow.draw(sp, gui::Font::ID::SystemLargestBold, td::ColorID::Red);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        if (_gameOver)
        {
            drawGameOver(rect);

            return;
        }

        _bounds.drawFillAndWire(td::ColorID::LightBlue, td::ColorID::SaddleBrown);

        if (_gameInProgress)
        {
            _food[_currentFruit].draw(_foodRect);

            if (_headPosHistory.size() > _stepsInElemWidth)
            {
                size_t drawUntilUndex = math::Min( (size_t) (_currentSnakeSize * _stepsInElemWidth), _headPosHistory.size() - 1);
                for (size_t i = _stepsInElemWidth; i <= drawUntilUndex; i += _stepsInElemWidth)
                {
                    auto bodyElemPos = _headPosHistory[i];
                    _snakeBodyElem.drawDeg(bodyElemPos.first, bodyElemPos.second);
                }
            }

            if (_openMouth)
                _snake.drawDeg(_snakePos, _snakeAngle);
            else
                _snakeOM.drawDeg(_snakePos, _snakeAngle);

        }

    }

    void checkCollisionWithWalls()
    {
        //make game easier, avoid collision with antenna
        auto snakeHeadRect = _snake.getBounds();
        snakeHeadRect.top = snakeHeadRect.left;
        snakeHeadRect.right = -snakeHeadRect.left;
        snakeHeadRect.bottom = -snakeHeadRect.left;

        auto snakeLeft = snakeHeadRect.left + _snakePos.x;
        if (snakeLeft < _boundsRect.left)    
            setGameOver();
        
        auto snakeRight = snakeHeadRect.right + _snakePos.x;
        if (snakeRight > _boundsRect.right)
            setGameOver();

        auto snakeTop = snakeHeadRect.top + _snakePos.y;
        if (snakeTop < _boundsRect.top)
            setGameOver();

        auto snakeBottom = snakeHeadRect.bottom + _snakePos.y;
        if (snakeBottom > _boundsRect.bottom)
            setGameOver();
    }

    void checkCollisionWithSelf()
    {
        if (_score <= _gracePeriod)
            return;

        gui::Rect snakeHeadRectInSceneCoord = translateToSceneCoord(_snake.getBounds(), 1., _snakePos);

        if (_headPosHistory.size() > _stepsInElemWidth)
        {
            size_t drawUntilUndex = math::Min( (size_t) (_currentSnakeSize * _stepsInElemWidth), _headPosHistory.size() - 1);
            for (size_t i = _stepsInElemWidth * 5; i <= drawUntilUndex; i += _stepsInElemWidth)
            {
                auto bodyElemPos = _headPosHistory[i];
                if (snakeHeadRectInSceneCoord.contains(_headPosHistory[i].first))
                {
                    setGameOver();
                }
            }
        }

    }

    void onEatFood()
    {
        _soundEating.play();
        _score++;
        _fruitScore[_currentFruit]++;
        
        if (_pUpdateScrollBar)
            (*_pUpdateScrollBar)(_currentFruit, _fruitScore[_currentFruit], _score);
        
        _currentSnakeSize = math::Min(_currentSnakeSize + _growthFactor, _maxSnakeSize);
        newFood();
    }

    void checkCollisionWithFood()
    {
        gui::Rect snakeHeadRectInSceneCoord = translateToSceneCoord(_snake.getBounds(), 0.3, _snakePos);//mali mult
        if (_foodRect.intersects(snakeHeadRectInSceneCoord))
        {
            onEatFood();
        }
    }

    const gui::Rect translateToSceneCoord(const gui::Rect& shapeCoord, gui::CoordType mult, const gui::Point& point)
    {
        return { mult * shapeCoord.left + point.x, mult * shapeCoord.top + point.y, mult * shapeCoord.right + point.x, mult * shapeCoord.bottom + point.y };
    }

    void handleSnakeMovement()
    {
        if (_gameOver)
            return;

        if (_direction == Direction::RIGHT)
        {
            _snakeAngle = 0.f;
            _snakePos.x += _stepSize;
        }
        else if (_direction == Direction::LEFT)
        {
            _snakeAngle = 180.f;
            _snakePos.x -= _stepSize;
        }
        else if (_direction == Direction::UP)
        {
            _snakeAngle = -90.f;
            _snakePos.y -= _stepSize;
        }
        else if (_direction == Direction::DOWN)
        {
            _snakeAngle = 90.f;
            _snakePos.y += _stepSize;
        }

        _headPosHistory.emplace(_snakePos, _snakeAngle);

        checkCollisionWithWalls();
        checkCollisionWithSelf();
        checkCollisionWithFood();
    }

    bool onKeyReleased(const gui::Key& key) override
    {
        return true;
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isVirtual())
        {
            gui::Key::Virtual chVirt = key.getVirtual();
            
            if (_direction == Direction::NONE)
            {
                if (chVirt == gui::Key::Virtual::Right)
                {
                    if (_gameInProgress)
                        _soundBackground.play(true);

                    _direction = Direction::RIGHT;
                    return true;
                }
                else if (chVirt == gui::Key::Virtual::Left)
                {
                    if (_gameInProgress)
                        _soundBackground.play(true);

                    _direction = Direction::LEFT;
                    return true;
                }
                else if (chVirt == gui::Key::Virtual::Up)
                {
                    if (_gameInProgress)
                        _soundBackground.play(true);

                    _direction = Direction::UP;
                    return true;
                }
                else if (chVirt == gui::Key::Virtual::Down)
                {
                    if (_gameInProgress)
                        _soundBackground.play(true);

                    _direction = Direction::DOWN;
                    return true;
                }
            }

            if (chVirt == gui::Key::Virtual::Right)
            {
                if (_direction != Direction::LEFT)
                    _direction = Direction::RIGHT;
                return true;
            }
            if (chVirt == gui::Key::Virtual::Left)
            {
                if (_direction != Direction::RIGHT)
                    _direction = Direction::LEFT;
                return true;
            }
            if (chVirt == gui::Key::Virtual::Up)
            {
                if (_direction != Direction::DOWN)
                    _direction = Direction::UP;
                return true;
            }
            if (chVirt == gui::Key::Virtual::Down)
            {
                if (_direction != Direction::UP)
                    _direction = Direction::DOWN;
                return true;
            }
        }
        
        //propagate call to base class (chek start/stop)
        return gui::Canvas::onKeyPressed(key);
    }

};
