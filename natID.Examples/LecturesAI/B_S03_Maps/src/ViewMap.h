//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include "MapModel.h"
#include <td/Timer.h>
#include <xml/DOMParser.h>
#include <vector>
#include <td/MutableString.h>

constexpr float cMargin = 0.05f;

class ViewMap : public gui::Canvas
{
protected:
    std::vector<td::String> _nodes;
    
    Model _model;
    
    gui::Sound _flightSound;
    gui::Sound _cheeringSound;
    gui::Size _size;
    gui::DrawableString _strInfo;
    std::function<void()> _fnUpdateMenuAndTB;
    td::Timer<false> _fpsTimer; //measures time between last FPS display and current frame
    td::Timer<false> _solutionTimer;
    gui::CoordType _infoHeight = 0;
    td::MutableString _mStrInfo;
    
    double _FPS = 0;        //frame per second
    int _nFrames = 0;       //used for frame per second computation
    
    bool _playSound = true;
protected:
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _model.updateModelSize(newSize);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        _mStrInfo.reset();
        if (isAnimating())
        {
            double dT = _fpsTimer.getDurationInSeconds();
            if (dT > 0.25) //update FPS each 0.25s
            {
                _fpsTimer.start();
                _FPS = ++_nFrames / dT;
                
                _nFrames = 0;
            }
            else
            {
                ++_nFrames;
            }
        }
        else
        {
            _fpsTimer.stop();
            _nFrames = 0;
        }
        
        auto time = _solutionTimer.getDurationInMilliseconds();
        if (_FPS > 0.09)
            _mStrInfo.appendFormat("FPS:%.1f\nt=%.1f ms", _FPS, time);
        else
            _mStrInfo.appendFormat("FPS:-.-\nt=%.3f ms", time);
        
        _model.draw(_mStrInfo);
        
        if (_infoHeight == 0)
        {
            const td::String& strInfo = _mStrInfo.getString();
            gui::Size infoSize;
            gui::DrawableString::measure(strInfo, infoSize, gui::Font::ID::SystemLargerBold);
            _infoHeight = infoSize.height;
        }
        
        const td::String& str = _mStrInfo.getString();
        if (!str.isEmpty())
        {
            auto loc = _model.getInfoLocation();
            switch (loc)
            {
                case Model::InfoLoc::TopLeft:
                {
                    gui::Point pt(5, 5); //5 pixels offset from the view's bottom
                    gui::DrawableString::draw(str, pt, gui::Font::ID::SystemLargerBold, td::ColorID::SysText);
                }
                    break;
                case Model::InfoLoc::BottomLeft:
                {
                    gui::Point pt(5,_size.height - _infoHeight - 5); //5 pixels offset from the view's bottom
                    gui::DrawableString::draw(str, pt, gui::Font::ID::SystemLargerBold, td::ColorID::SysText);
                }
                    break;
                case Model::InfoLoc::TopRight:
                {
                    gui::Point pt(_size.width - 5, 5); //5 pixels offset from the view's bottom
                    
                    gui::Rect r(0, pt.y, pt.x, _size.height);
                    //td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign
                    gui::DrawableString::draw(str, r, gui::Font::ID::SystemLargerBold, td::ColorID::SysText, td::TextAlignment::Right);
                }
                    break;
                case Model::InfoLoc::BottomRight:
                {
                    gui::Point pt(_size.width - 5, _size.height - _infoHeight - 5); //5 pixels offset from the view's bottom
                    
                    gui::Rect r(0, pt.y, pt.x, _size.height);
                    //td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign
                    gui::DrawableString::draw(str, r, gui::Font::ID::SystemLargerBold, td::ColorID::SysText, td::TextAlignment::Right);
                }
                    break;
                default:
                    assert(false);
            }
        }
    }
    
public:
    ViewMap(const std::function<void()>& fnUpdateMenuAndTB)
    : Canvas() //enable keyboard events
    , _flightSound(":flight")
    , _cheeringSound(":cheering")
    , _strInfo(tr("info"))
    , _fnUpdateMenuAndTB(fnUpdateMenuAndTB)
    {
        gui::Application* pApp = gui::getApplication();
        auto appProperties = pApp->getProperties();
        _playSound = appProperties->getValue("playSound", _playSound);
        
        _mStrInfo.reserve(128);
        setPreferredFrameRateRange(60, 60);
        enableResizeEvent(true); //required to obtain onResize events (virtual method in this class)
    }

    
    void threadCompleted()
    {
        auto& th = _model.getThread();
        if (th.joinable())
            th.join();
        
        stopAnimation();
        if (_flightSound.getState() != gui::Sound::State::NotPlaying)
            _flightSound.stop();
        _fnUpdateMenuAndTB();
        _solutionTimer.stop();
        if (_playSound && _model.isGoalFound())
            _cheeringSound.play(); //play once to celebrate the goal achievement
        reDraw();
    }
    
    void start()
    {
        if (_playSound)
            _flightSound.play(true);
        _fpsTimer.start();
        _solutionTimer.start();
        startAnimation();
        // Wrap member function 'stop' into a lambda or std::function
        auto callBackStop = std::make_shared<gui::thread::MainThreadFunction>([this](){
            threadCompleted();
        });
        
        _model.findPath(callBackStop);
        
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
    bool loadModel(const td::String& fileName)
    {
        _model.clean();
        reDraw(); //async call - redraw will happen later
        //if (_model.isEmpty())
        {
            xml::FileParser parser;
            if (!parser.parseFile(fileName))
                return false;
            
            const auto& root = parser.getRootNode();
            if (root->getName().cCompare("Map") != 0)
                return false;
            
            float ratioLong = 100;
            float ratioLat = 75;
            root.getAttribValue("ratioLong", ratioLong);
            root.getAttribValue("ratioLat", ratioLat);
            float scale = 1;
            root.getAttribValue("scale", scale);
            
            td::String infoLoc;
            root.getAttribValue("infoLoc", infoLoc);
            _model.setInfoLocPosition(infoLoc);
            
            
            auto towns = root.getChildNode("Towns");
            if (!towns)
                return false;
            
            //Towns
            auto town = towns.getChildNode("Town");
            if (!town)
                return false;
            auto nNodes = town.getCounter();
            _model.init(nNodes);
            
            auto it = town;
            double yMin = 0;
            it.getAttribValue("lat", yMin);
            double yMax = yMin;
            
            double xMin = 0;
            it.getAttribValue("long", xMin);
            double xMax = xMin;
            
            ++it;
            while (it.isOk())
            {
                double x =0;
                it.getAttribValue("long", x);
                xMin = math::Min(x, xMin);
                xMax = math::Max(x, xMax);
                
                double y =0;
                it.getAttribValue("lat", y);
                yMin = math::Min(y, yMin);
                yMax = math::Max(y, yMax);
                
                ++it;
            }
            xMin *= scale;
            xMax *= scale;
            yMin *= scale;
            yMax *= scale;
            
            xMin -= cMargin;
            xMax += cMargin;
            yMin -= cMargin;
            yMax += cMargin;
            
            double dx = xMax - xMin;
            double dy = yMax - yMin;
            if (dx == 0 || dy == 0)
                return false;
            
            _model.setDistanceRatios(ratioLat*dy, ratioLong*dx);
            //
            it = town;
            
            while (it.isOk())
            {
                td::String nodeID;
                if (!it.getAttribValue("id", nodeID))
                {
                    ++it;
                    continue;
                }
                
                if (_model.getNodeID(nodeID))
                {
                    mu::dbgLog("Node %s entered twice", nodeID.c_str());
                    ++it;
                    continue;
                }
                
                double x =0;
                if (!it.getAttribValue("long", x))
                {
                    ++it;
                    continue;
                }
                x *= scale;
                x = (x-xMin) / dx;
                
                double y =0;
                if (!it.getAttribValue("lat", y))
                {
                    ++it;
                    continue;
                }
                y *= scale;
                y = (yMax-y) / dy;
                td::String name;
                if (!it.getAttribValue("name", name))
                {
                    ++it;
                    continue;
                }
                
                //add town to Model
                gui::Point location(x, y);
                Town t(name, location, _size);
                _model.append(t, nodeID);
                ++it;
            }
            
            //Roads
            auto roads = root.getChildNode("Roads");
            if (!roads)
                return false;
            
            auto road = roads.getChildNode("Road");
            if (!road)
                return false;
            
            auto nRoads = road.getCounter();
            
            _model.reserveRoads(nRoads);
            
            //<Road fromNode="BROD" toNode="DO" type="1" name="A1"/>
            while (road.isOk())
            {
                td::String fromNode;
                road.getAttribValue("fromNode", fromNode);
                td::String toNode;
                road.getAttribValue("toNode", toNode);
                
                float cost = -1;
                road.getAttribValue("cost", cost);
                
                td::String roadName;
                road.getAttribValue("name", roadName);
                GraphType fromID = _model.getNodeID(fromNode);
                GraphType toID = _model.getNodeID(toNode);
                
                
                _model.addRoad(roadName, fromNode, toNode, _size, cost);
                ++road;
            }
            
            _model.checkTownsWithoutConnection();
        }
        return true;
    }
    
    Primitive::Options& getOptions()
    {
        return _model.getOptions();
    }
    
    const MapTownNameToID& getMapNameToID() const
    {
        return _model.getMapNameToID();
    }
    
    void refresh()
    {
        reDraw();
        _model.calcHeuristics();
    }
    
    void playSound(bool bPlay)
    {
        if (_model.isSearching())
        {
            auto soundPlayerState = _flightSound.getState();
            if (!bPlay)
            {
                if (soundPlayerState == gui::Sound::State::ContinuousPlay)
                    _flightSound.stop();
            }
            else
            {
                if (soundPlayerState == gui::Sound::State::NotPlaying)
                    _flightSound.play(true); //continuous play
            }
        }
        _playSound = bPlay;
    }
};
