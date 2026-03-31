//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "IShape2D.h"

class Shape2D: public IShape2D
{
    enum class PropID: td::UINT4 { Attribs=100, FillColor, LineColor, LineWidth, LinePattern};
protected:
    gui::Shape _shape;
    float _lineWidth;
    gui::Shape::Attribs _attribs;
    td::ColorID _fillColor;
    td::ColorID _lineColor;
    td::LinePattern _linePattern;
    
protected:
    bool isWithFill() const
    {
        if (_attribs == gui::Shape::Attribs::LineOnly)
            return false;
        return true;
    }
    
    bool isWithLine() const
    {
        if (_attribs == gui::Shape::Attribs::FillOnly)
            return false;
        return true;
    }
    
public:
    Shape2D(gui::Shape::Attribs attribs, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
    : _lineWidth(lineWidth)
    , _attribs(attribs)
    , _fillColor(fillColor)
    , _lineColor(lineColor)
    , _linePattern(linePattern)
    {
    }
    
    virtual ~Shape2D()
    {
        
    }
    
    void draw() const override
    {
        if (_attribs == gui::Shape::Attribs::LineAndFill)
            _shape.drawFillAndWire(_fillColor, _lineColor);
        else if (_attribs == gui::Shape::Attribs::LineOnly)
            _shape.drawWire(_lineColor);
        else
            _shape.drawFill(_fillColor);
    }
    
    void load(arch::ArchiveIn& ar) override
    {
//        td::BYTE attr = 0 ; //(td::BYTE) _attribs;
//        td::BYTE lnPattern = 0; //(td::BYTE) _linePattern;
        arch::EnumLoader<gui::Shape::Attribs> attr(_attribs, gui::Shape::Attribs::LineAndFill, gui::Shape::Attribs::LineAndFill);
        arch::EnumLoader<td::LinePattern> lnPattern(_linePattern, td::LinePattern::NA, td::LinePattern::Solid);
        ar >> _lineWidth >> attr >> _fillColor >> _lineColor >> lnPattern;
//        _attribs = (gui::Shape::Attribs) attr;
//        _linePattern = (td::LinePattern) lnPattern;
    }
    
    void save(arch::ArchiveOut& ar) const override
    {
//        td::BYTE attr = (td::BYTE) _attribs;
//        td::BYTE lnPattern = (td::BYTE) _linePattern;
        arch::EnumSaver<gui::Shape::Attribs> attr(_attribs);
        arch::EnumSaver<td::LinePattern> lnPattern(_linePattern);
        ar << _lineWidth << attr << _fillColor << _lineColor << lnPattern;
    }
    
    void release() override
    {
        delete this;
    }
    
    void initProperties(gui::Properties* properties) const
    {
        //if (createGroup)
        {
            auto& prop = properties->push_back();
            prop.setGroup(gui::tr("VisualData"));
        }
        
        {
            td::String lbl(gui::tr("Attribs"));
            td::Variant var(td::int4);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::Attribs, lbl, var);
            prop.setPresentation(gui::Property::Presentation::Combo);
            td::String* pStrItems = IShape2D::getAttribsDesc();
            prop.setStringList(pStrItems, 3);
        }
        
        td::Variant valColor(td::colorID);
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::FillColor, gui::tr("FillColor"), valColor);
        }
        
        {
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::LineColor, gui::tr("LineColor"), valColor);
        }
        
        {
            td::Variant val(td::real4);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::LineWidth, gui::tr("LineWidth"), val);
        }
        
        {
            td::Variant val(td::linePattern);
            auto& prop = properties->push_back();
            prop.set((td::UINT4) PropID::LinePattern, gui::tr("LinePattern"), val);
        }
    }
    
    void getValues(gui::PropertyValues& propValues) const override
    {
        td::INT4 iAttr= (td::INT4) _attribs;
        td::Variant attribs(iAttr);
        propValues.setValueByKey((td::UINT4)PropID::Attribs, attribs);
        
        td::Variant fillColor(_fillColor);
        propValues.setValueByKey((td::UINT4)PropID::FillColor, fillColor);
        
        td::Variant lineColor(_lineColor);
        propValues.setValueByKey((td::UINT4)PropID::LineColor, lineColor);
        
        td::Variant lineWidth(_lineWidth);
        propValues.setValueByKey((td::UINT4)PropID::LineWidth, lineWidth);
        
        td::Variant linePattern(_linePattern);
        propValues.setValueByKey((td::UINT4)PropID::LinePattern, linePattern);
        
    }
    
    void setValues(gui::PropertyValues& propValues) override
    {
        propValues.setUpdateCanvas(true);
        td::Variant attribs = propValues.getValueByKey((td::UINT4)PropID::Attribs);
        td::INT4 iAttr=0;
        attribs.getValue(iAttr);
        _attribs = (gui::Shape::Attribs) iAttr;
        
        td::Variant fillColor = propValues.getValueByKey((td::UINT4)PropID::FillColor);
        fillColor.getValue(_fillColor);
        
        td::Variant lineColor = propValues.getValueByKey((td::UINT4)PropID::LineColor);
        lineColor.getValue(_lineColor);
        
        float lineWidth0 = _lineWidth;
        td::Variant lineWidth = propValues.getValueByKey((td::UINT4)PropID::LineWidth);
        lineWidth.getValue(_lineWidth);
        if (lineWidth0 != _lineWidth)
            _shape.setLineWidth(_lineWidth);
        
        td::LinePattern linePattern0 = _linePattern;
        td::Variant linePattern = propValues.getValueByKey((td::UINT4)PropID::LinePattern);
        linePattern.getValue(_linePattern);
        
        if (linePattern0 != _linePattern)
            _shape.setLinePattern(_linePattern);
    }
    
    void setValue(td::UINT4 key, gui::PropertyValues& propValues) override
    {
        
    }
};
