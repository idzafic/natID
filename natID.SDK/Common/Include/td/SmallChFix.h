// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SmallChFix.h
    @brief Defines a fixed-length short character buffer stored as a packed integer. */
#pragma once
#include <td/Types.h>
#include <cstring>

namespace td
{
	class Variant;

	/// @brief Fixed-length character buffer of at most 7 bytes stored in a 64-bit integer union.
	/// @tparam N Maximum number of characters stored (must be <= 7).
	template <size_t N>
	class SmallChFix
	{
		union
		{
			char _chVal[8]; ///< Character storage array; zero-terminated at index N.
			td::LUINT8 _lVal; ///< Unsigned 64-bit integer view of the character buffer for fast comparison.
		};

	public:

		/// @brief Default constructor; zero-initialises the character buffer.
		SmallChFix()
		{
			static_assert (N <= 7, "Wrong size of SmallChFix! It should be less than 8!!");
			_lVal = 0;
		}

		/// @brief Returns the character at a given position.
		/// @param pos Zero-based index into the buffer (must be < N).
		/// @return The character at the specified position.
		char getAt(size_t pos) const
		{
			assert(pos < N);
			return _chVal[pos];
		}

		/// @brief Returns the raw 64-bit integer representation of the buffer.
		/// @return The buffer contents as an unsigned 64-bit integer.
		td::LUINT8 lu8Val() const
		{
			return _lVal;
		}

		/// @brief Constructs from a null-terminated C-string; truncates to N characters.
		/// @param val Source C-string; characters beyond N are silently dropped.
		SmallChFix(const char* val)
		{
			operator = (val);
		}

		/// @brief Copy constructor.
		/// @param val The SmallChFix to copy from.
		SmallChFix(const SmallChFix& val)
		{
			operator = (val);
		}

		/// @brief Returns a pointer to the internal null-terminated character buffer.
		/// @return Pointer to the first character in the buffer.
		const char* c_str() const
		{
			return &_chVal[0];
		}


		/// @brief Returns true if the first character is the null terminator.
		/// @return true if the buffer contains an empty string.
		bool isZero() const
		{
			return _chVal[0] == 0;
		}

		/// @brief Assigns a single character; only valid for SmallChFix<1>.
		/// @param ch The character to assign.
		void operator = (const char ch)
		{
			static_assert (N == 1, "Char assignment is allowed for SmallChFix<1> only!!");
			_chVal[0] = ch;
		}

		/// @brief Assigns from a null-terminated C-string; truncates to N characters.
		/// @param val Source C-string.
		void operator = (const char* val)
		{
			size_t len = strlen(val);
			if (len == 0)
				_chVal[0] = 0;
			else
			{
				if (len > N)
					len = N; //trim
				memcpy(_chVal, val, len);
				_chVal[len] = 0;
			}
		}

		/// @brief Copy assignment operator.
		/// @param val The SmallChFix to copy from.
		void operator = (const SmallChFix& val)
		{
			//operator = (val.c_str());
			_lVal = val._lVal;
			_chVal[N] = 0;
		}

		/// @brief Assigns from a Variant value.
		/// @param var The Variant whose string value is assigned to this buffer.
		/// @return Reference to this SmallChFix.
		inline SmallChFix& operator = (const Variant& var);
		//{
		//	SmallChFix& chFix = *this;
		//	var.getValue(chFix);
		//	return *this;
		//}

		/// @brief Lexicographic equality comparison with a C-string.
		/// @param val The C-string to compare against.
		/// @return true if the contents are equal.
		bool operator == (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x == 0;
		}

		/// @brief Lexicographic less-than comparison with a C-string.
		/// @param val The C-string to compare against.
		/// @return true if this buffer is lexicographically less than val.
		bool operator < (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x < 0;
		}

		/// @brief Lexicographic less-than-or-equal comparison with a C-string.
		/// @param val The C-string to compare against.
		/// @return true if this buffer is lexicographically less than or equal to val.
		bool operator <= (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x <= 0;
		}

		/// @brief Lexicographic greater-than comparison with a C-string.
		/// @param val The C-string to compare against.
		/// @return true if this buffer is lexicographically greater than val.
		bool operator > (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x > 0;
		}

		/// @brief Lexicographic greater-than-or-equal comparison with a C-string.
		/// @param val The C-string to compare against.
		/// @return true if this buffer is lexicographically greater than or equal to val.
		bool operator >= (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x >= 0;
		}

		/// @brief Lexicographic equality comparison with another SmallChFix.
		/// @param val The SmallChFix to compare against.
		/// @return true if the contents are equal.
		bool operator == (const SmallChFix& val) const
		{
			return operator == (val.c_str());
		}

		/// @brief Lexicographic less-than comparison with another SmallChFix.
		/// @param val The SmallChFix to compare against.
		/// @return true if this buffer is lexicographically less than val.
		bool operator < (const SmallChFix& val) const
		{
			return operator < (val.c_str());
		}

		/// @brief Lexicographic less-than-or-equal comparison with another SmallChFix.
		/// @param val The SmallChFix to compare against.
		/// @return true if this buffer is lexicographically less than or equal to val.
		bool operator <= (const SmallChFix& val) const
		{
			return operator <= (val.c_str());
		}

		/// @brief Lexicographic greater-than comparison with another SmallChFix.
		/// @param val The SmallChFix to compare against.
		/// @return true if this buffer is lexicographically greater than val.
		bool operator > (const SmallChFix& val) const
		{
			return operator > (val.c_str());
		}

		/// @brief Lexicographic greater-than-or-equal comparison with another SmallChFix.
		/// @param val The SmallChFix to compare against.
		/// @return true if this buffer is lexicographically greater than or equal to val.
		bool operator >= (const SmallChFix& val) const
		{
			return operator >= (val.c_str());
		}


		/// @brief Returns the fixed maximum number of characters (compile-time constant).
		/// @return The template parameter N.
		constexpr size_t getFixLen() const
		{
			return N;
		}


		/// @brief Returns the current string length (number of non-null characters).
		/// @return Length in characters, not including the null terminator.
		size_t length() const
		{
			return strlen(c_str());
		}

		/// @brief Returns the buffer capacity including the null terminator.
		/// @return N + 1.
		constexpr size_t getBuffLen() const
		{
			return N+1;
		}

		/// @brief Writes exactly N characters to a binary output stream.
		/// @tparam TSTREAM Binary output stream type.
		/// @param s The stream to write to.
		template <typename TSTREAM>
		inline void saveToBinaryStream(TSTREAM& s) const
		{
			s.put(&_chVal[0], N);
		}

		/// @brief Reads exactly N characters from a binary input stream.
		/// @tparam TSTREAM Binary input stream type.
		/// @param s The stream to read from.
		/// @return Always returns true.
		template <typename TSTREAM>
		inline bool loadFromBinaryStream(TSTREAM& s)
		{
			s.get(&_chVal[0], N);
			_chVal[N] = 0;
			return true;
		}

		/// @brief Sets the buffer contents from a null-terminated C-string.
		/// @param str Source C-string; equivalent to operator=(str).
		void fromString(const char* str)
		{
			operator = (str);
		}


	protected:
		/// @brief Stream insertion operator; writes the buffer contents as a C-string.
		/// @tparam CharT Character type of the stream.
		/// @tparam Traits Traits type of the stream.
		/// @param os The output stream.
		/// @param rhs The SmallChFix to stream.
		/// @return The output stream.
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const SmallChFix& rhs)
		{
			os << rhs.c_str();
			return os;
		}
	};

	/// @brief Returns the DataType identifier for a SmallChFix of any size.
	/// @tparam NCH The fixed character count (template parameter of SmallChFix).
	/// @return td::ch7 data-type identifier.
	template <size_t NCH>
	DataType getType(SmallChFix<NCH>)
	{
		return td::ch7;
	}

	typedef SmallChFix<7> ChFix7; ///< Convenience alias for a 7-character fixed string.
}
