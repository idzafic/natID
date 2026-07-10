// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PointAndSize.h
 @brief Aggregate combining a 2D point (origin) and a 2D size into a single structure. */
#pragma once
#include <td/Point.h>
#include <td/Size.h>

namespace td
{
	/// @brief Combines an origin Point and a Size into a single value type.
	/// @tparam T The numeric type used for all coordinates and dimensions.
	template <typename T>
	class PointAndSize
	{
	public:
		Point<T> point; ///< Origin (top-left) position.
		Size<T> size;   ///< Width and height dimensions.

		/// @brief Default constructor; zero-initializes both point and size.
		PointAndSize()
			: point()
			, size()
		{};

		/// @brief Constructs from explicit x, y, width, and height values.
		/// @param x Horizontal coordinate of the origin.
		/// @param y Vertical coordinate of the origin.
		/// @param w Width dimension.
		/// @param h Height dimension.
        PointAndSize(T x, T y, T w, T h)
            : point(x, y)
            , size(w, h)
        {};

		/// @brief Copy constructor.
		/// @param ps Source PointAndSize to copy.
		PointAndSize(const PointAndSize<T>& ps)
			: point(ps.point)
			, size(ps.size)
		{};

		/// @brief Constructs from a separate Point and Size.
		/// @param pointIn Origin point.
		/// @param sizeIn Size dimensions.
		PointAndSize(const Point<T>& pointIn, const Size<T>& sizeIn)
			: point(pointIn)
			, size(sizeIn)
		{};

		/// @brief Copy-assigns another PointAndSize to this instance.
		/// @param ps Source PointAndSize to copy.
		void operator = (const PointAndSize<T>& ps)
		{
			point = ps.point;
			size = ps.size;
		};

		/// @brief Clamps negative width and height values to zero.
        void makeNonNegative()
        {
            if (size.width < 0)
                size.width = 0;
            if (size.height < 0)
                size.height = 0;
        }


//		template<class Archive>
//		void serialize(Archive& ar, [[maybe_unused]] const unsigned int file_version) const
//		{
//			ar & point & size;
//		}

//		void c_str(char* buff, int buffLen) const
//		{
//			if (SNPRINTF(buff, buffLen, _TRUNCATE, "w=\"%g\" h=\"%g\"", width, height) < 0)
//				buff[buffLen - 1] = 0;
//		}
//
//		template <class TOSTREAM>
//		void show(TOSTREAM& o) const
//		{
//			o << "(" << width << "," << height << ")";
//		}
	};

//	template <typename TPRIM>
//	inline td::DataType getType(const td::Size<TPRIM>&) { return td::size; }
};
