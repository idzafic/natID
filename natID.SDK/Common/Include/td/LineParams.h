// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/DotPattern.h>
namespace td
{
using LineParams = struct _LineParams
{
    float width = 2.0f;
    td::ColorID colorIDForLightBackground = td::ColorID::SysText;
    td::ColorID colorIDForDarkBackground = td::ColorID::SysText;
    td::LinePattern pattern = td::LinePattern::Solid;
    td::DotPattern dotPattern = td::DotPattern::Pixel;
    
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
