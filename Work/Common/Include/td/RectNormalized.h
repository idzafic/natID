// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Rect.h>
#include <td/Point.h>
#include <td/Size.h>
#include <td/Circle.h>

namespace td
{

template <typename T>
class RectNormalized
{
public:
    T left;
    T top;
    T right;
    T bottom;

    inline T width() const
    {
        return right - left;
    }

    inline T height() const
    {
        return bottom - top;
    }

    inline bool contains(const RectNormalized<T>& r) const
    {
        return (left <= r.left && right >= r.right && top <= r.top && bottom >= r.bottom);
    }

    inline bool intersects (const RectNormalized<T>& r) const
    {
        return !(left > r.right || right < r.left || top > r.bottom || bottom < r.top);
        //if (!toRet)
        //{
        //	toRet = r.contains();
        //}
        //return toRet;
    }

    inline bool notIntersects (const RectNormalized<T>& r) const
    {
        return (left > r.right || right < r.left || top > r.bottom || bottom < r.top);
    }

    inline void operator = (T val)
    {
        left = right = top = bottom = val;
    }

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

    inline void inflate(T val)
    {
        left -= val;
        top -= val;
        bottom += val;
        right += val;
    }
    
    inline void inflate(T val, T minLeft, T minTop)
    {
        inflate(val);
        if (left < minLeft)
            left = minLeft;
        if (top < minTop)
            top = minTop;
    }
    
    void translate(T dx, T dy)
    {
        left += dx;
        right += dx;
        top += dy;
        bottom += dy;
    }
    
    void scale(T sx, T sy)
    {
        left *= sx;
        right *= sx;
        top *= sy;
        bottom *= sy;
    }

    void scale(T s)
    {
        left *= s;
        right *= s;
        top *= s;
        bottom *= s;
    }

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

    inline td::Coord space() const
    {
        return (td::Coord) (width() * height());
    }

    inline void center(Point<T>& center) const
    {
        center.x = (left + right)/2;
        center.y = (top + bottom) /2;
    }
    
    inline T getLengthOfSmallerSide() const
    {
        auto w = width();
        auto h = height();
        if (w < h)
            return w;
        return h;
    }
    
    inline Point<T> center() const
    {
        Point<T> center((left + right)/2, (top + bottom) /2);
        return center;
    }

    RectNormalized()
        //: left(0)
        //, right(0)
        //, top(0)
        //, bottom(0)
    {}

    RectNormalized(T l, T t, T r, T b)
        : left(l)
        , top(t)
        , right(r)
        , bottom(b)
    {
        normalize();
    }
    
    RectNormalized(bool makeNormalized, T l, T t, T r, T b)
        : left(l)
        , top(t)
        , right(r)
        , bottom(b)
    {
        if (makeNormalized)
            normalize();
    }
    
    RectNormalized(T l, T t, T widthAndHeight)
        : left(l)
        , top(t)
        , right(l + widthAndHeight)
        , bottom(t + widthAndHeight)
    {
    }
    
    RectNormalized(const td::Point<T>& origin, const td::Size<T>& size)
        : left(origin.x)
        , top(origin.y)
        , right(origin.x + size.width)
        , bottom(origin.y + size.height)
    {
    }
    
    RectNormalized(const td::Circle<T>& c)
    {
        left = c.center.x - c.r;
        top = c.center.y - c.r;
        right = c.center.x + c.r;
        bottom = c.center.y + c.r;
    }

    inline bool contains(const Point<T>& p) const
    {
        return (p.x >= left && p.x <= right && p.y >= top && p.y <= bottom);
    }
    
    inline void setOrigin(T x, T y)
    {
        auto width = right - left;
        auto height = bottom - top;
        left = x;
        top = y;
        right = left + width;
        bottom = top + height;
    }
    
    inline void setOrigin(const td::Point<T>& origin)
    {
        auto width = right - left;
        auto height = bottom - top;
        left = origin.x;
        top = origin.y;
        right = left + width;
        bottom = top + height;
    }
    
    inline void setOriginAndSize(const td::Point<T>& origin, const td::Point<T>& size)
    {
        left = origin.x;
        top = origin.y;
        right = left + size.width;
        bottom = top + size.height;
    }
    
    bool isNormalized() const
    {
        return ((left <= right) && (top <= bottom));
    }
    
    void operator -= (const td::Point<T>& delta)
    {
        left -= delta.x;
        right -= delta.x;
        top -= delta.y;
        bottom -= delta.y;
    }
    
    void operator += (const td::Point<T>& delta)
    {
        left += delta.x;
        right += delta.x;
        top += delta.y;
        bottom += delta.y;
    }
    
    inline const RectNormalized<T>& operator = (const RectNormalized<T>& r)
    {
        left = r.left;
        right = r.right;
        top = r.top;
        bottom = r.bottom;
        return *this;
    }
    
    inline const RectNormalized<T>& operator = (const Point<T>& pt)
    {
        left = pt.x;
        right = pt.x;
        top = pt.y;
        bottom = pt.y;
        return *this;
    }
    
    inline bool operator == (const RectNormalized<T>& r) const
    {
        return ((left == r.left) && (right == r.right) && (top == r.top) && (bottom == r.bottom));
    }
    
    inline bool operator != (const RectNormalized<T>& r) const
    {
        return ((left != r.left) || (right != r.right) || (top != r.top) || (bottom != r.bottom));
    }
    
    void translate(const Point<T>& delta)
    {
        left += delta.x;
        top += delta.y;
        right += delta.x;
        bottom += delta.y;
    }
    
    void toZero()
    {
        left = right = top = bottom = 0;
    }
    
    bool isZero() const
    {
        return ((left == 0) && (right == 0) && (top == 0) && (bottom == 0));
    }
    
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
    
    inline void setOriginAndSize(T x, T y, T width, T height)
    {
        left = x;
        top = y;
        right = left + width;
        bottom = top + height;
    }
    
    inline void setOriginAndSize(const td::Point<T>& origin, const td::Size<T>& size)
    {
        left = origin.x;
        top = origin.y;
        right = left + size.width;
        bottom = top + size.height;
    }
    
    inline void setSize(T width, T height)
    {
        right = left + width;
        bottom = top + height;
    }
    
    inline void setSize(const td::Size<T>& size)
    {
        right = left + size.width;
        bottom = top + size.height;
    }
    
    inline void setWidth(T width)
    {
        right = left + width;
    }
    
    inline void setLeftAndWidth(T x, T width)
    {
        left = x;
        right = left + width;
    }
    
    inline void setHeight(T height)
    {
        bottom = top + height;
    }
    
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

    RectNormalized(const Point<T>& tl, const Point<T>& br)
        : left(tl.x)
        , top(tl.y)
        , right(br.x)
        , bottom(br.y)
    {
        normalize();
    }

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
    
    inline T getMinDistance2(const Point<T>& pt) const
    {
        return getMinDistance2(pt.x, pt.y);
    }

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
    
    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "TL:(" << left << "," << top <<"), Size:(" << width() << "," << height()<<")";
    }

    template<class TArch>
    bool loadFromBinaryStream(TArch& ar)
    {
        ar >> left >> top >> right >> bottom;
        return true;
    }
    
    template<class TArch>
    void saveToBinaryStream(TArch& ar) const
    {
        ar << left << top << right << bottom;
    }
    
    template<class TArch>
    void serialize(TArch& ar, const unsigned int file_version) const
    {
        ar & left & top & right & bottom;
    }
};

template <typename TOWN, typename TCOMP >
class RectFPNormalized : public RectNormalized<TOWN>
{
public:
    RectFPNormalized(): RectNormalized<TOWN>(){}
    RectFPNormalized(TOWN L, TOWN T, TOWN R, TOWN B): RectNormalized<TOWN>(L, T, R, B){}
    bool contains(const RectNormalized<TCOMP>& rect) const
    {
        return (RectNormalized<TOWN>::left < rect.left &&
                RectNormalized<TOWN>::right > rect.right &&
                RectNormalized<TOWN>::top < rect.top &&
                RectNormalized<TOWN>::bottom > rect.bottom);
    }
};

} //namespace td
