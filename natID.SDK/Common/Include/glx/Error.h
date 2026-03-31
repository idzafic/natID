// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <td/String.h>

//namespace MTL
//{
//class Error;
//} //namespace MTL

namespace glx
{
class NATGLX_API Error
{
    td::LINT8 _code = 0;
    td::String _domain;
    td::String _localizedDescription;
    td::String _localizedRecoverySuggestion;
    td::String _localizedFailureReason;
private:
//    MTL::Error* _pError;
protected:
//    Error(MTL::Error* pError);
public:
    Error();
    void operator = (const Error& error);
    
    td::LINT8 code() const;
    td::String domain() const;
    //class Dictionary* userInfo() const;
    td::String localizedDescription() const;
//    class Array*      localizedRecoveryOptions() const;
    td::String localizedRecoverySuggestion() const;
    td::String localizedFailureReason() const;
//    void release();
};

} //namespace glx


