//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Sound.h>
//#include "QueenModel.h"
#include "Chess.h"

class ViewChessBoard : public gui::Canvas
{
protected:
    Chess _chess;
    gui::Sound _soundMove;
    gui::Sound _soundMissionSuccess;
    gui::Size _size;
    std::function<void()> _fnUpdateMenuAndTB;
protected:
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _chess.updateModelSize(newSize);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        _chess.draw();
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        _chess.canDragPiece(modelPoint);
    }

    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override
    {
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        if (_chess.finishUserMove(modelPoint))
        {
            _soundMove.play();
            if (_chess.canAIMakeMove())
            {
                //start move AI thread
                auto mainThreadCallBack = std::make_shared<gui::thread::MainThreadFunction1>(std::bind(&ViewChessBoard::chessEngineCallBack, this, std::placeholders::_1));
                _chess.moveAI(mainThreadCallBack);
            }
            
            reDraw();
        }
    }
    
    void onCursorDragged(const gui::InputDevice& inputDevice) override //onMouseDragged(const gui::Point& modelPoint, td::UINT4 keyModifiers)
    {
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        if (_chess.testDrag(modelPoint))
            reDraw();
    }
    
    void chessEngineCallBack(td::Variant param)
    {
        td::INT4 iVal = param.i4Val();
        Message msg = Message(iVal);
        switch (msg)
        {
            case Message::Stop:
                onWorkerCompleted();
                break;
            case Message::StopOnGoal:
                onWorkerCompleted();
                _soundMissionSuccess.play();
                break;
            case Message::AIMove:
            {
                _soundMove.play();
                //clean the thread state
                auto& th = _chess.getThread();
                if (th.joinable())
                    th.join();
            }
                break;
            default:
                assert(false); //we have to handle all message types
        }
        reDraw(); //invoke reDraw manually after each state change
    }
    
public:
    //enable mouse clicks, drags, etc..
    ViewChessBoard(const std::function<void()>& fnUpdateMenuAndTB)
    : Canvas({gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::CursorDrag})
    , _chess(8)
    , _soundMove(":move")
    , _soundMissionSuccess(":success")
    , _fnUpdateMenuAndTB(fnUpdateMenuAndTB)
    {
        setPreferredFrameRateRange(60, 60);
        enableResizeEvent(true); //required to obtain events whenever this view resizes (see onResize method in this class)
    }
    
    void onWorkerCompleted()
    {
        auto& th = _chess.getThread();
        if (th.joinable())
            th.join();
        _fnUpdateMenuAndTB();
        reDraw();
    }
    
    void stop()
    {
        _chess.stopPlaying();
        _fnUpdateMenuAndTB();
    }
    
    bool isPlaying() const
    {
        return _chess.isPlaying();
    }
    
    void refresh()
    {
        reDraw(); //this is async call and will be executed later
    }
    
    void startStop()
    {
        if (_chess.isPlaying())
        {
            stop();
        }
        else
        {
            _chess.start();
            
            auto mainThreadCallBack = std::make_shared<gui::thread::MainThreadFunction1>(std::bind(&ViewChessBoard::chessEngineCallBack, this, std::placeholders::_1));
            _chess.moveAI(mainThreadCallBack);
            
            _fnUpdateMenuAndTB();

        }
    }
    
    void reset()
    {
        if (_chess.isPlaying())
        {
            gui::Sound::play(gui::Sound::Type::Beep);
            return;
        }
        _chess.reset(true);
        reDraw();
    }
};
