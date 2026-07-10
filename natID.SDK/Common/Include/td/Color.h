// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Color.h
    @brief Defines the Color class representing an RGBA color value with component access and string conversion utilities. */
#pragma once
#include <td/Types.h>
#include <td/Conv.h>

namespace arch
{
	class ArchiveIn;
}

namespace td
{

    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    class StringBase; // < T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>;

	/// @brief Represents an RGBA color value packed into a single 32-bit unsigned integer.
	class Color
	{
	public:
		friend class arch::ArchiveIn;

	protected:
		td::UINT4 _color; ///< The packed RGBA color value stored as a 32-bit unsigned integer.

	public:

		/// @brief Constructs a Color from individual red, green, blue, and alpha component values.
		/// @param red The red component value (0-255).
		/// @param green The green component value (0-255).
		/// @param blue The blue component value (0-255).
		/// @param alpha The alpha (opacity) component value (0-255); defaults to fully opaque (0xFF).
		Color(td::UINT4 red, td::UINT4 green, td::UINT4 blue, td::UINT4 alpha = 0xFF)
		{
			setColor(red, green, blue, alpha);
		}

		/// @brief Copy constructor that creates a Color from an existing Color object.
		/// @param c The Color to copy.
		Color(const Color& c)
			: _color(c.getValue())
		{
		}

		/// @brief Default constructor that initializes the color to black (all components zero).
		Color()
			: _color(0)
		{
		}

		/// @brief Constructs a Color from a pre-packed 32-bit RGBA value.
		/// @param colorValue The packed RGBA value to use directly.
		Color(td::UINT4 colorValue)
			: _color(colorValue)
		{
		}

		/// @brief Sets the color components from integer values in the range 0-255.
		/// @tparam TINT Integer type for component values.
		/// @param red The red component value.
		/// @param green The green component value.
		/// @param blue The blue component value.
		/// @param alpha The alpha component value; defaults to fully opaque (0xFF).
		template <typename TINT>
		void setColor(TINT red, TINT green, TINT blue, TINT alpha = 0xFF)
		{
			//_color = td::GDIARGB(red, green, blue, alpha);
            _color = td::GLRGBA(red, green, blue, alpha);
		}

		/// @brief Sets the color components from floating-point values in the range 0.0 to 1.0.
		/// @param red The red component as a float (0.0-1.0).
		/// @param green The green component as a float (0.0-1.0).
		/// @param blue The blue component as a float (0.0-1.0).
		/// @param alpha The alpha component as a float (0.0-1.0); defaults to fully opaque (1.0).
        void setColorFromFloat(float red, float green, float blue, float alpha= 1)
        {
            _color =td::GLRGBA((td::BYTE) (red*255), (td::BYTE) (green*255), (td::BYTE) (blue*255), (td::BYTE) (alpha*255));
        }

		/// @brief Returns the packed RGBA color value as a 32-bit unsigned integer.
		/// @return The packed color value.
		td::UINT4 getValue() const
		{
			return _color;
		}

		/// @brief Returns the red component of the color.
		/// @return The red component value in the range 0-255.
		inline td::UINT4 red() const
		{
			return td::GetRColor(_color);
		}

		/// @brief Returns the green component of the color.
		/// @return The green component value in the range 0-255.
		inline td::UINT4 green() const
		{
			return td::GetGColor(_color);
		}

		/// @brief Returns the blue component of the color.
		/// @return The blue component value in the range 0-255.
		inline td::UINT4 blue() const
		{
			return td::GetBColor(_color);
		}

		/// @brief Returns the alpha (opacity) component of the color.
		/// @return The alpha component value in the range 0-255.
		inline td::UINT4 alpha() const
		{
			return td::GetAColor(_color);
		}

		/// @brief Returns the red component as a single byte.
		/// @return The red component as a td::BYTE value.
		inline td::BYTE R() const
		{
			return td::BYTE(td::GetRColor(_color));
		}

		/// @brief Returns the green component as a single byte.
		/// @return The green component as a td::BYTE value.
		inline td::BYTE G() const
		{
			return td::BYTE(td::GetGColor(_color));
		}

		/// @brief Returns the blue component as a single byte.
		/// @return The blue component as a td::BYTE value.
		inline td::BYTE B() const
		{
			return td::BYTE(td::GetBColor(_color));
		}

		/// @brief Returns the alpha component as a single byte.
		/// @return The alpha component as a td::BYTE value.
		inline td::BYTE A() const
		{
			return td::BYTE(td::GetAColor(_color));
		}

		/// @brief Sets the alpha (opacity) component while keeping the RGB components unchanged.
		/// @param alpha The new alpha component value in the range 0-255.
        inline void setAlpha(td::UINT4 alpha)
        {
            _color = td::GLRGBA(red(), green(), blue(), alpha);
        }

		/// @brief Computes the squared Euclidean distance between this color and another in RGB space.
		/// @param color The color to measure the distance to.
		/// @return The squared distance in RGB color space (ignores alpha).
        int getDistance2(const td::Color& color) const
        {
            int dR = red() - color.red();
            int dG = green() - color.green();
            int dB = blue() - color.blue();
            int toRet = dR * dR + dG*dG +dB*dB;
            return toRet;
        }

		/// @brief Compares two colors for equality based on their packed value.
		/// @param c The color to compare with.
		/// @return True if both colors have the same packed RGBA value.
		inline bool operator == (const td::Color& c) const
		{
			return _color == c._color;
		}

		/// @brief Compares two colors for inequality based on their packed value.
		/// @param c The color to compare with.
		/// @return True if the colors have different packed RGBA values.
		inline bool operator != (const td::Color& c) const
		{
			return _color != c._color;
		}

		/// @brief Checks whether the color is zero (black with zero alpha).
		/// @return True if the packed color value is 0.
		inline bool isZero() const
		{
			return (_color == 0);
		}


		/// @brief Returns a C-string representation of the color using a utility object.
		/// @tparam TUTIL The utility type providing a c_str(Color) method.
		/// @param u The utility object used to format the color.
		/// @return A null-terminated C-string representing the color.
        template <typename TUTIL>
        const char* c_str(TUTIL& u) const
        {
            return u.c_str(*this);
        }

		/// @brief Formats the color as a hex string in the form "#RRGGBBAA" into a caller-supplied buffer.
		/// @param buffer The output character buffer; must be at least 10 bytes.
		/// @param bufLen The length of the output buffer.
		/// @return The number of characters written (9), or 0 if the buffer is too small.
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

		/// @brief Formats the color as a hex string into a fixed-size array buffer.
		/// @tparam size The size of the buffer array; must be at least 10.
		/// @param buffer The fixed-size output array to receive the formatted string.
		/// @return The number of characters written.
        template <size_t size>
        inline int formc_str(char(&buffer)[size]) const
        {
            static_assert(size >= 10, "Too small buffer size in Color::formc_str");
            return formc_str(&buffer[0], size);
        }

		/// @brief Parses a color from a hex C-string in the form "#RRGGBB" or "#RRGGBBAA".
		/// @param str A null-terminated string starting with '#' followed by 6 or 8 hex digits.
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

		/// @brief Less-than comparison based on the packed color value.
		/// @param clr The color to compare with.
		/// @return True if this color's packed value is less than the other's.
        inline bool operator < (const td::Color& clr) const
        {
            return _color < clr._color;
        }

		/// @brief Less-than-or-equal comparison based on the packed color value.
		/// @param clr The color to compare with.
		/// @return True if this color's packed value is less than or equal to the other's.
        inline bool operator <= (const td::Color& clr) const
        {
            return _color <= clr._color;
        }

		/// @brief Greater-than comparison based on the packed color value.
		/// @param clr The color to compare with.
		/// @return True if this color's packed value is greater than the other's.
        inline bool operator > (const td::Color& clr) const
        {
            return _color > clr._color;
        }

		/// @brief Greater-than-or-equal comparison based on the packed color value.
		/// @param clr The color to compare with.
		/// @return True if this color's packed value is greater than or equal to the other's.
        inline bool operator >= (const td::Color& clr) const
        {
            return _color >= clr._color;
        }

		/// @brief Returns the color as a UTF-8 string object.
		/// @return A td::StringBase object containing the hex representation of this color.
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
            /// @brief Stream insertion operator that writes the color as a hex string to an output stream.
            /// @tparam CharT Character type of the stream.
            /// @tparam Traits Traits type of the stream.
            /// @param os The output stream to write to.
            /// @param rhs The Color to write.
            /// @return Reference to the output stream after writing.
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

	/// @brief Returns the DataType identifier for Color values.
	/// @return The td::color DataType constant.
	inline DataType getType(td::Color){	return color;}
}


