//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include "Primitive.h"

class Town : public Primitive
{
public:
    enum class State : td::BYTE {Unseen=0, Frontier, Explored};
    enum class CostChange : td::BYTE {InitialSet=0, UpdatedWithBetterCost, UpdateNotPossibleDueToLargerCost, UpdateNotPossibleNodeExplored};
protected:
    Transition _backTransition{0,0};
    gui::Point _location;
    float _airDistanceToGoal = 0; //Heuristics (from this node to the goal node)
    float _pathDistance = 0;    //real distance from the start node to this node
    float _fCost = 0;           //f-cost for A* (g + h)
    td::UINT4 _nRoads = 0;
    State _state = State::Unseen;
public:
    Town() = default;

    Town(const td::String& name, const gui::Point& location, const gui::Size& sz)
        : Primitive(name)
        , _location(location)
    {
        gui::Point center(location.x * sz.width, location.y * sz.height);
        gui::Circle c(center, cTownR);
        _shape.createCircle(c);
    }

    // Move constructor
    Town(Town&& t) noexcept
    : Primitive(std::move(t))
    , _backTransition(t._backTransition)
    , _location(t._location)
    , _airDistanceToGoal(t._airDistanceToGoal)
    , _pathDistance(t._pathDistance)
    , _fCost(t._fCost)
    , _nRoads(t._nRoads)
    {
    }

    // Move assignment
    Town& operator=(Town&& t) noexcept
    {
        if (this != &t)
        {
            Primitive::operator=(std::move(t));
            _backTransition = std::move(t._backTransition);
            _location = std::move(t._location);
            _airDistanceToGoal = t._airDistanceToGoal;
            _fCost = t._fCost;
            _nRoads = t._nRoads;

        }
        return *this;
    }

    // Delete copy
    Town(const Town&) = delete;
    Town& operator=(const Town&) = delete;

    const gui::Point& getLocation() const { return _location; }

    void updatePosition(const gui::Size& sz)
    {
        gui::Point newLocation(_location.x * sz.width, _location.y * sz.height);
        gui::Circle c(newLocation, cTownR);
        _shape.updateCircleNodes(c);
    }
    
protected:
    void showTownName(const Primitive::Options& options, td::ColorID colorID) const
    {
        if (!options.showAllTownNames)
        {
            if (_guiState != Status::OnPath)
                return;
        }
        td::TextAlignment align = td::TextAlignment::Left;
        gui::Point townLocation(_location.x * options.viewSize.width, _location.y * options.viewSize.height);
        gui::Rect r(townLocation, options.viewSize);
        if (_location.x > 0.9)
        {
            align = td::TextAlignment::Right;
            r.left = 0;
            r.right = townLocation.x;
        }
        else if (_location.x > 0.1)
        {
            align = td::TextAlignment::Center;
            r.left = townLocation.x - options.viewSize.width / 2;
            r.right = townLocation.x + options.viewSize.width / 2;
        }
        
        if (options.isHeuristicRequired())
        {
            td::MutableString& mStr = const_cast<td::MutableString&>(options.mStr);
            mStr.reset();
            mStr.appendFormat("%s[H=%.2f]", _name.c_str(), _airDistanceToGoal);
            gui::DrawableString::draw(mStr.getString(), r, gui::Font::ID::SystemNormal, colorID, align);
        }
        else
            gui::DrawableString::draw(_name, r, gui::Font::ID::SystemNormal, colorID, align);
    }
    
public:
    void draw(const Primitive::Options& options) const
    {
        if (_id == options.startID || _id == options.goalID)
        {
            //Draw start and goal flags
            gui::Point flagLocation(_location.x * options.viewSize.width, _location.y * options.viewSize.height-options.flagSize);
            gui::Size sz{gui::CoordType(options.flagSize), gui::CoordType(options.flagSize)};
            gui::Rect r(flagLocation, sz);
            if (_id == options.startID)
                options.imgStart.draw(r);
            else
                options.imgGoal.draw(r);
        }
        
        //draw town symbol using its color which is based on visited state of the town
        switch (_guiState)
        {
            case Primitive::Status::Unvisited:
                _shape.drawFill(options.normalColor);
                showTownName(options, td::ColorID::SysText);
                break;
            case Primitive::Status::Visited:
                _shape.drawFill(options.visitedColor);
                showTownName(options, td::ColorID::SysText);
                break;
            case Primitive::Status::OnPath:
            {
                _shape.drawFill(options.pathColor);
                showTownName(options, options.txtColor);
            }
                break;
            default:
                assert(false);
        }
    }
    
    void increementNoOfConnectedRoads()
    {
        ++_nRoads;
    }
    
    td::UINT4 getNoOfConnectedRoads() const
    {
        return _nRoads;
    }
    
    
    void setBackTrackTransition(const Transition& backTrackTransition)
    {
        //if (_backTransition.toNodeID == 0)
            _backTransition = backTrackTransition;
    }
    
    const Transition& getBackTrackTransition() const
    {
        return _backTransition;
    }
    
    float getCost() const
    {
        return _pathDistance;
    }
    
    //keeps minimum cost to this town (initial length is zero)
    CostChange setCost(float cost, const Transition& backTrackTransition)
    {
        if (_state == State::Explored)
            return CostChange::UpdateNotPossibleNodeExplored;
        
        if (_state == State::Unseen)
        {
            _state = State::Frontier;
            
            _pathDistance = cost;
            _backTransition = backTrackTransition;
            return CostChange::InitialSet;
        }
        
        if (_pathDistance <= cost)
            return CostChange::UpdateNotPossibleDueToLargerCost;
        
        _pathDistance = cost;
        _backTransition = backTrackTransition;
        
        return CostChange::UpdatedWithBetterCost;
    }
    
    CostChange setFCost(float fCost, float cost, const Transition& backTrackTransition)
    {
        if (_state == State::Explored)
            return CostChange::UpdateNotPossibleNodeExplored;
        
        if (_state == State::Unseen)
        {
            _state = State::Frontier;
            _fCost = fCost;
            _pathDistance = cost;
            _backTransition = backTrackTransition;
            return CostChange::InitialSet;
        }
        
        if (_fCost <= fCost)
            return CostChange::UpdateNotPossibleDueToLargerCost;
        
        _fCost = fCost;
        _pathDistance = cost;
        _backTransition = backTrackTransition;
        
        return CostChange::UpdatedWithBetterCost;
    }
    
    void setHeuristicCost(float cost)
    {
        _airDistanceToGoal = cost;
    }
    
    float getHeuristicCost() const
    {
        return _airDistanceToGoal;
    }
    
    void setFCost(float cost, const Transition& backTrackTransition)
    {
        if (_state == State::Unseen)
        {
            
        }
        if (_fCost > 0)
        {
            if (_fCost > cost)
            {
                _backTransition = backTrackTransition;
                _fCost = cost;
            }
        }
        else
        {
            _backTransition = backTrackTransition;
            _fCost = cost;
        }
    }
    
    float getFCost() const
    {
        return _fCost;
    }
    
    void reset()
    {
        Primitive::reset();
        _backTransition = {0,0};
        _airDistanceToGoal = 0;
        _pathDistance = 0;
        _fCost = 0;
        _state = State::Unseen;
    }
    
    void setState(State state)
    {
        _state = State::Explored;
    }
    
    bool isVisited() const
    {
        return (_state != State::Unseen);
    }
    
    bool isExplored() const
    {
        return (_state == State::Explored);
    }
};
