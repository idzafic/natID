// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Circle.h
    @brief Defines the Circle template class representing a circle with a center point and radius. */
#pragma once
#include <td/Types.h>
#include <td/Point.h>
#include <math/math.h>

namespace td
{

	/// @brief Represents a circle defined by a center point and a radius.
	/// @tparam T The numeric type used for coordinates and radius (e.g., float, double, int).
	template <typename T>
	class Circle
	{
	public:
        td::Point<T> center; ///< The center point of the circle.
		T r;                 ///< The radius of the circle.
    public:
		/// @brief Default constructor that initializes the circle with center at the origin and zero radius.
		Circle()
			: center(0,0)
            , r(0)
		{};

		/// @brief Constructs a circle with the given center point and radius.
		/// @param centerPoint The center point of the circle.
		/// @param radius The radius of the circle.
		Circle(const Point<T>& centerPoint, T radius)
			: center(centerPoint)
            , r(radius)
		{};

		/// @brief Constructs a circle with the given center coordinates and radius.
		/// @param centerX The x-coordinate of the center.
		/// @param centerY The y-coordinate of the center.
		/// @param radius The radius of the circle.
		Circle(T centerX, T centerY, T radius)
			: center(centerX, centerY)
            , r(radius)
		{};

		/// @brief Checks whether a given point lies within or on the boundary of this circle.
		/// @param p The point to test.
		/// @return True if the squared distance from p to the center is less than or equal to the squared radius.
        inline bool contains(const Point<T>& p) const
        {
            T dx = p.x-center.x;
            T dy = p.y-center.y;

            T distToCenter2 = dx*dx + dy*dy;
            T r2 = r*r;
            return (distToCenter2 <= r2);
        }

		/// @brief Assignment operator that copies the center and radius from another Circle.
		/// @param c The Circle to copy from.
		void operator = (const Circle& c)
		{
            center = c.center;
            r = c.r;
		};

		/// @brief Computes the axis-aligned bounding rectangle that fully encloses this circle.
		/// @tparam Rect A template class with left, top, right, and bottom fields.
		/// @param boundRect Output parameter that receives the computed bounding rectangle.
        template <template<class> class Rect>
        void getBoundingRect(Rect<T>& boundRect) const
        {
            boundRect.left = center.x - r;
            boundRect.top = center.y - r;
            boundRect.right = center.x + r;
            boundRect.bottom = center.y + r;
        }

		/// @brief Translates the circle by the given delta point, shifting its center.
		/// @param delta The translation vector to apply to the center.
        void translate(const Point<T>& delta)
        {
            center.x += delta.x;
            center.y += delta.y;
        }

		/// @brief Translates the circle by the given x and y offsets, shifting its center.
		/// @param dx The horizontal translation amount.
		/// @param dy The vertical translation amount.
        void translate(T dx, T dy)
        {
            center.x += dx;
            center.y += dy;
        }

		/// @brief Computes the squared minimum distance from a point to the nearest point on the rim of the circle.
		/// @param p The point from which to measure the distance to the rim.
		/// @return The squared distance from p to the closest point on the circle's circumference.
        T getMinDistance2(const Point<T>& p) const
        {
            T dx = p.x - center.x;
            T dy = p.y - center.y;
            T distToCenter = std::sqrt(dx * dx + dy*dy);
            T distToRim = r - distToCenter;
            T distToRim2 = distToRim * distToRim;
            return distToRim2;
        }

		/// @brief Formats the circle's center and radius into a C-string buffer as XML-style attributes.
		/// @param buff Output character buffer to receive the formatted string.
		/// @param buffLen Length of the output buffer in bytes.
		void c_str(char* buff, int buffLen) const
		{
			if (SNPRINTF(buff, buffLen, _TRUNCATE, "xc=\"%g\" yc=\"%g\" r=\"%g\" ", center.x, center.y, r) < 0)
				buff[buffLen - 1] = 0;
		}

		/// @brief Outputs the circle's center and radius to a stream.
		/// @tparam TOSTREAM The output stream type.
		/// @param o The output stream to write to.
		template <class TOSTREAM>
		void show(TOSTREAM& o) const
		{
            center.show(o);
            o << " r=" << r;
		}

		/// @brief Deserializes the circle's center and radius from a binary input archive.
		/// @tparam TArch The archive type providing the >> operator.
		/// @param ar The binary input archive to read from.
		/// @return Always returns true.
        template<class TArch>
        bool loadFromBinaryStream(TArch& ar)
        {
            ar >> center >> r;
            return true;
        }

		/// @brief Serializes the circle's center and radius to a binary output archive.
		/// @tparam TArch The archive type providing the << operator.
		/// @param ar The binary output archive to write to.
        template<class TArch>
        void saveToBinaryStream(TArch& ar) const
        {
            ar << center << r;
        }
	};

	/// @brief Returns the DataType identifier for Circle values.
	/// @tparam TPRIM The numeric type of the Circle.
	/// @return The td::circle DataType constant.
	template <typename TPRIM>
	inline td::DataType getType(const td::Circle<TPRIM>&) { return td::circle; }
};

