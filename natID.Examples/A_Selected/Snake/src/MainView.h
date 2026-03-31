#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ColorPicker.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Timer.h>
#include <gui/Sound.h>
#include "ViewSnake.h"

const float FPS = 50.0f; //desired frame per second
const float dT = 1 / FPS;

class MainView : public gui::View
{
private:
protected:
    ViewSnake _snake;
    gui::GridLayout _gl;
    gui::Timer _timer;
    std::function<void()>* _pUpdateMenuAndTB;
protected:
   
public:
    MainView(std::function<void()>* pUpdateMenuAndTB, std::function<void(td::INT1, int, int)>* pUpdateScrollBar)
    : _snake(pUpdateMenuAndTB, pUpdateScrollBar)
    , _gl(1, 1)
    , _timer(this, dT, false)
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    {   
        gui::GridComposer gc(_gl);
        gc.appendRow(_snake);
        setLayout(&_gl);
    }

    void focusOnCanvas() 
    {
        _snake.setFocus(true);
    }
    bool isRunning() const
    {
        return _snake.isGameInProgress();
    }
    bool setGameDifficulty(Difficulty d)
    {
        return _snake.setDifficulty(d);
    }

    void startStop()
    {
        if (isRunning())
        {
            _timer.stop();
            _snake.setGameOver();
            (*_pUpdateMenuAndTB)();
        }
        else
        {
            _timer.start();
            _snake.newGame();
            (*_pUpdateMenuAndTB)();
        }
    }
protected:
    bool onTimer(gui::Timer* pTimer) override
    {
        _snake.timeStep();
        return true;
    }

};
