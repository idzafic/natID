// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Rect.h
    @brief Defines a generic axis-aligned rectangle with left, top, right, bottom edges. */
#pragma once
#include <math/math.h>
#include <td/Point.h>

namespace td
{
	/// @brief Represents an axis-aligned rectangle defined by four edge coordinates.
	/// @tparam T The numeric type used for the coordinate values.
	template <typename T>
	class Rect
	{
	public:
		T left;   ///< Left edge coordinate.
		T top;    ///< Top edge coordinate.
		T right;  ///< Right edge coordinate.
		T bottom; ///< Bottom edge coordinate.

		/// @brief Returns the width of the rectangle as the absolute difference of right and left.
		/// @return Width of the rectangle.
		T width() const
		{
			return math::abs(right - left);
		};

		/// @brief Sets the right and bottom edges based on a given width and height.
		/// @param w Desired width; sets right = left + w.
		/// @param h Desired height; sets bottom = top + h.
		void setWidthAndHeight(T w, T h)
		{
			right = left + w;
			bottom = top + h;
		};

		/// @brief Returns the height of the rectangle as the absolute difference of bottom and top.
		/// @return Height of the rectangle.
		T height() const
		{
			return math::abs(bottom - top);
		};

		/// @brief Computes the centre point of the rectangle.
		/// @param center Output parameter that receives the centre coordinates.
		void center(Point<T>& center) const
		{
			center.x = (left + right)/2;
			center.y = (top + bottom) /2;
		};

		/// @brief Default constructor; initialises all edges to zero.
		Rect()
			: left(0)
			, right(0)
			, top(0)
			, bottom(0)
		{};

		/// @brief Constructs a Rect from explicit edge coordinates.
		/// @param l Left edge value.
		/// @param t Top edge value.
		/// @param r Right edge value.
		/// @param b Bottom edge value.
		Rect(T l, T t, T r, T b)
			: left(l)
			, right(r)
			, top(t)
			, bottom(b)
		{};

		/// @brief Constructs a Rect from two corner points.
		/// @param tl The top-left corner point.
		/// @param br The bottom-right corner point.
		Rect(const Point<T>& tl, const Point<T>& br)
			: left(tl.x)
			, right(br.x)
			, top(tl.y)
			, bottom(br.y)
		{};

		/// @brief Copy constructor.
		/// @param r The Rect to copy from.
		Rect(const Rect<T>& r)
			: left(r.left)
			, right(r.right)
			, top(r.top)
			, bottom(r.bottom)
		{};

		/// @brief Assignment operator.
		/// @param r The Rect to assign from.
		/// @return Reference to this Rect.
		const Rect<T>& operator = (const Rect& r)
		{
			left = r.left;
			right = r.right;
			top = r.top;
			bottom = r.bottom;
			return * this;
		};

		/// @brief Computes the minimum distance from a point to any edge of the rectangle.
		/// @param x The x-coordinate of the query point.
		/// @param y The y-coordinate of the query point.
		/// @return The minimum distance from the point to the nearest edge segment.
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

	/// @brief Returns the DataType identifier for a Rect of the given primitive type.
	/// @tparam TPRIM The primitive type of the Rect coordinates.
	/// @return td::rect data-type identifier.
	template <typename TPRIM>
	inline td::DataType getType(const td::Rect<TPRIM>&) { return td::rect; }
}
