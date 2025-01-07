//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Shape.h>
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>
#include <gui/Properties.h>
#include <gui/IProperty.h>

//Interface to 2d shapes (Rect, Circle,...)
class IShape2D : public gui::IProperty
{
private:
    static gui::Properties _circProperties;
    static gui::Properties _rectPropertes;
    static gui::Properties _roundRectPropertes;
    
public:
    enum class Type:unsigned char {Rect=0, RoundRect, Circle};
    enum class Tool {Selector=0, AddRect, AddRoundRect, AddCircle};
    
    //IShape2D interface
    virtual void draw() const = 0;
    virtual void getBoundingRect(gui::Rect& boundRect) = 0;
    virtual bool containsPoint(const gui::Point& pt) const = 0;
    virtual void load(arch::ArchiveIn& ar) = 0;
    virtual void save(arch::ArchiveOut& ar) const = 0;
    virtual Type getType() const = 0;
    virtual bool canBeSelected(const gui::Point& pt) const = 0;
    virtual void init() = 0;
    virtual void translate(const gui::Point& delta) = 0;
    virtual void release() = 0;
    
    //ShapeFactory
    static IShape2D* createRect(gui::Shape::Attribs attribs, const gui::Rect& rect, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern);
    
    static IShape2D* createRoundedRect(gui::Shape::Attribs attribs, const gui::Rect& r, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern);
    
    static IShape2D* createCircle(gui::Shape::Attribs attribs, const gui::Point& center, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern);
    
    //Factory tool
    static IShape2D::Tool currentTool;
    
    //some limits
    static float maxLineWidth;
    static gui::CoordType selectionDisance2;
    static gui::CoordType refreshOffset;
    
    //PropertyManager
    static void createProperties(IShape2D::Type shapeType, gui::Properties& properties);
    
    static gui::Properties* getProperties(IShape2D::Type shapeType);
    
    static td::String* getAttribsDesc();
    
    static void initProperties();
};

