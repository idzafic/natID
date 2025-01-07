// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace sc
{

class ISolutionBuffer
{
public:
    enum class Status {Error, UserStop, SuccessfullyCompleted};
    
    virtual void put() = 0;
    virtual void finalize(Status status) = 0;
};

} //namspace sc
