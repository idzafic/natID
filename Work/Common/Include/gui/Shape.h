// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <td/LineParams.h>

namespace gui
{

class NatShape;
class BezierHelper;

class NATGUI_API Shape
{
public:
    enum  class Type : unsigned char {Line = 0, Lines, Polyline, Polygon, FillPolygon, Rect, FillRect, RoundedRect, FillRoundedRect, Circle, FillCircle, Circles, FillCircles, Pie, FillPie, Arc, FillArc, Oval, FillOval, Bezier, TextAtPoint, TextInRect, Image, Symbol, NA};
    enum class Attribs : unsigned char {LineOnly=0, FillOnly, LineAndFill};
    class NATGUI_API Bezier
    {
        friend class BezierHelper;
        
        NatShape* _path = nullptr;
        float _lineWidth = 1;
        td::LinePattern _linePattern = td::LinePattern::Solid;
        Bezier() = delete;
    public:
        
        Bezier(NatShape* path, float lw, td::LinePattern lp)
        : _path(path)
        , _lineWidth(lw)
        , _linePattern(lp)
        {}
        
        Bezier(const Bezier& b)
        : _path(b._path)
        {}
        
        Bezier& moveTo(const Point& p);
        Bezier& lineTo(const Point& p, bool closePath = false);
        Bezier& quadraticTo(const Point& p, const Point& ctrlPoint, bool closePath = false);
        Bezier& cubicTo(const Point& p, const Point& ctrlPoint1, const Point& ctrlPoint2, bool closePath=false);
    };
    
protected:
    NatShape* _path;
    Shape(const Shape& shape) = delete;
    Shape& operator=(const Shape&) = delete;
public:
    Shape();
    // Move constructor
    Shape(Shape&& shape) noexcept;
    // Move assignment operator
    Shape& operator=(Shape&& ds) noexcept;
    
    ~Shape();
    bool isInitialized() const;
    void setLineWidth(float lineWidth);
    void setLinePattern(td::LinePattern linePattern);
    
    Bezier createBezier(float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    
    void createLines(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    
    void createPolyLine(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    void createPolygon(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    
    void createRect(const gui::Rect& rect, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    void createRoundedRect(const gui::Rect& rect, CoordType radius, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    void createOval(const gui::Rect& rect, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    void createArc(const gui::Circle& circle, float fromAngle, float toAngle, td::BYTE lineWidth, td::LinePattern linePattern=td::LinePattern::Solid, bool clockwise = false);
    void createPie(const gui::Circle& circle, float fromAngle, float toAngle, td::BYTE lineWidth, td::LinePattern linePattern=td::LinePattern::Solid, bool clockwise = false);
    
    void createCircle(const gui::Circle& circle, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    void createCircles(const gui::Circle* circles, size_t nCircles, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);
    
    void drawWire(td::ColorID lineColor) const;
    void drawWire(td::ColorID lineColor, float lineWidth) const; //override line width and pattern
    void drawFill(td::ColorID fillColor) const;
    void drawFillAndWire(td::ColorID fillColor, td::ColorID lineColor) const;
    void drawFillAndWire(td::ColorID fillColor, td::ColorID lineColor, float lineWidth) const; //override line width and pattern
    void drawWireAtPointWithScale(td::ColorID lineColor, const Point& pt, double scale) const;
    void drawFillAtPointWithScale(td::ColorID fillColor, const Point& pt, double scale) const;
    void drawFillAndWireAtPointWithScale(td::ColorID fillColor, td::ColorID lineColor, const Point& pt, double scale) const;
    
    static void drawRect(const Rect& r, td::ColorID lineColorID, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid);
    static void drawRect(const Rect& r, td::ColorID fillColor, td::ColorID lineColorID, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid);
    static void drawRect(const Rect& r, td::ColorID fillColor);
    
    static void drawRect(const Rect& r, float alphaChannel, td::ColorID fillColor);
    
    static void drawLine(const Point& p1, const Point& p2, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid);
    
    inline static void drawLine(const Point& p1, const Point& p2, const td::LineParams& lineParams, bool bIsDarkMode)
    {
        drawLine(p1, p2, lineParams.getColor(bIsDarkMode), lineParams.width, lineParams.pattern);
    }
    
    static void drawSelectionRect(const Rect& r, double scale = 1);
    
    void updateNodes(const gui::Point* points, size_t nPoints);
    void updateRectNodes(const gui::Rect& r);    
    void updateRoundRectNodes(const gui::Rect& r, CoordType radius);
    void updateCircleNodes(const gui::Circle& circle);
    void updateOvalNodes(const gui::Rect& r);
    void updateArcNodes(const gui::Circle& circle, float fromAngle, float toAngle);
    void updatePieNodes(const gui::Circle& circle, float fromAngle, float toAngle);
    
    void translateNodes(const gui::Point& delta);
    void translateRectNodes(const gui::Point& delta);
    void translateRoundRectNodes(const gui::Point& delta);
    void translateCircleNodes(const gui::Point& delta);
    void translateOvalNodes(const gui::Point& delta);
    void translateArcNodes(const gui::Point& delta);
    void translatePieNodes(const gui::Point& delta);
    
    void showDebugInfo() const;
    void release();
};

} //namepace gui

