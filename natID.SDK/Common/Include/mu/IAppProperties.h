// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IAppProperties.h
 *  @brief Interface for persistent application key-value property storage.
 */
#pragma once
#include <mu/muLib.h>
#include <td/String.h>
#include <td/Variant.h>

namespace mu
{
/// @brief Abstract interface for reading and writing application properties stored as key-value pairs.
class MAINUTILS_API IAppProperties
{
public:
    /// @brief Identifies which toolbar the label visibility setting applies to.
    enum class ToolBarType : td::BYTE {Main=0, ///< The main application toolbar.
                                       Report   ///< The report toolbar.
                                      };

    /// @brief Factory method that returns the singleton properties object for the given application domain and name.
    /// @param appDomain The application domain identifier.
    /// @param appName The application name.
    /// @return Pointer to the IAppProperties instance for this domain/name pair.
    static IAppProperties* getProperties(const td::String& appDomain, const td::String& appName);

    /// @brief Retrieves the value stored under a key, or a default if the key does not exist.
    /// @param key The property key to look up.
    /// @param defaultValue Value returned when the key is absent.
    /// @return The stored variant value or defaultValue.
    virtual const td::Variant getKeyValue(const td::String& key, const td::Variant& defaultValue) const = 0;

    /// @brief Stores a value under the specified key, creating or updating the entry.
    /// @param key The property key to set.
    /// @param newValue The value to associate with the key.
    virtual void setKeyValue(const td::String& key, const td::Variant& newValue) = 0;

    /// @brief Removes the entry identified by key from the property store.
    /// @param key The property key to delete.
    /// @return True if the key existed and was deleted; false otherwise.
    virtual bool deleteKey(const td::String& key) = 0;

    /// @brief Persists the current property store to its backing storage.
    /// @return True on success; false on write failure.
    virtual bool save() = 0;

    /// @brief Exports all properties to a file at the specified path.
    /// @param path Destination file path for the export.
    /// @return True on success; false on failure.
    virtual bool exportTo(const td::String& path) const = 0;

    /// @brief Serializes all properties to a human-readable string.
    /// @return String representation of all stored properties.
    virtual td::String toString() const = 0;

    /// @brief Returns the number of property keys currently stored.
    /// @return Count of keys in the property store.
    virtual size_t getNoOfKeys() const = 0;

    /// @brief Gets the toolbar label visibility setting for the specified toolbar type.
    /// @param tbType The toolbar whose label visibility is queried.
    /// @param defaultValue Default visibility returned when the property is not set.
    /// @return True if labels are visible; false otherwise.
    virtual bool getTBLabelVisibility(IAppProperties::ToolBarType tbType, bool defaultValue) = 0;

    /// @brief Sets the toolbar label visibility for the specified toolbar type.
    /// @param tbType The toolbar to configure.
    /// @param newValue True to show labels; false to hide them.
    virtual void setTBLabelVisibility(IAppProperties::ToolBarType tbType, bool newValue) = 0;

    /// @brief Releases this properties object and frees associated resources.
    virtual void release() = 0;

    //helper methods for easier data access
    /// @brief Stores a typed value (including enum types) under a C-string key.
    /// @tparam T Type of the value; enum types are converted to unsigned int.
    /// @param key Null-terminated property key.
    /// @param value Value to store.
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

    /// @brief Stores a typed value under a td::String key.
    /// @tparam T Type of the value.
    /// @param key Property key as a td::String.
    /// @param value Value to store.
    template <typename T>
    void setValue(const td::String& key, const T& value)
    {
        setKeyValue(key.c_str(), value);
    }

    /// @brief Stores a C-string value under a C-string key.
    /// @param key Null-terminated property key.
    /// @param value Null-terminated string value to store.
    void setValue(const char* key, const char* value)
    {
        td::String strValue(value);
        td::Variant varVal(strValue);
        td::String strKey(key);
        setKeyValue(strKey, varVal);
    }

    /// @brief Retrieves a string value by C-string key, returning a default if absent.
    /// @param key Null-terminated property key.
    /// @param defaultValue C-string default returned when the key does not exist.
    /// @return The stored string value or defaultValue wrapped in a td::String.
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

    /// @brief Retrieves a typed value (including enum types) by C-string key.
    /// @tparam T Type to retrieve; enum types are stored and retrieved as unsigned int.
    /// @param key Null-terminated property key.
    /// @param defaultValue Default value returned when the key does not exist.
    /// @return The stored value cast to T, or defaultValue.
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

    /// @brief Retrieves a typed value by td::String key.
    /// @tparam T Type to retrieve.
    /// @param key Property key as a td::String.
    /// @param defaultValue Default value returned when the key does not exist.
    /// @return The stored value cast to T, or defaultValue.
    template <typename T>
    const T getValue(const td::String& key, const T& defaultValue) const
    {
        return getValue(key.c_str(), defaultValue);
    }

};

} //namespace mu
