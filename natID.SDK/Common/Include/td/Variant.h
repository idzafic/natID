// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Variant.h
    @brief Type-safe discriminated union (Variant) supporting all td data types with operators, visitors, and stream serialization. */
#pragma once
#include <td/VariantBase.h>
#include <mu/Regionals.h>
#include <td/VariantVisitors.h>
#include <td/Conv.h>

class QVariant;
struct tagVARIANT;

#define TDVAR_TYPE_MASK   0x000000FF
#define TDVAR_DBTYPE_MASK 0x0000FF00
#define TDVAR_ISNULL      0x00010000
#define TDVAR_TOKENTYPE   0xFF000000

namespace td
{
/// @brief Type-safe discriminated union holding any td data type value.
///
/// Extends VariantBase with type identity flags, null-state tracking,
/// typed accessors, assignment/comparison operators, arithmetic operators,
/// locale-aware string conversion, and binary stream serialization.
class Variant : public VariantBase
{
    template <typename TVAR>
    friend struct PlusEqVisitor;

    template <typename TVAR>
    friend struct MinusEqVisitor;

    template <typename TVAR>
    friend struct MultEqVisitor;

    template <typename TVAR>
    friend struct DivEqVisitor;

protected:
    
    //typedef enum _tDataType {boolean, byte, word, sint2, uint4, int4, lint8, luint8, real4, real8, string8, enumerator, none} DataType;
    td::LUINT8 _flags; ///< Packed flags: bits 0–7 = DataType, bits 8–15 = DB DataType, bit 16 = null flag, bits 24–31 = token type, bits 32–63 = DB length.
public:
    /// @brief Return the primary DataType of the stored value.
    /// @return DataType enum value identifying the current value type.
    inline DataType getType() const
    {
        return (td::DataType) (_flags & TDVAR_TYPE_MASK);
    }

    /// @brief Return the database DataType (underlying DB column type) for string-like values.
    /// @return DataType of the associated database representation.
    inline DataType getDBType() const
    {
        //if (getType() == td::boolean)
        //    return td::chFix;
        return (td::DataType) ((_flags >> 8) & TDVAR_TYPE_MASK);
    }

    /// @brief Set the token type bits in the flags field.
    /// @param type Token type value (0–255) to store in bits 24–31 of _flags.
    inline void setTokenType(int type)
    {
        _flags &= 0xFFFFFFFF00FFFFFFUL;
        td::LUINT8 mask = type & 0x000000FF;
        mask <<= 24;
        _flags |= mask;
    }

    /// @brief Retrieve the token type stored in bits 24–31 of _flags.
    /// @return Token type value (0–255).
    inline int getTokenType() const
    {
        return (int) ((_flags >> 24) & 0x000000FF);
    }

    /// @brief Store the database column length in bits 32–63 of _flags.
    /// @param dbLen Maximum database column length to store.
    inline void setDBLen(td::UINT4 dbLen)
    {
        _flags &= 0x00000000FFFFFFFFUL;
        td::LUINT8 len = (td::LUINT8) dbLen;
        len <<= 32;
        _flags |= len;
    }

    /// @brief Retrieve the database column length from bits 32–63 of _flags.
    /// @return Database column length.
    inline td::UINT4 getDBLen() const
    {
        //if (getType() == td::boolean)
        //    return 1;

        td::LUINT8 len = _flags >> 32;
        td::UINT4 lenToRet = (td::UINT4)len;
        return lenToRet;
    }

    /// @brief Check whether the stored value is a numeric type.
    /// @return true if the current DataType is numeric.
    inline bool isNumeric() const
    {
        td::DataType dt = getType();
        return td::isNumeric(dt);
    }

    /// @brief Check whether the stored value is an integer type.
    /// @return true if the current DataType is an integer.
    inline bool isInteger() const
    {
        td::DataType dt = getType();
        return td::isInteger(dt);
    }

    /// @brief Check whether the stored value is a floating-point type.
    /// @return true if the current DataType is real4 or real8.
    inline bool isFloat() const
    {
        td::DataType dt = getType();
        return td::isFloat(dt);
    }

    /// @brief Check whether the stored value is a date or time type.
    /// @return true if the current DataType is date, time, or dateTime.
    inline bool isDateTime() const
    {
        td::DataType dt = getType();
        return td::isDateTime(dt);
    }

    /// @brief Check whether the stored value is a database string type (ch, nch, chFix, nchFix).
    /// @return true if the current DataType is a database string.
    inline bool isDBString() const
    {
        td::DataType dt = getType();
        return td::isDBString(dt);
    }

    /// @brief Parse a locale-specific string and assign the result to the stored value.
    /// @param pStr Null-terminated input string to parse.
    /// @param decSep Decimal separator character used in the string.
    /// @param thousandSep Thousand separator character to skip during parsing.
    inline void fromString(const char* pStr, const char decSep, const char thousandSep = ',')
    {
        if (!pStr)
        {
            toZero();
            return;
        }
        if (isNumeric())
        {
            char tmp[32];
            size_t iPos = 0;
            bool started = false;
            for (size_t i = 0; i < 31; ++i)
            {
                char ch = pStr[i];
                if (ch == 0)
                    break;
                else if (ch == '-' || ch == '+')
                {
                    if (started)
                        break;
                    else
                    {
                        tmp[iPos++] = ch;
                        started = true;
                        continue;
                    }
                }
                else if (ch == decSep)
                    ch = '.';
                else if (ch == thousandSep)
                    continue;
                else if (!td::isNumeric(ch))
                {
                    break;
                }
                tmp[iPos++] = ch;
                started = true;
            }
            tmp[iPos] = 0;
            setValueFromString(tmp);
            return;
        }
        setValueFromString(pStr);
    }
    
protected:

    //inline void setInitialType(td::DataType type)
    //{
    //    _flags = type;
    //}

    /// @brief Set the DB type for a SmallChFix value, mapping ch/chFix to ch7 internal storage.
    /// @param dtp Database DataType to encode (ch or chFix map to ch7; others stored directly).
    inline void setSmallChFixDBTYpe(td::DataType dtp)
    {
        if ( (dtp == td::ch) || (dtp == td::chFix))
        {
            _flags = (td::LUINT8) td::ch7;
            td::LUINT8 dbType = (td::LUINT8) dtp;
            dbType <<= 8;
            _flags |= dbType;
        }
        else
            _flags = dtp;
    }
            
    /// @brief Set the initial type flags when first constructing or reinitializing the Variant.
    /// @param dtp DataType to apply; string-like types (ch, nch, chFix, nchFix) are stored as string8 with the DB type encoded.
    inline void setInitialType(td::DataType dtp)
    {
        //if (dtp == td::ch7)
        //{
        //    _flags = dtp;
        //    setDBLen(7);
        //}
        //else
        if ((dtp == td::ch) || ((dtp == td::nch) || (dtp == td::chFix) || (dtp == td::nchFix)))
        {
            _flags = (td::LUINT8) td::string8;
            td::LUINT8 dbType = (td::LUINT8) dtp;
            dbType <<= 8;
            _flags |= dbType;
        }
        else
            _flags = dtp;
    }
    
    /// @brief Update the type field in _flags without touching DB-type or other flag bits.
    /// @param dtp New DataType to encode; string-like types update both primary and DB-type fields.
    inline void setType(td::DataType dtp)
    {
        _flags &= ~TDVAR_TYPE_MASK;
        //if (dtp == td::ch7)
        //{
        //    _flags |= (td::LUINT8)dtp;
        //    setDBLen(7);
        //}
        //else
        if ((dtp == td::ch) || ((dtp == td::nch) || (dtp == td::chFix) || (dtp == td::nchFix)))
        {
            _flags &= ~TDVAR_DBTYPE_MASK;
            _flags |= (td::LUINT8) td::string8;
            td::LUINT8 dbType = (td::LUINT8) dtp;
            dbType <<= 8;
            _flags |= dbType;
        }
        else
            _flags |= (td::LUINT8)dtp;
    }

    /// @brief Release any heap-allocated string buffer and reset the internal pointer to null.
    inline void clean()
    {
        if (getType() == td::string8)
        {
            td::String& str = reinterpret_cast<td::String&>(_strVal);
            if (str.relRef())
            {
                delete [] _strVal;
            }
        }
        _strVal = nullptr;
    }

    /// @brief Adopt a String value by sharing its reference-counted buffer.
    /// @param val Source String whose buffer will be shared (reference count is incremented).
    inline void takeFromString(const td::String& val)
    {
        clean();
        _strVal = reinterpret_cast<char*>(val.buffer);

        td::String& str = reinterpret_cast<td::String&>(_strVal);
        if (!str.addRef())
        {
            td::String strTmp = val.clone();
            td::UINT4  nRefsTmp = strTmp.getNoOfRefs();
//            strTmp.clone(val);
//            str.addRef();
            _strVal = strTmp.buffer;
            td::String& str2 = reinterpret_cast<td::String&>(_strVal);
            str2.addRef();
            td::UINT4  nRefsFinal = str2.getNoOfRefs();
            assert(nRefsFinal == 1);
        }
        setType(td::string8);
    }

public:
    /// @brief Default constructor; creates an empty Variant with type TD_NONE.
    Variant(void)
        : VariantBase()
        , _flags(td::TD_NONE)
    {
    }

    /// @brief Copy constructor.
    /// @param var Source Variant to copy.
    Variant(const Variant& var)
        : VariantBase()
        , _flags(var._flags)
    {
        
        if (getType() == td::string8)
        {
            strVal() = var.strVal();
        }
        else
        {
            _lu8Val = var._lu8Val;
        }
    }

    /// @brief Construct a Variant of the given data type with an uninitialized value.
    /// @param varType DataType to assign.
    Variant(td::DataType varType)
        : VariantBase()
    {
        setInitialType(varType);
    }

    /// @brief Construct a Variant of the given type, copying the raw storage from a VariantBase.
    /// @param varType DataType to assign.
    /// @param vb Source VariantBase whose raw storage is copied.
    Variant(td::DataType varType, const td::VariantBase& vb)
        : VariantBase(vb)
    {
        setInitialType(varType);
    }

    /// @brief Construct a Variant from a VariantBase, optionally adding a string reference.
    /// @param varType DataType to assign.
    /// @param vb Source VariantBase whose raw storage is copied.
    /// @param addRef If true and the type is string8, increments the string reference count.
    Variant(td::DataType varType, const td::VariantBase& vb, bool addRef)
        : VariantBase(vb)
    {
        setInitialType(varType);
        if (addRef)
        {
            if (varType == td::string8)
            {
                td::String& str = reinterpret_cast<td::String&>(_strVal);
                str.addRef();
            }
        }
    }
    /// @brief Construct a Variant with explicit C++ type, database type, and database column length.
    /// @param cppType Internal C++ DataType (e.g. ch7 for small fixed strings).
    /// @param dbType Database column DataType (e.g. chFix, ch).
    /// @param dbLen Maximum database column length in characters.
    Variant(td::DataType cppType, td::DataType dbType, td::WORD dbLen)
        : VariantBase()
    {
        if (cppType == td::ch7)
        {
            assert(dbType == td::chFix || dbType == td::ch);
            assert(dbLen > 0 && dbLen <= 7);

            if (dbLen > 7)
                dbLen = 7;
        }

        setInitialType(cppType);
        setDBTypeAndLen(dbType, dbLen);
    }

    /// @brief Construct a Variant of the given type, optionally marking it as null.
    /// @param varType DataType to assign.
    /// @param isNullIn If true, the variant is immediately marked as null.
    Variant(td::DataType varType, bool isNullIn)
        : VariantBase()
    {
        setInitialType(varType);

        if (isNullIn)
            setAsNull();
    }

    /// @brief Construct a boolean Variant.
    /// @param val Initial boolean value.
    Variant(bool val)
        : VariantBase(val)
        , _flags(td::boolean)
    {
        //_boolVal = val;
    }
    
    /// @brief Construct a void-pointer Variant.
    /// @param pPtr Raw pointer to store.
    Variant(void* pPtr)
        : VariantBase(pPtr)
        , _flags(td::ptrVoid)
    {
        //_boolVal = val;
    }

    /// @brief Construct a byte (BYTE) Variant.
    /// @param val Initial BYTE value.
    Variant(td::BYTE val)
        : VariantBase(val)
        , _flags(td::byte)
    {
        //_byteVal = val;
    }

    /// @brief Construct an unsigned 16-bit (WORD) Variant.
    /// @param val Initial WORD value.
    Variant(td::WORD val)
        : VariantBase(val)
        , _flags(td::word)
    {
        //_wordVal = val;
    }

    /// @brief Construct a signed 16-bit (INT2) Variant.
    /// @param val Initial INT2 value.
    Variant(td::INT2 val)
        : VariantBase(val)
        , _flags(td::int2)
    {
        //_i2Val = val;
    }

    /// @brief Construct a signed 32-bit (INT4) Variant.
    /// @param val Initial INT4 value.
    Variant(td::INT4 val)
        : VariantBase(val)
        , _flags(td::int4)
    {
        //_i4Val = val;
    }

    /// @brief Construct an unsigned 32-bit (UINT4) Variant.
    /// @param val Initial UINT4 value.
    Variant(td::UINT4 val)
        : VariantBase(val) //: _u4Val(val)
        , _flags(td::uint4)
    {
        //_u4Val = val;
    }

    /// @brief Construct an unsigned 64-bit (LUINT8) Variant.
    /// @param val Initial LUINT8 value.
    Variant(td::LUINT8 val)
        : VariantBase(val)
        , _flags(td::luint8)
    {
    }

    /// @brief Construct a signed 64-bit (LINT8) Variant.
    /// @param val Initial LINT8 value.
    Variant(td::LINT8 val)
        : VariantBase(val)
        , _flags(td::lint8)
    {
    }

    /// @brief Construct a single-precision float (real4) Variant.
    /// @param val Initial float value.
    Variant(float val)
        : VariantBase(val) //: _r4Val(val)
        , _flags(td::real4)
    {
        _r4Val = val;
    }

    /// @brief Construct a double-precision float (real8) Variant.
    /// @param val Initial double value.
    Variant(double val)
        : VariantBase(val)
        , _flags(td::real8)
    {
    }

    /// @brief Construct a date Variant.
    /// @param val Initial Date value.
    Variant(td::Date val)
        : VariantBase(val)  //: _dateVal(val.getValue())
        , _flags(td::date)
    {
    }

    /// @brief Construct a time-of-day Variant.
    /// @param val Initial Time value.
    Variant(td::Time val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::time)
    {
    }
    
    /// @brief Construct a Color Variant.
    /// @param val Initial Color value.
    Variant(td::Color val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::color)
    {
    }
    
    /// @brief Construct a ColorID Variant.
    /// @param val Initial ColorID value.
    Variant(td::ColorID val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::colorID)
    {
    }
    
    /// @brief Construct a LinePattern Variant.
    /// @param val Initial LinePattern value.
    Variant(td::LinePattern val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::linePattern)
    {
    }

    /// @brief Construct a date-and-time (DateTime) Variant.
    /// @param val Initial DateTime value.
    Variant(const td::DateTime& val)
        : VariantBase(val)
        , _flags(td::dateTime)
    {
    }

    
    /// @brief Construct a string (string8) Variant, sharing the source String buffer.
    /// @param val Source String to share.
    Variant(const td::String& val)
        : VariantBase(val) //: _strVal(nullptr)
        , _flags(td::string8)
    {
        //strVal() = val;
    }

    /// @brief Construct a database-string Variant with explicit DB type and column length.
    /// @param val Source String to share.
    /// @param dbType Database column DataType (ch, nch, chFix, or nchFix).
    /// @param dbLen Maximum database column length in characters.
    Variant(const td::String& val, td::DataType dbType, td::WORD dbLen)
        : VariantBase(val) //: _strVal(nullptr)
        , _flags(td::string8)
    {
        //strVal() = val;
        assert(dbType == td::chFix || dbType == td::ch || dbType == td::nch || dbType == td::nchFix);
        assert(dbLen > 0);
        setDBTypeAndLen(dbType, dbLen);
    }
    
    /// @brief Construct a Decimal (LINT8-backed) Variant.
    /// @tparam NDEC Number of decimal places.
    /// @param val Initial Decimal value.
    template<int NDEC >
    Variant(const Decimal<td::LINT8, NDEC>& val)
        : VariantBase(val)
        , _flags(td::getType(val))
    {
    }

    /// @brief Construct a SmallDecimal (INT4-backed) Variant.
    /// @tparam NDEC Number of decimal places.
    /// @param val Initial SmallDecimal value.
    template<int NDEC >
    Variant(const Decimal<td::INT4, NDEC>& val)
        : VariantBase(val)  //: _decVal(val.getValue())
        , _flags(td::getType(val))
    {
        //_sdecVal(val.getValue());
    }

    /// @brief Construct a SmallChFix Variant with an optional database type.
    /// @tparam NCH Fixed character capacity of the SmallChFix.
    /// @param val Initial SmallChFix value.
    /// @param dbType Database column type (chFix or ch); defaults to chFix.
    template<size_t NCH >
    Variant(const td::SmallChFix<NCH> val, td::DataType dbType=td::chFix)
        : VariantBase(val)  //: _decVal(val.getValue())
        , _flags(td::ch7)
    {
        assert(dbType == td::chFix || dbType == td::ch);
        setSmallChFixDBTYpe(dbType);
        setDBLen(NCH);
        //_sdecVal(val.getValue());
    }

    /// @brief Construct a string Variant from a compile-time string literal.
    /// @tparam size Size of the character array (deduced automatically).
    /// @param message Null-terminated string literal.
    template <size_t size>
    inline Variant(const char(&message)[size])
        : VariantBase()
        , _flags(td::string8)
    {
        if (size > 0)
        {
            strVal().fromKnownString(&message[0], size);
        }
    }
    
    /// @brief Construct a string Variant from a null-terminated C string pointer.
    /// @param pStr Null-terminated string to copy into the Variant.
    inline Variant(const char* pStr)
        : VariantBase()
        , _flags(td::string8)
    {
        strVal() = pStr;
    }

    /// @brief Destructor; releases any heap-allocated string buffer.
    ~Variant()
    {
        clean();
    }

    /// @brief Check whether this Variant has been marked as null.
    /// @return true if the null flag is set.
    bool isNull() const
    {
        return ( (_flags & TDVAR_ISNULL) != 0);
    }

    /// @brief Check whether this Variant holds a non-zero, non-null value.
    /// @return true if the value is neither zero nor null.
    bool isNonZero() const
    {
        if (isZero())
            return false;
        if (isNull())
            return false;
        return true;
    }

    /// @brief Mark this Variant as null by setting the null flag bit.
    void setAsNull()
    {
        _flags |= TDVAR_ISNULL;
    }

    
//    bool& boolVal()
//    {
//        assert(getType() == td::boolean);
//        return _boolVal;
//    }

    /// @brief Access the stored boolean value.
    /// @return true if the internal character is 'Y', false otherwise.
    bool boolVal() const
    {
        assert(getType() == td::boolean);
        return (_bVal == 'Y');
    }

    /// @brief Access the stored BYTE value by reference.
    /// @return Mutable reference to the BYTE storage.
    BYTE& byteVal()
    {
        assert(getType() == td::byte);
        return _byteVal;
    }

    /// @brief Read the stored BYTE value.
    /// @return Copy of the BYTE storage.
    BYTE byteVal() const
    {
        assert(getType() == td::byte);
        return _byteVal;
    }

    /// @brief Access the stored WORD value by reference.
    /// @return Mutable reference to the WORD storage.
    WORD& wordVal()
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    /// @brief Read the stored WORD value.
    /// @return Copy of the WORD storage.
    WORD wordVal() const
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    /// @brief Access the stored unsigned 16-bit value by reference (alias for wordVal).
    /// @return Mutable reference to the WORD storage.
    WORD& u2Val()
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    /// @brief Read the stored unsigned 16-bit value (alias for wordVal).
    /// @return Copy of the WORD storage.
    WORD u2Val() const
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    /// @brief Access the stored INT2 value by reference.
    /// @return Mutable reference to the INT2 storage.
    INT2& i2Val()
    {
        assert(getType() == td::int2);
        return _i2Val;
    }

    /// @brief Read the stored INT2 value.
    /// @return Copy of the INT2 storage.
    INT2 i2Val() const
    {
        assert(getType() == td::int2);
        return _i2Val;
    }

    /// @brief Access the stored UINT4 value by reference.
    /// @return Mutable reference to the UINT4 storage.
    UINT4& u4Val()
    {
        assert(getType() == td::uint4);
        return _u4Val;
    }

    /// @brief Read the stored UINT4 value.
    /// @return Copy of the UINT4 storage.
    UINT4 u4Val() const
    {
        assert(getType() == td::uint4);
        return _u4Val;
    }

    /// @brief Access the stored INT4 value by reference.
    /// @return Mutable reference to the INT4 storage.
    INT4& i4Val()
    {
        assert(getType() == td::int4);
        return _i4Val;
    }

    /// @brief Read the stored INT4 value.
    /// @return Copy of the INT4 storage.
    INT4 i4Val() const
    {
        assert(getType() == td::int4);
        return _i4Val;
    }

    /// @brief Access the stored LUINT8 value by reference.
    /// @return Mutable reference to the LUINT8 storage.
    LUINT8& lu8Val()
    {
        assert(getType() == td::luint8);
        return _lu8Val;
    }

    /// @brief Read the stored LUINT8 value.
    /// @return Copy of the LUINT8 storage.
    LUINT8 lu8Val() const
    {
        assert(getType() == td::luint8);
        return _lu8Val;
    }

    /// @brief Access the stored LINT8 value by reference.
    /// @return Mutable reference to the LINT8 storage.
    LINT8& li8Val()
    {
        assert(getType() == td::lint8);
        return _li8Val;
    }

    /// @brief Read the stored LINT8 value.
    /// @return Copy of the LINT8 storage.
    LINT8 li8Val() const
    {
        assert(getType() == td::lint8);
        return _li8Val;
    }

    /// @brief Access the stored single-precision float by reference.
    /// @return Mutable reference to the float storage.
    float& r4Val()
    {
        assert(getType() == td::real4);
        return _r4Val;
    }

    /// @brief Read the stored single-precision float value.
    /// @return Copy of the float storage.
    float r4Val() const
    {
        assert(getType() == td::real4);
        return _r4Val;
    }

    /// @brief Access the stored double-precision float by reference.
    /// @return Mutable reference to the double storage.
    double& r8Val()
    {
        assert(getType() == td::real8);
        return _r8Val;
    }

    /// @brief Read the stored double-precision float value.
    /// @return Copy of the double storage.
    double r8Val() const
    {
        assert(getType() == td::real8);
        return _r8Val;
    }

    /// @brief Access the stored Date value by reference.
    /// @return Mutable reference to the Date storage.
    Date& dateVal()
    {
        assert(getType() == td::date);
        return reinterpret_cast<Date&>((_dateVal));
    }

    /// @brief Read the stored Date value.
    /// @return Const reference to the Date storage.
    const Date& dateVal() const
    {
        assert(getType() == td::date);
        return reinterpret_cast<const Date&>((_dateVal));
    }
    
    /// @brief Access the stored Color value by reference.
    /// @return Mutable reference to the Color storage.
    Color& colorVal()
    {
        assert(getType() == td::color);
        return reinterpret_cast<Color&>((_colorVal));
    }

    /// @brief Read the stored Color value.
    /// @return Const reference to the Color storage.
    const Color& colorVal() const
    {
        assert(getType() == td::color);
        return reinterpret_cast<const Color&>((_colorVal));
    }
    
    /// @brief Access the stored ColorID value by reference.
    /// @return Mutable reference to the ColorID storage.
    ColorID& colorID()
    {
        assert(getType() == td::colorID);
        return _colorID;
    }

    /// @brief Read the stored ColorID value.
    /// @return Const reference to the ColorID storage.
    const ColorID& colorID() const
    {
        assert(getType() == td::colorID);
        return _colorID;
    }
    
    /// @brief Access the stored LinePattern value by reference.
    /// @return Mutable reference to the LinePattern storage.
    LinePattern& linePattern()
    {
        assert(getType() == td::linePattern);
        return _linePattern;
    }

    /// @brief Read the stored LinePattern value.
    /// @return Const reference to the LinePattern storage.
    const LinePattern& linePattern() const
    {
        assert(getType() == td::linePattern);
        return _linePattern;
    }
    
    /// @brief Access the stored DotPattern value by reference.
    /// @return Mutable reference to the DotPattern storage.
    DotPattern& dotPattern()
    {
        assert(getType() == td::dotPattern);
        return _dotPattern;
    }

    /// @brief Read the stored DotPattern value.
    /// @return Const reference to the DotPattern storage.
    const DotPattern& dotPattern() const
    {
        assert(getType() == td::dotPattern);
        return _dotPattern;
    }
    
    /// @brief Access the stored Anchor value by reference.
    /// @return Mutable reference to the Anchor storage.
    Anchor& anchor()
    {
        assert(getType() == td::anchor);
        return _anchor;
    }

    /// @brief Read the stored Anchor value.
    /// @return Const reference to the Anchor storage.
    const Anchor& anchor() const
    {
        assert(getType() == td::anchor);
        return _anchor;
    }
    
    /// @brief Access the stored Time value by reference.
    /// @return Mutable reference to the Time storage.
    Time& timeVal()
    {
        assert(getType() == td::time);
        return reinterpret_cast<Time&>((_timeVal));
    }

    /// @brief Read the stored Time value.
    /// @return Const reference to the Time storage.
    const Time& timeVal() const
    {
        assert(getType() == td::time);
        return reinterpret_cast<const Time&>((_timeVal));
    }

    /// @brief Access the stored DateTime value by reference.
    /// @return Mutable reference to the DateTime storage.
    DateTime& dtVal()
    {
        assert(getType() == td::dateTime);
        return reinterpret_cast<DateTime&>(_dtVal);
    }

    /// @brief Read the stored DateTime value.
    /// @return Const reference to the DateTime storage.
    const DateTime& dtVal() const
    {
        assert(getType() == td::dateTime);
        return reinterpret_cast<const DateTime&>(_dtVal);
    }

    /// @brief Extract the stored Decimal value into a typed Decimal output parameter.
    /// @tparam THOLDER Underlying integer holder type of the Decimal.
    /// @tparam NDEC Number of decimal places.
    /// @param toRet Output Decimal that receives the stored raw value.
    template< typename THOLDER, int NDEC >
    void decVal(Decimal<THOLDER, NDEC>& toRet)
    {
        assert(getType() == td::getType(toRet));
        toRet.setValue(_decVal);
    }

    //DEC VALUE
    /// @brief Access the stored Decimal0 value by reference.
    /// @return Mutable reference to the Decimal0 storage.
    Decimal0& dec0Val()
    {
        assert(getType() == td::decimal0);
        return reinterpret_cast<Decimal0 &>(_decVal);
    }

    /// @brief Read the stored Decimal0 value.
    /// @return Const reference to the Decimal0 storage.
    const Decimal0& dec0Val() const
    {
        assert(getType() == td::decimal0);
        return reinterpret_cast<const Decimal0 &>(_decVal);
    }

    /// @brief Access the stored Decimal1 value by reference.
    /// @return Mutable reference to the Decimal1 storage.
    Decimal1& dec1Val()
    {
        assert(getType() == td::decimal1);
        return reinterpret_cast<Decimal1 &>(_decVal);
    }

    /// @brief Read the stored Decimal1 value.
    /// @return Const reference to the Decimal1 storage.
    const Decimal1& dec1Val() const
    {
        assert(getType() == td::decimal1);
        return reinterpret_cast<const Decimal1 &>(_decVal);
    }

    /// @brief Access the stored Decimal2 value by reference.
    /// @return Mutable reference to the Decimal2 storage.
    Decimal2& dec2Val()
    {
        assert(getType() == td::decimal2);
        return reinterpret_cast<Decimal2&>(_decVal);
    }

    /// @brief Read the stored Decimal2 value.
    /// @return Const reference to the Decimal2 storage.
    const Decimal2& dec2Val() const
    {
        assert(getType() == td::decimal2);
        return reinterpret_cast<const Decimal2&>(_decVal);
    }

    /// @brief Access the stored Decimal3 value by reference.
    /// @return Mutable reference to the Decimal3 storage.
    Decimal3& dec3Val()
    {
        assert(getType() == td::decimal3);
        return reinterpret_cast<Decimal3&>(_decVal);
    }

    /// @brief Read the stored Decimal3 value.
    /// @return Const reference to the Decimal3 storage.
    const Decimal3& dec3Val() const
    {
        assert(getType() == td::decimal3);
        return reinterpret_cast<const Decimal3&>(_decVal);
    }

    /// @brief Access the stored Decimal4 value by reference.
    /// @return Mutable reference to the Decimal4 storage.
    Decimal4& dec4Val()
    {
        assert(getType() == td::decimal4);
        return reinterpret_cast<Decimal4&>(_decVal);
    }

    /// @brief Read the stored Decimal4 value.
    /// @return Const reference to the Decimal4 storage.
    const Decimal4& dec4Val() const
    {
        assert(getType() == td::decimal4);
        return reinterpret_cast<const Decimal4&>(_decVal);
    }

    /// @brief Access the stored SmallDecimal0 value by reference.
    /// @return Mutable reference to the SmallDecimal0 storage (backed by _i4Val).
    SmallDecimal0& sdec0Val()
    {
        assert(getType() == td::smallDecimal0);
        return reinterpret_cast<SmallDecimal0&>(_i4Val);
    }

    /// @brief Read the stored SmallDecimal0 value.
    /// @return Const reference to the SmallDecimal0 storage.
    const SmallDecimal0& sdec0Val() const
    {
        assert(getType() == td::smallDecimal0);
        return reinterpret_cast<const SmallDecimal0&>(_i4Val);
    }

    /// @brief Access the stored SmallDecimal1 value by reference.
    /// @return Mutable reference to the SmallDecimal1 storage.
    SmallDecimal1& sdec1Val()
    {
        assert(getType() == td::smallDecimal1);
        return reinterpret_cast<SmallDecimal1&>(_i4Val);
    }

    /// @brief Read the stored SmallDecimal1 value.
    /// @return Const reference to the SmallDecimal1 storage.
    const SmallDecimal1& sdec1Val() const
    {
        assert(getType() == td::smallDecimal1);
        return reinterpret_cast<const SmallDecimal1&>(_i4Val);
    }

    /// @brief Access the stored SmallDecimal2 value by reference.
    /// @return Mutable reference to the SmallDecimal2 storage.
    SmallDecimal2& sdec2Val()
    {
        assert(getType() == td::smallDecimal2);
        return reinterpret_cast<SmallDecimal2&>(_i4Val);
    }

    /// @brief Read the stored SmallDecimal2 value.
    /// @return Const reference to the SmallDecimal2 storage.
    const SmallDecimal2& sdec2Val() const
    {
        assert(getType() == td::smallDecimal2);
        return reinterpret_cast<const SmallDecimal2&>(_i4Val);
    }

    /// @brief Access the stored SmallDecimal3 value by reference.
    /// @return Mutable reference to the SmallDecimal3 storage.
    SmallDecimal3& sdec3Val()
    {
        assert(getType() == td::smallDecimal3);
        return reinterpret_cast<SmallDecimal3&>(_i4Val);
    }

    /// @brief Read the stored SmallDecimal3 value.
    /// @return Const reference to the SmallDecimal3 storage.
    const SmallDecimal3& sdec3Val() const
    {
        assert(getType() == td::smallDecimal3);
        return reinterpret_cast<const SmallDecimal3&>(_i4Val);
    }

    /// @brief Access the stored SmallDecimal4 value by reference.
    /// @return Mutable reference to the SmallDecimal4 storage.
    SmallDecimal4& sdec4Val()
    {
        assert(getType() == td::smallDecimal4);
        return reinterpret_cast<SmallDecimal4&>(_i4Val);
    }

    /// @brief Read the stored SmallDecimal4 value.
    /// @return Const reference to the SmallDecimal4 storage.
    const SmallDecimal4& sdec4Val() const
    {
        assert(getType() == td::smallDecimal4);
        return reinterpret_cast<const SmallDecimal4&>(_i4Val);
    }

    /// @brief Access the stored 7-character fixed string value by reference.
    /// @return Mutable reference to the ChFix7 storage.
    ChFix7& chFixVal()
    {
        assert(getType() == td::ch7);
        return reinterpret_cast<ChFix7&>(_chVal);
    }

    /// @brief Read the stored 7-character fixed string value.
    /// @return Const reference to the ChFix7 storage.
    const ChFix7& chFixVal() const
    {
        assert(getType() == td::ch7);
        return reinterpret_cast<const ChFix7&>(_chVal);
    }

    //END DEC VALUE

    // SMALL DEC
    // TODO: Odraditi za small dec value
    // END MALLDEC
    /// @brief Access the stored String value by reference.
    /// @return Mutable reference to the td::String storage.
    td::String& strVal()
    {
        assert(getType() == td::string8);
        return reinterpret_cast<td::String&>(_strVal);
    }

    /// @brief Read the stored String value.
    /// @return Const reference to the td::String storage.
    const td::String& strVal() const
    {
        assert(getType() == td::string8);
        return reinterpret_cast<const td::String&>(_strVal);
    }

    /// @brief Access the stored void pointer by reference.
    /// @return Mutable reference to the void* storage.
    void*& voidPtr()
    {
        assert(getType() == td::ptrVoid);
        return _voidPtr;
    }

    /// @brief Read the stored void pointer.
    /// @return Const void pointer.
    const void* voidPtr() const
    {
        assert(getType() == td::ptrVoid);
        return _voidPtr;
    }

    /// @brief Assign a boolean value and set the type to boolean.
    /// @param val Boolean value to store.
    /// @return Reference to this Variant.
    Variant& operator = (bool val)
    {
        clean();
        //_boolVal = val;
        if (val)
            _bVal = 'Y';
        else
            _bVal = 'N';

        setType(td::boolean);
        return *this;
    }

    /// @brief Assign a BYTE value.
    /// @param val BYTE value to store.
    /// @return Reference to this Variant.
    Variant& operator = (BYTE val)
    {
        clean();
        _byteVal = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a WORD value.
    /// @param val WORD value to store.
    /// @return Reference to this Variant.
    Variant& operator = (WORD val)
    {
        clean();
        _wordVal = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign an INT4 value.
    /// @param val INT4 value to store.
    /// @return Reference to this Variant.
    Variant& operator = (INT4 val)
    {
        clean();
        _i4Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign an INT2 value.
    /// @param val INT2 value to store.
    /// @return Reference to this Variant.
    Variant& operator = (INT2 val)
    {
        clean();
        _i2Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a UINT4 value.
    /// @param val UINT4 value to store.
    /// @return Reference to this Variant.
    Variant& operator = (UINT4 val)
    {
        clean();
        _u4Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a LUINT8 value.
    /// @param val LUINT8 value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const LUINT8& val)
    {
        clean();
        _lu8Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a LINT8 value.
    /// @param val LINT8 value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const LINT8& val)
    {
        clean();
        _li8Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a single-precision float value.
    /// @param val Float value to store.
    /// @return Reference to this Variant.
    Variant& operator = (float val)
    {
        clean();
        _r4Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a double-precision float value.
    /// @param val Double value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const double& val)
    {
        clean();
        _r8Val = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a Date value.
    /// @param val Date value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const td::Date& val)
    {
        clean();
        _dateVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }
    
    /// @brief Assign a Color value.
    /// @param val Color value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const td::Color& val)
    {
        clean();
        _colorVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }
    
    /// @brief Assign a ColorID value.
    /// @param val ColorID value to store.
    /// @return Reference to this Variant.
    Variant& operator = (td::ColorID val)
    {
        clean();
        _colorID = val;
        setType(td::getType(val));
        return *this;
    }
    
    /// @brief Assign a LinePattern value.
    /// @param val LinePattern value to store.
    /// @return Reference to this Variant.
    Variant& operator = (td::LinePattern val)
    {
        clean();
        _linePattern = val;
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a Time value.
    /// @param val Time value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const td::Time& val)
    {
        clean();
        _timeVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a DateTime value.
    /// @param val DateTime value to store.
    /// @return Reference to this Variant.
    Variant& operator = (const td::DateTime& val)
    {
        clean();
        _dtVal = reinterpret_cast<const td::LINT8&>(val);
        setType(td::getType(val));
        return *this;
    }
    
    /// @brief Assign a void pointer value.
    /// @param val Void pointer to store.
    /// @return Reference to this Variant.
    Variant& operator = (void* val)
    {
        clean();
        _voidPtr = val;
        setType(td::ptrVoid);
        return *this;
    }


    /// @brief Assign a const td::String value, sharing the reference-counted buffer.
    /// @param val Source String to share.
    /// @return Reference to this Variant.
    Variant& operator = (const td::String& val)
    {
        takeFromString(const_cast<td::String&>(val));
        return *this;
    }

    /// @brief Assign a td::String value, sharing the reference-counted buffer.
    /// @param val Source String to share.
    /// @return Reference to this Variant.
    Variant& operator = (td::String& val)
    {
        takeFromString(val);
        return *this;
    }

    /// @brief Assign a null-terminated C string, creating a new String buffer.
    /// @param val Null-terminated C string to copy; no-op if null.
    /// @return Reference to this Variant.
    Variant& operator = (const char* val)
    {
        clean();
        if (val == nullptr)
            return *this;

        td::String str(val);

        takeFromString(str);

        return *this;
    }

    /// @brief Assign a Decimal value.
    /// @tparam THOLDER Underlying integer holder type.
    /// @tparam NDEC Number of decimal places.
    /// @param val Decimal value to store.
    /// @return Reference to this Variant.
    template<typename THOLDER, int NDEC >
    Variant& operator = (const Decimal<THOLDER, NDEC>& val)
    {
        clean();
        _decVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }

    /// @brief Assign a SmallChFix value and set the DB type to chFix.
    /// @tparam NCH Fixed character capacity of the SmallChFix.
    /// @param val SmallChFix value to store.
    /// @return Reference to this Variant.
    template<size_t NCH >
    Variant& operator = (const SmallChFix<NCH>& val)
    {
        clean();
        _lu8Val = val.lu8Val();
        setInitialType(td::ch7);
        setDBTypeAndLen(td::chFix, NCH);
        return *this;
    }



    //Variant& operator = (const Variant& var)
    //{
    //    clean();

    //    td::DataType thisType(getType());
    //    td::DataType varType(var.getType());

    //    //_isNull = var._isNull;
    //    if (varType == td::string8)
    //    {
    //        if (thisType != td::string8)
    //            _flags = 0L;
    //        takeFromString(const_cast<td::String&>(var.strVal()));
    //    }
    //    else
    //    {
    //        _lu8Val = var._lu8Val;
    //    }

    //    //this prevents changing type due to implicit conversion to variant (_flags == 0) and
    //    if ( thisType != varType)
    //        _flags = var._flags;
    //    else
    //    {
    //        td::DataType thisDBType = getDBType();
    //        //td::DataType varDBType = var.getDBType();
    //        if (thisDBType == td::TD_NONE)
    //        {
    //            _flags = var._flags;
    //        }
    //    }
    //

    //    return *this;
    //}

    /// @brief Copy-assign another Variant, preserving all flags and value.
    /// @param var Source Variant to copy from.
    /// @return Reference to this Variant.
    Variant& operator = (const Variant& var)
    {
        td::DataType varType(var.getType());
                    
        if (varType == td::string8)
        {
            takeFromString(const_cast<td::String&>(var.strVal()));
        }
        else
        {
            _lu8Val = var._lu8Val;
        }

        _flags = var._flags;
        return *this;
    }

    /// @brief Set the database type and column length for string-like Variants.
    /// @param dtp Database column DataType (ch, nch, chFix, or nchFix).
    /// @param dbLen Maximum database column length.
    inline void setDBTypeAndLen(td::DataType dtp, td::WORD dbLen)
    {
        td::DataType cdt = getType();
        assert(cdt == td::ch7 || cdt == td::string8);

        td::LUINT8 cDT = (td::LUINT8) cdt;
        _flags &= ~TDVAR_TYPE_MASK;
        if ((dtp == td::ch) || ((dtp == td::nch) || (dtp == td::chFix) || (dtp == td::nchFix)))
        {
            _flags &= ~TDVAR_DBTYPE_MASK;
            _flags |= cDT;
            td::LUINT8 dbType = (td::LUINT8) dtp;
            dbType <<= 8;
            _flags |= dbType;
            setDBLen(dbLen);
        }
        else
            _flags |= (td::LUINT8)dtp;
    }

    //implicit operator
#ifdef USE_VARIANT_IMPLICIT_CONVERSION

    operator td::BYTE() const
    {
        return byteVal();
    }

    operator td::INT2() const
    {
        return i2Val();
    }

    operator td::WORD() const
    {
        return wordVal();
    }
    operator td::INT4() const
    {
        return i4Val();
    }
    operator td::UINT4() const
    {
        return u4Val();
    }
    operator td::LINT8() const
    {
        return li8Val();
    }
    operator td::LUINT8() const
    {
        return lu8Val();
    }
    operator float() const
    {
        return r4Val();
    }
    operator double() const
    {
        return r8Val();
    }
    operator const td::Date& () const
    {
        return dateVal();
    }
    operator const td::Time& () const
    {
        return timeVal();
    }
    operator const td::DateTime& () const
    {
        return dtVal();
    }
    operator const td::Decimal0& () const
    {
        return dec0Val();
    }
    operator const td::Decimal1& () const
    {
        return dec1Val();
    }
    operator const td::Decimal2& () const
    {
        return dec2Val();
    }
    operator const td::Decimal3& () const
    {
        return dec3Val();
    }
    operator const td::Decimal4& () const
    {
        return dec4Val();
    }
    operator const td::SmallDecimal0& () const
    {
        return sdec0Val();
    }
    operator const td::SmallDecimal1& () const
    {
        return sdec1Val();
    }
    operator const td::SmallDecimal2& () const
    {
        return sdec2Val();
    }
    operator const td::SmallDecimal3& () const
    {
        return sdec3Val();
    }
    operator const td::SmallDecimal4& () const
    {
        return sdec4Val();
    }
    operator const td::String& () const
    {
        return strVal();
    }
    operator bool() const
    {
        return boolVal();
    }
    operator const void* () const
    {
        return voidPtr();
    }
    operator const ChFix7& () const
    {
        return chFixVal();
    }
#endif

    //comparisson operators
    /// @brief Test equality with another Variant of the same type.
    /// @param var Variant to compare against.
    /// @return true if the types and values are equal.
    bool operator == (const Variant& var) const
    {
        if (var.getType() != getType())
            return false;
        EqualityVisitor<Variant> vis(var);
        return applyConstVisitor<EqualityVisitor<Variant>, bool>(vis, getType());
    }

    /// @brief Test inequality with another Variant.
    /// @param var Variant to compare against.
    /// @return true if the types or values differ.
    bool operator != (const Variant& var) const
    {
        if (var.getType() != getType())
            return true;

        EqualityVisitor<Variant> vis(var);
        return !applyConstVisitor<EqualityVisitor<Variant>, bool>(vis, getType());
    }
    
    /// @brief Test whether this Variant is less than another of the same type.
    /// @param var Variant to compare against (must have the same DataType).
    /// @return true if this value is less than var.
    bool operator < (const Variant& var) const
    {
        assert(var.getType() == getType());
        LessThanVisitor<Variant> vis(var);
        return applyConstVisitor<LessThanVisitor<Variant>, bool>(vis, getType());
    }

    /// @brief Test whether this Variant is greater than another of the same type.
    /// @param var Variant to compare against (must have the same DataType).
    /// @return true if this value is greater than var.
    bool operator > (const Variant& var) const
    {
        assert(var.getType() == getType());
        GreatherThanVisitor<Variant> vis(var);
        return applyConstVisitor<GreatherThanVisitor<Variant>, bool>(vis, getType());
    }

    /// @brief Test whether this Variant is less than or equal to another of the same type.
    /// @param var Variant to compare against (must have the same DataType).
    /// @return true if this value is less than or equal to var.
    bool operator <= (const Variant& var) const
    {
        assert(var.getType() == getType());
        LessEqualThanVisitor<Variant> vis(var);
        return applyConstVisitor<LessEqualThanVisitor<Variant>, bool>(vis, getType());
    }

    /// @brief Test whether this Variant is greater than or equal to another of the same type.
    /// @param var Variant to compare against (must have the same DataType).
    /// @return true if this value is greater than or equal to var.
    bool operator >= (const Variant& var) const
    {
        assert(var.getType() == getType());
        GreatherEqualThanVisitor<Variant> vis(var);
        return applyConstVisitor<GreatherEqualThanVisitor<Variant>, bool>(vis, getType());
    }

    /// @brief Check whether the stored value is zero (or logically empty for strings/dates).
    /// @return true if the value is zero/empty.
    bool isZero() const
    {
        IsZeroVisitor vis;
        return applyConstVisitor<IsZeroVisitor, bool>(vis, getType());

        //return _lu8Val == 0;
    }

    /// @brief Test equality with a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value equals val.
    template <typename TVAL>
    bool operator == (const TVAL& val) const
    {
        return getVal(val) == val;
    }

    /// @brief Test whether the stored value is greater than a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value is greater than val.
    template <typename TVAL>
    bool operator > (const TVAL& val) const
    {
        return getVal(val) > val;
    }

    /// @brief Test whether the stored value is less than a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value is less than val.
    template <typename TVAL>
    bool operator < (const TVAL& val) const
    {
        return getVal(val) < val;
    }

    /// @brief Test whether the stored value is greater than or equal to a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value is greater than or equal to val.
    template <typename TVAL>
    bool operator >= (const TVAL& val) const
    {
        return getVal(val) >= val;
    }

    /// @brief Test whether the stored value is less than or equal to a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value is less than or equal to val.
    template <typename TVAL>
    bool operator <= (const TVAL& val) const
    {
        return getVal(val) <= val;
    }

    /// @brief Test inequality with a typed value.
    /// @tparam TVAL Type of the right-hand operand.
    /// @param val Value to compare against.
    /// @return true if the stored value differs from val.
    template <typename TVAL>
    bool operator != (const TVAL& val) const
    {
        return getVal(val) != val;
    }

    /// @brief Access the stored bool pointer by reference.
    /// @return Mutable reference to the bool* storage.
    bool*& ptrBool()
    {
        assert(getType() == td::ptrBoolean);
        return _ptrBool;
    }
    /// @brief Read the stored bool pointer.
    /// @return Const bool pointer.
    const bool* ptrBool() const
    {
        assert(getType() == td::ptrBoolean);
        return _ptrBool;
    }

    /// @brief Access the stored BYTE pointer by reference.
    /// @return Mutable reference to the BYTE* storage.
    BYTE*& ptrByte()
    {
        assert(getType() == td::ptrByte);
        return _ptrByte;
    }

    /// @brief Read the stored BYTE pointer.
    /// @return Const BYTE pointer.
    const BYTE* ptrByte() const
    {
        assert(getType() == td::ptrByte);
        return _ptrByte;
    }

    /// @brief Access the stored WORD pointer by reference.
    /// @return Mutable reference to the WORD* storage.
    WORD*& ptrWord()
    {
        assert(getType() == td::ptrWord);
        return _ptrWord;
    }

    /// @brief Read the stored WORD pointer.
    /// @return Const WORD pointer.
    const WORD* ptrWord() const
    {
        assert(getType() == td::ptrWord);
        return _ptrWord;
    }

    /// @brief Access the stored INT2 pointer by reference.
    /// @return Mutable reference to the INT2* storage.
    INT2*& ptrI2()
    {
        assert(getType() == td::ptrInt2);
        return _ptrI2;
    }

    /// @brief Read the stored INT2 pointer.
    /// @return Const INT2 pointer.
    const INT2* ptrI2() const
    {
        assert(getType() == td::ptrInt2);
        return _ptrI2;
    }

    /// @brief Access the stored UINT4 pointer by reference.
    /// @return Mutable reference to the UINT4* storage.
    UINT4*& ptrU4()
    {
        assert(getType() == td::ptrUint4);
        return _ptrU4;
    }

    /// @brief Read the stored UINT4 pointer.
    /// @return Const UINT4 pointer.
    const UINT4* ptrU4() const
    {
        assert(getType() == td::ptrUint4);
        return _ptrU4;
    }

    /// @brief Access the stored INT4 pointer by reference.
    /// @return Mutable reference to the INT4* storage.
    INT4*& ptrI4()
    {
        assert(getType() == td::ptrInt4);
        return _ptrI4;
    }

    /// @brief Read the stored INT4 pointer.
    /// @return Const INT4 pointer.
    const INT4* ptrI4() const
    {
        assert(getType() == td::ptrUint4);
        return _ptrI4;
    }

    /// @brief Access the stored LUINT8 pointer by reference.
    /// @return Mutable reference to the LUINT8* storage.
    LUINT8*& ptrLU8()
    {
        assert(getType() == td::ptrLuint8);
        return _ptrLU8;
    }

    /// @brief Read the stored LUINT8 pointer.
    /// @return Const LUINT8 pointer.
    const LUINT8* ptrLU8() const
    {
        assert(getType() == td::ptrLuint8);
        return _ptrLU8;
    }

    /// @brief Access the stored LINT8 pointer by reference.
    /// @return Mutable reference to the LINT8* storage.
    LINT8*& ptrLI8()
    {
        assert(getType() == td::ptrLint8);
        return _ptrLI8;
    }

    /// @brief Read the stored LINT8 pointer.
    /// @return Const LINT8 pointer.
    const LINT8* ptrLI8() const
    {
        assert(getType() == td::ptrLint8);
        return _ptrLI8;
    }

    /// @brief Access the stored float pointer by reference.
    /// @return Mutable reference to the float* storage.
    float*& ptrR4()
    {
        assert(getType() == td::ptrReal4);
        return _ptrR4;
    }

    /// @brief Read the stored float pointer.
    /// @return Const float pointer.
    const float* ptrR4() const
    {
        assert(getType() == td::ptrReal4);
        return _ptrR4;
    }

    /// @brief Access the stored double pointer by reference.
    /// @return Mutable reference to the double* storage.
    double*& ptrR8()
    {
        assert(getType() == td::ptrReal8);
        return _ptrR8;
    }

    /// @brief Read the stored double pointer.
    /// @return Const double pointer.
    const double* ptrR8() const
    {
        assert(getType() == td::ptrReal8);
        return _ptrR8;
    }

    /// @brief Access the stored Date pointer by reference.
    /// @return Mutable reference to the Date* storage.
    Date*& ptrDate()
    {
        assert(getType() == td::ptrDate);
        return _ptrDate;
    }

    /// @brief Read the stored Date pointer.
    /// @return Const Date pointer.
    const Date* ptrDate() const
    {
        assert(getType() == td::ptrDate);
        return _ptrDate;
    }

    /// @brief Access the stored Time pointer by reference.
    /// @return Mutable reference to the Time* storage.
    Time*& ptrTime()
    {
        assert(getType() == td::ptrTime);
        return _ptrTime;
    }

    /// @brief Read the stored Time pointer.
    /// @return Const Time pointer.
    const Time* ptrTime() const
    {
        assert(getType() == td::ptrTime);
        return _ptrTime;
    }
    /// @brief Access the stored DateTime pointer by reference.
    /// @return Mutable reference to the DateTime* storage.
    DateTime*& ptrDT()
    {
        assert(getType() == td::ptrDateTime);
        return _ptrDT;
    }

    /// @brief Read the stored DateTime pointer.
    /// @return Const DateTime pointer.
    const DateTime* ptrDT() const
    {
        assert(getType() == td::ptrDateTime);
        return _ptrDT;
    }

    /// @brief Access the stored Decimal0 pointer by reference.
    /// @return Mutable reference to the Decimal0* storage.
    Decimal0*& ptrDec0()
    {
        assert(getType() == td::ptrDecimal0);
        return _ptrDec0;
    }

    /// @brief Read the stored Decimal0 pointer.
    /// @return Const Decimal0 pointer.
    const Decimal0* ptrDec0() const
    {
        assert(getType() == td::ptrDecimal0);
        return _ptrDec0;
    }

    /// @brief Access the stored Decimal1 pointer by reference.
    /// @return Mutable reference to the Decimal1* storage.
    Decimal1*& ptrDec1()
    {
        assert(getType() == td::ptrDecimal1);
        return _ptrDec1;
    }

    /// @brief Read the stored Decimal1 pointer.
    /// @return Const Decimal1 pointer.
    const Decimal1* ptrDec1() const
    {
        assert(getType() == td::ptrDecimal1);
        return _ptrDec1;
    }

    /// @brief Access the stored Decimal2 pointer by reference.
    /// @return Mutable reference to the Decimal2* storage.
    Decimal2*& ptrDec2()
    {
        assert(getType() == td::ptrDecimal2);
        return _ptrDec2;
    }

    /// @brief Read the stored Decimal2 pointer.
    /// @return Const Decimal2 pointer.
    const Decimal2* ptrDec2() const
    {
        assert(getType() == td::ptrDecimal2);
        return _ptrDec2;
    }

    /// @brief Access the stored Decimal3 pointer by reference.
    /// @return Mutable reference to the Decimal3* storage.
    Decimal3*& ptrDec3()
    {
        assert(getType() == td::ptrDecimal3);
        return _ptrDec3;
    }

    /// @brief Read the stored Decimal3 pointer.
    /// @return Const Decimal3 pointer.
    const Decimal3* ptrDec3() const
    {
        assert(getType() == td::ptrDecimal3);
        return _ptrDec3;
    }

    /// @brief Access the stored Decimal4 pointer by reference.
    /// @return Mutable reference to the Decimal4* storage.
    Decimal4*& ptrDec4()
    {
        assert(getType() == td::ptrDecimal4);
        return _ptrDec4;
    }

    /// @brief Read the stored Decimal4 pointer.
    /// @return Const Decimal4 pointer.
    const Decimal4* ptrDec4() const
    {
        assert(getType() == td::ptrDecimal4);
        return _ptrDec4;
    }

    /// @brief Access the stored SmallDecimal0 pointer by reference.
    /// @return Mutable reference to the SmallDecimal0* storage.
    SmallDecimal0*& ptrSDec0()
    {
        assert(getType() == td::ptrSmalldecimal0);
        return _ptrSDec0;
    }

    /// @brief Read the stored SmallDecimal0 pointer.
    /// @return Const SmallDecimal0 pointer.
    const SmallDecimal0* ptrSDec0() const
    {
        assert(getType() == td::ptrSmalldecimal0);
        return _ptrSDec0;
    }

    /// @brief Access the stored SmallDecimal1 pointer by reference.
    /// @return Mutable reference to the SmallDecimal1* storage.
    SmallDecimal1*& ptrSDec1()
    {
        assert(getType() == td::ptrSmalldecimal1);
        return _ptrSDec1;
    }

    /// @brief Read the stored SmallDecimal1 pointer.
    /// @return Const SmallDecimal1 pointer.
    const SmallDecimal1* ptrSDec1() const
    {
        assert(getType() == td::ptrSmalldecimal1);
        return _ptrSDec1;
    }

    /// @brief Access the stored SmallDecimal2 pointer by reference.
    /// @return Mutable reference to the SmallDecimal2* storage.
    SmallDecimal2*& ptrSDec2()
    {
        assert(getType() == td::ptrSmalldecimal2);
        return _ptrSDec2;
    }

    /// @brief Read the stored SmallDecimal2 pointer.
    /// @return Const SmallDecimal2 pointer.
    const SmallDecimal2* ptrSDec2() const
    {
        assert(getType() == td::ptrSmalldecimal2);
        return _ptrSDec2;
    }

    /// @brief Access the stored SmallDecimal3 pointer by reference.
    /// @return Mutable reference to the SmallDecimal3* storage.
    SmallDecimal3*& ptrSDec3()
    {
        assert(getType() == td::ptrSmalldecimal3);
        return _ptrSDec3;
    }

    /// @brief Read the stored SmallDecimal3 pointer.
    /// @return Const SmallDecimal3 pointer.
    const SmallDecimal3* ptrSDec3() const
    {
        assert(getType() == td::ptrSmalldecimal3);
        return _ptrSDec3;
    }

    /// @brief Access the stored SmallDecimal4 pointer by reference.
    /// @return Mutable reference to the SmallDecimal4* storage.
    SmallDecimal4*& ptrSDec4()
    {
        assert(getType() == td::ptrSmalldecimal4);
        return _ptrSDec4;
    }

    /// @brief Read the stored SmallDecimal4 pointer.
    /// @return Const SmallDecimal4 pointer.
    const SmallDecimal4* ptrSDec4() const
    {
        assert(getType() == td::ptrSmalldecimal4);
        return _ptrSDec4;
    }

    /// @brief Access the stored Variant pointer by reference.
    /// @return Mutable reference to the Variant* storage.
    Variant*& ptrVar()
    {
        assert(getType() == td::ptrVariant);
        return _ptrVar;
    }

    /// @brief Read the stored Variant pointer.
    /// @return Const Variant pointer.
    const Variant* ptrVar() const
    {
        assert(getType() == td::ptrVariant);
        return _ptrVar;
    }

    /// @brief Assign a bool pointer value and set the type to ptrBoolean.
    /// @param val Pointer to a bool to store.
    /// @return Reference to this Variant.
    Variant& operator = (bool* val)
    {
        clean();
        _ptrBool = val;
        setType(td::ptrBoolean);
        return *this;
    }

    /// @brief Assign a BYTE pointer value.
    /// @param val Pointer to a BYTE to store.
    /// @return Reference to this Variant.
    Variant& operator = (BYTE* val)
    {
        clean();
        _ptrByte = val;
        setType(td::ptrByte);
        return *this;
    }

    /// @brief Assign a WORD pointer value.
    /// @param val Pointer to a WORD to store.
    /// @return Reference to this Variant.
    Variant& operator = (WORD* val)
    {
        clean();
        _ptrWord = val;
        setType(td::ptrWord);
        return *this;
    }
            
    /// @brief Assign an INT2 pointer value.
    /// @param val Pointer to an INT2 to store.
    /// @return Reference to this Variant.
    Variant& operator = (INT2* val)
    {
        clean();
        _ptrI2 = val;
        setType(td::ptrInt2);
        return *this;
    }
    /// @brief Assign a UINT4 pointer value.
    /// @param val Pointer to a UINT4 to store.
    /// @return Reference to this Variant.
    Variant& operator = (UINT4* val)
    {
        clean();
        _ptrU4 = val;
        setType(td::ptrUint4);
        return *this;
    }
    /// @brief Assign an INT4 pointer value.
    /// @param val Pointer to an INT4 to store.
    /// @return Reference to this Variant.
    Variant& operator = (INT4* val)
    {
        clean();
        _ptrI4 = val;
        setType(td::ptrInt4);
        return *this;
    }
    /// @brief Assign a LUINT8 pointer value.
    /// @param val Pointer to a LUINT8 to store.
    /// @return Reference to this Variant.
    Variant& operator = (LUINT8* val)
    {
        clean();
        _ptrLU8 = val;
        setType(td::ptrLuint8);
        return *this;
    }
    
    /// @brief Assign a LINT8 pointer value.
    /// @param val Pointer to a LINT8 to store.
    /// @return Reference to this Variant.
    Variant& operator = (LINT8* val)
    {
        clean();
        _ptrLI8 = val;
        setType(td::ptrLint8);
        return *this;
    }
    
    /// @brief Assign a float pointer value.
    /// @param val Pointer to a float to store.
    /// @return Reference to this Variant.
    Variant& operator = (float* val)
    {
        clean();
        _ptrR4 = val;
        setType(td::ptrReal4);
        return *this;
    }
    /// @brief Assign a double pointer value.
    /// @param val Pointer to a double to store.
    /// @return Reference to this Variant.
    Variant& operator = (double* val)
    {
        clean();
        _ptrR8 = val;
        setType(td::ptrReal8);
        return *this;
    }
    /// @brief Assign a Date pointer value.
    /// @param val Pointer to a Date to store.
    /// @return Reference to this Variant.
    Variant& operator = (Date* val)
    {
        clean();
        _ptrDate = val;
        setType(td::ptrDate);
        return *this;
    }
    
    /// @brief Assign a Time pointer value.
    /// @param val Pointer to a Time to store.
    /// @return Reference to this Variant.
    Variant& operator = (Time* val)
    {
        clean();
        _ptrTime = val;
        setType(td::ptrTime);
        return *this;
    }
    
    /// @brief Assign a DateTime pointer value.
    /// @param val Pointer to a DateTime to store.
    /// @return Reference to this Variant.
    Variant& operator = (DateTime* val)
    {
        clean();
        _ptrDT = val;
        setType(td::ptrDateTime);
        return *this;
    }
    /// @brief Assign a Decimal0 pointer value.
    /// @param val Pointer to a Decimal0 to store.
    /// @return Reference to this Variant.
    Variant& operator = (Decimal0* val)
    {
        clean();
        _ptrDec0 = val;
        setType(td::ptrDecimal0);
        return *this;
    }
    /// @brief Assign a Decimal1 pointer value.
    /// @param val Pointer to a Decimal1 to store.
    /// @return Reference to this Variant.
    Variant& operator = (Decimal1* val)
    {
        clean();
        _ptrDec1 = val;
        setType(td::ptrDecimal1);
        return *this;
    }
    /// @brief Assign a Decimal2 pointer value.
    /// @param val Pointer to a Decimal2 to store.
    /// @return Reference to this Variant.
    Variant& operator = (Decimal2* val)
    {
        clean();
        _ptrDec2 = val;
        setType(td::ptrDecimal2);
        return *this;
    }
    /// @brief Assign a Decimal3 pointer value.
    /// @param val Pointer to a Decimal3 to store.
    /// @return Reference to this Variant.
    Variant& operator = (Decimal3* val)
    {
        clean();
        _ptrDec3 = val;
        setType(td::ptrDecimal3);
        return *this;
    }
    /// @brief Assign a Decimal4 pointer value.
    /// @param val Pointer to a Decimal4 to store.
    /// @return Reference to this Variant.
    Variant& operator = (Decimal4* val)
    {
        clean();
        _ptrDec4 = val;
        setType(td::ptrDecimal4);
        return *this;
    }

    /// @brief Assign a SmallDecimal0 pointer value.
    /// @param val Pointer to a SmallDecimal0 to store.
    /// @return Reference to this Variant.
    Variant& operator = (SmallDecimal0* val)
    {
        clean();
        _ptrSDec0 = val;
        setType(td::ptrSmalldecimal0);
        return *this;
    }
    /// @brief Assign a SmallDecimal1 pointer value.
    /// @param val Pointer to a SmallDecimal1 to store.
    /// @return Reference to this Variant.
    Variant& operator = (SmallDecimal1* val)
    {
        clean();
        _ptrSDec1 = val;
        setType(td::ptrSmalldecimal1);
        return *this;
    }
    /// @brief Assign a SmallDecimal2 pointer value.
    /// @param val Pointer to a SmallDecimal2 to store.
    /// @return Reference to this Variant.
    Variant& operator = (SmallDecimal2* val)
    {
        clean();
        _ptrSDec2 = val;
        setType(td::ptrSmalldecimal2);
        return *this;
    }
    /// @brief Assign a SmallDecimal3 pointer value.
    /// @param val Pointer to a SmallDecimal3 to store.
    /// @return Reference to this Variant.
    Variant& operator = (SmallDecimal3* val)
    {
        clean();
        _ptrSDec3 = val;
        setType(td::ptrSmalldecimal3);
        return *this;
    }
    /// @brief Assign a SmallDecimal4 pointer value.
    /// @param val Pointer to a SmallDecimal4 to store.
    /// @return Reference to this Variant.
    Variant& operator = (SmallDecimal4* val)
    {
        clean();
        _ptrSDec4 = val;
        setType(td::ptrSmalldecimal4);
        return *this;
    }
    /// @brief Assign a Variant pointer value.
    /// @param val Pointer to another Variant to store.
    /// @return Reference to this Variant.
    Variant& operator = (Variant* val)
    {
        clean();
        _ptrVar = val;
        setType(td::ptrVariant);
        return *this;
    }

    /// @brief Assign the raw value from a VariantBase, dispatching via visitor.
    /// @param vb Source VariantBase whose value is copied into the current type slot.
    /// @return Reference to this Variant.
    Variant& operator = (const td::VariantBase& vb)
    {
        BaseToVariantVisitor<td::VariantBase> vis(vb);
        applyVisitor<BaseToVariantVisitor<td::VariantBase>, void>(vis, getType());
        return *this;
    }

    /// @brief Re-initialize the Variant to a new data type, optionally zeroing the value.
    /// @param dtp New DataType to assign.
    /// @param toZero If true, sets the raw storage to zero after changing the type.
    void init(td::DataType dtp, bool toZero = false)
    {
        clean();
        setInitialType(dtp);

        if (dtp == td::ch7)
        {
            setDBTypeAndLen(td::chFix, 7);
        }
        //if ((dtp == td::ch) || ((dtp == td::nch) || (dtp == td::chFix) || (dtp == td::nchFix)))
        //    _flags = td::string8;
        //else
        //    _flags = dtp;
        if (toZero)
            _lu8Val = 0;
    }

    /// @brief Reset the stored value to zero (releases string memory if applicable).
    inline void toZero()
    {
        if (getType() == td::string8)
        {
            td::String& str = reinterpret_cast<td::String&>(_strVal);
            if (str.relRef())
            {
                delete[] _strVal;
            }
            _strVal = nullptr;
        }
        else
            _lu8Val = 0;
    }


    
    /// @brief Convert the stored value to a C string using a utility formatter.
    /// @tparam TUTIL Utility formatter type with a c_str() method.
    /// @param util Reference to the utility formatter.
    /// @return Null-terminated C string representation of the stored value.
    template <class TUTIL>
    const char* c_str(TUTIL& util) const
    {
        ToCStrVisitor<TUTIL> vis(util);
        return applyConstVisitor<ToCStrVisitor<TUTIL>, const char*>(vis, getType());
    }
    
    
    /// @brief Format the stored value to a C string using a locale registry and format options.
    /// @tparam TREG Registry/locale helper type providing formatting context.
    /// @param pReg Pointer to the locale/format registry.
    /// @param nDecOrTimeFormat Number of decimal places, or time format code.
    /// @param showThousendSepOrTimeFmt Non-zero to show thousand separator or a secondary time format flag.
    /// @param floatFmt Floating-point display format (default Decimal).
    /// @return Null-terminated formatted C string.
    template <class TREG>
    const char* c_str(TREG* pReg, int nDecOrTimeFormat, int showThousendSepOrTimeFmt = 1, td::FormatFloat floatFmt= td::FormatFloat::Decimal) const
    {
        FormatCStrVisitor<TREG> vis(pReg, nDecOrTimeFormat, showThousendSepOrTimeFmt, floatFmt);
        return applyConstVisitor<FormatCStrVisitor<TREG>, const char*>(vis, getType());
    }


    //void fromString(const char* str)
    /// @brief Parse a C string and store the result into the current type slot.
    /// @param str Null-terminated string to parse.
    void setValueFromString(const char* str)
    {
        FromStringVisitor vis(str);
        applyVisitor<FromStringVisitor, void>(vis, getType());
    }

    /// @brief Parse a td::String and store the result into the current type slot.
    /// @param str Source String to parse.
    void setValueFromString(const td::String& str)
    {
        setValueFromString(str.c_str());
    }

    /// @brief Convert a numeric value to the current type and store the result.
    /// @tparam TNUM Numeric source type.
    /// @param numVal Numeric value to convert and store.
    template <typename TNUM>
    void setValueFromNumeric(const TNUM& numVal)
    //void fromNumeric(const TNUM& val)
    {
        FromNumericVisitor<TNUM> vis(numVal);
        applyVisitor<FromNumericVisitor<TNUM>, void>(vis, getType());
    }

    /// @brief Set the stored value from an integer by casting it to the current type.
    /// @tparam TINTEGER Integer source type.
    /// @param val Integer value to cast and store.
    template <typename TINTEGER>
    void setTo(TINTEGER val)
    {
        SetToIntegerVisitor<TINTEGER> vis(val);
        applyVisitor<SetToIntegerVisitor<TINTEGER>, void>(vis, getType());
    }

    /// @brief Convert the stored value to a numeric type.
    /// @tparam TNUM Target numeric type (e.g. int, double).
    /// @return The stored value converted to TNUM.
    template <typename TNUM>
    inline TNUM toNumber() const
    {
        ToNumberVisitor<TNUM> vis;
        return applyConstVisitor<ToNumberVisitor<TNUM>, TNUM>(vis, getType());
    }
    
    /// @brief Clamp the stored numeric value to [minValue, maxValue].
    /// @param minValue Lower bound; values below this are set to minValue.
    /// @param maxValue Upper bound; values above this are set to maxValue.
    void clip(double minValue, double maxValue)
    {
        if (!isNumeric())
            return;
        double currValue = toNumber<double>();
        bool clipped = false;
        
        if (currValue < minValue)
        {
            currValue = minValue;
            clipped = true;
        }
        
        if (currValue > maxValue)
        {
            currValue = maxValue;
            clipped = true;
        }
        
        if (clipped)
        {
            setValueFromNumeric(currValue);
        }
    }
            
    /// @brief Convert the stored value to a td::String.
    /// @return String representation of the stored value.
    inline td::String toString() const
    {
        ToStringVisitor vis;
        return applyConstVisitor<ToStringVisitor, td::String>(vis, getType());
    }

    /// @brief Copy the stored value into a raw void pointer target of the given type.
    /// @param ptr Destination buffer where the value will be written.
    /// @param pointerType DataType expected at ptr; must match the current type.
    inline void getValue(void* ptr, td::DataType pointerType) const
    {
        td::DataType dt = getType();
        assert(dt == pointerType);
        if (dt != pointerType)
            return;
        CopyToVoidPtrVisitor vis(ptr);
        applyConstVisitor<CopyToVoidPtrVisitor, void>(vis, getType());
    }

    /// @brief Type-coercing assignment: converts var's value to this Variant's current type.
    /// @param var Source Variant; its value is converted and assigned even if types differ.
    inline void assign(const td::Variant& var)
    {
        td::DataType t1 = getType();
        td::DataType t2 = var.getType();
        if (t1 == t2)
        {
            operator = (var);
            return;
        }

        if (t1 == td::TD_NONE)
        {
            init(t2, true);
            operator = (var);
            return;
        }

        AssignVisitor<td::Variant> vis(var);
        applyVisitor<AssignVisitor<td::Variant>, void>(vis, getType());
    }

    /// @brief Replace the stored numeric value with its absolute value.
    /// @return Const reference to this Variant after the operation.
    inline const td::Variant& abs()
    {
        AbsVisitor vis;
        applyVisitor<AbsVisitor, void>(vis, getType());
        return *this;
    }
    
    /// @brief Add another Variant's value to this one in place.
    /// @param var Variant whose value is added.
    /// @return Const reference to this Variant after the addition.
    inline const td::Variant& operator += (const td::Variant& var)
    {
        PlusEqVisitor<td::Variant> vis(var);
        applyVisitor<PlusEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    /// @brief Subtract another Variant's value from this one in place.
    /// @param var Variant whose value is subtracted.
    /// @return Const reference to this Variant after the subtraction.
    inline const td::Variant& operator -= (const td::Variant& var)
    {
        MinusEqVisitor<td::Variant> vis(var);
        applyVisitor<MinusEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    /// @brief Multiply this Variant's value by another in place.
    /// @param var Variant whose value is used as the multiplier.
    /// @return Const reference to this Variant after the multiplication.
    inline const td::Variant& operator *= (const td::Variant& var)
    {
        MultEqVisitor<td::Variant> vis(var);
        applyVisitor<MultEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    /// @brief Divide this Variant's value by another in place.
    /// @param var Variant whose value is used as the divisor.
    /// @return Const reference to this Variant after the division.
    inline const td::Variant& operator /= (const td::Variant& var)
    {
        DivEqVisitor<td::Variant> vis(var);
        applyVisitor<DivEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }
protected:
    /// @brief Return a mutable reference to the BoolCh storage (used by template comparisons).
    /// @return Reference to the internal BoolCh field.
    inline td::BoolCh& getVal(td::BoolCh){ return reinterpret_cast<td::BoolCh&>(_bVal); }

//    inline bool& getVal(bool){ return _boolVal; }

    /// @brief Return a mutable reference to the BYTE storage (used by template comparisons).
    /// @return Reference to the internal BYTE field.
    inline td::BYTE& getVal(td::BYTE){ return byteVal(); }

    /// @brief Return a mutable reference to the INT2 storage. @return Reference to _i2Val.
    inline td::INT2& getVal(td::INT2){ return i2Val(); }
    /// @brief Return a mutable reference to the WORD storage. @return Reference to _wordVal.
    inline td::WORD& getVal(td::WORD) { return wordVal(); }
    /// @brief Return a mutable reference to the INT4 storage. @return Reference to _i4Val.
    inline td::INT4& getVal(td::INT4) { return i4Val(); }
    /// @brief Return a mutable reference to the UINT4 storage. @return Reference to _u4Val.
    inline td::UINT4& getVal(td::UINT4) { return u4Val(); }
    /// @brief Return a mutable reference to the LINT8 storage. @return Reference to _li8Val.
    inline td::LINT8& getVal(td::LINT8){ return li8Val(); }
    /// @brief Return a mutable reference to the LUINT8 storage. @return Reference to _lu8Val.
    inline td::LUINT8& getVal(td::LUINT8){ return lu8Val(); }
    /// @brief Return a mutable reference to the float storage. @return Reference to _r4Val.
    inline float& getVal(float){ return r4Val(); }
    /// @brief Return a mutable reference to the double storage. @return Reference to _r8Val.
    inline double& getVal(double){ return r8Val(); }
    /// @brief Return a mutable reference to the String storage. @return Reference to strVal().
    inline td::String& getVal(td::String&){ return strVal(); }
    /// @brief Return a mutable reference to the Date storage. @return Reference to dateVal().
    inline td::Date& getVal(td::Date){ return dateVal(); }
    /// @brief Return a mutable reference to the Time storage. @return Reference to timeVal().
    inline td::Time& getVal(td::Time){ return timeVal(); }
    /// @brief Return a mutable reference to the DateTime storage. @return Reference to dtVal().
    inline td::DateTime& getVal(td::DateTime){ return dtVal(); }
    /// @brief Return a mutable reference to the ChFix7 storage. @return Reference to chFixVal().
    inline td::ChFix7& getVal(td::ChFix7){ return chFixVal(); }
    /// @brief Return a mutable reference to the Color storage. @return Reference to colorVal().
    inline td::Color& getVal(td::Color){ return colorVal(); }
    /// @brief Return a mutable reference to the ColorID storage. @return Reference to colorID().
    inline td::ColorID& getVal(td::ColorID){ return colorID(); }
    /// @brief Return a mutable reference to the LinePattern storage. @return Reference to linePattern().
    inline td::LinePattern& getVal(td::LinePattern){ return linePattern(); }
    /// @brief Return a mutable reference to the DotPattern storage. @return Reference to dotPattern().
    inline td::DotPattern& getVal(td::DotPattern){ return dotPattern(); }
    /// @brief Return a mutable reference to the Anchor storage. @return Reference to anchor().
    inline td::Anchor& getVal(td::Anchor){ return anchor(); }
    /// @brief Return a mutable reference to the SmallDecimal0 storage. @return Reference to sdec0Val().
    inline td::SmallDecimal0& getVal(td::SmallDecimal0){ return sdec0Val(); }
    /// @brief Return a mutable reference to the SmallDecimal1 storage. @return Reference to sdec1Val().
    inline td::SmallDecimal1& getVal(td::SmallDecimal1){ return sdec1Val(); }
    /// @brief Return a mutable reference to the SmallDecimal2 storage. @return Reference to sdec2Val().
    inline td::SmallDecimal2& getVal(td::SmallDecimal2){ return sdec2Val(); }
    /// @brief Return a mutable reference to the SmallDecimal3 storage. @return Reference to sdec3Val().
    inline td::SmallDecimal3& getVal(td::SmallDecimal3){ return sdec3Val(); }
    /// @brief Return a mutable reference to the SmallDecimal4 storage. @return Reference to sdec4Val().
    inline td::SmallDecimal4& getVal(td::SmallDecimal4){ return sdec4Val(); }
    /// @brief Return a mutable reference to the Decimal0 storage. @return Reference to dec0Val().
    inline td::Decimal0& getVal(td::Decimal0){ return dec0Val(); }
    /// @brief Return a mutable reference to the Decimal1 storage. @return Reference to dec1Val().
    inline td::Decimal1& getVal(td::Decimal1){ return dec1Val(); }
    /// @brief Return a mutable reference to the Decimal2 storage. @return Reference to dec2Val().
    inline td::Decimal2& getVal(td::Decimal2){ return dec2Val(); }
    /// @brief Return a mutable reference to the Decimal3 storage. @return Reference to dec3Val().
    inline td::Decimal3& getVal(td::Decimal3){ return dec3Val(); }
    /// @brief Return a mutable reference to the Decimal4 storage. @return Reference to dec4Val().
    inline td::Decimal4& getVal(td::Decimal4){ return dec4Val(); }


    //const
    /// @brief Return a const reference to the BoolCh storage. @return Const reference to _bVal.
    inline const td::BoolCh& getVal(td::BoolCh)const { return reinterpret_cast<const td::BoolCh&>(_bVal); }
    /// @brief Return the stored boolean as INT4. @return 1 if true, 0 if false.
    inline td::INT4 getVal(bool)const { return boolVal(); }
    /// @brief Return the stored BYTE value. @return Copy of _byteVal.
    inline td::BYTE getVal(td::BYTE)const{ return byteVal(); }
    /// @brief Return the stored INT2 value. @return Copy of _i2Val.
    inline td::INT2 getVal(td::INT2)const{ return i2Val(); }
    /// @brief Return the stored WORD value. @return Copy of _wordVal.
    inline td::WORD getVal(td::WORD)const { return wordVal(); }
    /// @brief Return the stored INT4 value. @return Copy of _i4Val.
    inline td::INT4 getVal(td::INT4)const { return i4Val(); }
    /// @brief Return the stored UINT4 value. @return Copy of _u4Val.
    inline td::UINT4 getVal(td::UINT4) const{ return u4Val(); }
    /// @brief Return the stored LINT8 value. @return Copy of _li8Val.
    inline td::LINT8 getVal(td::LINT8)const{ return li8Val(); }
    /// @brief Return the stored LUINT8 value. @return Copy of _lu8Val.
    inline td::LUINT8 getVal(td::LUINT8)const{ return lu8Val(); }
    /// @brief Return the stored float value. @return Copy of _r4Val.
    inline float getVal(float)const{ return r4Val(); }
    /// @brief Return the stored double value. @return Copy of _r8Val.
    inline double getVal(double)const{ return r8Val(); }
    /// @brief Return a const reference to the String storage. @return Const reference to strVal().
    inline const td::String& getVal(const td::String&) const { return strVal(); }

    //inline const td::String& getValreinterpret_cast<td::String&>(const) { return strVal(); }

    /// @brief Return a const reference to the Date storage. @return Const reference to dateVal().
    inline const td::Date& getVal(td::Date)const{ return dateVal(); }
    /// @brief Return a const reference to the Time storage. @return Const reference to timeVal().
    inline const td::Time& getVal(td::Time)const{ return timeVal(); }
    /// @brief Return a const reference to the DateTime storage. @return Const reference to dtVal().
    inline const td::DateTime& getVal(td::DateTime)const{ return dtVal(); }
    /// @brief Return a const reference to the ChFix7 storage. @return Const reference to chFixVal().
    inline const td::ChFix7& getVal(td::ChFix7)const{ return chFixVal(); }
    /// @brief Return a const reference to the Color storage. @return Const reference to colorVal().
    inline const td::Color& getVal(td::Color)const{ return colorVal(); }
    /// @brief Return a const reference to the ColorID storage. @return Const reference to colorID().
    inline const td::ColorID& getVal(td::ColorID)const{ return colorID(); }
    /// @brief Return a const reference to the LinePattern storage. @return Const reference to linePattern().
    inline const td::LinePattern& getVal(td::LinePattern)const{ return linePattern(); }
    /// @brief Return a const reference to the DotPattern storage. @return Const reference to dotPattern().
    inline const td::DotPattern& getVal(td::DotPattern)const{ return dotPattern(); }
    /// @brief Return a const reference to the Anchor storage. @return Const reference to anchor().
    inline const td::Anchor& getVal(td::Anchor)const{ return anchor(); }
    /// @brief Return a const reference to the SmallDecimal0 storage. @return Const reference to _smallDecVal.
    inline const td::SmallDecimal0& getVal(td::SmallDecimal0)const{ return reinterpret_cast<const td::SmallDecimal0&>(_smallDecVal); }
    /// @brief Return a const reference to the SmallDecimal1 storage. @return Const reference to _smallDecVal.
    inline const td::SmallDecimal1& getVal(td::SmallDecimal1)const{ return reinterpret_cast<const td::SmallDecimal1&>(_smallDecVal); }
    /// @brief Return a const reference to the SmallDecimal2 storage. @return Const reference to _smallDecVal.
    inline const td::SmallDecimal2& getVal(td::SmallDecimal2)const{ return reinterpret_cast<const td::SmallDecimal2&>(_smallDecVal); }
    /// @brief Return a const reference to the SmallDecimal3 storage. @return Const reference to _smallDecVal.
    inline const td::SmallDecimal3& getVal(td::SmallDecimal3)const{ return reinterpret_cast<const td::SmallDecimal3&>(_smallDecVal); }
    /// @brief Return a const reference to the SmallDecimal4 storage. @return Const reference to _smallDecVal.
    inline const td::SmallDecimal4& getVal(td::SmallDecimal4)const{ return reinterpret_cast<const td::SmallDecimal4&>(_smallDecVal); }
    /// @brief Return a const reference to the Decimal0 storage. @return Const reference to _decVal.
    inline const td::Decimal0& getVal(td::Decimal0)const{ return reinterpret_cast<const td::Decimal0&>(_decVal); }
    /// @brief Return a const reference to the Decimal1 storage. @return Const reference to _decVal.
    inline const td::Decimal1& getVal(td::Decimal1)const{ return reinterpret_cast<const td::Decimal1&>(_decVal); }
    /// @brief Return a const reference to the Decimal2 storage. @return Const reference to _decVal.
    inline const td::Decimal2& getVal(td::Decimal2)const{ return reinterpret_cast<const td::Decimal2&>(_decVal); }
    /// @brief Return a const reference to the Decimal3 storage. @return Const reference to _decVal.
    inline const td::Decimal3& getVal(td::Decimal3)const{ return reinterpret_cast<const td::Decimal3&>(_decVal); }
    /// @brief Return a const reference to the Decimal4 storage. @return Const reference to _decVal.
    inline const td::Decimal4& getVal(td::Decimal4)const{ return reinterpret_cast<const td::Decimal4&>(_decVal); }

public:

    /// @brief Read the stored value into an output parameter of the matching type.
    /// @tparam TIN Expected value type; must match the current DataType.
    /// @param val Output parameter that receives the stored value.
    template <typename TIN>
    void getValue(TIN& val) const
    {
        val = getVal(val);
    }

    /// @brief Assign a C string to this Variant (only valid when the current type is string8).
    /// @param pStrIn Null-terminated string to store.
    void setValue(const char* pStrIn)
    {
        td::DataType thisType = getType();
        td::DataType varType = td::string8;
        assert(varType == thisType);
        if (varType != thisType)
            return;
        strVal() = pStrIn;
    }

    /// @brief Assign a td::String to this Variant (only valid when the current type is string8).
    /// @param strIn Source String to store.
    void setValue(const td::String& strIn)
    {
        td::DataType thisType = getType();
        td::DataType varType = td::string8;
        assert(varType == thisType);
        if (varType != thisType)
            return;
        strVal() = strIn;
    }

    /// @brief Copy the value from another Variant of the same type into this one.
    /// @param var Source Variant; must have the same DataType as this.
    void setValue(const td::Variant& var)
    {
        td::DataType thisType = getType();
        td::DataType varType = var.getType();
        assert(varType == thisType);
        if (varType != thisType)
            return;
        if (thisType == td::string8)
        {
            strVal() = var.strVal();
        }
        else if (thisType == td::string16)
        {
            assert(false);
        }
        else
            _lu8Val = var._lu8Val;
    }

    /// @brief Assign a typed value to this Variant (type must match current DataType).
    /// @tparam TVAL Value type to store.
    /// @param val Value to assign; its DataType must equal the current type.
    template <typename TVAL>
    void setValue(const TVAL& val)
    {
        td::DataType thisType = getType();
        td::DataType varType = td::getType(val);
        assert(varType == thisType);
        if (varType != thisType)
            return;
        TVAL& myVal = getVal(val);
        myVal = val;
    }

    /// @brief Return a null-terminated C string for string-typed Variants (string8 or ch7).
    /// @return Pointer to the stored character data; asserts for non-string types.
    const char* getConstStr() const
    {
        td::DataType type = getType();
        if (type == td::string8)
            return strVal().c_str();
        else if (type == td::ch7)
            return chFixVal().c_str();
        
        assert(false && "Variant::getConstStr() called on non string!"); //

        return nullptr;
    }

    //for  integration with Qt
    /// @brief Construct a Variant from a Qt QVariant.
    /// @param qVar Source QVariant to convert.
    inline Variant(const QVariant& qVar);

    /// @brief Assign a Qt QVariant to this Variant.
    /// @param qVar Source QVariant to convert.
    /// @return Reference to this Variant.
    inline Variant& operator = (const QVariant& qVar);

    /// @brief Convert this Variant to a Qt QVariant.
    /// @return A QVariant representing the same value.
    inline QVariant toQVariant() const;

    //for integration with Windows (Visual C++)
    /// @brief Assign a Windows COM VARIANT to this Variant.
    /// @param var Source tagVARIANT structure to convert.
    /// @return Reference to this Variant.
    inline Variant& operator = (const struct tagVARIANT& var);

    template <typename TVAR>
    friend TVAR operator + (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator - (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator * (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator / (const TVAR& var1, const TVAR& var2);

    /// @brief Serialize the stored value to an output stream.
    /// @tparam TSTREAM Output stream type.
    /// @param s Destination stream.
    /// @return Reference to the stream after writing.
    template <class TSTREAM>
    TSTREAM& toStream(TSTREAM& s) const
    {
        ToStreamVisitor<TSTREAM> vis(s);
        return applyConstVisitor<ToStreamVisitor<TSTREAM>, TSTREAM&>(vis, getType());
    }

    /// @brief Deserialize the stored value from an input stream.
    /// @tparam TSTREAM Input stream type.
    /// @param s Source stream to read from.
    /// @return Reference to the stream after reading.
    template <class TSTREAM>
    TSTREAM& fromStream(TSTREAM& s)
    {
        FromStreamVisitor<TSTREAM> vis(s);
        return applyVisitor<FromStreamVisitor<TSTREAM>, TSTREAM&>(vis, getType());
    }

    //save type and value only
    /// @brief Write the DataType byte and value to a binary stream.
    /// @tparam TSTREAM Binary output stream type supporting operator<<.
    /// @param s Destination binary stream.
    template <typename TSTREAM>
    inline void saveToBinaryStream(TSTREAM& s) const
    {
        td::DataType dt = getType();
        td::BYTE bw = (td::BYTE) dt;
        s << bw;
        toStream(s);
        //s << _flags;    //flags will be kept unchanged during serialization, only value will be stored,
    }

    //check if type is equal to requared and then load value
    /// @brief Read the DataType byte and value from a binary stream, verifying the type matches.
    /// @tparam TSTREAM Binary input stream type supporting operator>>.
    /// @param s Source binary stream.
    /// @return true if the stream DataType matched the current type and the value was loaded; false otherwise.
    template <typename TSTREAM>
    inline bool loadFromBinaryStream(TSTREAM& s)
    {
        td::BYTE bw = 0; // (td::BYTE) dt;
        s >> bw;
        td::DataType dtIn = (td::DataType) bw;
        td::DataType dt = getType();
        assert(dt == dtIn);
        if (dt != dtIn)
        {
            //set variant type to none and reset data
            //toZero();
            init(td::TD_NONE, true);
            return false;
        }
            
        fromStream(s);
        return true;
        //s >> _flags;
        //return s;
    }

    protected:
        template<class CharT, class Traits>
        friend std::basic_ostream<CharT, Traits>&
            operator<<(std::basic_ostream<CharT, Traits>& os,  const Variant& rhs)
        {
            return rhs.toStream(os);
        }

        template<class CharT, class Traits>
        friend std::basic_ostream<CharT, Traits>&
            operator>>(std::basic_ostream<CharT, Traits>& os, Variant& rhs)
        {
            return rhs.fromStream(os);
        }
};

/// @brief Return the DataType of a Variant (free-function overload for uniform getType() calls).
/// @param var Variant whose type is queried.
/// @return DataType of var.
inline td::DataType getType(const td::Variant& var)
{
    return var.getType();
}

/// @brief Assign the value of a td::Variant into this VariantBase via visitor dispatch.
/// @param var Source Variant to copy from.
inline void VariantBase::operator = (const td::Variant& var)
{
    BaseToVariantVisitor<td::Variant> vis(var);
    applyVisitor<BaseToVariantVisitor<td::Variant>, void>(vis, var.getType());
}

/// @brief Add two Variants, promoting to a common numeric type when needed.
/// @param var1 Left-hand operand.
/// @param var2 Right-hand operand.
/// @return A new Variant holding the sum.
inline td::Variant operator + (const td::Variant& var1, const td::Variant& var2)
{
    td::DataType t1 = td::getType(var1);
    td::DataType t2 = td::getType(var2);
    if (t1 == t2)
    {
        td::Variant v(var1);
        v += var2;
        return v;
    }
    else
    {
        if (td::isInteger(t1) && td::isInteger(t2))
        {
            td::Variant v(var1.template toNumber<td::INT4>()  + var2.template toNumber<td::INT4>());
            return v;
        }
    }

    td::Variant v(var1.template toNumber<double>() + var2.template toNumber<double> ());
    return v;
}

/// @brief Subtract two Variants, promoting to a common numeric type when needed.
/// @param var1 Left-hand operand.
/// @param var2 Right-hand operand.
/// @return A new Variant holding the difference.
inline td::Variant operator - (const td::Variant& var1, const td::Variant& var2)
{
    td::DataType t1 = td::getType(var1);
    td::DataType t2 = td::getType(var2);
    if (t1 == t2)
    {
        td::Variant v(var1);
        v -= var2;
        return v;
    }
    else
    {
        if (td::isInteger(t1) && td::isInteger(t2))
        {
            td::Variant v(var1.template toNumber<td::INT4> () - var2.template toNumber<td::INT4> ());
            return v;
        }
    }

    td::Variant v(var1.template toNumber<double>() - var2.template toNumber<double>());
    return v;
}

/// @brief Multiply two Variants, with boolean short-circuit and integer/float promotion.
/// @param var1 Left-hand operand.
/// @param var2 Right-hand operand.
/// @return A new Variant holding the product.
inline td::Variant operator * (const td::Variant& var1, const td::Variant& var2)
{
    td::DataType t1 = td::getType(var1);
    td::DataType t2 = td::getType(var2);
    if (t1 == t2)
    {
        td::Variant v(var1);
        v *= var2;
        return v;
    }
    else
    {
        if (t1 == td::boolean)
        {
            if (var1.boolVal())
                return var2;
            else
                return td::Variant(t2);
        }
        else if (t2 == td::boolean)
        {
            if (var2.boolVal())
                return var1;
            else
                return td::Variant(t1);
        }
        else if (td::isInteger(t1) && td::isInteger(t2))
        {
            td::Variant v(var1.template toNumber<td::INT4>() * var2.template toNumber<td::INT4>());
            return v;

        }
    }

    td::Variant v(var1.template toNumber<double>() * var2.template toNumber<double>());
    return v;
}

/// @brief Divide two Variants using integer or floating-point division based on type.
/// @param var1 Dividend.
/// @param var2 Divisor.
/// @return A new Variant holding the quotient.
inline td::Variant operator / (const td::Variant& var1, const td::Variant& var2)
{
    td::DataType t1 = td::getType(var1);
    td::DataType t2 = td::getType(var2);

    if (td::isInteger(t1) && td::isInteger(t2))
    {
        td::Variant v(var1.template toNumber<td::INT4>() / var2.template toNumber<td::INT4>());
        return v;
    }

    td::Variant v(var1.template toNumber<double>() / var2.template toNumber<double>());
    return v;
}

/// @brief Assign the value stored in a Variant to this StringBase (out-of-class definition).
/// @tparam T_CHAR Character type of the StringBase.
/// @tparam Encoding String encoding of the StringBase.
/// @tparam SPACE_FOR_SIZE Internal size-storage parameter.
/// @tparam EXTERN_ALLOCATOR Whether an external allocator is used (must be false).
/// @param var Source Variant whose string value is extracted.
/// @return Reference to this StringBase.
template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
inline td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::operator = (const td::Variant& var)
{
    static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
    assert(!EXTERN_ALLOCATOR);
    td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str = *this;
    var.template getValue<td::StringBase< T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR> >(str);
    return *this;
}

/// @brief Assign the value stored in a Variant to this SmallChFix (out-of-class definition).
/// @tparam N Fixed character capacity of the SmallChFix.
/// @param var Source Variant whose ch7 value is extracted.
/// @return Reference to this SmallChFix.
template <size_t N>
inline SmallChFix<N>& SmallChFix<N>::operator = (const Variant& var)
{
    SmallChFix& chFix = *this;
    var.getValue(chFix);
    return *this;
}

} //namespace td
