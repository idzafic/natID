//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Shape2D.h"
#include <td/Circle.h>

class Circle: public Shape2D
{
    td::Circle<gui::CoordType> _circle;
    enum class PropID: td::UINT4 { Xc=200, Yc, Rc};
public:
    Circle(gui::Shape::Attribs attribs, const gui::Point& center, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
    : Shape2D(attribs, fillColor, lineColor, lineWidth, linePattern)
    , _circle(center, radius)
    {
    }
    
    virtual Type getType() const
    {
        return Type::Circle;
    }
    
    void init()
    {
        if (_circle.r > 0)
            _shape.createCircle(_circle, _lineWidth, _linePattern);
    }
    
    virtual void getBoundingRect(gui::Rect& boundRect)
    {
        _circle.getBoundingRect(boundRect);
    }
    
    virtual bool containsPoint(const gui::Point& pt) const
    {
        return false;
    }
    
    virtual void load(arch::ArchiveIn& ar)
    {
        ar >> _circle;
        Shape2D::load(ar);
    }
    virtual void save(arch::ArchiveOut& ar) const
    {
        ar << _circle;
        Shape2D::save(ar);
    }
    
    void initProperties(gui::Properties* properties) const
    {
        Shape2D::initProperties(properties);
        
        {
            auto& prop = properties->push_back();
            prop.setGroup(gui::tr("PosAndRadius"));
        }
        
        {
            td::Variant val(_circle.center.x);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Xc, "x:", val);
        }
        
        {
            td::Variant val(_circle.center.y);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Yc, "y:", val);
        }
        
        {
            td::Variant val(_circle.r);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Rc, "r:", val);
        }
    }
    
    virtual void getValues(gui::PropertyValues& propValues) const
    {
        td::Variant x(_circle.center.x);
        propValues.setValueByKey((td::UINT4)PropID::Xc, x);
        
        td::Variant y(_circle.center.y);
        propValues.setValueByKey((td::UINT4)PropID::Yc, y);
        
        td::Variant r(_circle.r);
        propValues.setValueByKey((td::UINT4)PropID::Rc, r);
        
        Shape2D::getValues(propValues);
    }
    
    virtual void setValues(gui::PropertyValues& propValues)
    {
        gui::Rect rBefore;
        getBoundingRect(rBefore);
        if (isWithLine())
            rBefore.inflate(_lineWidth+IShape2D::refreshOffset);
        propValues.setRectBeforeUpdate(rBefore);
        
        td::Variant x = propValues.getValueByKey((td::UINT4)PropID::Xc);
        x.getValue(_circle.center.x);
        
        td::Variant y = propValues.getValueByKey((td::UINT4)PropID::Yc);
        y.getValue(_circle.center.y);
        
        td::Variant r = propValues.getValueByKey((td::UINT4)PropID::Rc);
        r.getValue(_circle.r);
        
        Shape2D::setValues(propValues);
        
        gui::Rect rAfter;
        getBoundingRect(rAfter);
        if (isWithLine())
            rAfter.inflate(_lineWidth+IShape2D::refreshOffset);
        propValues.setRectAfterUpdate(rAfter);
        
        if (rBefore != rAfter)
            _shape.updateCircleNodes(_circle);
    }
    
    virtual bool canBeSelected(const gui::Point& pt) const
    {
        if (_circle.contains(pt))
        {
            if (isWithFill())
                return true;
            auto minDistance = _circle.getMinDistance2(pt);
            if (minDistance <= IShape2D::selectionDisance2)
                return true;
        }
        return false;
    }
    
    virtual void translate(const gui::Point& delta)
    {
        _circle.translate(delta);
        _shape.translateCircleNodes(delta);
        //or
        //_shape.updateCircleNodes(_circle);
    }
};
