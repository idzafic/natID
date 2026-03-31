// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/VariantBase.h>
#include <mu/Regionals.h>
#include <td/VariantVisitors.h>
#include <td/Conv.h>

class QVariant;
struct tagVARIANT;

#define TDVAR_TYPE_MASK   0x000000FF
#define TDVAR_DBTYPE_MASK 0x0000FF00
#define TDVAR_ISNULL	  0x00010000
#define TDVAR_TOKENTYPE   0xFF000000

namespace td
{
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
    td::LUINT8 _flags;
public:
    inline DataType getType() const
    {
        return (td::DataType) (_flags & TDVAR_TYPE_MASK);
    }

    inline DataType getDBType() const
    {
        //if (getType() == td::boolean)
        //	return td::chFix;
        return (td::DataType) ((_flags >> 8) & TDVAR_TYPE_MASK);
    }

    inline void setTokenType(int type)
    {
        _flags &= 0xFFFFFFFF00FFFFFFUL;
        td::LUINT8 mask = type & 0x000000FF;
        mask <<= 24;
        _flags |= mask;
    }

    inline int getTokenType() const
    {
        return (int) ((_flags >> 24) & 0x000000FF);
    }

    inline void setDBLen(td::UINT4 dbLen)
    {
        _flags &= 0x00000000FFFFFFFFUL;
        td::LUINT8 len = (td::LUINT8) dbLen;
        len <<= 32;
        _flags |= len;
    }

    inline td::UINT4 getDBLen() const
    {
        //if (getType() == td::boolean)
        //	return 1;

        td::LUINT8 len = _flags >> 32;
        td::UINT4 lenToRet = (td::UINT4)len;
        return lenToRet;
    }

    inline bool isNumeric() const
    {
        td::DataType dt = getType();
        return td::isNumeric(dt);
    }

    inline bool isInteger() const
    {
        td::DataType dt = getType();
        return td::isInteger(dt);
    }

    inline bool isFloat() const
    {
        td::DataType dt = getType();
        return td::isFloat(dt);
    }

    inline bool isDateTime() const
    {
        td::DataType dt = getType();
        return td::isDateTime(dt);
    }

    inline bool isDBString() const
    {
        td::DataType dt = getType();
        return td::isDBString(dt);
    }

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
    //	_flags = type;
    //}

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
            
    inline void setInitialType(td::DataType dtp)
    {
        //if (dtp == td::ch7)
        //{
        //	_flags = dtp;
        //	setDBLen(7);
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
    
    inline void setType(td::DataType dtp)
    {
        _flags &= ~TDVAR_TYPE_MASK;
        //if (dtp == td::ch7)
        //{
        //	_flags |= (td::LUINT8)dtp;
        //	setDBLen(7);
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

    inline void clean()
    {
        if (getType() == td::string8)
        {
            td::String& str = (td::String&) _strVal;
            if (str.relRef())
            {
                delete [] _strVal;
            }
        }
        _strVal = nullptr;
    }

    inline void takeFromString(const td::String& val)
    {
        clean();
        _strVal = (char*) val.buffer;

        td::String& str = (td::String&) _strVal;
        if (!str.addRef())
        {
            td::String strTmp = val.clone();
            td::UINT4  nRefsTmp = strTmp.getNoOfRefs();
//            strTmp.clone(val);
//            str.addRef();
            _strVal = strTmp.buffer;
            td::String& str2 = (td::String&) _strVal;
            str2.addRef();
            td::UINT4  nRefsFinal = str2.getNoOfRefs();
            assert(nRefsFinal == 1);
        }
        setType(td::string8);
    }

public:
    Variant(void)
        : VariantBase()
        , _flags(td::TD_NONE)
    {
    }

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

    Variant(td::DataType varType)
        : VariantBase()
    {
        setInitialType(varType);
    }

    Variant(td::DataType varType, const td::VariantBase& vb)
        : VariantBase(vb)
    {
        setInitialType(varType);
    }

    Variant(td::DataType varType, const td::VariantBase& vb, bool addRef)
        : VariantBase(vb)
    {
        setInitialType(varType);
        if (addRef)
        {
            if (varType == td::string8)
            {
                td::String& str = (td::String&) _strVal;
                str.addRef();
            }
        }
    }
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

    Variant(td::DataType varType, bool isNullIn)
        : VariantBase()
    {
        setInitialType(varType);

        if (isNullIn)
            setAsNull();
    }

    Variant(bool val)
        : VariantBase(val)
        , _flags(td::boolean)
    {
        //_boolVal = val;
    }
    
    Variant(void* pPtr)
        : VariantBase(pPtr)
        , _flags(td::ptrVoid)
    {
        //_boolVal = val;
    }

    Variant(td::BYTE val)
        : VariantBase(val)
        , _flags(td::byte)
    {
        //_byteVal = val;
    }

    Variant(td::WORD val)
        : VariantBase(val)
        , _flags(td::word)
    {
        //_wordVal = val;
    }

    Variant(td::INT2 val)
        : VariantBase(val)
        , _flags(td::int2)
    {
        //_i2Val = val;
    }

    Variant(td::INT4 val)
        : VariantBase(val)
        , _flags(td::int4)
    {
        //_i4Val = val;
    }

    Variant(td::UINT4 val)
        : VariantBase(val) //: _u4Val(val)
        , _flags(td::uint4)
    {
        //_u4Val = val;
    }

    Variant(td::LUINT8 val)
        : VariantBase(val)
        , _flags(td::luint8)
    {
    }

    Variant(td::LINT8 val)
        : VariantBase(val)
        , _flags(td::lint8)
    {
    }

    Variant(float val)
        : VariantBase(val) //: _r4Val(val)
        , _flags(td::real4)
    {
        _r4Val = val;
    }

    Variant(double val)
        : VariantBase(val)
        , _flags(td::real8)
    {
    }

    Variant(td::Date val)
        : VariantBase(val)  //: _dateVal(val.getValue())
        , _flags(td::date)
    {
    }

    Variant(td::Time val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::time)
    {
    }
    
    Variant(td::Color val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::color)
    {
    }
    
    Variant(td::ColorID val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::colorID)
    {
    }
    
    Variant(td::LinePattern val)
        : VariantBase(val) //: _timeVal(val.getValue())
        , _flags(td::linePattern)
    {
    }

    Variant(const td::DateTime& val)
        : VariantBase(val)
        , _flags(td::dateTime)
    {
    }

    
    Variant(const td::String& val)
        : VariantBase(val) //: _strVal(nullptr)
        , _flags(td::string8)
    {
        //strVal() = val;
    }

    Variant(const td::String& val, td::DataType dbType, td::WORD dbLen)
        : VariantBase(val) //: _strVal(nullptr)
        , _flags(td::string8)
    {
        //strVal() = val;
        assert(dbType == td::chFix || dbType == td::ch || dbType == td::nch || dbType == td::nchFix);
        assert(dbLen > 0);
        setDBTypeAndLen(dbType, dbLen);
    }
    
    template<int NDEC >
    Variant(const Decimal<td::LINT8, NDEC>& val)
        : VariantBase(val)
        , _flags(td::getType(val))
    {
    }

    template<int NDEC >
    Variant(const Decimal<td::INT4, NDEC>& val)
        : VariantBase(val)  //: _decVal(val.getValue())
        , _flags(td::getType(val))
    {
        //_sdecVal(val.getValue());
    }

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
    
    inline Variant(const char* pStr)
        : VariantBase()
        , _flags(td::string8)
    {
        strVal() = pStr;
    }

    ~Variant()
    {
        clean();
    }

    bool isNull() const
    {
        return ( (_flags & TDVAR_ISNULL) != 0);
    }

    bool isNonZero() const
    {
        if (isZero())
            return false;
        if (isNull())
            return false;
        return true;
    }

    void setAsNull()
    {
        _flags |= TDVAR_ISNULL;
    }

    
//    bool& boolVal()
//    {
//    	assert(getType() == td::boolean);
//    	return _boolVal;
//    }

    bool boolVal() const
    {
        assert(getType() == td::boolean);
        return (_bVal == 'Y');
    }

    BYTE& byteVal()
    {
        assert(getType() == td::byte);
        return _byteVal;
    }

    BYTE byteVal() const
    {
        assert(getType() == td::byte);
        return _byteVal;
    }

    WORD& wordVal()
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    WORD wordVal() const
    {
        assert(getType() == td::word);
        return _wordVal;
    }
    
    WORD& u2Val()
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    WORD u2Val() const
    {
        assert(getType() == td::word);
        return _wordVal;
    }

    INT2& i2Val()
    {
        assert(getType() == td::int2);
        return _i2Val;
    }

    INT2 i2Val() const
    {
        assert(getType() == td::int2);
        return _i2Val;
    }

    UINT4& u4Val()
    {
        assert(getType() == td::uint4);
        return _u4Val;
    }

    UINT4 u4Val() const
    {
        assert(getType() == td::uint4);
        return _u4Val;
    }

    INT4& i4Val()
    {
        assert(getType() == td::int4);
        return _i4Val;
    }

    INT4 i4Val() const
    {
        assert(getType() == td::int4);
        return _i4Val;
    }

    LUINT8& lu8Val()
    {
        assert(getType() == td::luint8);
        return _lu8Val;
    }

    LUINT8 lu8Val() const
    {
        assert(getType() == td::luint8);
        return _lu8Val;
    }

    LINT8& li8Val()
    {
        assert(getType() == td::lint8);
        return _li8Val;
    }

    LINT8 li8Val() const
    {
        assert(getType() == td::lint8);
        return _li8Val;
    }

    float& r4Val()
    {
        assert(getType() == td::real4);
        return _r4Val;
    }

    float r4Val() const
    {
        assert(getType() == td::real4);
        return _r4Val;
    }

    double& r8Val()
    {
        assert(getType() == td::real8);
        return _r8Val;
    }

    double r8Val() const
    {
        assert(getType() == td::real8);
        return _r8Val;
    }

    Date& dateVal()
    {
        assert(getType() == td::date);
        return (Date&)(_dateVal);
    }

    const Date& dateVal() const
    {
        assert(getType() == td::date);
        return (Date&)(_dateVal);
    }
    
    Color& colorVal()
    {
        assert(getType() == td::color);
        return (Color&)(_colorVal);
    }

    const Color& colorVal() const
    {
        assert(getType() == td::color);
        return (Color&)(_colorVal);
    }
    
    ColorID& colorID()
    {
        assert(getType() == td::colorID);
        return _colorID;
    }
    
    const ColorID& colorID() const
    {
        assert(getType() == td::colorID);
        return _colorID;
    }
    
    LinePattern& linePattern()
    {
        assert(getType() == td::linePattern);
        return _linePattern;
    }

    const LinePattern& linePattern() const
    {
        assert(getType() == td::linePattern);
        return _linePattern;
    }
    
    DotPattern& dotPattern()
    {
        assert(getType() == td::dotPattern);
        return _dotPattern;
    }
    
    const DotPattern& dotPattern() const
    {
        assert(getType() == td::dotPattern);
        return _dotPattern;
    }
    
    Anchor& anchor()
    {
        assert(getType() == td::anchor);
        return _anchor;
    }
    
    const Anchor& anchor() const
    {
        assert(getType() == td::anchor);
        return _anchor;
    }
    
    Time& timeVal()
    {
        assert(getType() == td::time);
        return (Time&)(_timeVal);
    }

    const Time& timeVal() const
    {
        assert(getType() == td::time);
        return (Time&)(_timeVal);
    }

    DateTime& dtVal()
    {
        assert(getType() == td::dateTime);
        return (DateTime&) _dtVal;
    }

    const DateTime& dtVal() const
    {
        assert(getType() == td::dateTime);
        return (DateTime&) _dtVal;
    }

    template< typename THOLDER, int NDEC >
    void decVal(Decimal<THOLDER, NDEC>& toRet)
    {
        assert(getType() == td::getType(toRet));
        toRet.setValue(_decVal);
    }

    //DEC VALUE
    Decimal0& dec0Val()
    {
        assert(getType() == td::decimal0);
        return (Decimal0 &) _decVal;
    }

    const Decimal0& dec0Val() const
    {
        assert(getType() == td::decimal0);
        return (Decimal0 &) _decVal;
    }

    Decimal1& dec1Val()
    {
        assert(getType() == td::decimal1);
        return (Decimal1 &) _decVal;
    }

    const Decimal1& dec1Val() const
    {
        assert(getType() == td::decimal1);
        return (Decimal1 &) _decVal;
    }

    Decimal2& dec2Val()
    {
        assert(getType() == td::decimal2);
        return (Decimal2&) _decVal;
    }

    const Decimal2& dec2Val() const
    {
        assert(getType() == td::decimal2);
        return (Decimal2&) _decVal;
    }

    Decimal3& dec3Val()
    {
        assert(getType() == td::decimal3);
        return (Decimal3&) _decVal;
    }

    const Decimal3& dec3Val() const
    {
        assert(getType() == td::decimal3);
        return (Decimal3&) _decVal;
    }

    Decimal4& dec4Val()
    {
        assert(getType() == td::decimal4);
        return (Decimal4&) _decVal;
    }

    const Decimal4& dec4Val() const
    {
        assert(getType() == td::decimal4);
        return (Decimal4&) _decVal;
    }

    SmallDecimal0& sdec0Val()
    {
        assert(getType() == td::smallDecimal0);
        return (SmallDecimal0&) _i4Val;
    }

    const SmallDecimal0& sdec0Val() const
    {
        assert(getType() == td::smallDecimal0);
        return (SmallDecimal0&) _i4Val;
    }

    SmallDecimal1& sdec1Val()
    {
        assert(getType() == td::smallDecimal1);
        return (SmallDecimal1&) _i4Val;
    }

    const SmallDecimal1& sdec1Val() const
    {
        assert(getType() == td::smallDecimal1);
        return (SmallDecimal1&) _i4Val;
    }

    SmallDecimal2& sdec2Val()
    {
        assert(getType() == td::smallDecimal2);
        return (SmallDecimal2&) _i4Val;
    }

    const SmallDecimal2& sdec2Val() const
    {
        assert(getType() == td::smallDecimal2);
        return (SmallDecimal2&) _i4Val;
    }

    SmallDecimal3& sdec3Val()
    {
        assert(getType() == td::smallDecimal3);
        return (SmallDecimal3&) _i4Val;
    }

    const SmallDecimal3& sdec3Val() const
    {
        assert(getType() == td::smallDecimal3);
        return (SmallDecimal3&) _i4Val;
    }

    SmallDecimal4& sdec4Val()
    {
        assert(getType() == td::smallDecimal4);
        return (SmallDecimal4&) _i4Val;
    }

    const SmallDecimal4& sdec4Val() const
    {
        assert(getType() == td::smallDecimal4);
        return (SmallDecimal4&) _i4Val;
    }

    ChFix7& chFixVal()
    {
        assert(getType() == td::ch7);
        return (ChFix7&)_chVal;
    }

    const ChFix7& chFixVal() const
    {
        assert(getType() == td::ch7);
        return (ChFix7&)_chVal;
    }

    //END DEC VALUE

    // SMALL DEC
    // TODO: Odraditi za small dec value
    // END MALLDEC
    td::String& strVal()
    {
        assert(getType() == td::string8);
        return (td::String&) _strVal;
    }

    const td::String& strVal() const
    {
        assert(getType() == td::string8);
        return (td::String&) _strVal;
    }

    void*& voidPtr()
    {
        assert(getType() == td::ptrVoid);
        return _voidPtr;
    }

    const void* voidPtr() const
    {
        assert(getType() == td::ptrVoid);
        return _voidPtr;
    }

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

    Variant& operator = (BYTE val)
    {
        clean();
        _byteVal = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (WORD val)
    {
        clean();
        _wordVal = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (INT4 val)
    {
        clean();
        _i4Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (INT2 val)
    {
        clean();
        _i2Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (UINT4 val)
    {
        clean();
        _u4Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const LUINT8& val)
    {
        clean();
        _lu8Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const LINT8& val)
    {
        clean();
        _li8Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (float val)
    {
        clean();
        _r4Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const double& val)
    {
        clean();
        _r8Val = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const td::Date& val)
    {
        clean();
        _dateVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }
    
    Variant& operator = (const td::Color& val)
    {
        clean();
        _colorVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }
    
    Variant& operator = (td::ColorID val)
    {
        clean();
        _colorID = val;
        setType(td::getType(val));
        return *this;
    }
    
    Variant& operator = (td::LinePattern val)
    {
        clean();
        _linePattern = val;
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const td::Time& val)
    {
        clean();
        _timeVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }

    Variant& operator = (const td::DateTime& val)
    {
        clean();
        _dtVal = (td::LINT8&) val;
        setType(td::getType(val));
        return *this;
    }
    
    Variant& operator = (void* val)
    {
        clean();
        _voidPtr = val;
        setType(td::ptrVoid);
        return *this;
    }


    Variant& operator = (const td::String& val)
    {
        takeFromString(const_cast<td::String&>(val));
        return *this;
    }

    Variant& operator = (td::String& val)
    {
        takeFromString(val);
        return *this;
    }

    Variant& operator = (const char* val)
    {
        clean();
        if (val == nullptr)
            return *this;

        td::String str(val);

        takeFromString(str);

        return *this;
    }

    template<typename THOLDER, int NDEC >
    Variant& operator = (const Decimal<THOLDER, NDEC>& val)
    {
        clean();
        _decVal = val.getValue();
        setType(td::getType(val));
        return *this;
    }

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
    //	clean();

    //	td::DataType thisType(getType());
    //	td::DataType varType(var.getType());

    //	//_isNull = var._isNull;
    //	if (varType == td::string8)
    //	{
    //		if (thisType != td::string8)
    //			_flags = 0L;
    //		takeFromString(const_cast<td::String&>(var.strVal()));
    //	}
    //	else
    //	{
    //		_lu8Val = var._lu8Val;
    //	}

    //	//this prevents changing type due to implicit conversion to variant (_flags == 0) and
    //	if ( thisType != varType)
    //		_flags = var._flags;
    //	else
    //	{
    //		td::DataType thisDBType = getDBType();
    //		//td::DataType varDBType = var.getDBType();
    //		if (thisDBType == td::TD_NONE)
    //		{
    //			_flags = var._flags;
    //		}
    //	}
    //

    //	return *this;
    //}

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
    bool operator == (const Variant& var) const
    {
        if (var.getType() != getType())
            return false;
        EqualityVisitor<Variant> vis(var);
        return applyConstVisitor<EqualityVisitor<Variant>, bool>(vis, getType());
    }

    bool operator != (const Variant& var) const
    {
        if (var.getType() != getType())
            return true;

        EqualityVisitor<Variant> vis(var);
        return !applyConstVisitor<EqualityVisitor<Variant>, bool>(vis, getType());
    }
    
    bool operator < (const Variant& var) const
    {
        assert(var.getType() == getType());
        LessThanVisitor<Variant> vis(var);
        return applyConstVisitor<LessThanVisitor<Variant>, bool>(vis, getType());
    }

    bool operator > (const Variant& var) const
    {
        assert(var.getType() == getType());
        GreatherThanVisitor<Variant> vis(var);
        return applyConstVisitor<GreatherThanVisitor<Variant>, bool>(vis, getType());
    }

    bool operator <= (const Variant& var) const
    {
        assert(var.getType() == getType());
        LessEqualThanVisitor<Variant> vis(var);
        return applyConstVisitor<LessEqualThanVisitor<Variant>, bool>(vis, getType());
    }

    bool operator >= (const Variant& var) const
    {
        assert(var.getType() == getType());
        GreatherEqualThanVisitor<Variant> vis(var);
        return applyConstVisitor<GreatherEqualThanVisitor<Variant>, bool>(vis, getType());
    }

    bool isZero() const
    {
        IsZeroVisitor vis;
        return applyConstVisitor<IsZeroVisitor, bool>(vis, getType());

        //return _lu8Val == 0;
    }

    template <typename TVAL>
    bool operator == (const TVAL& val) const
    {
        return getVal(val) == val;
    }

    template <typename TVAL>
    bool operator > (const TVAL& val) const
    {
        return getVal(val) > val;
    }

    template <typename TVAL>
    bool operator < (const TVAL& val) const
    {
        return getVal(val) < val;
    }

    template <typename TVAL>
    bool operator >= (const TVAL& val) const
    {
        return getVal(val) >= val;
    }

    template <typename TVAL>
    bool operator <= (const TVAL& val) const
    {
        return getVal(val) <= val;
    }

    template <typename TVAL>
    bool operator != (const TVAL& val) const
    {
        return getVal(val) != val;
    }

    bool*& ptrBool()
    {
        assert(getType() == td::ptrBoolean);
        return _ptrBool;
    }
    const bool* ptrBool() const
    {
        assert(getType() == td::ptrBoolean);
        return _ptrBool;
    }

    BYTE*& ptrByte()
    {
        assert(getType() == td::ptrByte);
        return _ptrByte;
    }

    const BYTE* ptrByte() const
    {
        assert(getType() == td::ptrByte);
        return _ptrByte;
    }
    
    WORD*& ptrWord()
    {
        assert(getType() == td::ptrWord);
        return _ptrWord;
    }

    const WORD* ptrWord() const
    {
        assert(getType() == td::ptrWord);
        return _ptrWord;
    }
    INT2*& ptrI2()
    {
        assert(getType() == td::ptrInt2);
        return _ptrI2;
    }

    const INT2* ptrI2() const
    {
        assert(getType() == td::ptrInt2);
        return _ptrI2;
    }
    UINT4*& ptrU4()
    {
        assert(getType() == td::ptrUint4);
        return _ptrU4;
    }

    const UINT4* ptrU4() const
    {
        assert(getType() == td::ptrUint4);
        return _ptrU4;
    }
    INT4*& ptrI4()
    {
        assert(getType() == td::ptrInt4);
        return _ptrI4;
    }
    const INT4* ptrI4() const
    {
        assert(getType() == td::ptrUint4);
        return _ptrI4;
    }

    LUINT8*& ptrLU8()
    {
        assert(getType() == td::ptrLuint8);
        return _ptrLU8;
    }

    const LUINT8* ptrLU8() const
    {
        assert(getType() == td::ptrLuint8);
        return _ptrLU8;
    }
    LINT8*& ptrLI8()
    {
        assert(getType() == td::ptrLint8);
        return _ptrLI8;
    }
    const LINT8* ptrLI8() const
    {
        assert(getType() == td::ptrLint8);
        return _ptrLI8;
    }

    float*& ptrR4()
    {
        assert(getType() == td::ptrReal4);
        return _ptrR4;
    }
    const float* ptrR4() const
    {
        assert(getType() == td::ptrReal4);
        return _ptrR4;
    }

    double*& ptrR8()
    {
        assert(getType() == td::ptrReal8);
        return _ptrR8;
    }
    const double* ptrR8() const
    {
        assert(getType() == td::ptrReal8);
        return _ptrR8;
    }
    Date*& ptrDate()
    {
        assert(getType() == td::ptrDate);
        return _ptrDate;
    }
    const Date* ptrDate() const
    {
        assert(getType() == td::ptrDate);
        return _ptrDate;
    }
    Time*& ptrTime()
    {
        assert(getType() == td::ptrTime);
        return _ptrTime;
    }
    const Time* ptrTime() const
    {
        assert(getType() == td::ptrTime);
        return _ptrTime;
    }
    DateTime*& ptrDT()
    {
        assert(getType() == td::ptrDateTime);
        return _ptrDT;
    }

    const DateTime* ptrDT() const
    {
        assert(getType() == td::ptrDateTime);
        return _ptrDT;
    }
    Decimal0*& ptrDec0()
    {
        assert(getType() == td::ptrDecimal0);
        return _ptrDec0;
    }
    const Decimal0* ptrDec0() const
    {
        assert(getType() == td::ptrDecimal0);
        return _ptrDec0;
    }
    Decimal1*& ptrDec1()
    {
        assert(getType() == td::ptrDecimal1);
        return _ptrDec1;
    }
    const Decimal1* ptrDec1() const
    {
        assert(getType() == td::ptrDecimal1);
        return _ptrDec1;
    }
    Decimal2*& ptrDec2()
    {
        assert(getType() == td::ptrDecimal2);
        return _ptrDec2;
    }
    const Decimal2* ptrDec2() const
    {
        assert(getType() == td::ptrDecimal2);
        return _ptrDec2;
    }
    Decimal3*& ptrDec3()
    {
        assert(getType() == td::ptrDecimal3);
        return _ptrDec3;
    }
    const Decimal3* ptrDec3() const
    {
        assert(getType() == td::ptrDecimal3);
        return _ptrDec3;
    }
    Decimal4*& ptrDec4()
    {
        assert(getType() == td::ptrDecimal4);
        return _ptrDec4;
    }
    const Decimal4* ptrDec4() const
    {
        assert(getType() == td::ptrDecimal4);
        return _ptrDec4;
    }
    SmallDecimal0*& ptrSDec0()
    {
        assert(getType() == td::ptrSmalldecimal0);
        return _ptrSDec0;
    }
    const SmallDecimal0* ptrSDec0() const
    {
        assert(getType() == td::ptrSmalldecimal0);
        return _ptrSDec0;
    }
    SmallDecimal1*& ptrSDec1()
    {
        assert(getType() == td::ptrSmalldecimal1);
        return _ptrSDec1;
    }
    const SmallDecimal1* ptrSDec1() const
    {
        assert(getType() == td::ptrSmalldecimal1);
        return _ptrSDec1;
    }
    SmallDecimal2*& ptrSDec2()
    {
        assert(getType() == td::ptrSmalldecimal2);
        return _ptrSDec2;
    }
    const SmallDecimal2* ptrSDec2() const
    {
        assert(getType() == td::ptrSmalldecimal2);
        return _ptrSDec2;
    }
    SmallDecimal3*& ptrSDec3()
    {
        assert(getType() == td::ptrSmalldecimal3);
        return _ptrSDec3;
    }
    const SmallDecimal3* ptrSDec3() const
    {
        assert(getType() == td::ptrSmalldecimal3);
        return _ptrSDec3;
    }
    SmallDecimal4*& ptrSDec4()
    {
        assert(getType() == td::ptrSmalldecimal4);
        return _ptrSDec4;
    }
    const SmallDecimal4* ptrSDec4() const
    {
        assert(getType() == td::ptrSmalldecimal4);
        return _ptrSDec4;
    }
    Variant*& ptrVar()
    {
        assert(getType() == td::ptrVariant);
        return _ptrVar;
    }
    const Variant* ptrVar() const
    {
        assert(getType() == td::ptrVariant);
        return _ptrVar;
    }

    Variant& operator = (bool* val)
    {
        clean();
        _ptrBool = val;
        setType(td::ptrBoolean);
        return *this;
    }

    Variant& operator = (BYTE* val)
    {
        clean();
        _ptrByte = val;
        setType(td::ptrByte);
        return *this;
    }

    Variant& operator = (WORD* val)
    {
        clean();
        _ptrWord = val;
        setType(td::ptrWord);
        return *this;
    }
            
    Variant& operator = (INT2* val)
    {
        clean();
        _ptrI2 = val;
        setType(td::ptrInt2);
        return *this;
    }
    Variant& operator = (UINT4* val)
    {
        clean();
        _ptrU4 = val;
        setType(td::ptrUint4);
        return *this;
    }
    Variant& operator = (INT4* val)
    {
        clean();
        _ptrI4 = val;
        setType(td::ptrInt4);
        return *this;
    }
    Variant& operator = (LUINT8* val)
    {
        clean();
        _ptrLU8 = val;
        setType(td::ptrLuint8);
        return *this;
    }
    
    Variant& operator = (LINT8* val)
    {
        clean();
        _ptrLI8 = val;
        setType(td::ptrLint8);
        return *this;
    }
    
    Variant& operator = (float* val)
    {
        clean();
        _ptrR4 = val;
        setType(td::ptrReal4);
        return *this;
    }
    Variant& operator = (double* val)
    {
        clean();
        _ptrR8 = val;
        setType(td::ptrReal8);
        return *this;
    }
    Variant& operator = (Date* val)
    {
        clean();
        _ptrDate = val;
        setType(td::ptrDate);
        return *this;
    }
    
    Variant& operator = (Time* val)
    {
        clean();
        _ptrTime = val;
        setType(td::ptrTime);
        return *this;
    }
    
    Variant& operator = (DateTime* val)
    {
        clean();
        _ptrDT = val;
        setType(td::ptrDateTime);
        return *this;
    }
    Variant& operator = (Decimal0* val)
    {
        clean();
        _ptrDec0 = val;
        setType(td::ptrDecimal0);
        return *this;
    }
    Variant& operator = (Decimal1* val)
    {
        clean();
        _ptrDec1 = val;
        setType(td::ptrDecimal1);
        return *this;
    }
    Variant& operator = (Decimal2* val)
    {
        clean();
        _ptrDec2 = val;
        setType(td::ptrDecimal2);
        return *this;
    }
    Variant& operator = (Decimal3* val)
    {
        clean();
        _ptrDec3 = val;
        setType(td::ptrDecimal3);
        return *this;
    }
    Variant& operator = (Decimal4* val)
    {
        clean();
        _ptrDec4 = val;
        setType(td::ptrDecimal4);
        return *this;
    }

    Variant& operator = (SmallDecimal0* val)
    {
        clean();
        _ptrSDec0 = val;
        setType(td::ptrSmalldecimal0);
        return *this;
    }
    Variant& operator = (SmallDecimal1* val)
    {
        clean();
        _ptrSDec1 = val;
        setType(td::ptrSmalldecimal1);
        return *this;
    }
    Variant& operator = (SmallDecimal2* val)
    {
        clean();
        _ptrSDec2 = val;
        setType(td::ptrSmalldecimal2);
        return *this;
    }
    Variant& operator = (SmallDecimal3* val)
    {
        clean();
        _ptrSDec3 = val;
        setType(td::ptrSmalldecimal3);
        return *this;
    }
    Variant& operator = (SmallDecimal4* val)
    {
        clean();
        _ptrSDec4 = val;
        setType(td::ptrSmalldecimal4);
        return *this;
    }
    Variant& operator = (Variant* val)
    {
        clean();
        _ptrVar = val;
        setType(td::ptrVariant);
        return *this;
    }

    Variant& operator = (const td::VariantBase& vb)
    {
        BaseToVariantVisitor<td::VariantBase> vis(vb);
        applyVisitor<BaseToVariantVisitor<td::VariantBase>, void>(vis, getType());
        return *this;
    }

    void init(td::DataType dtp, bool toZero = false)
    {
        clean();
        setInitialType(dtp);

        if (dtp == td::ch7)
        {
            setDBTypeAndLen(td::chFix, 7);
        }
        //if ((dtp == td::ch) || ((dtp == td::nch) || (dtp == td::chFix) || (dtp == td::nchFix)))
        //	_flags = td::string8;
        //else
        //	_flags = dtp;
        if (toZero)
            _lu8Val = 0;
    }

    inline void toZero()
    {
        if (getType() == td::string8)
        {
            td::String& str = (td::String&) _strVal;
            if (str.relRef())
            {
                delete[] _strVal;
            }
            _strVal = nullptr;
        }
        else
            _lu8Val = 0;
    }


    
    template <class TUTIL>
    const char* c_str(TUTIL& util) const
    {
        ToCStrVisitor<TUTIL> vis(util);
        return applyConstVisitor<ToCStrVisitor<TUTIL>, const char*>(vis, getType());
    }
    
    
    template <class TREG>
    const char* c_str(TREG* pReg, int nDecOrTimeFormat, int showThousendSepOrTimeFmt = 1, td::FormatFloat floatFmt= td::FormatFloat::Decimal) const
    {
        FormatCStrVisitor<TREG> vis(pReg, nDecOrTimeFormat, showThousendSepOrTimeFmt, floatFmt);
        return applyConstVisitor<FormatCStrVisitor<TREG>, const char*>(vis, getType());
    }


    //void fromString(const char* str)
    void setValueFromString(const char* str)
    {
        FromStringVisitor vis(str);
        applyVisitor<FromStringVisitor, void>(vis, getType());
    }

    void setValueFromString(const td::String& str)
    {
        setValueFromString(str.c_str());
    }

    template <typename TNUM>
    void setValueFromNumeric(const TNUM& numVal)
    //void fromNumeric(const TNUM& val)
    {
        FromNumericVisitor<TNUM> vis(numVal);
        applyVisitor<FromNumericVisitor<TNUM>, void>(vis, getType());
    }

    template <typename TINTEGER>
    void setTo(TINTEGER val)
    {
        SetToIntegerVisitor<TINTEGER> vis(val);
        applyVisitor<SetToIntegerVisitor<TINTEGER>, void>(vis, getType());
    }

    template <typename TNUM>
    inline TNUM toNumber() const
    {
        ToNumberVisitor<TNUM> vis;
        return applyConstVisitor<ToNumberVisitor<TNUM>, TNUM>(vis, getType());
    }
    
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
            
    inline td::String toString() const
    {
        ToStringVisitor vis;
        return applyConstVisitor<ToStringVisitor, td::String>(vis, getType());
    }

    inline void getValue(void* ptr, td::DataType pointerType) const
    {
        td::DataType dt = getType();
        assert(dt == pointerType);
        if (dt != pointerType)
            return;
        CopyToVoidPtrVisitor vis(ptr);
        applyConstVisitor<CopyToVoidPtrVisitor, void>(vis, getType());
    }

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

    inline const td::Variant& abs()
    {
        AbsVisitor vis;
        applyVisitor<AbsVisitor, void>(vis, getType());
        return *this;
    }
    
    inline const td::Variant& operator += (const td::Variant& var)
    {
        PlusEqVisitor<td::Variant> vis(var);
        applyVisitor<PlusEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    inline const td::Variant& operator -= (const td::Variant& var)
    {
        MinusEqVisitor<td::Variant> vis(var);
        applyVisitor<MinusEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    inline const td::Variant& operator *= (const td::Variant& var)
    {
        MultEqVisitor<td::Variant> vis(var);
        applyVisitor<MultEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }

    inline const td::Variant& operator /= (const td::Variant& var)
    {
        DivEqVisitor<td::Variant> vis(var);
        applyVisitor<DivEqVisitor<td::Variant>, void>(vis, getType());
        return *this;
    }
protected:
    inline td::BoolCh& getVal(td::BoolCh){ return (td::BoolCh&)_bVal; }

//    inline bool& getVal(bool){ return _boolVal; }
    
    inline td::BYTE& getVal(td::BYTE){ return byteVal(); }

    inline td::INT2& getVal(td::INT2){ return i2Val(); }

    inline td::WORD& getVal(td::WORD) { return wordVal(); }

    inline td::INT4& getVal(td::INT4) { return i4Val(); }

    inline td::UINT4& getVal(td::UINT4) { return u4Val(); }

    inline td::LINT8& getVal(td::LINT8){ return li8Val(); }

    inline td::LUINT8& getVal(td::LUINT8){ return lu8Val(); }

    inline float& getVal(float){ return r4Val(); }

    inline double& getVal(double){ return r8Val(); }

    inline td::String& getVal(td::String&){ return strVal(); }

    inline td::Date& getVal(td::Date){ return dateVal(); }

    inline td::Time& getVal(td::Time){ return timeVal(); }

    inline td::DateTime& getVal(td::DateTime){ return dtVal(); }

    inline td::ChFix7& getVal(td::ChFix7){ return chFixVal(); }
    
    inline td::Color& getVal(td::Color){ return colorVal(); }
    
    inline td::ColorID& getVal(td::ColorID){ return colorID(); }
    
    inline td::LinePattern& getVal(td::LinePattern){ return linePattern(); }
    inline td::DotPattern& getVal(td::DotPattern){ return dotPattern(); }
    inline td::Anchor& getVal(td::Anchor){ return anchor(); }
                    
    inline td::SmallDecimal0& getVal(td::SmallDecimal0){ return sdec0Val(); }
    inline td::SmallDecimal1& getVal(td::SmallDecimal1){ return sdec1Val(); }
    inline td::SmallDecimal2& getVal(td::SmallDecimal2){ return sdec2Val(); }
    inline td::SmallDecimal3& getVal(td::SmallDecimal3){ return sdec3Val(); }
    inline td::SmallDecimal4& getVal(td::SmallDecimal4){ return sdec4Val(); }

    inline td::Decimal0& getVal(td::Decimal0){ return dec0Val(); }
    inline td::Decimal1& getVal(td::Decimal1){ return dec1Val(); }
    inline td::Decimal2& getVal(td::Decimal2){ return dec2Val(); }
    inline td::Decimal3& getVal(td::Decimal3){ return dec3Val(); }
    inline td::Decimal4& getVal(td::Decimal4){ return dec4Val(); }


    //const
    inline const td::BoolCh& getVal(td::BoolCh)const { return (td::BoolCh&)_bVal; }
    
    inline td::INT4 getVal(bool)const { return boolVal(); }

    inline td::BYTE getVal(td::BYTE)const{ return byteVal(); }

    inline td::INT2 getVal(td::INT2)const{ return i2Val(); }

    inline td::WORD getVal(td::WORD)const { return wordVal(); }

    inline td::INT4 getVal(td::INT4)const { return i4Val(); }

    inline td::UINT4 getVal(td::UINT4) const{ return u4Val(); }

    inline td::LINT8 getVal(td::LINT8)const{ return li8Val(); }

    inline td::LUINT8 getVal(td::LUINT8)const{ return lu8Val(); }

    inline float getVal(float)const{ return r4Val(); }

    inline double getVal(double)const{ return r8Val(); }

    inline const td::String& getVal(const td::String&) const { return strVal(); }

    //inline const td::String& getVal(td::String&) const { return strVal(); }

    inline const td::Date& getVal(td::Date)const{ return dateVal(); }

    inline const td::Time& getVal(td::Time)const{ return timeVal(); }

    inline const td::DateTime& getVal(td::DateTime)const{ return dtVal(); }

    inline const td::ChFix7& getVal(td::ChFix7)const{ return chFixVal(); }
    
    inline const td::Color& getVal(td::Color)const{ return colorVal(); }
    inline const td::ColorID& getVal(td::ColorID)const{ return colorID(); }
    inline const td::LinePattern& getVal(td::LinePattern)const{ return linePattern(); }
    inline const td::DotPattern& getVal(td::DotPattern)const{ return dotPattern(); }
    inline const td::Anchor& getVal(td::Anchor)const{ return anchor(); }

    inline const td::SmallDecimal0& getVal(td::SmallDecimal0)const{ return (td::SmallDecimal0&) _smallDecVal; }
    inline const td::SmallDecimal1& getVal(td::SmallDecimal1)const{ return (td::SmallDecimal1&) _smallDecVal; }
    inline const td::SmallDecimal2& getVal(td::SmallDecimal2)const{ return (td::SmallDecimal2&) _smallDecVal; }
    inline const td::SmallDecimal3& getVal(td::SmallDecimal3)const{ return (td::SmallDecimal3&) _smallDecVal; }
    inline const td::SmallDecimal4& getVal(td::SmallDecimal4)const{ return (td::SmallDecimal4&) _smallDecVal; }

    inline const td::Decimal0& getVal(td::Decimal0)const{ return (td::Decimal0&) _decVal; }
    inline const td::Decimal1& getVal(td::Decimal1)const{ return (td::Decimal1&) _decVal; }
    inline const td::Decimal2& getVal(td::Decimal2)const{ return (td::Decimal2&) _decVal; }
    inline const td::Decimal3& getVal(td::Decimal3)const{ return (td::Decimal3&) _decVal; }
    inline const td::Decimal4& getVal(td::Decimal4)const{ return (td::Decimal4&) _decVal; }

public:

    template <typename TIN>
    void getValue(TIN& val) const
    {
        val = getVal(val);
    }

    void setValue(const char* pStrIn)
    {
        td::DataType thisType = getType();
        td::DataType varType = td::string8;
        assert(varType == thisType);
        if (varType != thisType)
            return;
        strVal() = pStrIn;
    }

    void setValue(const td::String& strIn)
    {
        td::DataType thisType = getType();
        td::DataType varType = td::string8;
        assert(varType == thisType);
        if (varType != thisType)
            return;
        strVal() = strIn;
    }

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
    inline Variant(const QVariant& qVar);
    inline Variant& operator = (const QVariant& qVar);
    inline QVariant toQVariant() const;
    
    //for integration with Windows (Visual C++)
    inline Variant& operator = (const struct tagVARIANT& var);

    template <typename TVAR>
    friend TVAR operator + (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator - (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator * (const TVAR& var1, const TVAR& var2);

    template <typename TVAR>
    friend TVAR operator / (const TVAR& var1, const TVAR& var2);
    
    template <class TSTREAM>
    TSTREAM& toStream(TSTREAM& s) const
    {
        ToStreamVisitor<TSTREAM> vis(s);
        return applyConstVisitor<ToStreamVisitor<TSTREAM>, TSTREAM&>(vis, getType());
    }

    template <class TSTREAM>
    TSTREAM& fromStream(TSTREAM& s)
    {
        FromStreamVisitor<TSTREAM> vis(s);
        return applyVisitor<FromStreamVisitor<TSTREAM>, TSTREAM&>(vis, getType());
    }

    //save type and value only
    template <typename TSTREAM>
    inline void saveToBinaryStream(TSTREAM& s) const
    {
        td::DataType dt = getType();
        td::BYTE bw = (td::BYTE) dt;
        s << bw;
        toStream(s);
        //s << _flags;	//flags will be kept unchanged during serialization, only value will be stored,
    }

    //check if type is equal to requared and then load value
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

inline td::DataType getType(const td::Variant& var)
{
    return var.getType();
}

inline void VariantBase::operator = (const td::Variant& var)
{
    BaseToVariantVisitor<td::Variant> vis(var);
    applyVisitor<BaseToVariantVisitor<td::Variant>, void>(vis, var.getType());
}

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

template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
inline td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::operator = (const td::Variant& var)
{
    static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
    assert(!EXTERN_ALLOCATOR);
    td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str = *this;
    var.template getValue<td::StringBase< T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR> >(str);
    return *this;
}

template <size_t N>
inline SmallChFix<N>& SmallChFix<N>::operator = (const Variant& var)
{
    SmallChFix& chFix = *this;
    var.getValue(chFix);
    return *this;
}

} //namespace td
