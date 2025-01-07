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
#include <td/Point.h>
#include <math/math.h>

namespace td
{

	template <typename T>
	class Circle
	{
	public:
        td::Point<T> center;
		T r;
    public:
		Circle()
			: center(0,0)
            , r(0)
		{};

		Circle(const Point<T>& centerPoint, T radius)
			: center(centerPoint)
            , r(radius)
		{};

		Circle(T centerX, T centerY, T radius)
			: center(centerX, centerY)
            , r(radius)
		{};

        inline bool contains(const Point<T>& p) const
        {
            T dx = p.x-center.x;
            T dy = p.y-center.y;
            
            T distToCenter2 = dx*dx + dy*dy;
            T r2 = r*r;
            return (distToCenter2 <= r2);
        }
        
		void operator = (const Circle& c)
		{
            center = c.center;
            r = c.r;
		};
        
        template <template<class> class Rect>
        void getBoundingRect(Rect<T>& boundRect) const
        {
            boundRect.left = center.x - r;
            boundRect.top = center.y - r;
            boundRect.right = center.x + r;
            boundRect.bottom = center.y + r;
        }
        
        void translate(const Point<T>& delta)
        {
            center.x += delta.x;
            center.y += delta.y;
        }
        
        void translate(T dx, T dy)
        {
            center.x += dx;
            center.y += dy;
        }
        
        T getMinDistance2(const Point<T>& p) const
        {
            T dx = p.x - center.x;
            T dy = p.y - center.y;
            T distToCenter = std::sqrt(dx * dx + dy*dy);
            T distToRim = r - distToCenter;
            T distToRim2 = distToRim * distToRim;
            return distToRim2;
        }

		void c_str(char* buff, int buffLen) const
		{
			if (SNPRINTF(buff, buffLen, _TRUNCATE, "xc=\"%g\" yc=\"%g\" r=\"%g\" ", center.x, center.y, r) < 0)
				buff[buffLen - 1] = 0;
		}

		template <class TOSTREAM>
		void show(TOSTREAM& o) const
		{			
            center.show(o);
            o << " r=" << r;
		}
        
        template<class TArch>
        bool loadFromBinaryStream(TArch& ar)
        {
            ar >> center >> r;
            return true;
        }
        
        template<class TArch>
        void saveToBinaryStream(TArch& ar) const
        {
            ar << center << r;
        }
	};

	template <typename TPRIM>
	inline td::DataType getType(const td::Circle<TPRIM>&) { return td::circle; }
};

