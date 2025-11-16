//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Sound.h>
#include "QueenModel.h"

class ViewChessBoard : public gui::Canvas
{
protected:
    QueenModel _model;
    gui::Sound _soundQueenPlaced;
    gui::Sound _soundCollectPieces;
    gui::Size _size;
    std::function<void()> _fnUpdateMenuAndTB;
protected:
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _model.updateModelSize(newSize);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        _model.draw();
    }
    
public:
    ViewChessBoard(const std::function<void()>& fnUpdateMenuAndTB)
    : Canvas({gui::InputDevice::Event::Keyboard}) //enable keyboard events
    , _soundQueenPlaced(":move")
    , _soundCollectPieces(":collect")
    , _fnUpdateMenuAndTB(fnUpdateMenuAndTB)
    {
        setPreferredFrameRateRange(60, 60);
        enableResizeEvent(true); //required to obtain events whenever this view resizes (see onResize method in this class)
    }
    
    void onWorkerCompleted()
    {
        auto& th = _model.getThread();
        if (th.joinable())
            th.join();
        
        if (isAnimating())
            stopAnimation();
        _fnUpdateMenuAndTB();
        reDraw();
    }
    
    void start()
    {
        auto& options = _model.getOptions();
        startAnimation();
        // Wrap member function 'stop' into a lambda or std::function
        auto callBackStop = std::make_shared<gui::thread::MainThreadFunction1>([this](td::Variant param){
            td::INT4 iVal = param.i4Val();
            Message msg = Message(iVal);
            switch (msg)
            {
                case Message::Stop:
                    onWorkerCompleted();
                    break;
                case Message::PlayQueenPlaced:
                    if (_model.getOptions().playSoundOnQueenPlacement)
                        _soundQueenPlaced.play();
                    break;
                case Message::PlayPiecesCollected:
                    if (_model.getOptions().playSoundOnQueenPlacement)
                        _soundCollectPieces.play();
                    break;
                default:
                    assert(false); //we have to handle all message types
            }
        });
        
        _model.solve(callBackStop);
        
        _fnUpdateMenuAndTB();
    }
    
    void stop()
    {
        _model.stopSearching();
    }
    
    bool isRunning() const
    {
        return isAnimating();
    }
private:
    
public:
    
    QueenModel::Options& getOptions()
    {
        return _model.getOptions();
    }
    
    void refresh()
    {
        reDraw(); //this is async call and will be executed later
    }
    
    void startStop()
    {
        if (isRunning())
        {
            stop();
        }
        else
        {
            start();
        }
    }
};
