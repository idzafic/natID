// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PaperInfo.h @brief Defines paper size and orientation for printing support. */
#pragma once

namespace gui
{

/// @brief Describes physical paper properties including size and orientation.
class PaperInfo
{
public:
    /// @brief Standard paper size options.
    enum class Size : unsigned char {A3, ///< A3 paper format.
                                     A4, ///< A4 paper format.
                                     Letter ///< US Letter paper format.
                                    };
    /// @brief Paper orientation options.
    enum class Orientation:  unsigned char {Portrait, ///< Portrait (vertical) orientation.
                                            Landscape  ///< Landscape (horizontal) orientation.
                                           };
    Size size = Size::A4; ///< Currently selected paper size.
    Orientation orientation = Orientation::Portrait; ///< Currently selected paper orientation.

    /// @brief Default constructor initialising with A4 portrait defaults.
    PaperInfo(){}

    /// @brief Copy constructor.
    /// @param pi Source PaperInfo to copy from.
    PaperInfo(const PaperInfo& pi)
    : size(pi.size)
    , orientation(pi.orientation)
    {}

    /// @brief Constructs a PaperInfo with specified size and orientation.
    /// @param papSize Desired paper size.
    /// @param orient Desired paper orientation.
    PaperInfo(PaperInfo::Size papSize, PaperInfo::Orientation orient)
    : size(papSize)
    , orientation(orient)
    {

    }
};

}
