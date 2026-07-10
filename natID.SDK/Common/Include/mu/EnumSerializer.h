// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file EnumSerializer.h
 * @brief Bidirectional mapping between enum integer values and their string names.
 */
#pragma once
#include <td/String.h>
#include <map>
#include <sstream>

namespace mu
{
/// @brief Provides bidirectional serialization between enum integer values and string names.
class EnumSerializer
{
    std::map<td::String, td::UINT4> mapNameToEnum;          ///< Maps string name to integer enum value
    std::map<td::UINT4, const td::String*> mapEnumToName;   ///< Maps integer enum value to its name string

    /// @brief Formats an error message listing valid enum names when an unknown name is given.
    /// @param strName The unrecognized enum name.
    /// @param strErr Output string receiving the formatted error message.
    void formatErrorName(const td::String& strName, td::String& strErr) const
    {
        std::ostringstream ss;
        ss << "Wrong enum name: \'" << strName.c_str() << "\'. Expected one of: [";
        std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
        bool first = true;
        while (it != mapEnumToName.end())
        {
            if (first)
                ss << it->second->c_str();
            else
                ss << ',' << it->second->c_str();
            ++it;
            first = false;
        }
        ss << ']';
        ss << std::ends;

        strErr = ss.str().c_str();
    }

    /// @brief Formats an error message listing valid enum values when an unknown value is given.
    /// @param val The unrecognized integer value.
    /// @param strErr Output string receiving the formatted error message.
    void formatErrorValue(td::UINT4 val, td::String& strErr) const
    {
        std::ostringstream ss;
        ss << "Wrong enum value: \'" << val << "\'. Expected one of: [";
        std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
        bool first = true;
        while (it != mapEnumToName.end())
        {
            if (first)
                ss << it->first;
            else
                ss << ',' << it->first;
            first = false;
            ++it;
        }
        ss << ']';
        ss << std::ends;
        strErr = ss.str().c_str();
    }

public:
    /// @brief Default constructor.
    EnumSerializer()
    {
    }

    /// @brief Registers a name-to-value mapping for an enum entry.
    /// @param name String name of the enum constant.
    /// @param val Integer value of the enum constant.
    void add(const char* name, td::UINT4 val)
    {
        //typedef std::pair <td::String, td::UINT4> tEnumPair;
        std::pair <td::String, td::UINT4> toInsert(name, val);
        std::pair< std::map <td::String, td::UINT4>::iterator, bool> valRet = mapNameToEnum.insert(toInsert);
        mapEnumToName[val] = &(valRet.first->first);
        //mapNameToEnum[name] = val;
        //mapEnumToName[val] = name;

    }

    /// @brief Looks up the integer value for a given enum name string.
    /// @param strEnumName The string name to look up.
    /// @return The corresponding integer value.
    /// @throws td::String Error message if the name is not registered.
    td::UINT4 getIntValue(const td::String& strEnumName) const
    {
        std::map<td::String, td::UINT4>::const_iterator it = mapNameToEnum.find(strEnumName);
        if (it != mapNameToEnum.end())
        {
            return it->second;
        }

        td::String strErr;
        formatErrorName(strEnumName, strErr);
        throw strErr;
        return mapEnumToName.begin()->first;
    }

    /// @brief Looks up a typed enum value for a given name string.
    /// @tparam T Enum type to cast the result to.
    /// @param strEnumName The string name to look up.
    /// @return The corresponding enum value of type T.
    /// @throws td::String Error message if the name is not registered.
    template <typename T>
    T getValue(const td::String& strEnumName) const
    {
        std::map<td::String, td::UINT4>::const_iterator it = mapNameToEnum.find(strEnumName);
        if (it != mapNameToEnum.end())
        {
            return (T) it->second;
        }

        td::String strErr;
        formatErrorName(strEnumName, strErr);
        throw strErr;
        return (T) mapEnumToName.begin()->first;
    }

    /// @brief Returns the string name for a given integer enum value.
    /// @param val Integer enum value to look up.
    /// @return Const reference to the registered name string.
    /// @throws td::String Error message if the value is not registered.
    const td::String& getName(td::UINT4 val) const
    {
        std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.find(val);
        if (it != mapEnumToName.end())
        {
            return *(it->second);
        }

        td::String strErr;
        formatErrorValue(val, strErr);
        throw strErr;
    }

    /// @brief Writes all registered enum names to a stream, comma-separated.
    /// @tparam T Enum type (used for type identity only).
    /// @param ss Output stream.
    template<typename T>
    void show(std::ostream& ss) const
    {
        bool first = true;
        std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
        while (it != mapEnumToName.end())
        {
            if (!first)
                ss << ", ";
            else
                first = false;

            ss << it->second->c_str();
            ++it;
        }
        ss << std::endl;
    }

    /// @brief Writes all registered enum names and their integer values to a stream.
    /// @tparam T Enum type (used for type name display).
    /// @param ss Output stream.
    template<typename T>
    void showDetailed(std::ostream& ss) const
    {
        ss << "Enum: " << typeid(T).name() << std::endl;

        std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
        while (it != mapEnumToName.end())
        {
            ss << it->second->c_str() << "=" << it->first << ", ";
            ++it;
        }
        ss << std::endl;
    }
};
};
