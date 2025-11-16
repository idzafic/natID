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
	class Size
	{
	public:
		T width;
		T height;
		Size()
			: width(0)
			, height(0)
		{};

		Size(const Size<T>& p)
			: width(p.width)
			, height(p.height)
		{};

		Size(T w, T h)
			: width(w)
			, height(h)
		{};
        
        void toZero()
        {
            width = 0;
            height = 0;
        }
        
        void scale(double scale)
        {
            width *= scale;
            height *= scale;
        }

		void operator = (const Size& p)
		{
			width = p.width;
			height = p.height;
		}
        
		template <typename T2>
		void operator = (const Size<T2>& p)
		{
			width = T(p.width);
			height = T(p.height);
		}

        void operator += (const Size& p)
        {
            width += p.width;
            height += p.height;
        }
        
        Size& operator *= (double scale)
        {
            width *= scale;
            height *= scale;
            return *this;
        }
        
        bool operator == (const Size& s) const
        {
            return ((width == s.width) && (height == s.height));
        }

        bool operator != (const Size& s) const
        {
            return ((width != s.width) || (height != s.height));
        }

		template<class Archive>
		void serialize(Archive& ar, [[maybe_unused]] const unsigned int file_version) const
		{
			ar & width & height;
		}

		void c_str(char* buff, int buffLen) const
		{
			if (SNPRINTF(buff, buffLen, _TRUNCATE, "w=\"%g\" h=\"%g\"", width, height) < 0)
				buff[buffLen - 1] = 0;
		}

		template <class TOSTREAM>
		void show(TOSTREAM& o) const
		{			
			o << "(" << width << "," << height << ")";
		}
	};

	//template <typename TPRIM>
	//inline td::DataType getType(const td::Size<TPRIM>&) { return td::size; }
};

