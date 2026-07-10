// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VariantDesc.h
    @brief Variant extended with a human-readable description string and input/output position metadata. */
#pragma once
#include <td/Variant.h>
#include <td/SmallChFix.h>

namespace td
{
/// @brief A Variant that additionally carries a description string, input/output positions, and output type metadata.
///
/// Used in contexts such as parameter lists, script variables, and database column descriptors
/// where a Variant value must be annotated with a label and placement information.
class VariantDesc : public Variant
{
public:
	td::String desc;      ///< Human-readable description or label for this variant.
	td::INT2 posIn;       ///< Input position index (-1 if not set).
	td::INT2 posOut;      ///< Output position index (-1 if not set).
	td::DataType outType; ///< DataType of the output representation.
	int outLen;           ///< Maximum length of the output representation (0 if not applicable).

	/// @brief Default constructor; initializes all fields to their "not set" defaults.
	VariantDesc()
		: Variant()
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a specific data type and output length.
	/// @param dt DataType for both the stored value and the output type.
	/// @param outLen Maximum output length.
	VariantDesc(td::DataType dt, int outLen)
		: Variant(dt)
		, posIn(-1)
		, posOut(-1)
		, outType(dt)
		, outLen(outLen)
	{
	}

	/// @brief Construct from an existing Variant without a description.
	/// @param var Source Variant to copy the value from.
	VariantDesc(const Variant& var)
		: Variant(var)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct from an existing Variant with a C string description.
	/// @param pDesc Null-terminated description string.
	/// @param var Source Variant to copy the value from.
	VariantDesc(const char* pDesc, const Variant& var)
		: Variant(var)
		, desc(pDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a compile-time string literal description and no value.
	/// @tparam size Size of the string literal array (deduced automatically).
	/// @param strDesc Null-terminated string literal used as the description.
	template <size_t size>
	VariantDesc(const char (&strDesc)[size])
		:Variant()
		, desc(strDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a compile-time string literal description and an existing Variant value.
	/// @tparam size Size of the string literal array (deduced automatically).
	/// @param strDesc Null-terminated string literal used as the description.
	/// @param var Source Variant to copy the value from.
	template <size_t size>
	VariantDesc(const char (&strDesc)[size], const Variant& var)
		:Variant(var)
		, desc(strDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a C string pointer description and no value.
	/// @param pDesc Null-terminated description string.
	VariantDesc(const char* pDesc)
		: Variant()
		, desc(pDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a td::String description and an existing Variant value.
	/// @param strDesc Description string.
	/// @param var Source Variant to copy the value from.
	VariantDesc(const td::String& strDesc, const Variant& var)
		: Variant(var)
		, desc(strDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct with a td::String description and no value.
	/// @param strDesc Description string.
	VariantDesc(const td::String& strDesc)
		: Variant()
		, desc(strDesc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Copy constructor.
	/// @param var Source VariantDesc to copy from.
	VariantDesc(const VariantDesc& var)
		: Variant(var)
		, desc(var.desc)
		, posIn(-1)
		, posOut(-1)
		, outType(td::TD_NONE)
		, outLen(0)
	{
	}

	/// @brief Construct from a SmallChFix value with a fixed character-field output type.
	/// @tparam NCH Fixed character capacity of the SmallChFix.
	/// @param var SmallChFix value to store.
	template <size_t NCH>
            VariantDesc(const td::SmallChFix<NCH>& var)
                    : Variant(td::chFix)
		, posIn(-1)
		, posOut(-1)
		, outType(td::chFix)
		, outLen(NCH)
	{
		memcpy(&_chVal[0], var.c_str(), NCH);
		_chVal[NCH] = 0;
	}

	/// @brief Check whether both the input and output positions have been assigned.
	/// @return true if both posIn and posOut are non-negative.
	bool isPaired() const
	{
		return ( (posOut >=0) && (posIn >= 0) );
	}
	//template <class TSTREAM>
	//TSTREAM& toStream(TSTREAM& s)
	//{
	//	s << desc << ":";
	//	return Variant::toStream(s);
	//}

	/// @brief Serialize the descriptor and its value to an output stream.
	/// @tparam TSTREAM Stream type supporting the << operator.
	/// @param s Destination output stream.
	/// @return Reference to the output stream after writing.
	template <class TSTREAM>
	TSTREAM& toStream(TSTREAM& s) const
	{
		s << desc << "[" << posIn << ", " << posOut <<"][" << mu::Utils::c_str(getType()) << ", " <<mu::Utils::c_str(outType);
		if (outLen)
			s << "(" << outLen << ")";
		s << "]=";
		return Variant::toStream(s);
	}

	/// @brief Assign a SmallChFix value and update the output type and length metadata.
	/// @tparam NCH Fixed character capacity of the SmallChFix.
	/// @param val SmallChFix value to assign.
	/// @return Reference to this VariantDesc.
	template <size_t NCH>
	VariantDesc& operator = (SmallChFix<NCH> val)
	{
		Variant::operator=(val.c_str());
                outType = td::nchFix;
		outLen = (int) NCH;
		return *this;
	}

	/// @brief Generic assignment operator forwarding to the base Variant.
	/// @tparam TVAL Type of the value to assign.
	/// @param val Value to assign.
	/// @return Reference to this VariantDesc.
	template <typename TVAL>
	VariantDesc& operator = (TVAL val)
	{
		Variant::operator=(val);
		return *this;
	}

protected:
	template<class CharT, class Traits>
	friend std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& os,  const VariantDesc& rhs)
	{
		return rhs.toStream(os);
	}

};
}
