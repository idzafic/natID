//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Transformation.h>
#include <math/Constants.h>
#include <math/math.h>
#include <gui/Sound.h>
#include <gui/DrawableString.h>
#include <gui/Font.h>
#include <vector>
#include <map>
#include "Graph.h"
#include <thread/Thread.h>
#include <functional>
#include <gui/Thread.h>
#include <gui/Image.h>
#include <td/MutableString.h>

const float cTownR=4;

using CallBack = std::function<void(int)>;
using MapTownNameToID = std::map<td::String, GraphType>;

enum class Algorithm : td::BYTE {BFS=0, DFS, DFSWithHeuristics, GreedyBestFS, UCS, AStar};

class Primitive
{
public:
    class Options
    {
    public:
        gui::Image imgStart;
        gui::Image imgGoal;
        gui::Size viewSize;
        td::MutableString mStr;
        GraphType startID = 1;
        GraphType goalID = 10;
        float ratioLong = 400;
        float ratioLat = 375;
        int sleepMS = 500;
        int flagSize = 32;
        
        Algorithm algorithm = Algorithm::BFS;
        td::ColorID normalColor;
        td::ColorID visitedColor;
        td::ColorID pathColor;
        td::ColorID txtColor;
        bool showAllTownNames = true;
        bool showRoadNames = false;
        bool showRoadLengths = true;
        Options()
        : imgStart(":source")
        , imgGoal(":goal")
        {
            mStr.reserve(256); //for faster drawing
            gui::Application* pApp = gui::getApplication();
            auto appProperties = pApp->getProperties();
            showAllTownNames = appProperties->getValue("showTownNames", showAllTownNames);
            showRoadNames = appProperties->getValue("showRoadNames", showRoadNames);
            showRoadLengths = appProperties->getValue("showRoadLengths", showRoadLengths);
        }
        
        bool isHeuristicRequired() const
        {
            return false;
            //return ((algorithm == Algorithm::DFSWithHeuristics) || (algorithm == Algorithm::AStar));
        }
    };
    enum class Status : td::BYTE {Unvisited=0, Visited, OnPath};

protected:
    td::String _name;
    gui::Shape _shape;
    GraphType _id = 0;
    Status _guiState = Status::Unvisited;
    
protected:
    void reset()
    {
        _guiState = Status::Unvisited;
    }
public:
    Primitive() = default;

    Primitive(const td::String& name)
        : _name(name)
    {
    }

    // Move constructor
    Primitive(Primitive&& p) noexcept
    : _name(p._name)
    , _shape(std::move(p._shape))
    , _id(p._id)
    , _guiState(p._guiState)
    {
    }

    // Move assignment
    Primitive& operator=(Primitive&& p) noexcept
    {
        if (this != &p)
        {
            _name = p._name;
            _shape = std::move(p._shape);
            _id = p._id;
            _guiState = p._guiState;
        }
        return *this;
    }

    // Deleted copy operations (Shape is non-copyable)
    Primitive(const Primitive&) = delete;
    Primitive& operator=(const Primitive&) = delete;
    
    const td::String& getName() const
    {
        return _name;
    }
    
    void setGUIState(Status status)
    {
        _guiState = status;
    }
    
    Primitive::Status getGUIState() const
    {
        return _guiState;
    }
    
    void setID(GraphType primID)
    {
        _id = primID;
    }
    
    GraphType getID() const
    {
        return _id;
    }
};


