// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace gui
{

class PaperInfo
{
public:
    enum class Size : unsigned char {A3, A4, Letter};
    enum class Orientation:  unsigned char {Portrait, Landscape};
    Size size = Size::A4;
    Orientation orientation = Orientation::Portrait;
    PaperInfo(){}
    
    PaperInfo(const PaperInfo& pi)
    : size(pi.size)
    , orientation(pi.orientation)
    {}
    
    PaperInfo(PaperInfo::Size papSize, PaperInfo::Orientation orient)
    : size(papSize)
    , orientation(orient)
    {
        
    }
};

}
