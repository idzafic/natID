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
#include <td/Conv.h>
//#include <td/ColorID.h>

namespace arch
{	
	class ArchiveIn;
}

namespace td
{

    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    class StringBase; // < T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>;

	class Color
	{
	public:		
		friend class arch::ArchiveIn;

	protected:
		td::UINT4 _color;
		
	public:		

		Color(td::UINT4 red, td::UINT4 green, td::UINT4 blue, td::UINT4 alpha = 0xFF)
		{
			setColor(red, green, blue, alpha);
		}		

		Color(const Color& c)
			: _color(c.getValue())
		{
		}

		Color()
			: _color(0)
		{			
		}

		Color(td::UINT4 colorValue)
			: _color(colorValue)
		{
		}

		template <typename TINT>
		void setColor(TINT red, TINT green, TINT blue, TINT alpha = 0xFF)
		{
			//_color = td::GDIARGB(red, green, blue, alpha);
            _color = td::GLRGBA(red, green, blue, alpha);
		}
        
        void setColorFromFloat(float red, float green, float blue, float alpha= 1)
        {
            _color =td::GLRGBA((td::BYTE) (red*255), (td::BYTE) (green*255), (td::BYTE) (blue*255), (td::BYTE) (alpha*255));
        }

		td::UINT4 getValue() const
		{
			return _color;
		}

		inline td::UINT4 red() const
		{
			return td::GetRColor(_color);
		}

		inline td::UINT4 green() const
		{
			return td::GetGColor(_color);
		}

		inline td::UINT4 blue() const
		{
			return td::GetBColor(_color);
		}

		inline td::UINT4 alpha() const
		{
			return td::GetAColor(_color);
		}

		inline td::BYTE R() const
		{
			return td::BYTE(td::GetRColor(_color));
		}

		inline td::BYTE G() const
		{
			return td::BYTE(td::GetGColor(_color));
		}

		inline td::BYTE B() const
		{
			return td::BYTE(td::GetBColor(_color));
		}

		inline td::BYTE A() const
		{
			return td::BYTE(td::GetAColor(_color));
		}
        
        inline void setAlpha(td::UINT4 alpha)
        {
            _color = td::GLRGBA(red(), green(), blue(), alpha);
        }
        
        int getDistance2(const td::Color& color) const
        {
            int dR = red() - color.red();
            int dG = green() - color.green();
            int dB = blue() - color.blue();
            int toRet = dR * dR + dG*dG +dB*dB;
            return toRet;
        }

		inline bool operator == (const td::Color& c) const
		{
			return _color == c._color;
		}

		inline bool operator != (const td::Color& c) const
		{
			return _color != c._color;
		}

		inline bool isZero() const
		{
			return (_color == 0);
		}


        template <typename TUTIL>
        const char* c_str(TUTIL& u) const
        {
            return u.c_str(*this);
        }
        
        int formc_str(char* buffer, int bufLen) const
        {
            assert(bufLen >= 10);
			if (bufLen < 1)
				return 0;
			if (bufLen < 10)
			{
				buffer[0] = 0;
				return 0;
			}

            buffer[0] = '#';
            td::toHex( (const char*) &_color, 4, &buffer[1]);
            buffer[9]=0;
            return 9;
        }
        
        template <size_t size>
        inline int formc_str(char(&buffer)[size]) const
        {
            static_assert(size >= 10, "Too small buffer size in Color::formc_str");
            return formc_str(&buffer[0], size);
        }
        
        inline void fromString(const char* str)
        {
            _color = 0;
            if (str == 0)
                return;
            if (*str == 0)
                return;
            if (*str != '#')
                return;
            auto strLen = strlen(str);
            if (strLen < 7)
                return;
            td::BYTE red = 0;
            td::hexToDec(&str[1], 2, red);
            td::BYTE green = 0;
            td::hexToDec(&str[3], 2, green);
            td::BYTE blue = 0;
            td::hexToDec(&str[5], 2, blue);
            td::BYTE alpha = 255;
            if (strLen >= 9)
                alpha = td::hexToDec(&str[7], 2, alpha);
            setColor(red, green, blue, alpha);
        }
        
        inline bool operator < (const td::Color& clr)
        {
            return _color < clr._color;
        }
        
        inline bool operator <= (const td::Color& clr)
        {
            return _color <= clr._color;
        }
        
        inline bool operator > (const td::Color& clr)
        {
            return _color > clr._color;
        }
        
        inline bool operator >= (const td::Color& clr)
        {
            return _color >= clr._color;
        }
        
        inline bool operator == (const td::Color& clr)
        {
            return _color == clr._color;
        }
        
        td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;
        
		//void fromString(const char* str)
		//{
		//	_color = 0;			
		//	if (str == 0)
		//		return;
		//	if (*str == 0)
		//		return;

		//	td::UINT4 red=0;
		//	td::UINT4 green=0;
		//	td::UINT4 blue=0;
		//	td::UINT4 alpha=0;
		//	sscanf(str, "#%02d%02d%02d%02d", &red, &green, &blue, &alpha);
		//	setColor(red, green, blue, alpha);
		//}

		//int toString(char* buffer, int bufLen) const
		//{
		//	int nLen = SNPRINTF(buffer, bufLen, "#%02d-%d-%d", getYear(), getMonth(), getDay());
		//	if (nLen < 0)
		//	{
		//		buffer[bufLen-1] = 0;
		//	}
		//	return nLen;
		//}
        protected:
            template<class CharT, class Traits>
            friend std::basic_ostream<CharT, Traits>&
                operator<<(std::basic_ostream<CharT, Traits>& os,  const Color& rhs)
            {
                char tmp[64];
                rhs.formc_str(tmp, 64);
                os  << tmp;
                return os;
            }
	};

	inline DataType getType(td::Color){	return color;}
}

	
