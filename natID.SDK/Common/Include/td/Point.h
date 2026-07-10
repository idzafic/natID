// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Point.h
 @brief 2D point template class and related free functions for geometric operations. */
#pragma once
#include <td/Types.h>
#include <type_traits>
#include <cmath>

namespace td
{

/// @brief A 2D point with x and y coordinates of type T.
/// @tparam T The numeric type used for the coordinates.
template <typename T>
class Point
{
public:
    T x; ///< Horizontal coordinate.
    T y; ///< Vertical coordinate.

    /// @brief Default constructor; initializes both coordinates to zero.
    Point()
        : x(0)
        , y(0)
    {}

    /// @brief Copy constructor.
    /// @param p The point to copy from.
    Point(const Point<T>& p)
        : x(p.x)
        , y(p.y)
    {}

    /// @brief Constructs a point with the given coordinates.
    /// @param xCoord Horizontal coordinate.
    /// @param yCoord Vertical coordinate.
    Point(T xCoord, T yCoord)
        : x(xCoord)
        , y(yCoord)
    {}

    /// @brief Resets both coordinates to zero.
    void toZero()
    {
        x = 0;
        y = 0;
    }

    /// @brief Copy-assigns the coordinates from another point.
    /// @param p The source point.
    void operator = (const Point& p)
    {
        x = p.x;
        y = p.y;
    }

    /// @brief Equality comparison.
    /// @param p The point to compare against.
    /// @return true if both x and y coordinates are equal.
    bool operator == (const Point& p) const
    {
        return ((x == p.x) && (y==p.y));
    }

    /// @brief Casts coordinates to td::INT4 and stores the result in @p p.
    /// @param p Output point that receives the truncated integer coordinates.
    void castToInt(Point<td::INT4>& p) const
    {
        p.x = (td::INT4) x;
        p.y = (td::INT4) y;
    }

    /// @brief Serializes the x and y coordinates using the provided archive.
    /// @tparam Archive Archive type supporting the & operator.
    /// @param ar The archive to serialize into or from.
    /// @param file_version Unused version parameter (for compatibility).
    template<class Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int file_version) const
    {
        ar & x & y;
    }

    /// @brief Writes the coordinates as XML attributes into a character buffer.
    /// @param buff Output buffer.
    /// @param buffLen Size of the output buffer in bytes.
    void c_str(char* buff, int buffLen) const
    {
        if (SNPRINTF(buff, buffLen, _TRUNCATE, "x=\"%g\" y=\"%g\"", x, y) < 0)
            buff[buffLen - 1] = 0;
    }

    /// @brief Outputs the point in (x,y) notation to a stream.
    /// @tparam TOSTREAM Stream type that supports the << operator.
    /// @param o The output stream.
    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "(" << x << "," << y << ")";
    }

    /// @brief Deserializes x and y from a binary stream.
    /// @tparam Archive Binary input stream type supporting the >> operator.
    /// @param ar The input stream.
    /// @return true on success.
    template<class Archive>
    bool loadFromBinaryStream(Archive& ar)
    {
        ar >> x >> y;
        return true;
    }

    /// @brief Serializes x and y into a binary stream.
    /// @tparam Archive Binary output stream type supporting the << operator.
    /// @param ar The output stream.
    template<class Archive>
    void saveToBinaryStream(Archive& ar) const
    {
        ar << x << y;
    }

    /// @brief Assigns coordinates from a point of a different numeric type with implicit conversion.
    /// @tparam T2 Source coordinate type.
    /// @param pIn Source point.
    template <typename T2>
    void operator = (const Point<T2>& pIn)
    {
        x = T(pIn.x);
        y = T(pIn.y);
    }

    /// @brief Subtracts the coordinates of another point from this point.
    /// @param pIn The point whose coordinates are subtracted.
    void operator -= (const Point<T>& pIn)
    {
        x -= pIn.x;
        y -= pIn.y;
    }

    /// @brief Adds the coordinates of another point to this point.
    /// @param pIn The point whose coordinates are added.
    void operator += (const Point<T>& pIn)
    {
        x += pIn.x;
        y += pIn.y;
    }

    /// @brief Scales both coordinates by a scalar factor.
    /// @param scale The scaling factor.
    void operator *= (const T& scale)
    {
        x *= scale;
        y *= scale;
    }

    /// @brief Divides both coordinates by a scalar value.
    /// @param invScale The divisor.
    void operator /= (const T& invScale)
    {
        x /= invScale;
        y /= invScale;
    }

    /// @brief Translates the point by (dx, dy).
    /// @param dx Horizontal displacement.
    /// @param dy Vertical displacement.
    void translate(const T& dx, const T& dy)
    {
        x += dx;
        y += dy;
    }

    /// @brief Translates the point by the coordinates of another point.
    /// @param dP Displacement point.
    void translate(const Point<T>& dP)
    {
        x += dP.x;
        y += dP.y;
    }

    /// @brief Scales coordinates independently in the x and y directions.
    /// @param sx Horizontal scale factor.
    /// @param sy Vertical scale factor.
    void scale(T sx, T sy)
    {
        x *= sx;
        y *= sy;
    }

    /// @brief Scales both coordinates uniformly.
    /// @param s Scale factor applied to both axes.
    void scale(T s)
    {
        x *= s;
        y *= s;
    }

    /// @brief Computes the Euclidean distance from this point to another point.
    /// @tparam T2 Coordinate type of the target point.
    /// @param dP Target point.
    /// @return Euclidean distance as a common numeric type of T and T2.
    template <typename T2>
    auto getEuclidianDistanceTo(const Point<T2>& dP) const
    {
        using RT = std::common_type_t<T, T2>;
        RT dx = static_cast<RT>(dP.x) - static_cast<RT>(x);
        RT dy = static_cast<RT>(dP.y) - static_cast<RT>(y);
        return std::sqrt(dx * dx + dy * dy);
    }

    //dot product is scalar
    /// @brief Computes the dot product of this point (treated as a 2D vector) with another.
    /// @tparam T2 Coordinate type of the second vector.
    /// @param b The second vector.
    /// @return Scalar dot product using the common numeric type of T and T2.
    template <typename T2>
    inline std::common_type_t<T, T2> dot(const Point<T2>& b)
    {
        using RT = std::common_type_t<T, T2>;
        return static_cast<RT>(x) * static_cast<RT>(b.x) + static_cast<RT>(y) * static_cast<RT>(b.y);
    }

    /// @brief Computes the 2D cross product (scalar z-component) of this vector with another.
    /// @tparam T2 Coordinate type of the second vector.
    /// @param b The second vector.
    /// @return Scalar cross product (x*b.y - y*b.x) using the common numeric type.
    template <typename T2>
    inline std::common_type_t<T, T2> cross(const Point<T2>& b)
    {
        using RT = std::common_type_t<T, T2>;
        return static_cast<RT>(x) * static_cast<RT>(b.y) - static_cast<RT>(y) * static_cast<RT>(b.x);
    }

};


//some methods
/// @brief Computes the squared distance from point (x, y) to the nearest point on segment AB.
/// @tparam T Numeric type for all coordinates.
/// @param x1 X coordinate of segment endpoint A.
/// @param y1 Y coordinate of segment endpoint A.
/// @param x2 X coordinate of segment endpoint B.
/// @param y2 Y coordinate of segment endpoint B.
/// @param x  X coordinate of the query point.
/// @param y  Y coordinate of the query point.
/// @return Squared minimum distance from (x, y) to the segment AB.
template <typename T>
inline T getDistanceFromAB(T x1, T y1, T x2, T y2, T x, T y)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T L =dx*dx + dy*dy;

    T D = (x - x1) * dx + (y- y1) * dy;

    T A;

    if(L == 0)
    {
        //duz je duzine nula x1=x2, y1=y2
        dx = x - x1;
        dy = y - y1;
        A = dx * dx + dy * dy;
        return A;
    }

    T r= D/L;

    T X, Y; //closest point

    //najbliza tacka na duzi je tacka A
    if (r <= 0)
    {
        X = x1;
        Y = y1;
    }
    //najbliza tacka B
    else if (r >= 1)
    {
        X = x2;
        Y = y2;
    }
    //najbliza tacka se nalazi izmedju A i B
    else
    {
        A = r*dx;

        X = x1 + A;

        A = r*dy;
        Y = y1 + A;
    }

    dx = X - x;
    dy = Y - y;
    A = dx * dx + dy * dy;
    return A;
}

// Addition
/// @brief Adds two points component-wise, promoting to the common type.
/// @tparam T1 Type of the first point's coordinates.
/// @tparam T2 Type of the second point's coordinates.
/// @param p1 First point.
/// @param p2 Second point.
/// @return New point with coordinates p1 + p2 in the common type.
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator+ (const Point<T1>& p1, const Point<T2>& p2)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>(static_cast<RT>(p1.x) + static_cast<RT>(p2.x),
                     static_cast<RT>(p1.y) + static_cast<RT>(p2.y));
}

// Subtraction
/// @brief Subtracts the second point from the first component-wise, promoting to the common type.
/// @tparam T1 Type of the first point's coordinates.
/// @tparam T2 Type of the second point's coordinates.
/// @param p1 First point (minuend).
/// @param p2 Second point (subtrahend).
/// @return New point with coordinates p1 - p2 in the common type.
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator- (const Point<T1>& p1, const Point<T2>& p2)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>(static_cast<RT>(p1.x) - static_cast<RT>(p2.x),
                     static_cast<RT>(p1.y) - static_cast<RT>(p2.y));
}

// Element-wise multiplication
/// @brief Multiplies two points component-wise, promoting to the common type.
/// @tparam T1 Type of the first point's coordinates.
/// @tparam T2 Type of the second point's coordinates.
/// @param a First point.
/// @param b Second point.
/// @return New point with element-wise product in the common type.
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator* (const Point<T1>& a, const Point<T2>& b)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>{static_cast<RT>(a.x) * static_cast<RT>(b.x),
                     static_cast<RT>(a.y) * static_cast<RT>(b.y)};
}

// Element-wise division
/// @brief Divides the first point by the second component-wise, promoting to the common type.
/// @tparam T1 Type of the dividend point's coordinates.
/// @tparam T2 Type of the divisor point's coordinates.
/// @param a Dividend point.
/// @param b Divisor point.
/// @return New point with element-wise quotient in the common type.
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator/ (const Point<T1>& a, const Point<T2>& b)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>{static_cast<RT>(a.x) / static_cast<RT>(b.x),
                     static_cast<RT>(a.y) / static_cast<RT>(b.y)};
}


/// @brief Computes the squared distance from point (x, y) to the nearest point on the segment defined by two Point pointers.
/// @tparam T Numeric type for all coordinates.
/// @param p1 Pointer to the first segment endpoint.
/// @param p2 Pointer to the second segment endpoint.
/// @param x  X coordinate of the query point.
/// @param y  Y coordinate of the query point.
/// @return Squared minimum distance from (x, y) to the segment.
template <typename T>
inline T getDistanceFromAB(const Point<T>* p1, const Point<T>* p2, T x, T y)
{
    T x2 = p2->x;
    T x1 = p1->x;
    T y2 = p2->y;
    T y1 = p1->y;

    T dx = x2 - x1;
    T dy = y2 - y1;

    T L =dx*dx + dy*dy;

    T D = (x - x1) * dx + (y- y1) * dy;

    T A;

    if(L == 0)
    {
        //duz je duzine nula x1=x2, y1=y2
        dx = x - x1;
        dy = y - y1;
        A = dx * dx + dy * dy;
        return A;
    }

    T r= D/L;

    T X, Y; //closest point

    //najbliza tacka na duzi je tacka A
    if (r <= 0)
    {
        X = x1;
        Y = y1;
    }
    //najbliza tacka B
    else if (r >= 1)
    {
        X = x2;
        Y = y2;
    }
    //najbliza tacka se nalazi izmedju A i B
    else
    {
        A = r*dx;

        X = x1 + A;

        A = r*dy;
        Y = y1 + A;
    }

    dx = X - x;
    dy = Y - y;
    A = dx * dx + dy * dy;
    return A;
}

/// @brief Returns the td::DataType identifier for a Point value.
/// @tparam TPRIM Coordinate type of the point.
/// @return td::point data type identifier.
template <typename TPRIM>
inline td::DataType getType(const td::Point<TPRIM>&) { return td::point; }
};
