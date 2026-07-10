// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Shape.h
 * @brief Provides the Shape class for creating and drawing 2D geometric shapes.
 */
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

/// @brief Represents a 2D geometric shape that can be created, updated, and drawn.
class NATGUI_API Shape
{
public:
    /// @brief Enumerates all supported shape types.
    enum  class Type : unsigned char {Line = 0, Lines, Polyline, Polygon, FillPolygon, Rect, FillRect, RoundedRect, FillRoundedRect, Circle, FillCircle, Circles, FillCircles, Pie, FillPie, Arc, FillArc, Oval, FillOval, Bezier, TextAtPoint, TextInRect, Image, Symbol, NA};
    /// @brief Enumerates rendering attribute modes (line only, fill only, or both).
    enum class Attribs : unsigned char {LineOnly=0, FillOnly, LineAndFill};

    /// @brief Helper class for building Bezier curves incrementally.
    class NATGUI_API Bezier
    {
        friend class BezierHelper;

        NatShape* _path = nullptr; ///< Internal native path handle.
        float _lineWidth = 1; ///< Width of the Bezier curve line.
        td::LinePattern _linePattern = td::LinePattern::Solid; ///< Line pattern used for drawing.
        td::BYTE _open = 1; ///< Indicates whether the path is still open.
        Bezier() = delete;
    public:

        /// @brief Constructs a Bezier builder with a given path, line width, and pattern.
        /// @param path Native shape path handle.
        /// @param lw Line width.
        /// @param lp Line pattern.
        Bezier(NatShape* path, float lw, td::LinePattern lp)
        : _path(path)
        , _lineWidth(lw)
        , _linePattern(lp)
        {}

        /// @brief Copy constructor.
        /// @param b Source Bezier to copy from.
        Bezier(const Bezier& b)
        : _path(b._path)
        {}

        /// @brief Moves the current drawing position to a point without drawing.
        /// @param p Target point.
        /// @return Reference to this Bezier for chaining.
        Bezier& moveTo(const Point& p);

        /// @brief Draws a straight line to the given point.
        /// @param p Target point.
        /// @param closePath Whether to close the path after this segment.
        /// @return Reference to this Bezier for chaining.
        Bezier& lineTo(const Point& p, bool closePath = false);

        /// @brief Draws a quadratic Bezier curve to the given point.
        /// @param p Target point.
        /// @param ctrlPoint Control point for the curve.
        /// @param closePath Whether to close the path after this segment.
        /// @return Reference to this Bezier for chaining.
        Bezier& quadraticTo(const Point& p, const Point& ctrlPoint, bool closePath = false);

        /// @brief Draws a cubic Bezier curve to the given point.
        /// @param p Target point.
        /// @param ctrlPoint1 First control point.
        /// @param ctrlPoint2 Second control point.
        /// @param closePath Whether to close the path after this segment.
        /// @return Reference to this Bezier for chaining.
        Bezier& cubicTo(const Point& p, const Point& ctrlPoint1, const Point& ctrlPoint2, bool closePath=false);

        /// @brief Finalizes and closes the Bezier path.
        void end();
    };

protected:
    NatShape* _path; ///< Internal native shape path.
    Shape(const Shape& shape) = delete;
    Shape& operator=(const Shape&) = delete;
public:
    /// @brief Default constructor. Creates an uninitialized shape.
    Shape();
    // Move constructor
    /// @brief Move constructor.
    /// @param shape Source shape to move from.
    Shape(Shape&& shape) noexcept;
    // Move assignment operator
    /// @brief Move assignment operator.
    /// @param ds Source shape to move-assign from.
    /// @return Reference to this shape.
    Shape& operator=(Shape&& ds) noexcept;

    /// @brief Destructor. Releases the internal path resource.
    ~Shape();

    /// @brief Returns whether this shape has been initialized.
    /// @return True if initialized, false otherwise.
    bool isInitialized() const;

    /// @brief Sets the line width for this shape.
    /// @param lineWidth Desired line width.
    void setLineWidth(float lineWidth);

    /// @brief Sets the line pattern for this shape.
    /// @param linePattern Desired line pattern.
    void setLinePattern(td::LinePattern linePattern);

    /// @brief Creates and returns a Bezier path builder associated with this shape.
    /// @param lineWidth Line width for the Bezier.
    /// @param linePattern Line pattern for the Bezier.
    /// @return A Bezier builder object.
    Bezier createBezier(float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates a set of disconnected line segments.
    /// @param points Array of points defining the lines.
    /// @param nPoints Number of points.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createLines(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates a connected polyline through the given points.
    /// @param points Array of points.
    /// @param nPoints Number of points.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createPolyLine(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates a closed polygon from the given points.
    /// @param points Array of points.
    /// @param nPoints Number of points.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createPolygon(const gui::Point* points, size_t nPoints, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates a rectangle shape.
    /// @param rect Bounding rectangle.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createRect(const gui::Rect& rect, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates a rounded rectangle shape.
    /// @param rect Bounding rectangle.
    /// @param radius Corner radius.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createRoundedRect(const gui::Rect& rect, CoordType radius, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates an oval (ellipse) inscribed in the given rectangle.
    /// @param rect Bounding rectangle.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createOval(const gui::Rect& rect, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates an arc shape.
    /// @param circle Circle defining center and radius.
    /// @param fromAngle Start angle in radians.
    /// @param toAngle End angle in radians.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    /// @param clockwise Direction of arc.
    void createArc(const gui::Circle& circle, float fromAngle, float toAngle, td::BYTE lineWidth, td::LinePattern linePattern=td::LinePattern::Solid, bool clockwise = false);

    /// @brief Creates a pie (sector) shape.
    /// @param circle Circle defining center and radius.
    /// @param fromAngle Start angle in radians.
    /// @param toAngle End angle in radians.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    /// @param clockwise Direction of pie sector.
    void createPie(const gui::Circle& circle, float fromAngle, float toAngle, td::BYTE lineWidth, td::LinePattern linePattern=td::LinePattern::Solid, bool clockwise = false);

    /// @brief Creates a circle shape.
    /// @param circle Circle definition.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createCircle(const gui::Circle& circle, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Creates multiple circles at once.
    /// @param circles Array of circle definitions.
    /// @param nCircles Number of circles.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    void createCircles(const gui::Circle* circles, size_t nCircles, float lineWidth=1, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Draws the shape outline using the specified color.
    /// @param lineColor Color for the outline.
    void drawWire(td::ColorID lineColor) const;

    /// @brief Draws the shape outline overriding line width and pattern.
    /// @param lineColor Color for the outline.
    /// @param lineWidth Override line width.
    void drawWire(td::ColorID lineColor, float lineWidth) const; //override line width and pattern

    /// @brief Draws the shape filled with the specified color.
    /// @param fillColor Fill color.
    void drawFill(td::ColorID fillColor) const;

    /// @brief Draws the shape with both fill and outline.
    /// @param fillColor Fill color.
    /// @param lineColor Outline color.
    void drawFillAndWire(td::ColorID fillColor, td::ColorID lineColor) const;

    /// @brief Draws the shape with fill and outline, overriding line width and pattern.
    /// @param fillColor Fill color.
    /// @param lineColor Outline color.
    /// @param lineWidth Override line width.
    void drawFillAndWire(td::ColorID fillColor, td::ColorID lineColor, float lineWidth) const; //override line width and pattern

    /// @brief Draws the shape outline translated to a point and scaled.
    /// @param lineColor Outline color.
    /// @param pt Translation point.
    /// @param scale Scale factor.
    void drawWireAtPointWithScale(td::ColorID lineColor, const Point& pt, double scale) const;

    /// @brief Draws the shape fill translated to a point and scaled.
    /// @param fillColor Fill color.
    /// @param pt Translation point.
    /// @param scale Scale factor.
    void drawFillAtPointWithScale(td::ColorID fillColor, const Point& pt, double scale) const;

    /// @brief Draws the shape with fill and outline translated to a point and scaled.
    /// @param fillColor Fill color.
    /// @param lineColor Outline color.
    /// @param pt Translation point.
    /// @param scale Scale factor.
    void drawFillAndWireAtPointWithScale(td::ColorID fillColor, td::ColorID lineColor, const Point& pt, double scale) const;

    /// @brief Draws a rectangle outline.
    /// @param r Rectangle to draw.
    /// @param lineColorID Outline color.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    static void drawRect(const Rect& r, td::ColorID lineColorID, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Draws a filled rectangle with an outline.
    /// @param r Rectangle to draw.
    /// @param fillColor Fill color.
    /// @param lineColorID Outline color.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    static void drawRect(const Rect& r, td::ColorID fillColor, td::ColorID lineColorID, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid);

    /// @brief Draws a filled rectangle with no outline.
    /// @param r Rectangle to draw.
    /// @param fillColor Fill color.
    static void drawRect(const Rect& r, td::ColorID fillColor);

    /// @brief Draws a filled rectangle with an alpha channel.
    /// @param r Rectangle to draw.
    /// @param alphaChannel Alpha transparency value.
    /// @param fillColor Fill color.
    static void drawRect(const Rect& r, float alphaChannel, td::ColorID fillColor);

    /// @brief Draws a line between two points.
    /// @param p1 Start point.
    /// @param p2 End point.
    /// @param lineColor Line color.
    /// @param lineWidth Line width.
    /// @param linePattern Line pattern.
    /// @param alphaChannel Alpha transparency value.
    static void drawLine(const Point& p1, const Point& p2, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern=td::LinePattern::Solid, float alphaChannel = 1.0f);

    /// @brief Draws a line between two points using line parameters and color mode.
    /// @param p1 Start point.
    /// @param p2 End point.
    /// @param lineParams Line parameters including color, width, and pattern.
    /// @param bIsDarkMode True if dark mode is active.
    inline static void drawLine(const Point& p1, const Point& p2, const td::LineParams& lineParams, bool bIsDarkMode)
    {
        drawLine(p1, p2, lineParams.getColor(bIsDarkMode), lineParams.width, lineParams.pattern);
    }

    /// @brief Draws a selection rectangle with an optional scale.
    /// @param r Rectangle to draw.
    /// @param scale Scale factor.
    static void drawSelectionRect(const Rect& r, double scale = 1);

    /// @brief Updates the points of an existing polyline or polygon shape.
    /// @param points New array of points.
    /// @param nPoints Number of points.
    void updateNodes(const gui::Point* points, size_t nPoints);

    /// @brief Updates the nodes of a rectangle shape.
    /// @param r New rectangle.
    void updateRectNodes(const gui::Rect& r);

    /// @brief Updates the nodes of a rounded rectangle shape.
    /// @param r New rectangle.
    /// @param radius New corner radius.
    void updateRoundRectNodes(const gui::Rect& r, CoordType radius);

    /// @brief Updates the nodes of a circle shape.
    /// @param circle New circle definition.
    void updateCircleNodes(const gui::Circle& circle);

    /// @brief Updates the nodes of an oval shape.
    /// @param r New bounding rectangle.
    void updateOvalNodes(const gui::Rect& r);

    /// @brief Updates the nodes of an arc shape.
    /// @param circle Circle definition.
    /// @param fromAngle New start angle.
    /// @param toAngle New end angle.
    /// @param clockwise Direction of arc.
    void updateArcNodes(const gui::Circle& circle, float fromAngle, float toAngle, bool clockwise = false);

    /// @brief Updates the nodes of a pie shape.
    /// @param circle Circle definition.
    /// @param fromAngle New start angle.
    /// @param toAngle New end angle.
    /// @param clockwise Direction of pie.
    void updatePieNodes(const gui::Circle& circle, float fromAngle, float toAngle, bool clockwise = false);

    /// @brief Translates all polyline/polygon nodes by a delta.
    /// @param delta Translation vector.
    void translateNodes(const gui::Point& delta);

    /// @brief Translates rectangle nodes by a delta.
    /// @param delta Translation vector.
    void translateRectNodes(const gui::Point& delta);

    /// @brief Translates rounded rectangle nodes by a delta.
    /// @param delta Translation vector.
    void translateRoundRectNodes(const gui::Point& delta);

    /// @brief Translates circle nodes by a delta.
    /// @param delta Translation vector.
    void translateCircleNodes(const gui::Point& delta);

    /// @brief Translates oval nodes by a delta.
    /// @param delta Translation vector.
    void translateOvalNodes(const gui::Point& delta);

    /// @brief Translates arc nodes by a delta.
    /// @param delta Translation vector.
    void translateArcNodes(const gui::Point& delta);

    /// @brief Translates pie nodes by a delta.
    /// @param delta Translation vector.
    void translatePieNodes(const gui::Point& delta);

    /// @brief Prints debug information about this shape to the console.
    void showDebugInfo() const;

    /// @brief Releases the internal path resource.
    void release();
};

} //namepace gui
