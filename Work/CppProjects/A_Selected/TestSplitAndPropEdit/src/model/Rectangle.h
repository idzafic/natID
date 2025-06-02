//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Shape2D.h"

class Rectangle: public Shape2D
{
    enum class PropID: td::UINT4 { Xr=200, Yr, Width, Height};
protected:
    gui::Rect _rect;
public:
    Rectangle(gui::Shape::Attribs attribs, const gui::Rect& rect, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
    : Shape2D(attribs, fillColor, lineColor, lineWidth, linePattern)
    , _rect(rect)
    {
    }

    Type getType() const override
    {
        return Type::Rect;
    }
    
    void init() override
    {
        if (_rect.width() > 0 && _rect.height() > 0)
            _shape.createRect(_rect, _lineWidth, _linePattern);
    }
    
    void getBoundingRect(gui::Rect& boundRect) override
    {
        boundRect = _rect;
    }
    
    bool containsPoint(const gui::Point& pt) const override
    {
        return _rect.contains(pt);
    }
    
    void load(arch::ArchiveIn& ar) override
    {
        Shape2D::load(ar);
        ar >> _rect;
    }
    
    void save(arch::ArchiveOut& ar) const override
    {
        Shape2D::save(ar);
        ar << _rect;
    }
    
    void initProperties(gui::Properties* properties) const
    {
        Shape2D::initProperties(properties);

        //group
        {
            auto& prop = properties->push_back();
            prop.setGroup(gui::tr("SizeAndPosition"));
        }
        
        
        td::Variant val(_rect.top);
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Xr, gui::tr("Left"), val);
        }
        
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Yr, gui::tr("Top"), val);
        }
        
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Width, gui::tr("With"), val);
        }
        
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Height, gui::tr("Height"), val);
        }
    }
    
    void getValues(gui::PropertyValues& propValues) const override
    {
        td::Variant x(_rect.left);
        propValues.setValueByKey((td::UINT4)PropID::Xr, x);
        
        td::Variant y(_rect.top);
        propValues.setValueByKey((td::UINT4)PropID::Yr, y);
        
        td::Variant w(_rect.width());
        propValues.setValueByKey((td::UINT4)PropID::Width, w);

        td::Variant h(_rect.height());
        propValues.setValueByKey((td::UINT4)PropID::Height, h);
        
        Shape2D::getValues(propValues);
    }
    
    void setValues(gui::PropertyValues& propValues) override
    {
        gui::Rect rBefore(_rect);
        if (isWithLine())
            rBefore.inflate(_lineWidth+IShape2D::refreshOffset);
        
        propValues.setRectBeforeUpdate(rBefore);
        
        td::Variant x = propValues.getValueByKey((td::UINT4)PropID::Xr);
        x.getValue(_rect.left);
        
        td::Variant y = propValues.getValueByKey((td::UINT4)PropID::Yr);
        y.getValue(_rect.top);
        
        td::Variant varW = propValues.getValueByKey((td::UINT4)PropID::Width);
        gui::CoordType w;
        varW.getValue(w);
        _rect.setWidth(w);
        
        td::Variant varH = propValues.getValueByKey((td::UINT4)PropID::Height);
        gui::CoordType h;
        varH.getValue(h);
        _rect.setHeight(h);
        gui::Rect rectAfter(_rect);
        
        Shape2D::setValues(propValues);
        gui::Rect rAfter(_rect);
        
        if (isWithLine())
            rAfter.inflate(_lineWidth+IShape2D::refreshOffset);
        propValues.setRectAfterUpdate(rAfter);
        
        if (getType() == IShape2D::Type::Rect)
        {
            if (rBefore != rAfter)
                _shape.updateRectNodes(_rect);
        }
    }
    
    bool canBeSelected(const gui::Point& pt) const override
    {
        if (_rect.contains(pt))
        {
            if (isWithFill())
                return true;
            auto minDistance = _rect.getMinDistance2(pt);
            if (minDistance <= IShape2D::selectionDisance2)
                return true;
        }
        return false;
    }
    
    void translate(const gui::Point& delta) override
    {
        _rect.translate(delta);
        _shape.translateRectNodes(delta);
        //or
        //_shape.updateRectNodes(_rect);
    }
};
