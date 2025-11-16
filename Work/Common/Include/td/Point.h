// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <type_traits>

namespace td
{

template <typename T>
class Point
{
public:
    T x;
    T y;
    Point()
        : x(0)
        , y(0)
    {}

    Point(const Point<T>& p)
        : x(p.x)
        , y(p.y)
    {}

    Point(T xCoord, T yCoord)
        : x(xCoord)
        , y(yCoord)
    {}
    
    void toZero()
    {
        x = 0;
        y = 0;
    }

    void operator = (const Point& p)
    {
        x = p.x;
        y = p.y;
    }
    
    bool operator == (const Point& p) const
    {
        return ((x == p.x) && (y==p.y));
    }

    void castToInt(Point<td::INT4>& p) const
    {
        p.x = (td::INT4) x;
        p.y = (td::INT4) y;
    }

    template<class Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int file_version) const
    {
        ar & x & y;
    }

    void c_str(char* buff, int buffLen) const
    {
        if (SNPRINTF(buff, buffLen, _TRUNCATE, "x=\"%g\" y=\"%g\"", x, y) < 0)
            buff[buffLen - 1] = 0;
    }

    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "(" << x << "," << y << ")";
    }
    
    template<class Archive>
    bool loadFromBinaryStream(Archive& ar)
    {
        ar >> x >> y;
        return true;
    }
    
    template<class Archive>
    void saveToBinaryStream(Archive& ar) const
    {
        ar << x << y;
    }
    
    template <typename T2>
    void operator = (const Point<T2>& pIn)
    {
        x = T(pIn.x);
        y = T(pIn.y);
    }
    
    void operator -= (const Point<T>& pIn)
    {
        x -= pIn.x;
        y -= pIn.y;
    }
    
    void operator += (const Point<T>& pIn)
    {
        x += pIn.x;
        y += pIn.y;
    }
    
    void operator *= (const T& scale)
    {
        x *= scale;
        y *= scale;
    }

    void operator /= (const T& invScale)
    {
        x /= invScale;
        y /= invScale;
    }
    
    void translate(const T& dx, const T& dy)
    {
        x += dx;
        y += dy;
    }
    
    void translate(const Point<T>& dP)
    {
        x += dP.x;
        y += dP.y;
    }

    void scale(T sx, T sy)
    {
        x *= sx;
        y *= sy;
    }

    void scale(T s)
    {
        x *= s;
        y *= s;
    }
    template <typename T2>
    auto getEuclidianDistanceTo(const Point<T2>& dP) const
    {
        using RT = std::common_type_t<T, T2>;
        RT dx = static_cast<RT>(dP.x) - static_cast<RT>(x);
        RT dy = static_cast<RT>(dP.y) - static_cast<RT>(y);
        return std::sqrt(dx * dx + dy * dy);
    }
    
    //dot product is scalar
    template <typename T2>
    inline std::common_type_t<T, T2> dot(const Point<T2>& b)
    {
        using RT = std::common_type_t<T, T2>;
        return static_cast<RT>(x) * static_cast<RT>(b.x) + static_cast<RT>(y) * static_cast<RT>(b.y);
    }
    
    template <typename T2>
    inline std::common_type_t<T, T2> cross(const Point<T2>& b)
    {
        using RT = std::common_type_t<T, T2>;
        return static_cast<RT>(x) * static_cast<RT>(b.y) - static_cast<RT>(y) * static_cast<RT>(b.x);
    }

};


//some methods
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
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator+ (const Point<T1>& p1, const Point<T2>& p2)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>(static_cast<RT>(p1.x) + static_cast<RT>(p2.x),
                     static_cast<RT>(p1.y) + static_cast<RT>(p2.y));
}

// Subtraction
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator- (const Point<T1>& p1, const Point<T2>& p2)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>(static_cast<RT>(p1.x) - static_cast<RT>(p2.x),
                     static_cast<RT>(p1.y) - static_cast<RT>(p2.y));
}

// Element-wise multiplication
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator* (const Point<T1>& a, const Point<T2>& b)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>{static_cast<RT>(a.x) * static_cast<RT>(b.x),
                     static_cast<RT>(a.y) * static_cast<RT>(b.y)};
}

// Element-wise division
template <typename T1, typename T2>
inline Point<std::common_type_t<T1, T2>> operator/ (const Point<T1>& a, const Point<T2>& b)
{
    using RT = std::common_type_t<T1, T2>;
    return Point<RT>{static_cast<RT>(a.x) / static_cast<RT>(b.x),
                     static_cast<RT>(a.y) / static_cast<RT>(b.y)};
}


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

template <typename TPRIM>
inline td::DataType getType(const td::Point<TPRIM>&) { return td::point; }
};

