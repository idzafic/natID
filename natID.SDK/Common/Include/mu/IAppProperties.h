// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/muLib.h>
#include <td/String.h>
#include <td/Variant.h>

namespace mu
{
class MAINUTILS_API IAppProperties
{
public:
    enum class ToolBarType : td::BYTE {Main=0, Report};
    
    static IAppProperties* getProperties(const td::String& appDomain, const td::String& appName);
    virtual const td::Variant getKeyValue(const td::String& key, const td::Variant& defaultValue) const = 0;
    virtual void setKeyValue(const td::String& key, const td::Variant& newValue) = 0;
    virtual bool deleteKey(const td::String& key) = 0;
    virtual bool save() = 0;
    virtual bool exportTo(const td::String& path) const = 0;
    virtual td::String toString() const = 0;
    virtual size_t getNoOfKeys() const = 0;
    virtual bool getTBLabelVisibility(IAppProperties::ToolBarType tbType, bool defaultValue) = 0;
    virtual void setTBLabelVisibility(IAppProperties::ToolBarType tbType, bool newValue) = 0;
    virtual void release() = 0;
    
    //helper methods for easier data access
    template <typename T>
    void setValue(const char* key, const T& value)
    {
        if constexpr (std::is_enum_v<T>)
        {
            unsigned int enumVal = static_cast<unsigned int>(value);
            td::Variant varVal(enumVal);
            td::String strKey(key);
            setKeyValue(strKey, varVal);
        }
        else
        {
            td::Variant varVal(value);
            td::String strKey(key);
            setKeyValue(strKey, varVal);
        }
        
    }
    template <typename T>
    void setValue(const td::String& key, const T& value)
    {
        setKeyValue(key.c_str(), value);
    }
    
    void setValue(const char* key, const char* value)
    {
        td::String strValue(value);
        td::Variant varVal(strValue);
        td::String strKey(key);
        setKeyValue(strKey, varVal);
    }
    
    inline td::String getValue(const char* key, const char* defaultValue) const
    {
        td::String strDefVal(defaultValue);
        td::Variant varDefaultValue(strDefVal);
        td::String strKey(key);
        const td::Variant& retVarVal = getKeyValue(strKey, varDefaultValue);
        td::String retVal;
        retVarVal.getValue(retVal);
        return retVal;
    }
    
    template <typename T>
    const T getValue(const char* key, const T& defaultValue) const
    {
        if constexpr (std::is_enum_v<T>)
        {
            td::Variant varDefaultValue(static_cast<unsigned int>(defaultValue));
            td::String strKey(key);
            const td::Variant& retVarVal = getKeyValue(strKey, varDefaultValue);

            unsigned int val;
            retVarVal.getValue(val);
            return static_cast<T>(val);
        }
        else
        {
            td::Variant varDefaultValue(defaultValue);
            td::String strKey(key);
            const td::Variant& retVarVal = getKeyValue(strKey, varDefaultValue);
            T retVal;
            retVarVal.getValue(retVal);
            return retVal;
        }
        
    }
    
    template <typename T>
    const T getValue(const td::String& key, const T& defaultValue) const
    {
        return getValue(key.c_str(), defaultValue);
    }

};

} //namespace mu
