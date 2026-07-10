// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file RectNormalized.h
    @brief Defines a normalized (left <= right, top <= bottom) axis-aligned rectangle. */
#pragma once
#include <td/Rect.h>
#include <td/Point.h>
#include <td/Size.h>
#include <td/Circle.h>

namespace td
{

/// @brief Axis-aligned rectangle guaranteed to satisfy left <= right and top <= bottom.
/// @tparam T The numeric type used for all coordinate values.
template <typename T>
class RectNormalized
{
public:
    T left;   ///< Left edge coordinate (smallest x value).
    T top;    ///< Top edge coordinate (smallest y value).
    T right;  ///< Right edge coordinate (largest x value).
    T bottom; ///< Bottom edge coordinate (largest y value).

    /// @brief Returns the width of the rectangle.
    /// @return right - left.
    inline T width() const
    {
        return right - left;
    }

    /// @brief Returns the height of the rectangle.
    /// @return bottom - top.
    inline T height() const
    {
        return bottom - top;
    }

    /// @brief Tests whether the rectangle satisfies the normalisation invariant.
    /// @return true if right >= left and bottom >= top.
    inline bool isNormalized() const
    {
        return ( (right >= left) && (bottom >= top));
    }

    /// @brief Tests whether this rectangle fully contains another rectangle.
    /// @param r The rectangle to test for containment.
    /// @return true if r is entirely within this rectangle.
    inline bool contains(const RectNormalized<T>& r) const
    {
        return (left <= r.left && right >= r.right && top <= r.top && bottom >= r.bottom);
    }

    /// @brief Tests whether this rectangle overlaps with another rectangle.
    /// @param r The rectangle to test for intersection.
    /// @return true if the two rectangles overlap.
    inline bool intersects (const RectNormalized<T>& r) const
    {
        return !(left > r.right || right < r.left || top > r.bottom || bottom < r.top);
    }

    /// @brief Tests whether this rectangle does NOT overlap with another rectangle.
    /// @param r The rectangle to test.
    /// @return true if the two rectangles do not overlap.
    inline bool notIntersects (const RectNormalized<T>& r) const
    {
        return (left > r.right || right < r.left || top > r.bottom || bottom < r.top);
    }

    /// @brief Sets all four edges to the same scalar value.
    /// @param val The value to assign to left, right, top, and bottom.
    inline void operator = (T val)
    {
        left = right = top = bottom = val;
    }

    /// @brief Computes the intersection of this rectangle with another.
    /// @param r    The other rectangle.
    /// @param rOut Output rectangle receiving the intersection region.
    /// @return true if the rectangles intersect; false if they do not (rOut is zeroed).
    inline bool getIntersection(const RectNormalized<T>& r, RectNormalized<T>& rOut) const
    {
        if (intersects(r))
        {
            rOut.left = math::Max(left, r.left);
            rOut.top = math::Max(top, r.top);
            rOut.right = math::Min(right, r.right);
            rOut.bottom = math::Min(bottom, r.bottom);
            rOut.normalize();
            return true;
        }
        rOut.left = 0;
        rOut.right = 0;
        rOut.top = 0;
        rOut.bottom = 0;
        return false;
    }

    /// @brief Expands this rectangle to be the union of itself and another rectangle.
    /// @param r The rectangle to union with.
    inline void unija(const RectNormalized<T>& r)
    {
        if (r.left < left)
            left = r.left;
        if (r.top < top)
            top = r.top;
        if (r.right > right)
            right = r.right;
        if (r.bottom > bottom)
            bottom = r.bottom;
    }

    /// @brief Uniformly inflates (expands) the rectangle by a scalar value.
    /// @param val Amount to subtract from left and top and add to right and bottom.
    inline void inflate(T val)
    {
        left -= val;
        top -= val;
        bottom += val;
        right += val;
    }

    /// @brief Inflates the rectangle by separate horizontal and vertical amounts.
    /// @param valX Amount to expand horizontally (subtracted from left, added to right).
    /// @param valY Amount to expand vertically (subtracted from top, added to bottom).
    inline void inflate(T valX, T valY)
    {
        left -= valX;
        top -= valY;
        right += valX;
        bottom += valY;
    }

    /// @brief Inflates the rectangle while clamping left and top to minimum values.
    /// @param val    Uniform inflation amount.
    /// @param minLeft Minimum allowed value for left after inflation.
    /// @param minTop  Minimum allowed value for top after inflation.
    inline void inflate(T val, T minLeft, T minTop)
    {
        inflate(val);
        if (left < minLeft)
            left = minLeft;
        if (top < minTop)
            top = minTop;
    }

    /// @brief Translates the rectangle by a displacement vector.
    /// @param dx Horizontal displacement.
    /// @param dy Vertical displacement.
    void translate(T dx, T dy)
    {
        left += dx;
        right += dx;
        top += dy;
        bottom += dy;
    }

    /// @brief Scales the rectangle edges by separate x and y factors from the origin.
    /// @param sx Horizontal scale factor.
    /// @param sy Vertical scale factor.
    void scale(T sx, T sy)
    {
        left *= sx;
        right *= sx;
        top *= sy;
        bottom *= sy;
    }

    /// @brief Uniformly scales the rectangle edges from the origin.
    /// @param s Uniform scale factor.
    void scale(T s)
    {
        left *= s;
        right *= s;
        top *= s;
        bottom *= s;
    }

    /// @brief Expands the rectangle to encompass a given point.
    /// @param p The point to include in the rectangle.
    inline void encompass(const Point<T>& p) //encompass -> unija
    {
        if (p.x < left)
        {
            left = p.x;
        }
        else if (p.x > right)
        {
            right = p.x;
        }

        if (p.y > bottom)
        {
            bottom = p.y;
        }
        else if (p.y < top)
        {
            top = p.y;
        }
    }

    /// @brief Returns the area of the rectangle.
    /// @return Product of width and height cast to td::Coord.
    inline td::Coord space() const
    {
        return (td::Coord) (width() * height());
    }

    /// @brief Writes the centre point into an output parameter.
    /// @param center Output parameter that receives the centre coordinates.
    inline void center(Point<T>& center) const
    {
        center.x = (left + right)/2;
        center.y = (top + bottom) /2;
    }

    /// @brief Returns the length of the shorter side of the rectangle.
    /// @return The minimum of width() and height().
    inline T getLengthOfSmallerSide() const
    {
        auto w = width();
        auto h = height();
        if (w < h)
            return w;
        return h;
    }

    /// @brief Returns the centre point of the rectangle.
    /// @return A Point at the geometric centre.
    inline Point<T> center() const
    {
        Point<T> center((left + right)/2, (top + bottom) /2);
        return center;
    }

    /// @brief Default constructor; leaves all edges uninitialised.
    RectNormalized()
        //: left(0)
        //, right(0)
        //, top(0)
        //, bottom(0)
    {}

    /// @brief Constructs a normalised rectangle from four edge coordinates.
    /// @param l Left edge value.
    /// @param t Top edge value.
    /// @param r Right edge value.
    /// @param b Bottom edge value.
    RectNormalized(T l, T t, T r, T b)
        : left(l)
        , top(t)
        , right(r)
        , bottom(b)
    {
        normalize();
    }

    /// @brief Constructs a rectangle from four edge coordinates with optional normalisation.
    /// @param makeNormalized If true, swaps edges to ensure left<=right and top<=bottom.
    /// @param l Left edge value.
    /// @param t Top edge value.
    /// @param r Right edge value.
    /// @param b Bottom edge value.
    RectNormalized(bool makeNormalized, T l, T t, T r, T b)
        : left(l)
        , top(t)
        , right(r)
        , bottom(b)
    {
        if (makeNormalized)
            normalize();
    }

    /// @brief Constructs a square rectangle from an origin and a single side length.
    /// @param l              Left edge value.
    /// @param t              Top edge value.
    /// @param widthAndHeight Side length applied to both width and height.
    RectNormalized(T l, T t, T widthAndHeight)
        : left(l)
        , top(t)
        , right(l + widthAndHeight)
        , bottom(t + widthAndHeight)
    {
    }

    /// @brief Constructs a rectangle from an origin point and a size.
    /// @param origin Top-left corner.
    /// @param size   Width and height of the rectangle.
    RectNormalized(const td::Point<T>& origin, const td::Size<T>& size)
        : left(origin.x)
        , top(origin.y)
        , right(origin.x + size.width)
        , bottom(origin.y + size.height)
    {
    }

    /// @brief Constructs the bounding rectangle of a circle.
    /// @param c The circle whose bounding box is computed.
    RectNormalized(const td::Circle<T>& c)
    {
        left = c.center.x - c.r;
        top = c.center.y - c.r;
        right = c.center.x + c.r;
        bottom = c.center.y + c.r;
    }

    /// @brief Tests whether a point is inside (or on the boundary of) the rectangle.
    /// @param p The point to test.
    /// @return true if the point lies within the rectangle boundaries.
    inline bool contains(const Point<T>& p) const
    {
        return (p.x >= left && p.x <= right && p.y >= top && p.y <= bottom);
    }

    /// @brief Repositions the rectangle by moving its top-left corner, preserving size.
    /// @param x New left edge coordinate.
    /// @param y New top edge coordinate.
    inline void setOrigin(T x, T y)
    {
        auto width = right - left;
        auto height = bottom - top;
        left = x;
        top = y;
        right = left + width;
        bottom = top + height;
    }

    /// @brief Repositions the rectangle to a new origin point, preserving size.
    /// @param origin New top-left corner.
    inline void setOrigin(const td::Point<T>& origin)
    {
        auto width = right - left;
        auto height = bottom - top;
        left = origin.x;
        top = origin.y;
        right = left + width;
        bottom = top + height;
    }

    /// @brief Sets the rectangle from an origin point and a size point.
    /// @param origin Top-left corner.
    /// @param size   Point whose x and y represent width and height respectively.
    inline void setOriginAndSize(const td::Point<T>& origin, const td::Point<T>& size)
    {
        left = origin.x;
        top = origin.y;
        right = left + size.width;
        bottom = top + size.height;
    }


    /// @brief Translates the rectangle by subtracting a point delta.
    /// @param delta The displacement to subtract.
    void operator -= (const td::Point<T>& delta)
    {
        left -= delta.x;
        right -= delta.x;
        top -= delta.y;
        bottom -= delta.y;
    }

    /// @brief Translates the rectangle by adding a point delta.
    /// @param delta The displacement to add.
    void operator += (const td::Point<T>& delta)
    {
        left += delta.x;
        right += delta.x;
        top += delta.y;
        bottom += delta.y;
    }

    /// @brief Assignment operator from another RectNormalized.
    /// @param r The source rectangle.
    /// @return Reference to this rectangle.
    inline const RectNormalized<T>& operator = (const RectNormalized<T>& r)
    {
        left = r.left;
        right = r.right;
        top = r.top;
        bottom = r.bottom;
        return *this;
    }

    /// @brief Assigns a degenerate (zero-area) rectangle at a point location.
    /// @param pt The point; sets all four edges to the point coordinates.
    /// @return Reference to this rectangle.
    inline const RectNormalized<T>& operator = (const Point<T>& pt)
    {
        left = pt.x;
        right = pt.x;
        top = pt.y;
        bottom = pt.y;
        return *this;
    }

    /// @brief Equality comparison with another RectNormalized.
    /// @param r The rectangle to compare against.
    /// @return true if all four edges are equal.
    inline bool operator == (const RectNormalized<T>& r) const
    {
        return ((left == r.left) && (right == r.right) && (top == r.top) && (bottom == r.bottom));
    }

    /// @brief Inequality comparison with another RectNormalized.
    /// @param r The rectangle to compare against.
    /// @return true if any edge differs.
    inline bool operator != (const RectNormalized<T>& r) const
    {
        return ((left != r.left) || (right != r.right) || (top != r.top) || (bottom != r.bottom));
    }

    /// @brief Translates the rectangle by a point delta.
    /// @param delta The displacement vector.
    void translate(const Point<T>& delta)
    {
        left += delta.x;
        top += delta.y;
        right += delta.x;
        bottom += delta.y;
    }

    /// @brief Returns the top-left and bottom-right corners as separate points.
    /// @param tl Output: top-left corner.
    /// @param br Output: bottom-right corner.
    void getDiagonal1(Point<T>& tl, Point<T>& br) const
    {
        tl.x = left;
        tl.y = top;
        br.x = right;
        br.y = bottom;
    }

    /// @brief Returns the top-right and bottom-left corners as separate points.
    /// @param tr Output: top-right corner.
    /// @param bl Output: bottom-left corner.
    void getDiagonal2(Point<T>& tr, Point<T>& bl) const
    {
        tr.x = right;
        tr.y = top;
        bl.x = left;
        bl.y = bottom;
    }

    /// @brief Sets all four edges to zero.
    void toZero()
    {
        left = right = top = bottom = 0;
    }

    /// @brief Tests whether all four edges are zero.
    /// @return true if left, right, top, and bottom are all zero.
    bool isZero() const
    {
        return ((left == 0) && (right == 0) && (top == 0) && (bottom == 0));
    }

    /// @brief Expands this rectangle to the union of itself and another rectangle.
    /// @param r The rectangle to union with; only applied if this rectangle is normalised.
    void unite(const td::RectNormalized<T>& r)
    {
        if (isNormalized())
        {
            if (r.left < left)
                left = r.left;
            if (r.right > right)
                right = r.right;
            if (r.top < top)
                top = r.top;
            if (r.bottom > bottom)
                bottom = r.bottom;
        }
        else
        {
            operator = (r);
        }
    }

    /// @brief Expands this rectangle to include a single point.
    /// @param pt The point to include.
    void unite(const td::Point<T>& pt)
    {
        if (isNormalized())
        {
            if (pt.x < left)
                left = pt.x;
            if (pt.x > right)
                right = pt.x;
            if (pt.y < top)
                top = pt.y;
            if (pt.y > bottom)
                bottom = pt.y;
        }
        else
        {
            operator = (pt);
        }
    }

    /// @brief Sets the rectangle from scalar origin and size components.
    /// @param x      Left edge coordinate.
    /// @param y      Top edge coordinate.
    /// @param width  Desired width.
    /// @param height Desired height.
    inline void setOriginAndSize(T x, T y, T width, T height)
    {
        left = x;
        top = y;
        right = left + width;
        bottom = top + height;
    }

    /// @brief Sets the rectangle from an origin point and a Size object.
    /// @param origin Top-left corner.
    /// @param size   Width and height.
    inline void setOriginAndSize(const td::Point<T>& origin, const td::Size<T>& size)
    {
        left = origin.x;
        top = origin.y;
        right = left + size.width;
        bottom = top + size.height;
    }

    /// @brief Sets the rectangle's size while keeping its origin fixed.
    /// @param width  New width.
    /// @param height New height.
    inline void setSize(T width, T height)
    {
        right = left + width;
        bottom = top + height;
    }

    /// @brief Sets the rectangle's size from a Size object while keeping its origin fixed.
    /// @param size New width and height.
    inline void setSize(const td::Size<T>& size)
    {
        right = left + size.width;
        bottom = top + size.height;
    }

    /// @brief Sets the width while keeping the left edge fixed.
    /// @param width New width.
    inline void setWidth(T width)
    {
        right = left + width;
    }

    /// @brief Sets the left edge and width simultaneously.
    /// @param x     New left edge coordinate.
    /// @param width New width.
    inline void setLeftAndWidth(T x, T width)
    {
        left = x;
        right = left + width;
    }

    /// @brief Sets the height while keeping the top edge fixed.
    /// @param height New height.
    inline void setHeight(T height)
    {
        bottom = top + height;
    }

    /// @brief Sets the top edge and height simultaneously.
    /// @param y      New top edge coordinate.
    /// @param height New height.
    inline void setTopAndHeight(T y, T height)
    {
        top = y;
        bottom = top + height;
    }

//    void translate(T dx, T dy)
//    {
//        left += dx;
//        right += dx;
//        top += dy;
//        bottom += dy;
//    }

    /// @brief Swaps left/right and/or top/bottom if necessary to enforce normalisation.
    inline void normalize()
    {
        T tmp;
        if (right < left)
        {
            tmp = left;
            left = right;
            right = tmp;
        }

        if (bottom < top)
        {
            tmp = bottom;
            bottom = top;
            top = tmp;
        }
    }

    /// @brief Constructs a normalised rectangle from two corner points.
    /// @param tl Top-left corner (normalised after construction).
    /// @param br Bottom-right corner (normalised after construction).
    RectNormalized(const Point<T>& tl, const Point<T>& br)
        : left(tl.x)
        , top(tl.y)
        , right(br.x)
        , bottom(br.y)
    {
        normalize();
    }

    /// @brief Copy constructor.
    /// @param r The RectNormalized to copy from.
    RectNormalized(const RectNormalized<T>& r)
        : left(r.left)
        , top(r.top)
        , right(r.right)
        , bottom(r.bottom)
    {}

//    inline void operator = (const RectNormalized& r)
//    {
//        left = r.left;
//        right = r.right;
//        top = r.top;
//        bottom = r.bottom;
//    }

    /// @brief Returns the minimum squared distance from a point to any edge of the rectangle.
    /// @param x X-coordinate of the query point.
    /// @param y Y-coordinate of the query point.
    /// @return Minimum distance to the nearest edge.
    inline T getMinDistance2(T x, T y) const
    {
        T minDistance = getDistanceFromAB(left, top, right,  top, x, y);

        T distance = getDistanceFromAB(left, top, left,  bottom, x, y);

        if (distance < minDistance)
            minDistance = distance;

        distance = getDistanceFromAB(left, bottom, right,  bottom, x, y);
        if (distance < minDistance)
            minDistance = distance;

        distance = getDistanceFromAB(right, top, right,  bottom, x, y);
        if (distance < minDistance)
            minDistance = distance;

        return minDistance;
    }

    /// @brief Returns the minimum distance from a Point to any edge of the rectangle.
    /// @param pt The query point.
    /// @return Minimum distance to the nearest edge.
    inline T getMinDistance2(const Point<T>& pt) const
    {
        return getMinDistance2(pt.x, pt.y);
    }

    /// @brief Tests whether a point is close enough to any edge to be considered selectable.
    /// @param x           X-coordinate of the query point.
    /// @param y           Y-coordinate of the query point.
    /// @param minDistance Maximum selection distance threshold.
    /// @return true if the point is within minDistance of any edge.
    inline bool isSelectable(T x, T y, T minDistance) const
    {
         if (getDistanceFromAB(left, top, right,  top, x, y) < minDistance)
             return true;

        if (getDistanceFromAB(left, top, left,  bottom, x, y) < minDistance)
            return true;

        if (getDistanceFromAB(left, bottom, right,  bottom, x, y) < minDistance)
            return true;

        if (getDistanceFromAB(right, top, right,  bottom, x, y) < minDistance)
            return true;

        return false;
    }

    /// @brief Writes a human-readable description to an output stream.
    /// @tparam TOSTREAM Output stream type.
    /// @param o The output stream.
    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "TL:(" << left << "," << top <<"), Size:(" << width() << "," << height()<<")";
    }

    /// @brief Deserializes the rectangle from a binary input archive.
    /// @tparam TArch Binary archive type.
    /// @param ar The archive to read from.
    /// @return Always returns true.
    template<class TArch>
    bool loadFromBinaryStream(TArch& ar)
    {
        ar >> left >> top >> right >> bottom;
        return true;
    }

    /// @brief Serializes the rectangle to a binary output archive.
    /// @tparam TArch Binary archive type.
    /// @param ar The archive to write to.
    template<class TArch>
    void saveToBinaryStream(TArch& ar) const
    {
        ar << left << top << right << bottom;
    }

    /// @brief Boost-style serialize method for the rectangle.
    /// @tparam TArch Archive type.
    /// @param ar           The archive.
    /// @param file_version Unused version parameter.
    template<class TArch>
    void serialize(TArch& ar, const unsigned int file_version) const
    {
        ar & left & top & right & bottom;
    }
};

/// @brief A RectNormalized that overrides contains() to use strict inequality for floating-point types.
/// @tparam TOWN  Coordinate type of this rectangle.
/// @tparam TCOMP Coordinate type of the compared rectangle.
template <typename TOWN, typename TCOMP >
class RectFPNormalized : public RectNormalized<TOWN>
{
public:
    /// @brief Default constructor.
    RectFPNormalized(): RectNormalized<TOWN>(){}

    /// @brief Constructs from four explicit edge coordinates.
    /// @param L Left edge.
    /// @param T Top edge.
    /// @param R Right edge.
    /// @param B Bottom edge.
    RectFPNormalized(TOWN L, TOWN T, TOWN R, TOWN B): RectNormalized<TOWN>(L, T, R, B){}

    /// @brief Tests strict containment of a TCOMP rectangle inside this rectangle.
    /// @param rect The rectangle to test for strict containment.
    /// @return true if rect lies strictly inside this rectangle (not touching the boundary).
    bool contains(const RectNormalized<TCOMP>& rect) const
    {
        return (RectNormalized<TOWN>::left < rect.left &&
                RectNormalized<TOWN>::right > rect.right &&
                RectNormalized<TOWN>::top < rect.top &&
                RectNormalized<TOWN>::bottom > rect.bottom);
    }
};

} //namespace td
