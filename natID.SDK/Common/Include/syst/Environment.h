// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <cstdlib>
#include <fo/FileOperations.h>
#include <cnt/PushBackVector.h>
#include <cnt/StringBuilder.h>
#include <map>

namespace syst
{
	class Environment
	{
	public:
        ///returns value environment variable if exists, otherwise empty string
		inline static td::String getVariable(const char* envVar)
		{
            if (!envVar)
                return td::String();

#if defined(MU_WINDOWS)
            char* buffer = nullptr;
            size_t len = 0;
            if (_dupenv_s(&buffer, &len, envVar) == 0 && buffer != nullptr) {
                td::String result(buffer);
                free(buffer);  // must free memory allocated by _dupenv_s
                return result;
            }
            return td::String();

#else // POSIX (Linux, macOS, etc.)
            const char* pEnvVarValue = std::getenv(envVar);
            td::String str(pEnvVarValue);
            return str;
#endif
		}
        
        inline static td::String getVariable(const td::String& envVar)
        {
            if (envVar.length() == 0)
                return td::String();

#if defined(MU_WINDOWS)
            char* buffer = nullptr;
            size_t len = 0;
            if (_dupenv_s(&buffer, &len, envVar.c_str()) == 0 && buffer != nullptr) {
                td::String result(buffer);
                std::free(buffer);  // free allocated buffer
                return result;
            }
            return td::String();
#else
            const char* value = std::getenv(envVar.c_str());
            if (value != nullptr)
                return td::String(value);
            return td::String();
#endif
        }

        /// returns value of environment variable if exists, otherwise name of environment variable
        inline static td::String getVariable2(const char* envVar)
        {
            if (envVar == nullptr)
                return td::String();

#if defined(MU_WINDOWS)
            char* buffer = nullptr;
            size_t len = 0;
            if (_dupenv_s(&buffer, &len, envVar) == 0 && buffer != nullptr) {
                td::String result(buffer);
                std::free(buffer);
                return result;
            }
            return td::String(envVar);
#else
            const char* value = std::getenv(envVar);
            if (value != nullptr)
                return td::String(value);
            return td::String(envVar);
#endif
        }

        /// returns value of environment variable if exists, otherwise name of environment variable
        inline static td::String getVariable2(const td::String& envVar)
        {
            if (envVar.length() == 0)
                return td::String();

#if defined(MU_WINDOWS)
            char* buffer = nullptr;
            size_t len = 0;
            if (_dupenv_s(&buffer, &len, envVar.c_str()) == 0 && buffer != nullptr) {
                td::String result(buffer);
                std::free(buffer);
                return result;
            }
            return envVar;
#else
            const char* value = std::getenv(envVar.c_str());
            if (value != nullptr)
                return td::String(value);
            return envVar;
#endif
        }
        
        ///replaces environment variables that start with $ in a path with slash (/) as separator'
        inline static td::String replaceVariablesInPath(const td::String& path, std::map<td::String, td::String>* pVarMap = nullptr)
        {
            if (path.length() < 1)
                return path;
            
            auto nToReplace = path.getCount('$');
            if (nToReplace == 0)
            {
                char ch=path.at(0);
                if (ch == '~')
                {
                    fo::fs::path homePath;
                    mu::getHomePath(homePath);
                    char ch2 = path.at(1);
                    int pos = 1;
#ifdef MU_WINDOWS
                    if (ch2 == '\\')
                        pos = 2;
#else
                    if (ch2 == '/')
                        pos = 2;
#endif
                    const char* pStr = path.c_str() + pos;
                    fo::fs::path resPath = homePath / pStr;
                    td::String toRet = resPath.string();
                    return toRet;
                }
                return path;
            }
            auto nSeparators = path.getCount('/');
            cnt::PushBackVector<td::String> parts;
            cnt::StringBuilderSmall sb;
            
            if (nSeparators == 0)
            {
                parts.reserve(nToReplace+1);
                bool beginsWithSepartor = (path.getAt(0) == '$');
                path.split('$', parts, false);
                bool bFirst = true;
                for (const auto& part : parts)
                {
                    if (part.length() > 0)
                    {
                        if (bFirst)
                        {
                            if (beginsWithSepartor)
                            {
                                if (pVarMap)
                                {
                                    auto it = pVarMap->find(part);
                                    if (it != pVarMap->end())
                                        sb << it->second;
                                    else
                                        sb << Environment::getVariable2(part);
                                }
                                else
                                    sb << Environment::getVariable2(part);
                            }
                            else
                                sb << part;
                            bFirst = false;
                        }
                        else
                        {
                            if (pVarMap)
                            {
                                auto it = pVarMap->find(part);
                                if (it != pVarMap->end())
                                    sb << it->second;
                                else
                                    sb << Environment::getVariable2(part);
                            }
                            else
                                sb << Environment::getVariable2(part);
                        }
                            
                    }
                }
            }
            else
            {
                parts.reserve(nSeparators+1);
                cnt::PushBackVector<td::String> parts2;
                path.split('/', parts, false);
                bool beginsWithSepartor = (path.getAt(0) == '/');
                bool endsWithSeparator = path.endsWith("/", 1);
                if (beginsWithSepartor)
                    sb << '/';
                bool bFirst = true;
                for (const auto& part : parts)
                {
                    if (!bFirst)
                        sb << '/';
                    else
                        bFirst = false;
                    
                    auto nVars = part.getCount('$');
                    if (nVars > 0)
                    {
                        if (parts2.capacity() == 0)
                            parts2.reserve(nToReplace+1);
                        bool partBeginsWithSepartor = (part.getAt(0) == '$');
                        part.split('$', parts2, false);
                        bool bFirstSubPart = true;
                        for (const auto& subPart : parts2)
                        {
                            if (subPart.length() > 0)
                            {
                                if (bFirstSubPart)
                                {
                                    if (partBeginsWithSepartor)
                                    {
                                        if (pVarMap)
                                        {
                                            const char* pVar = subPart.c_str();
                                            auto it = pVarMap->find(subPart);
                                            if (it != pVarMap->end())
                                                sb << it->second;
                                            else
                                                sb << Environment::getVariable2(subPart);
                                        }
                                        else
                                            sb << Environment::getVariable2(subPart);
                                        
                                    }
                                    else
                                        sb << part;
                                    partBeginsWithSepartor = false;
                                }
                                else
                                {
                                    if (pVarMap)
                                    {
                                        auto it = pVarMap->find(subPart);
                                        if (it != pVarMap->end())
                                            sb << it->second;
                                        else
                                            sb << Environment::getVariable2(subPart);
                                    }
                                    else
                                        sb << Environment::getVariable2(subPart);
                                }
                            }
                        }
                    }
                    else
                        sb << part;
                }
                if (endsWithSeparator)
                    sb << '/';
            }
            return sb.toString();
        }
        
        ///replaces environment variables that start with $ in a path with slash (/) as separator'
        inline static td::String replaceVariablesInPath(const fo::fs::path& path, std::map<td::String, td::String>* pVarMap = nullptr)
        {
            td::String strPath = path.string();
            return replaceVariablesInPath(strPath, pVarMap);
        }
        
        ///replaces environment variables that start with $ in a path with slash (/) as separator'
        inline static td::String replaceVariablesInPath(const char* varPath, std::map<td::String, td::String>* pVarMap = nullptr)
        {
            td::String strPath(varPath);
            return replaceVariablesInPath(strPath, pVarMap);
        }
	};
}
