// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <td/String.h>

namespace sc
{

using TranslateFunction = td::String (*)(const char*);

class ILog
{
public:
    enum class Type : unsigned char {Info=0, Warning, Error};
    using Entry = struct LE
    {
        td::String str;
//        td::UINT4 location;
        td::UINT4 lineNo; //1-based
        td::UINT4 colNo; //1-based
    };

    virtual void clean() = 0;
    virtual void setModulePosition(td::UINT2) = 0;
    virtual void enableCollecting(bool bCollectLogs) = 0;
    virtual void add(const Entry&, Type type = Type::Error) = 0;
    virtual void setTranslationFn(TranslateFunction) = 0;
    virtual void addWoTranslation(const td::String& str, Type type = Type::Info) = 0;
    virtual void translateAndAdd(const char* pMsg, Type type = Type::Error) = 0;
    virtual void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type = Type::Error) = 0;
    //last one is used to send params that will be formatted (param placeholder is @
    virtual void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type, const char* param1, ...) = 0;
    virtual void refresh() = 0;
    virtual td::UINT4 getNoOfErrors() const = 0;
};

class LogEnabler
{
    sc::ILog* _pLog;
    LogEnabler() = delete;
public:
    LogEnabler(sc::ILog* pLog)
    : _pLog(pLog)
    {
        if (_pLog)
        {
            _pLog->clean();
            _pLog->enableCollecting(true);
        }
        
    }
    ~LogEnabler()
    {
        if (_pLog)
        {
            _pLog->enableCollecting(false);
            _pLog->refresh();
        }
    }
};

} //sc

