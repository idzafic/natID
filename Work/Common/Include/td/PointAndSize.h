// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Point.h>
#include <td/Size.h>

namespace td
{
	template <typename T>
	class PointAndSize
	{
	public:
		Point<T> point;
		Size<T> size;
        
		PointAndSize()
			: point()
			, size()
		{};

        PointAndSize(T x, T y, T w, T h)
            : point(x, y)
            , size(w, h)
        {};
        
		PointAndSize(const PointAndSize<T>& ps)
			: point(ps.point)
			, size(ps.size)
		{};

		PointAndSize(const Point<T>& pointIn, const Size<T>& sizeIn)
			: point(pointIn)
			, size(sizeIn)
		{};

		void operator = (const PointAndSize<T>& ps)
		{
			point = ps.point;
			size = ps.size;
		};


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

