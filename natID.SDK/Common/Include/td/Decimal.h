// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Decimal.h
    @brief Defines the Decimal fixed-point numeric template and associated arithmetic operators. */
#pragma once
#include <td/Types.h>
#include <cassert>
#ifdef MU_CLANG
#pragma clang diagnostic ignored "-Wundefined-inline"
#endif

namespace td
{
	//class Variant;

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	class StringBase; // < T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>;

	/// @brief Fixed-point decimal number stored as an integer scaled by 10^NDEC.
	/// @tparam THOLDER Integer type used for internal storage (e.g. INT4 or LINT8).
	/// @tparam NDEC    Number of decimal places (0–4).
	template <typename THOLDER, int NDEC>
	class Decimal
	{
	protected:
		THOLDER _value; ///< Scaled integer value; actual value equals _value / 10^NDEC.

		/// @brief Rounds a floating-point value to the nearest scaled integer.
		/// @tparam T Floating-point type.
		/// @param val The value to round.
		/// @return The rounded scaled integer.
		template <typename T>
		inline THOLDER round(T val)
		{
			if (val > getMinVal())
				return (THOLDER) (val * getExp() + (T)0.5);
			else if (val < -getMinVal())
				return (THOLDER) (val * getExp() - (T)0.5);
			return 0;
		}

	public:
		/// @brief Constructs from a double value.
		/// @param val The floating-point value to store.
		Decimal(double val)
			: _value(round(val))
		{
		}
		/// @brief Constructs from a float value.
		/// @param val The floating-point value to store.
		Decimal(float val)
			: _value(round(val))
		{
		}

		/// @brief Constructs from a 32-bit integer (treated as a scaled value).
		/// @param val The raw scaled integer.
		Decimal(td::INT4 val)
			: _value(val)
		{
		}

		/// @brief Constructs from a 64-bit integer (treated as a scaled value).
		/// @param val The raw scaled integer.
		Decimal(td::LINT8 val)
			: _value(val)
		{
		}

        /// @brief Copy constructor.
        /// @param dec The Decimal to copy.
        Decimal( const Decimal& dec)
            : _value(dec.getValue())
        {
        }

		/// @brief Constructs from a Decimal with a different holder or decimal count.
		/// @tparam THOLDER2 Storage type of the source Decimal.
		/// @tparam NDEC2    Decimal places of the source Decimal.
		/// @param dec The source Decimal.
		template <typename THOLDER2, int NDEC2>
		Decimal(const Decimal<THOLDER2, NDEC2>& dec)
		{
			operator = (dec);
		}

		//Decimal(THOLDER intPart, THOLDER decPart)
		//{
		//	_value = intPart << getNumberOfDecimals();
		//	_value |= decPart;
		//}

		/// @brief Default constructor; leaves _value uninitialised.
		Decimal()
		{
		}

		/// @brief Returns the scale exponent (10^NDEC).
		/// @return 1, 10, 100, 1000, or 10000 for NDEC 0–4.
		inline THOLDER getExp() const
		{
			assert (NDEC >= 0 && NDEC <= 4);
			if (NDEC == 0)
				return 1;
			else if (NDEC == 2)
				return 100;
			else if (NDEC == 1)
				return 10;
			else if (NDEC == 3)
				return 1000;
			//else if (NDEC == 4)
			return 10000;
		}

		/// @brief Sets the value from separate integer and fractional parts.
		/// @param intPart  The integer part of the value.
		/// @param decPart  The fractional digits (will be scaled to NDEC places).
		/// @param negative true if the overall value is negative.
		/// @param nScale   Number of decimal digits in decPart (defaults to NDEC).
		void fromIntegers(THOLDER intPart, THOLDER decPart, bool negative, int nScale = NDEC)
		{
			//if (NDEC == 0)
				//decPart = 0;

			if (decPart != 0)
			{
				if (nScale < 0)
					nScale = 0;

				if (nScale < NDEC)
				{
					for (int i = nScale; i < NDEC; ++i)
						decPart *= 10;
				}
				else if (nScale > NDEC)
				{
					THOLDER divider = 1;
					for (int i = nScale; i > NDEC; --i)
						divider *= 10;
					double dDecPart = (double)decPart / divider;
					decPart /= divider;
					if (NDEC == 0)
					{
						if (dDecPart >= 0.5)
							++intPart;
						decPart = 0;
					}
					else if ((dDecPart - (int)dDecPart) >= 0.5)
						++decPart;
				}
			}

			_value = intPart;
			_value *= getExp();
			_value += decPart;

			if (negative)
				_value = (-_value);
		}
	protected:
		//for decimal
		/// @brief Scans integer and fractional parts from a C-string for LINT8 holder types.
		/// @param str     Null-terminated input string.
		/// @param intPart Output: integer part.
		/// @param decPart Output: fractional part digits.
		/// @param nScale  Output: number of fractional digits found.
		inline void scanParts(const char* str, td::LINT8& intPart, td::LINT8& decPart, int& nScale) const
		{
			nScale = td::findFromRight(str, '.');
			if (nScale > 0)
			{
				if (*str == '.')
					SSCANF(str, "." FMT_LINT8, &decPart);
				else
					SSCANF(str, FMT_LINT8 "." FMT_LINT8, &intPart, &decPart);
			}
			else
			{
				SSCANF(str, FMT_LINT8, &intPart);
			}

			//if (nScale > 0)
			//{
			//	if (*str == '.')
			//		SSCANF(str, ".%lld", &decPart);
			//	else
			//		SSCANF(str, "%lld.%lld", &intPart, &decPart);
			//}
			//else
			//{
			//	SSCANF(str, "%lld", &intPart);
			//}
		}

		//for smalldecimal
		/// @brief Scans integer and fractional parts from a C-string for INT4 holder types.
		/// @param str     Null-terminated input string.
		/// @param intPart Output: integer part.
		/// @param decPart Output: fractional part digits.
		/// @param nScale  Output: number of fractional digits found.
		inline void scanParts(const char* str, td::INT4& intPart, td::INT4& decPart, int& nScale) const
		{
			nScale = td::findFromRight(str, '.');
			if (nScale > 0)
			{
				if (*str == '.')
					SSCANF(str, ".%d", &decPart);
				else
					SSCANF(str, "%d.%d", &intPart, &decPart);
			}
			else
			{
				SSCANF(str, "%d", &intPart);
			}
		}
	public:
		/// @brief Parses the decimal value from a null-terminated C-string.
		/// @param str Null-terminated input string (e.g. "3.14").
		inline void fromString(const char* str)
		{
			THOLDER intPart=0;
			THOLDER decPart=0;

			if (!str)
			{
				fromIntegers(intPart, decPart, false);
				return;
			}
			if (*str== 0)
			{
				fromIntegers(intPart, decPart, false);
				return;
			}

			bool negative = false;
			if (*str == '-')
			{
				negative = true;
				++str;
			}

			int nScale = 0;
			scanParts(str, intPart, decPart, nScale);
			fromIntegers(intPart, decPart, negative, nScale);
		}

		/// @brief Sets the decimal value by parsing a C-string.
		/// @param str Null-terminated input string.
		void setValueFromString(const char* str)
		{
			fromString(str);
		}

		/// @brief Parses the decimal value from a StringBase.
		/// @tparam T_CHAR Character type.
		/// @tparam Encoding String encoding.
		/// @tparam SPACE_FOR_SIZE Internal size parameter.
		/// @tparam EXTERN_ALLOCATOR External allocator flag.
		/// @param str The source string.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		/// @brief Sets the decimal value from a StringBase.
		/// @tparam T_CHAR Character type.
		/// @tparam Encoding String encoding.
		/// @tparam SPACE_FOR_SIZE Internal size parameter.
		/// @tparam EXTERN_ALLOCATOR External allocator flag.
		/// @param str The source string.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

	protected:
		/// @brief Formats INT4-based decimal parts into a character buffer.
		/// @param buffer  Destination buffer.
		/// @param buffLen Buffer size in bytes.
		/// @param intPart Integer part of the value.
		/// @param decPart Fractional part of the value.
		/// @return Number of characters written.
		inline int formc_str(char* buffer, int buffLen, td::INT4 intPart, td::INT4 decPart) const
		{
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"

			int nLen = 0;
			if (_value < 0)
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-%d.%0*d", intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-%d", intPart);
			}
			else
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "%d.%0*d", intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "%d", intPart);
			}
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
			return nLen;
		}


		/// @brief Formats LINT8-based decimal parts into a character buffer.
		/// @param buffer  Destination buffer.
		/// @param buffLen Buffer size in bytes.
		/// @param intPart Integer part of the value.
		/// @param decPart Fractional part of the value.
		/// @return Number of characters written.
		inline int formc_str(char* buffer, int buffLen, td::LINT8 intPart, td::LINT8 decPart) const
		{
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"
			int nLen = 0;
			if (_value < 0)
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-" FMT_LINT8 ".%0*" FMT_LINT8_EXT, intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-" FMT_LINT8, intPart);
			}
			else
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, FMT_LINT8 ".%0*" FMT_LINT8_EXT, intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, FMT_LINT8, intPart);
			}
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
			return nLen;
		}

	public:

		/// @brief Formats the decimal into a caller-supplied character buffer.
		/// @param buffer  Destination buffer.
		/// @param buffLen Buffer size in bytes.
		/// @return Number of characters written.
		int formc_str(char* buffer, int buffLen) const
		{
			int nLen = formc_str(buffer, buffLen, getIntPart(), getDecPart());
			if (nLen < 0)
			{
				buffer[buffLen - 1] = 0; //just in case
				return buffLen -1;
			}
			return nLen;
		}

		/// @brief Formats the decimal into a fixed-size character array.
		/// @tparam size Array size (must be >= 20).
		/// @param buffer Destination array.
		/// @return Number of characters written.
		template <size_t size>
		inline int formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in Date::formc_str");
			return formc_str(&buffer[0], size);
		}

		/// @brief Converts the decimal to a td::String.
		/// @return String representation of the value.
		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		/// @brief Returns a C-string representation using a utility object.
		/// @tparam TUTIL Utility type providing a c_str(const Decimal&) method.
		/// @param u The utility object.
		/// @return Null-terminated formatted string.
		template <typename TUTIL>
		const char* c_str(TUTIL& u) const
		{
			return u.c_str(*this);
		}

		//const char* toString() const
		//{
		//	char buffer[24];
		//	...
		//}

		/// @brief Returns the minimum representable positive value for rounding purposes.
		/// @return A small float threshold depending on NDEC.
		inline float getMinVal() const
		{
			assert (NDEC >= 0 && NDEC <= 4);
			if (NDEC == 0)
				return 0.1f;
			else if (NDEC == 2)
				return 1e-3f;
			else if (NDEC == 1)
				return 1e-2f;
			else if (NDEC == 3)
				return 1e-4f;
			//else if (NDEC == 4)
			return 1e-5f;
		}

		/// @brief Returns the raw scaled integer value.
		/// @return The internal integer representation.
		inline THOLDER getValue() const
		{
			return _value;
		}

		/// @brief Sets the raw scaled integer value directly.
		/// @param val The new scaled integer value.
		inline void setValue(THOLDER val)
		{
			_value = val;
		}

		/// @brief Writes the actual floating-point value into an output parameter.
		/// @tparam T Target numeric type.
		/// @param val Output parameter receiving the computed value.
		template <typename T>
		inline THOLDER value(T& val) const
		{
			val = (T)_value / getExp();
		}

		/// @brief Returns the value as a double-precision floating-point number.
		/// @return The value as a double.
		double getAsFloat() const
		{
			THOLDER intPart = getIntPart();
			THOLDER decPart = getDecPart();
			if (_value < 0)
				return -((double) intPart + (double) decPart / getExp());
			return (double) intPart + (double) decPart / getExp();
			//return (double) _value / getExp();
		}

		/// @brief Returns the integer part (digits before the decimal point).
		/// @return Absolute integer part of the value.
		THOLDER getIntPart() const
		{
			if (_value < 0)
				return ((-_value) / getExp());

			return _value / getExp();
		}

		/// @brief Returns the fractional part (digits after the decimal point, scaled).
		/// @return Absolute fractional digits scaled to NDEC places.
		THOLDER getDecPart() const
		{
			if (_value < 0)
				return ((-_value) % getExp());

			return (_value % getExp());
		}

		/// @brief Returns the number of decimal places (NDEC).
		/// @return The template parameter NDEC.
		inline int getNumberOfDecimals() const
		{
			return NDEC;
		}

		/// @brief Equality comparison with a raw holder value.
		/// @param val The value to compare against.
		/// @return true if the internal value equals val.
		bool operator == (const THOLDER val) const
		{
			return (_value == val);
		}

		/// @brief Less-than comparison with a raw holder value.
		/// @param val The value to compare against.
		/// @return true if the internal value is less than val.
		bool operator < (const THOLDER val) const
		{
			return (_value < val);
		}

		/// @brief Greater-than comparison with a raw holder value.
		/// @param val The value to compare against.
		/// @return true if the internal value is greater than val.
		bool operator > (const THOLDER val) const
		{
			return (_value > val);
		}

		/// @brief Less-than-or-equal comparison with a raw holder value.
		/// @param val The value to compare against.
		/// @return true if the internal value is less than or equal to val.
		bool operator <= (const THOLDER val) const
		{
			return (_value <= val);
		}

		/// @brief Greater-than-or-equal comparison with a raw holder value.
		/// @param val The value to compare against.
		/// @return true if the internal value is greater than or equal to val.
		bool operator >= (const THOLDER val) const
		{
			return (_value >= val);
		}

		/// @brief Equality comparison with another Decimal of the same type.
		/// @param val The Decimal to compare against.
		/// @return true if both values are equal.
		bool operator == (const Decimal& val) const
		{
			return (_value == val.getValue());
		}

		/// @brief Inequality comparison with another Decimal.
		/// @param val The Decimal to compare against.
		/// @return true if the values differ.
		bool operator != (const Decimal& val) const
		{
			return (_value != val.getValue());
		}

		/// @brief Less-than-or-equal comparison with another Decimal.
		/// @param val The Decimal to compare against.
		/// @return true if this value is less than or equal to val.
		bool operator <= (const Decimal& val) const
		{
			return (_value <= val.getValue());
		}

		/// @brief Greater-than-or-equal comparison with another Decimal.
		/// @param val The Decimal to compare against.
		/// @return true if this value is greater than or equal to val.
		bool operator >= (const Decimal& val) const
		{
			return (_value >= val.getValue());
		}

		/// @brief Less-than comparison with another Decimal.
		/// @param val The Decimal to compare against.
		/// @return true if this value is strictly less than val.
		bool operator < (const Decimal& val) const
		{
			return (_value < val.getValue());
		}

		/// @brief Greater-than comparison with another Decimal.
		/// @param val The Decimal to compare against.
		/// @return true if this value is strictly greater than val.
		bool operator > (const Decimal& val) const
		{
			return (_value > val.getValue());
		}

		/// @brief Adds another Decimal to this one in-place.
		/// @param val The Decimal to add.
		void operator += (const Decimal& val)
		{
			_value += val.getValue();
		}

		/// @brief Adds a float value to this Decimal in-place.
		/// @param val The float value to add.
		void operator += (float val)
		{
			_value += round(val);
		}

		/// @brief Adds a 32-bit integer (whole units) to this Decimal in-place.
		/// @param val The integer value to add.
		void operator += (td::INT4 val)
		{
			_value += (THOLDER)(val * getExp());
		}

		/// @brief Adds a 64-bit integer (whole units) to this Decimal in-place.
		/// @param val The integer value to add.
		void operator += (td::LINT8 val)
		{
			_value += (THOLDER)(val * getExp());
		}

		/// @brief Adds a double value to this Decimal in-place.
		/// @param val The double value to add.
		void operator += (double val)
		{
			_value += round(val);
		}

		/// @brief Subtracts another Decimal from this one in-place.
		/// @param val The Decimal to subtract.
		void operator -= (const Decimal& val)
		{
			_value -= val.getValue();
		}

		/// @brief Subtracts a float value from this Decimal in-place.
		/// @param val The float value to subtract.
		void operator -= (float val)
		{
			_value -= round(val);
		}

		/// @brief Subtracts a double value from this Decimal in-place.
		/// @param val The double value to subtract.
		void operator -= (double val)
		{
			_value -= round(val);
		}

		/// @brief Assigns from a Decimal with a potentially different holder or precision.
		/// @tparam THOLDER2 Storage type of the source Decimal.
		/// @tparam NDEC2    Decimal places of the source Decimal.
		/// @param dec The source Decimal.
		/// @return Reference to this Decimal.
		template <typename THOLDER2, int NDEC2>
		Decimal& operator = (const Decimal<THOLDER2, NDEC2>& dec)
		{
			if (NDEC == NDEC2)
			{
				_value = (THOLDER)dec.getValue();
			}
			else
			{
				//TODO: Poboljsati
				double val = dec.getAsFloat();
				operator = (val);
			}
			return *this;
		}

		/// @brief Assigns from a Variant value.
		/// @tparam TVARIANT Variant type providing a getValue(Decimal&) member.
		/// @param var The source Variant.
		/// @return Reference to this Decimal.
		template <class TVARIANT>
		inline Decimal& operator = (const TVARIANT& var)
		{
			Decimal& dec = *this;
			var.getValue(dec);
			return *this;
		}

		/// @brief Multiplies this Decimal by a 32-bit integer in-place.
		/// @param val The integer multiplier.
		void operator *= (td::INT4 val)
		{
			_value *= val; //(THOLDER)(val * getExp());
		}

		/// @brief Multiplies this Decimal by an unsigned 64-bit integer in-place.
		/// @param val The integer multiplier.
		void operator *= (td::LUINT8 val)
		{
			_value *= val; //(THOLDER)(val * getExp());
		}

		/// @brief Multiplies this Decimal by another Decimal of potentially different precision.
		/// @tparam TINHOLDER Storage type of the other Decimal.
		/// @tparam NDECIN    Decimal places of the other Decimal.
		/// @param dec The multiplier Decimal.
		template<typename TINHOLDER, int NDECIN>
		void operator *= (const Decimal<TINHOLDER, NDECIN>& dec)
		{
			//TODO: Poboljsati
			double val = getAsFloat();
			double valIn = dec.getAsFloat();
			operator = (val * valIn);
		}

		/// @brief Divides this Decimal by another Decimal of potentially different precision.
		/// @tparam TINHOLDER Storage type of the divisor Decimal.
		/// @tparam NDECIN    Decimal places of the divisor Decimal.
		/// @param dec The divisor Decimal.
		template<typename TINHOLDER, int NDECIN>
		void operator /= (const Decimal<TINHOLDER, NDECIN>& dec)
		{
			//TODO: Poboljsati
			double val = getAsFloat();
			double valIn = dec.getAsFloat();
			operator = (val / valIn);
		}

		/// @brief Stores the product of two Decimals into this Decimal.
		/// @tparam THOLDER1 Storage type of the first operand.
		/// @tparam NDEC1    Decimal places of the first operand.
		/// @tparam THOLDER2 Storage type of the second operand.
		/// @tparam NDEC2    Decimal places of the second operand.
		/// @param dec1 First operand.
		/// @param dec2 Second operand.
		template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
		void mult(const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
		{
			double val1 = dec1.getAsFloat();
			double val2 = dec2.getAsFloat();
			operator = (val1 * val2);
		}

		/// @brief Stores the quotient of two Decimals into this Decimal.
		/// @tparam THOLDER1 Storage type of the numerator.
		/// @tparam NDEC1    Decimal places of the numerator.
		/// @tparam THOLDER2 Storage type of the denominator.
		/// @tparam NDEC2    Decimal places of the denominator.
		/// @param dec1 Numerator.
		/// @param dec2 Denominator.
		template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
		void div(const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
		{
			double val1 = dec1.getAsFloat();
			double val2 = dec2.getAsFloat();
			operator = (val1 / val2);
		}

		/// @brief Assigns from a float value.
		/// @param val The float value to assign.
		void operator = (float val)
		{
			if (val != 0)
				_value = round(val);
			else
				_value = 0;
		}

		/// @brief Assigns from a double value.
		/// @param val The double value to assign.
		void operator = (double val)
		{
			if (val != 0)
				_value = round(val);
			else
				_value = 0;
		}

		/// @brief Assigns from a 32-bit integer (whole units).
		/// @param val The integer value to assign.
		void operator = (td::INT4 val)
		{
			if (val != 0)
				_value = (THOLDER)(val * getExp());
			else
				_value = 0;
		}

		/// @brief Assigns from a 64-bit integer (whole units).
		/// @param val The integer value to assign.
		void operator = (td::LINT8 val)
		{
			if (val != 0)
				_value = (THOLDER)(val * getExp());
			else
				_value = 0;
		}

        /// @brief Deserializes the decimal value from a binary input archive.
        /// @tparam Archive Binary archive type.
        /// @param ar The archive to read from.
        /// @return Always returns true.
        template<class Archive>
        bool loadFromBinaryStream(Archive& ar)
        {
            ar >> _value;
            return true;
        }

        /// @brief Serializes the decimal value to a binary output archive.
        /// @tparam Archive Binary archive type.
        /// @param ar The archive to write to.
        template<class Archive>
        void saveToBinaryStream(Archive& ar) const
        {
            ar << _value;
        }

	protected:
		//template<class CharT, class Traits>
		//friend std::basic_ostream<CharT, Traits>&
#ifndef NATRPT_EXPORTS
		/// @brief Stream insertion operator; outputs the decimal as a formatted string.
		/// @tparam OSTREAM Output stream type.
		/// @param os  The output stream.
		/// @param rhs The Decimal to write.
		/// @return The output stream.
		template<class OSTREAM>
		friend OSTREAM& operator<<(OSTREAM& os,  const Decimal& rhs)
		{
			char tmp[32];
			rhs.formc_str(tmp, 32);
			os  << tmp;
			return os;
		}

        /// @brief Stream extraction operator; parses the decimal from a stream.
        /// @tparam ISTREAM Input stream type.
        /// @param is  The input stream.
        /// @param rhs The Decimal to populate.
        /// @return The input stream.
        template<class ISTREAM>
        friend ISTREAM& operator>>(ISTREAM& is, Decimal& rhs)
        {
            std::string str;
            is >> str;
            rhs.fromString(str.c_str());
            return is;
        }
#endif
	};

	/// @brief Equality operator for two Decimals of the same type.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if both raw values are equal.
	template<typename THOLDER1, int NDEC1>
	bool operator == (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() == dec2.getValue();
	}

	/// @brief Inequality operator for two Decimals.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if the raw values differ.
	template<typename THOLDER1, int NDEC1>
	bool operator != (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() != dec2.getValue();
	}

	/// @brief Greater-than operator for two Decimals.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if dec1 > dec2.
	template<typename THOLDER1, int NDEC1>
	bool operator > (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() > dec2.getValue();
	}

	/// @brief Greater-than-or-equal operator for two Decimals.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if dec1 >= dec2.
	template<typename THOLDER1, int NDEC1>
	bool operator >= (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() >= dec2.getValue();
	}

	/// @brief Less-than operator for two Decimals.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if dec1 < dec2.
	template<typename THOLDER1, int NDEC1>
	bool operator < (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() < dec2.getValue();
	}

	/// @brief Less-than-or-equal operator for two Decimals.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Left operand.
	/// @param dec2 Right operand.
	/// @return true if dec1 <= dec2.
	template<typename THOLDER1, int NDEC1>
	bool operator <= (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() <= dec2.getValue();
	}

	/// @brief Division operator for two Decimals of the same type; returns double.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Numerator.
	/// @param dec2 Denominator.
	/// @return Floating-point quotient.
	template<typename THOLDER1, int NDEC1>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / dec2.getAsFloat();
	}

	/// @brief Division operator for two Decimals of different types; returns double.
	/// @tparam THOLDER1 Holder type of the numerator.
	/// @tparam NDEC1    Decimal places of the numerator.
	/// @tparam THOLDER2 Holder type of the denominator.
	/// @tparam NDEC2    Decimal places of the denominator.
	/// @param dec1 Numerator.
	/// @param dec2 Denominator.
	/// @return Floating-point quotient.
	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / dec2.getAsFloat();
	}

	/// @brief Multiplication operator for two Decimals of the same type; returns double.
	/// @tparam THOLDER Holder type.
	/// @tparam NDEC    Decimal places.
	/// @param dec1 First factor.
	/// @param dec2 Second factor.
	/// @return Floating-point product.
	template<typename THOLDER, int NDEC>
	double operator * (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * dec2.getAsFloat();
	}

	/// @brief Multiplication operator for two Decimals of different types; returns double.
	/// @tparam THOLDER1 Holder type of the first factor.
	/// @tparam NDEC1    Decimal places of the first factor.
	/// @tparam THOLDER2 Holder type of the second factor.
	/// @tparam NDEC2    Decimal places of the second factor.
	/// @param dec1 First factor.
	/// @param dec2 Second factor.
	/// @return Floating-point product.
	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator * (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * dec2.getAsFloat();
	}

	/// @brief Addition operator for two Decimals of different types; returns double.
	/// @tparam THOLDER1 Holder type of the first operand.
	/// @tparam NDEC1    Decimal places of the first operand.
	/// @tparam THOLDER2 Holder type of the second operand.
	/// @tparam NDEC2    Decimal places of the second operand.
	/// @param dec1 First operand.
	/// @param dec2 Second operand.
	/// @return Floating-point sum.
	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator + (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() + dec2.getAsFloat();
	}

	/// @brief Subtraction operator for two Decimals of the same type; returns a Decimal.
	/// @tparam THOLDER Holder type.
	/// @tparam NDEC    Decimal places.
	/// @param dec1 Minuend.
	/// @param dec2 Subtrahend.
	/// @return Decimal result of dec1 - dec2.
	template<typename THOLDER, int NDEC>
	Decimal<THOLDER, NDEC> operator - (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{
		Decimal<THOLDER, NDEC> res(dec1);
		res -= dec2;
		return res;
	}

	/// @brief Subtraction operator for two Decimals of different types; returns double.
	/// @tparam THOLDER1 Holder type of the minuend.
	/// @tparam NDEC1    Decimal places of the minuend.
	/// @tparam THOLDER2 Holder type of the subtrahend.
	/// @tparam NDEC2    Decimal places of the subtrahend.
	/// @param dec1 Minuend.
	/// @param dec2 Subtrahend.
	/// @return Floating-point difference.
	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator - (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() - dec2.getAsFloat();
	}

	/// @brief Addition operator for two Decimals of the same type; returns a Decimal.
	/// @tparam THOLDER Holder type.
	/// @tparam NDEC    Decimal places.
	/// @param dec1 First addend.
	/// @param dec2 Second addend.
	/// @return Decimal sum.
	template<typename THOLDER, int NDEC>
	Decimal<THOLDER, NDEC> operator + (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{
		Decimal<THOLDER, NDEC> res(dec1);
		res += dec2;
		return res;
	}

	/// @brief Adds a Decimal and a scalar right-hand side; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TRHS     Right-hand side type.
	/// @param dec1 Decimal operand.
	/// @param rhs  Scalar operand.
	/// @return Decimal sum.
	template<typename THOLDER1, int NDEC1, typename TRHS>
	Decimal<THOLDER1, NDEC1> operator + (const Decimal<THOLDER1, NDEC1>& dec1, TRHS rhs)
	{
		//TODO: Poboljsati
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = rhs;
		dec2 += dec1;
		return dec2;
	}

	//template<typename THOLDER1, int NDEC1>
	//Decimal<THOLDER1, NDEC1> operator - (const Decimal<THOLDER1, NDEC1>& dec1, td::INT4 rhs)
	//{...}

	/// @brief Adds a scalar left-hand side and a Decimal; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TLHS     Left-hand side type.
	/// @param lhs  Scalar operand.
	/// @param dec1 Decimal operand.
	/// @return Decimal sum.
	template<typename THOLDER1, int NDEC1, typename TLHS>
	Decimal<THOLDER1, NDEC1> operator + (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = lhs;
		dec2 += dec1;
		return dec2;
	}

	/// @brief Subtracts a Decimal from a scalar left-hand side; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TLHS     Left-hand side type.
	/// @param lhs  Scalar minuend.
	/// @param dec1 Decimal subtrahend.
	/// @return Decimal difference.
	template<typename THOLDER1, int NDEC1, typename TLHS>
	Decimal<THOLDER1, NDEC1> operator - (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = lhs;
		dec2 -= dec1;
		return dec2;
	}


	/// @brief Subtracts a scalar from a Decimal; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TRHS     Right-hand side type.
	/// @param dec1 Decimal minuend.
	/// @param rhs  Scalar subtrahend.
	/// @return Decimal difference.
	template<typename THOLDER1, int NDEC1, typename TRHS>
	Decimal<THOLDER1, NDEC1> operator - (const Decimal<THOLDER1, NDEC1>& dec1,  TRHS rhs)
	{
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = rhs;
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 -= dec2;
		return dec3;
	}

	/// @brief Multiplies a Decimal by a 32-bit integer; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Decimal operand.
	/// @param rhs  Integer multiplier.
	/// @return Decimal product.
	template<typename THOLDER1, int NDEC1>
	Decimal<THOLDER1, NDEC1> operator * (const Decimal<THOLDER1, NDEC1>& dec1, td::INT4 rhs)
	{
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 *= rhs;
		return dec3;
	}

	/// @brief Multiplies a 32-bit integer by a Decimal; returns a Decimal.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param lhs  Integer multiplier.
	/// @param dec1 Decimal operand.
	/// @return Decimal product.
	template<typename THOLDER1, int NDEC1>
	Decimal<THOLDER1, NDEC1> operator * (td::INT4 lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 *= lhs;
		return dec3;
		//return dec1.getAsFloat() * lhs;
	}

	/// @brief Multiplies a double by a Decimal; returns a double.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param lhs  Double multiplier.
	/// @param dec1 Decimal operand.
	/// @return Double product.
	template<typename THOLDER1, int NDEC1>
	double operator * (double lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * lhs;
	}

	/// @brief Multiplies a Decimal by a double; returns a double.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @param dec1 Decimal operand.
	/// @param rhs  Double multiplier.
	/// @return Double product.
	template<typename THOLDER1, int NDEC1>
	double operator * (const Decimal<THOLDER1, NDEC1>& dec1, double rhs)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * rhs;
	}

	/// @brief Divides a Decimal by a scalar; returns a double.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TRHS     Right-hand side type.
	/// @param dec1 Decimal numerator.
	/// @param rhs  Scalar denominator.
	/// @return Double quotient.
	template<typename THOLDER1, int NDEC1, typename TRHS>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, TRHS rhs)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / rhs;
	}

	//template<typename THOLDER1, int NDEC1>
	//double operator / (const Decimal<THOLDER1, NDEC1>& dec1, double rhs)
	//{
	//	//TODO: Poboljsati
	//	return dec1.getAsFloat() / rhs;
	//}

	/// @brief Divides a scalar by a Decimal; returns a double.
	/// @tparam THOLDER1 Holder type.
	/// @tparam NDEC1    Decimal places.
	/// @tparam TLHS     Left-hand side type.
	/// @param lhs  Scalar numerator.
	/// @param dec1 Decimal denominator.
	/// @return Double quotient.
	template<typename THOLDER1, int NDEC1, typename TLHS>
	double operator / (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		//TODO: Poboljsati
		return lhs / dec1.getAsFloat();
	}


	//template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	//double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	//{...}

	//template<typename THOLDER1, int NDEC1>
	//double operator / (int lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	//{...}

	typedef Decimal<td::LINT8, 0> Decimal0; ///< 64-bit decimal with 0 decimal places.
	typedef Decimal<td::LINT8, 1> Decimal1; ///< 64-bit decimal with 1 decimal place.
	typedef Decimal<td::LINT8, 2> Decimal2; ///< 64-bit decimal with 2 decimal places.
	typedef Decimal<td::LINT8, 3> Decimal3; ///< 64-bit decimal with 3 decimal places.
	typedef Decimal<td::LINT8, 4> Decimal4; ///< 64-bit decimal with 4 decimal places.

	typedef Decimal<td::INT4, 0> SmallDecimal0; ///< 32-bit decimal with 0 decimal places.
	typedef Decimal<td::INT4, 1> SmallDecimal1; ///< 32-bit decimal with 1 decimal place.
	typedef Decimal<td::INT4, 2> SmallDecimal2; ///< 32-bit decimal with 2 decimal places.
	typedef Decimal<td::INT4, 3> SmallDecimal3; ///< 32-bit decimal with 3 decimal places.
	typedef Decimal<td::INT4, 4> SmallDecimal4; ///< 32-bit decimal with 4 decimal places.

	/// @brief Returns the DataType identifier for Decimal0.
	/// @return td::decimal0.
	inline DataType getType(const Decimal0&) {return decimal0;}
	/// @brief Returns the DataType identifier for Decimal1.
	/// @return td::decimal1.
	inline DataType getType(const Decimal1&) {return decimal1;}
	/// @brief Returns the DataType identifier for Decimal2.
	/// @return td::decimal2.
	inline DataType getType(const Decimal2&) {return decimal2;}
	/// @brief Returns the DataType identifier for Decimal3.
	/// @return td::decimal3.
	inline DataType getType(const Decimal3&) {return decimal3;}
	/// @brief Returns the DataType identifier for Decimal4.
	/// @return td::decimal4.
	inline DataType getType(const Decimal4&) {return decimal4;}

	/// @brief Returns the DataType identifier for SmallDecimal0.
	/// @return td::smallDecimal0.
	inline DataType getType(const SmallDecimal0&) {return smallDecimal0;}
	/// @brief Returns the DataType identifier for SmallDecimal1.
	/// @return td::smallDecimal1.
	inline DataType getType(const SmallDecimal1&) {return smallDecimal1;}
	/// @brief Returns the DataType identifier for SmallDecimal2.
	/// @return td::smallDecimal2.
	inline DataType getType(const SmallDecimal2&) {return smallDecimal2;}
	/// @brief Returns the DataType identifier for SmallDecimal3.
	/// @return td::smallDecimal3.
	inline DataType getType(const SmallDecimal3&) {return smallDecimal3;}
	/// @brief Returns the DataType identifier for SmallDecimal4.
	/// @return td::smallDecimal4.
	inline DataType getType(const SmallDecimal4&) {return smallDecimal4;}
};
