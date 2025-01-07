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

template <typename T>
inline Point<T> operator + (const Point<T> p1, const Point<T>& p2)
{
    Point res(p1);
    res += p2;
    return res;
}

template <typename T>
inline Point<T> operator - (const Point<T> p1, const Point<T>& p2)
{
    Point res(p1);
    res -= p2;
    return res;
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

