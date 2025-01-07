//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Rectangle.h"


class RoundedRectangle: public Rectangle
{
    enum class PropID: td::UINT4 { Rr=300};
protected:
    float _radius;
public:
    RoundedRectangle(gui::Shape::Attribs attribs, const gui::Rect& rect, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
    : Rectangle(attribs, rect, fillColor, lineColor, lineWidth, linePattern)
    , _radius(radius)
    {
    }
    
    virtual Type getType() const
    {
        return Type::RoundRect;
    }
    
    void init()
    {
        if (_rect.width() > 0 && _rect.height() > 0)
            _shape.createRoundedRect(_rect, _radius, _lineWidth, _linePattern);
    }
    
    virtual void load(arch::ArchiveIn& ar)
    {
        ar >> _radius;
        Rectangle::load(ar);
    }
    virtual void save(arch::ArchiveOut& ar) const
    {
        ar << _radius;
        Rectangle::save(ar);
    }
    
    void initProperties(gui::Properties* properties) const
    {
        Rectangle::initProperties(properties);
        
        {
            td::Variant val(_radius);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Rr, gui::tr("RoundRadius"), val);
        }
    }
    
    virtual void getValues(gui::PropertyValues& propValues) const
    {
        td::Variant r(_radius);
        propValues.setValueByKey((td::UINT4)PropID::Rr, r);
        Rectangle::getValues(propValues);
    }
    
    virtual void setValues(gui::PropertyValues& propValues)
    {
        float oldRadius = _radius;
        td::Variant r = propValues.getValueByKey((td::UINT4)PropID::Rr);
        r.getValue(_radius);
        gui::Rect rOld(_rect);
        Rectangle::setValues(propValues);
        if ( (rOld != _rect) || (oldRadius != _radius))
            _shape.updateRoundRectNodes(_rect, _radius);
    }
    
    virtual void translate(const gui::Point& delta)
    {
        _rect.translate(delta);
        _shape.translateRoundRectNodes(delta);
        //or
        //_shape.updateRoundRectNodes(_rect, _radius);
    }

};
