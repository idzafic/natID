// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file GUID.h
    @brief 128-bit globally unique identifier (GUID / UUID) type. */
#pragma once
#include <mu/mu.h>

namespace td
{
	/// @brief Represents a 128-bit globally unique identifier stored as a union of several interpretations.
	class GUID
	{
	public:
		union
		{
			char str[16]; ///< Raw 16-byte representation.
            struct
            {
                td::LUINT8 hu; ///< High 64 bits as unsigned integer.
                td::LUINT8 lu; ///< Low 64 bits as unsigned integer.
            };
			struct
			{
				td::LINT8 hi; ///< High 64 bits as signed integer.
				td::LINT8 lo; ///< Low 64 bits as signed integer.
			};
			struct
			{
				td::UINT4 i0; ///< First 32-bit word.
				td::UINT4 i1; ///< Second 32-bit word.
				td::UINT4 i2; ///< Third 32-bit word.
				td::UINT4 i3; ///< Fourth 32-bit word.
			};
		};
	public:
		/// @brief Default constructor. Does not initialise the value; call reset() to generate a new GUID.
		GUID()
		{
			//boost::uuids::uuid::const_iterator it(tmpUID.begin());
			//boost::uuids::uuid::const_iterator itEnd(tmpUID.end());
			//int i=0;
			//while (it != itEnd)
			//{
			//	str[i++] = *it;
			//	++it;
			//}
		}

		//void fromBinString(const char* pStr)
		//{
		//	memcpy(str, pStr, 16);
		//}

		//void toBinString(char* pStr) const
		//{
		//	memcpy (pStr, str, 16);
		//}

		/// @brief Copy constructor.
		/// @param uid The GUID to copy.
		GUID(const GUID& uid)
		{
			memcpy(str, uid.str, 16);
		}

		/// @brief Constructs a GUID and immediately generates a new random value.
		/// @param unused Ignored boolean parameter used only to distinguish this overload.
		GUID( bool)
		{
			reset();
		}

		/// @brief Constructs a GUID from explicit high and low 64-bit unsigned values.
		/// @param inpHi The high 64 bits.
		/// @param inpLo The low 64 bits.
		GUID(td::LUINT8 inpHi, td::LUINT8 inpLo)
			:hi(inpHi)
			,lo(inpLo)
		{
		}

		//td::LUINT8& hi()
		//{
		//	return hi;
		//}

		//td::LUINT8& lo()
		//{
		//	return lo;
		//}

		/// @brief Copy assignment operator.
		/// @param uid The GUID to assign.
		void operator = (const GUID& uid)
		{
			memcpy(str, uid.str, 16);
		}

		/// @brief Generates a new random GUID value and stores it in this object.
		void reset()
		{
			mu::getGUID(str);
			//boost::uuids::uuid tmpUID = boost::uuids::uuid(boost::uuids::random_generator()());
			//memcpy(str, (char*) tmpUID.data, 16);
		}

		/// @brief Equality comparison.
		/// @param uid The GUID to compare with.
		/// @return True if both GUIDs are identical.
		bool operator == (const GUID& uid) const
		{
			return ((hi == uid.hi) && (lo == uid.lo));
		}

		/// @brief Inequality comparison.
		/// @param uid The GUID to compare with.
		/// @return True if the GUIDs differ.
		bool operator != (const GUID& uid) const
		{
			return ((hi != uid.hi) || (lo != uid.lo));
		}

		/// @brief Less-than comparison (lexicographic on hi then lo).
		/// @param uid The GUID to compare with.
		/// @return True if this GUID is less than uid.
		bool operator < (const GUID& uid) const
		{
			return ((hi < uid.hi) || ((hi == uid.hi) && (lo < uid.lo)));
		}

		/// @brief Less-than-or-equal comparison.
		/// @param uid The GUID to compare with.
		/// @return True if this GUID is less than or equal to uid.
		bool operator <= (const GUID& uid) const
		{
			return ((hi <= uid.hi) || ((hi == uid.hi) && (lo <= uid.lo)));
		}

		/// @brief Greater-than-or-equal comparison.
		/// @param uid The GUID to compare with.
		/// @return True if this GUID is greater than or equal to uid.
		bool operator >= (const GUID& uid) const
		{
			return ((hi >= uid.hi) || ((hi == uid.hi) && (lo >= uid.lo)));
		}

		/// @brief Greater-than comparison.
		/// @param uid The GUID to compare with.
		/// @return True if this GUID is greater than uid.
		bool operator > (const GUID& uid) const
		{
			return ((hi > uid.hi) || ((hi == uid.hi) && (lo > uid.lo)));
		}

		/// @brief Outputs the GUID in "hi:lo" unsigned decimal form to a stream.
		/// @tparam TSTR Stream type with operator<<.
		/// @param s The destination stream.
		template <class TSTR>
		void show(TSTR& s) const
		{
			s << (td::LUINT8)hi << ":" << (td::LUINT8) lo;
			//s << i0 << "-" << i1 << "-" << i2 << "-" << i3 << "\n";
		}

		/// @brief Outputs the GUID in "hi:lo" signed decimal form to a stream.
		/// @tparam TSTR Stream type with operator<<.
		/// @param s The destination stream.
		template <class TSTR>
		void showSigned(TSTR& s) const
		{
			s << (td::LINT8)hi << ":" << (td::LINT8)lo;
			//s << i0 << "-" << i1 << "-" << i2 << "-" << i3 << "\n";
		}

	protected:
		//output to ostream
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const GUID& guid)
		{
			guid.show(os); //show in simplified form

			//os << std::hex << std::setfill('0')
			//	<< setw(2) << (int)guid.str[0]
			//	<< setw(2) << (int)guid.str[1]
			//	<< setw(2) << (int)guid.str[2]
			//	<< setw(2) << (int)guid.str[3]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[4]
			//	<< setw(2) << (int)guid.str[5]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[6]
			//	<< setw(2) << (int)guid.str[7]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[8]
			//	<< setw(2) << (int)guid.str[9]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[10]
			//	<< setw(2) << (int)guid.str[11]
			//	<< setw(2) << (int)guid.str[12]
			//	<< setw(2) << (int)guid.str[13]
			//	<< setw(2) << (int)guid.str[14]
			//	<< setw(2) << (int)guid.str[15];
			return os;
		}
	};

	/// @brief Returns the DataType tag for GUID.
	/// @return td::guid
	inline DataType getType(const GUID&) {return guid;}
}
