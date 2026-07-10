// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Region.h
    @brief Defines 3D origin, size, and region structures for specifying texture sub-regions. */
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
    /// @brief Represents a 3D integer origin point (x, y, z) within a texture.
    struct Origin
    {
        uint32_t x = 0; ///< X coordinate of the origin.
        uint32_t y = 0; ///< Y coordinate of the origin.
        uint32_t z = 0; ///< Z coordinate of the origin.
    };

    // Matches Metal's MTLSize
    /// @brief Represents the 3D dimensions (width, height, depth) of a texture region.
    struct Size3D
    {
        uint32_t width = 0;  ///< Width of the region in texels.
        uint32_t height = 0; ///< Height of the region in texels.
        uint32_t depth = 0;  ///< Depth of the region in texels; for 2D textures set to 1
    };

    // Matches Metal's MTLRegion
    /// @brief Represents a rectangular sub-region of a texture, defined by an origin and a 3D size.
    class Region
    {
    public:
        Origin origin{}; ///< The starting origin (top-left-front corner) of the region.
        Size3D size{};   ///< The dimensions of the region.

        /// @brief Default constructor; initializes origin and size to zero.
        Region() = default;
        /// @brief Constructs a region from an explicit origin and size.
        /// @param o The origin of the region.
        /// @param s The size of the region.
        Region(Origin o, Size3D s) : origin(o), size(s) {}

#ifdef MU_MACOS
        /// @brief Converts this Region to a Metal MTL::Region.
        operator MTL::Region() const;
#endif

        /// @brief Creates a 2D region (depth = 1) from position and dimensions.
        /// @param x X coordinate of the top-left corner.
        /// @param y Y coordinate of the top-left corner.
        /// @param width Width of the region in texels.
        /// @param height Height of the region in texels.
        /// @return A Region covering the specified 2D area.
        static Region Make2D(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            return Region({x,y,0},{width,height,1});
        }
        /// @brief Creates a 3D region from position and dimensions.
        /// @param x X coordinate of the front-top-left corner.
        /// @param y Y coordinate of the front-top-left corner.
        /// @param z Z coordinate of the front-top-left corner.
        /// @param width Width of the region in texels.
        /// @param height Height of the region in texels.
        /// @param depth Depth of the region in texels.
        /// @return A Region covering the specified 3D volume.
        static Region Make3D(uint32_t x, uint32_t y, uint32_t z, uint32_t width, uint32_t height, uint32_t depth)
        {
            return Region({x,y,z},{width,height,depth});
        }
    };
}
