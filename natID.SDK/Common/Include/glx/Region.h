// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cstdint>
#include <algorithm>

#ifdef MU_MACOS
namespace MTL
{
    class Region;
}
#endif

namespace glx
{
    // Matches Metal's MTLOrigin
    struct Origin
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;
    };

    // Matches Metal's MTLSize
    struct Size3D
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0; // for 2D textures set to 1
    };

    // Matches Metal's MTLRegion
    class Region 
    {
    public:
        Origin origin{};
        Size3D size{};

        Region() = default;
        Region(Origin o, Size3D s) : origin(o), size(s) {}

#ifdef MU_MACOS
        operator MTL::Region() const;
#endif

        static Region Make2D(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            return Region({x,y,0},{width,height,1});
        }
        static Region Make3D(uint32_t x, uint32_t y, uint32_t z, uint32_t width, uint32_t height, uint32_t depth)
        {
            return Region({x,y,z},{width,height,depth});
        }
    };
}