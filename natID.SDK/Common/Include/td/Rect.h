// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <math/math.h>
#include <td/Point.h>

namespace td
{
	template <typename T>
	class Rect
	{
	public:
		T left;
		T top;
		T right;
		T bottom;

		T width() const
		{
			return math::abs(right - left);
		};

		void setWidthAndHeight(T w, T h)
		{
			right = left + w;
			bottom = top + h;
		};

		T height() const
		{
			return math::abs(bottom - top);
		};

		void center(Point<T>& center) const
		{
			center.x = (left + right)/2;
			center.y = (top + bottom) /2;
		};

		Rect()
			: left(0)
			, right(0)
			, top(0)
			, bottom(0)
		{};

		Rect(T l, T t, T r, T b)
			: left(l)
			, right(r)
			, top(t)
			, bottom(b)
		{};

		Rect(const Point<T>& tl, const Point<T>& br)
			: left(tl.x)
			, right(br.x)
			, top(tl.y)
			, bottom(br.y)
		{};

		Rect(const Rect<T>& r)
			: left(r.left)
			, right(r.right)
			, top(r.top)
			, bottom(r.bottom)
		{};

		const Rect<T>& operator = (const Rect& r)
		{
			left = r.left;
			right = r.right;
			top = r.top;
			bottom = r.bottom;
			return * this;
		};

		T getMinDistance(T x, T y) const
		{
			double minDistance = getDistanceFromAB(left, top, right,  top, x, y);

			double distance = getDistanceFromAB(left, top, left,  bottom, x, y);

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

		//template<class Archive>
		//void serialize(Archive& ar, const unsigned int file_version) const
		//{
		//	ar & left & top & right & bottom;
		//}
	};

	//std::ostream & operator<<(std::ostream &os, const Rect<td::INT4>& r)
	//{
	//	return os << r.left << r.top << r.right << r.bottom;
	//}
	
	template <typename TPRIM>
	inline td::DataType getType(const td::Rect<TPRIM>&) { return td::rect; }
}
