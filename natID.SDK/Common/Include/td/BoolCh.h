// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BoolCh.h
    @brief Defines the BoolCh class representing a boolean value stored as a character ('Y' or 'N'). */
#pragma once
#include <td/Types.h>

namespace td
{
	/// @brief Represents a boolean value as a two-character C-string ('Y' for true, 'N' for false), suitable for database storage.
	class BoolCh
	{
		char _bVal[2]; ///< Internal character storage: _bVal[0] is 'Y' or 'N', _bVal[1] is the null terminator.
    protected:
        /// @brief Stream insertion operator that writes the boolean character representation to an output stream.
        /// @tparam CharT Character type of the stream.
        /// @tparam Traits Traits type of the stream.
        /// @param os The output stream to write to.
        /// @param rhs The BoolCh value to write.
        /// @return Reference to the output stream after writing.
        template<class CharT, class Traits>
        friend std::basic_ostream<CharT, Traits>&
            operator << (std::basic_ostream<CharT, Traits>& os,  const BoolCh& rhs)
        {
            os << rhs._bVal;
            return os;
        }

        /// @brief Stream extraction operator that reads a character from an input stream and sets the boolean value.
        /// @tparam CharT Character type of the stream.
        /// @tparam Traits Traits type of the stream.
        /// @param is The input stream to read from.
        /// @param rhs The BoolCh object to populate; set to 'Y' if the character is 'Y' or 'y', otherwise 'N'.
        /// @return Reference to the input stream after reading.
        template<class CharT, class Traits>
        friend std::basic_istream<CharT, Traits>&
            operator>>(std::basic_istream<CharT, Traits>& is, BoolCh& rhs)
        {
            char ch;
            is >> ch;
            if (ch == 'Y' || ch == 'y')
                ch = 'Y';
            else
                ch = 'N';
            rhs._bVal[0] = ch;
            rhs._bVal[1] = 0;
            return is;
        }
	public:

		/// @brief Default constructor that initializes the value to false ('N').
		BoolCh()
			: _bVal{'N', 0}
		{
			//_bVal[0] = 'N';
			//_bVal[1] = 0;
		}

		/// @brief Constructs a BoolCh from a native bool value.
		/// @param val The boolean value to store; sets the character to 'Y' if true, 'N' if false.
		BoolCh(bool val)
			: _bVal{ 'N', 0 }
		{
			if (val)
				_bVal[0] = 'Y';
		}

		//could be implicit conversion instead ??
		//operator bool() const
		//{
		//	return (_bVal == 'Y');
		//}

		/// @brief Function-call operator that returns the stored boolean value.
		/// @return True if the stored character is 'Y', false otherwise.
		bool operator() () const
		{
			return (_bVal[0] == 'Y');
		}

		/// @brief Returns a C-string representation of the boolean value ("true" or "false").
		/// @return Pointer to a null-terminated string: "true" if the value is 'Y', "false" otherwise.
		const char* c_str() const
		{
			if (_bVal[0] == 'Y')
				return td::boolToStr(true);
			return td::boolToStr(false);
		}

		/// @brief Returns the internal character representation as a null-terminated C-string ("Y" or "N").
		/// @return Pointer to the internal _bVal array.
		const char* yesno_str() const
		{
			return &_bVal[0];
		}

		/// @brief Assigns a native bool value to this BoolCh.
		/// @param val The boolean value to assign; sets the character to 'Y' if true, 'N' if false.
		/// @return Reference to this BoolCh object.
		BoolCh& operator = (bool val)
		{
			if (val)
				_bVal[0] = 'Y';
			else
				_bVal[0] = 'N';
			return *this;
		}

		/// @brief Assigns another BoolCh value to this one.
		/// @param val The BoolCh value to copy.
		/// @return Reference to this BoolCh object.
		BoolCh& operator = (BoolCh val)
		{
			_bVal[0] = val._bVal[0];
			return *this;
		}

		/// @brief Compares this BoolCh with another BoolCh for equality.
		/// @param val The BoolCh value to compare with.
		/// @return True if both objects store the same character value.
        inline bool operator == (const BoolCh& val) const
        {
            return (_bVal[0] == val._bVal[0]);
        }

		/// @brief Assigns the value from a generic variable type by calling its getValue method.
		/// @tparam TVAR The type of the variable providing the value.
		/// @param var The variable whose getValue method is called to populate this BoolCh.
		/// @return Reference to this BoolCh object.
		template <class TVAR>
		inline BoolCh& operator = (const TVAR& var)
		{
			BoolCh& bch = *this;
			var.getValue(bch);
			return *this;
		}

		/// @brief Compares this BoolCh with a native bool value for equality.
		/// @param val The boolean value to compare with.
		/// @return True if the stored value matches the given boolean.
		bool operator == (bool val) const
		{
			if (val)
				return (_bVal[0] == 'Y');

			return (_bVal[0] == 'N');
		}

		/// @brief Compares this BoolCh with a native bool value for inequality.
		/// @param val The boolean value to compare with.
		/// @return True if the stored value does not match the given boolean.
		bool operator != (bool val) const
		{
			if (val)
				return (_bVal[0] != 'Y');

			return (_bVal[0] != 'N');
		}

		/// @brief Less-than comparison with a native bool value.
		/// @param val The boolean value to compare with.
		/// @return True if this value is false and val is true (i.e., 'N' < 'Y').
		bool operator < (bool val) const
		{
			if (val)
				return (_bVal[0] == 'N');

			return false;
		}

		/// @brief Less-than-or-equal comparison with a native bool value.
		/// @param val The boolean value to compare with.
		/// @return True if this value is false and val is true.
		bool operator <= (bool val) const
		{
			if (val)
				return (_bVal[0] == 'N');

			return false;
		}

		/// @brief Greater-than comparison with a native bool value.
		/// @param val The boolean value to compare with.
		/// @return True if this value is true and val is false (i.e., 'Y' > 'N').
		bool operator > (bool val) const
		{
			if (val)
				return false;

			return (_bVal[0] == 'Y');
		}

		/// @brief Greater-than-or-equal comparison with a native bool value.
		/// @param val The boolean value to compare with.
		/// @return True if this value is true and val is false.
		bool operator >= (bool val) const
		{
			if (val)
				return false;

			return (_bVal[0] == 'Y');
		}

		/// @brief Formats the boolean value into a caller-supplied character buffer as "Y\0" or "N\0".
		/// @param buffer Pointer to the output character buffer; must not be null.
		/// @param bufLen Length of the output buffer; must be at least 2.
		/// @return The number of bytes written (2 on success, 0 if bufLen is too small).
		int formc_str(char* buffer, int bufLen) const
		{
			assert(buffer);
			assert(bufLen >= 2);

			if (bufLen < 2)
				return 0;

			buffer[0] = 'N';
			buffer[1] = 0;

			if (_bVal[0] == 'Y')
				buffer[0] = 'Y';

			return 2;
		}

	};

	/// @brief Returns the DataType identifier for BoolCh values.
	/// @return The td::boolean DataType constant.
	inline DataType getType(td::BoolCh) { return boolean; }
}
