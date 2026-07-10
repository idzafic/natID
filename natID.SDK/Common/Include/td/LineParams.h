// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LineParams.h
    @brief Aggregate structure holding all visual parameters needed to render a line. */
#pragma once
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/DotPattern.h>
namespace td
{
/// @brief Holds the complete set of parameters used to draw a line: width, color (for light and dark backgrounds), dash pattern, and dot marker shape.
using LineParams = struct _LineParams
{
    float width = 2.0f;                                                      ///< Line width in device-independent units.
    td::ColorID colorIDForLightBackground = td::ColorID::SysText;            ///< Color used when the background is light.
    td::ColorID colorIDForDarkBackground = td::ColorID::SysText;             ///< Color used when the background is dark.
    td::LinePattern pattern = td::LinePattern::Solid;                        ///< Dash/dot pattern for the line.
    td::DotPattern dotPattern = td::DotPattern::Pixel;                       ///< Marker shape drawn at data points.

    /// @brief Resolves the effective color for the given background brightness.
    /// @param bIsDark True if the current background is dark, false if light.
    /// @return The resolved ColorID to use for rendering.
    inline td::ColorID getColor(bool bIsDark) const
    {
        if (bIsDark)
        {
            if (colorIDForDarkBackground == td::ColorID::SysText)
            {
                if (bIsDark)
                    return td::ColorID::White;
                return td::ColorID::Black;
            }
            return colorIDForDarkBackground;
        }
        if (colorIDForLightBackground == td::ColorID::SysText)
        {
            if (bIsDark)
                return td::ColorID::White;
            return td::ColorID::Black;
        }
        return colorIDForLightBackground;
    }
};

} //namespace td
