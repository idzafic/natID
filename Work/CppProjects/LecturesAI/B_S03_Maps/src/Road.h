//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include "Town.h"

class Road : public Primitive
{
    Edge _edge;
    float _length = 0;
public:
    Road() = default;

    Road(std::vector<Town>& towns, const td::String& name, const gui::Size& sz,
         GraphType fromNode, GraphType toNode, float cost)
        : Primitive(name)
        , _edge(fromNode, toNode)
    {
        const auto& fromPoint = towns[fromNode - 1].getLocation();
        const auto& toPoint   = towns[toNode - 1].getLocation();
        gui::Point pt1(fromPoint.x * sz.width, fromPoint.y *  sz.height);
        gui::Point pt2(toPoint.x * sz.width, toPoint.y *  sz.height);
        gui::Point points[] = { pt1, pt2 };
        _length = cost;

        _shape.createLines(points, 2);
    }

    // Move constructor
    Road(Road&& r) noexcept
    : Primitive(std::move(r))
    , _edge(r._edge)
    , _length(r._length)
    {
    }

    // Move assignment
    Road& operator=(Road&& r) noexcept
    {
        if (this != &r)
        {
            Primitive::operator=(std::move(r));
            _edge = r._edge;
            _length = r._length;
        }
        return *this;
    }

    // Delete copy
    Road(const Road&) = delete;
    Road& operator=(const Road&) = delete;

    void draw(const std::vector<Town>& towns, Primitive::Options& options) const
    {
        auto status =  _guiState; // getStatus(towns);
        switch (status)
        {
            case Primitive::Status::Unvisited:
                _shape.drawWire(options.normalColor, cLineWidthUnvisited);
                break;
            case Primitive::Status::Visited:
                _shape.drawWire(options.visitedColor, cLineWidthVisited);
                break;
            case Primitive::Status::OnPath:
                _shape.drawWire(options.pathColor, cLineWidthOnPath);
                break;
            default:
                assert(false);
        }
        
        if ((options.showRoadNames && !_name.isEmpty()) || options.showRoadLengths)
        {
            options.mStr.reset();
            
            if ((options.showRoadNames && !_name.isEmpty()))
                options.mStr.appendFormat("%s", _name.c_str());
            if (options.showRoadLengths)
                options.mStr.appendFormat("[%.1f]", getLength());
            
            const auto& fromPoint = towns[_edge.fromNodeID - 1].getLocation();
            const auto& toPoint   = towns[_edge.toNodeID - 1].getLocation();

            gui::Point pt1(fromPoint.x * options.viewSize.width, fromPoint.y * options.viewSize.height);
            gui::Point pt2(toPoint.x * options.viewSize.width, toPoint.y * options.viewSize.height);
            gui::Point roadCenter ((pt1.x + pt2.x)/2, (pt1.y + pt2.y)/2);
            gui::DrawableString::draw(options.mStr.getString(), roadCenter, gui::Font::ID::SystemNormal, td::ColorID::SysText);
        }
    }

    void updatePosition(std::vector<Town>& towns, const gui::Size& sz)
    {
        const auto& fromPoint = towns[_edge.fromNodeID - 1].getLocation();
        const auto& toPoint   = towns[_edge.toNodeID - 1].getLocation();

        gui::Point pt1(fromPoint.x * sz.width, fromPoint.y * sz.height);
        gui::Point pt2(toPoint.x * sz.width, toPoint.y * sz.height);
        gui::Point points[] = { pt1, pt2 };

        _shape.updateNodes(points, 2);
    }
    
    const Edge& getEdge() const
    {
        return _edge;
    }
    
    GraphType getOtherSide(GraphType oneSide) const
    {
        if (_edge.fromNodeID != oneSide)
            return _edge.fromNodeID;
        return _edge.toNodeID;
    }
    
    float getLength() const
    {
        return _length;
    }
    
    void setLength(float length)
    {
        _length = length;
    }
};


