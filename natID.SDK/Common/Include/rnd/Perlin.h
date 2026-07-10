// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Perlin.h
 *  @brief Template implementation of Perlin coherent noise with octave accumulation, normalization, and serialization support.
 */
# pragma once
#include <algorithm>
# include <array>
# include <numeric>
# include <random>

namespace rnd
{

//SIZE should be power of 2 (eg. 256, 512, 1024,...)
/// @brief Generates Perlin coherent noise in 1D, 2D, and 3D with optional multi-octave accumulation.
///        TValue is the floating-point type used for computations (e.g., float or double).
///        SIZE must be a power of two (e.g., 256, 512, 1024).
template <class TValue, size_t SIZE = 256>
class PerlinNoise
{

private:

    std::uint8_t p[2*SIZE]; ///< Doubled permutation table used to avoid index wrapping during gradient lookups.

    /// @brief Applies the quintic fade curve to t, smoothing gradient interpolation.
    /// @param t Input value to fade.
    /// @return Faded value: 6t^5 - 15t^4 + 10t^3.
    static constexpr TValue fade(TValue t) noexcept
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    /// @brief Performs linear interpolation between a and b by factor t.
    /// @param t Interpolation factor in [0, 1].
    /// @param a Start value.
    /// @param b End value.
    /// @return Interpolated value a + t*(b-a).
    static constexpr TValue lerp(TValue t, TValue a, TValue b) noexcept
    {
        return a + t * (b - a);
    }

    /// @brief Computes the dot product of a pseudo-random gradient selected by hash with the distance vector (x, y, z).
    /// @param hash Hash value used to select the gradient direction.
    /// @param x    X component of the distance from the lattice point.
    /// @param y    Y component of the distance from the lattice point.
    /// @param z    Z component of the distance from the lattice point.
    /// @return Dot product of the chosen gradient with the distance vector.
    static constexpr TValue grad(std::uint8_t hash, TValue x, TValue y, TValue z) noexcept
    {
        const std::uint8_t h = hash & 15;
        const TValue u = h < 8 ? x : y;
        const TValue v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    /// @brief Computes the normalisation weight for a given number of octaves.
    /// @param octaves Number of octave layers to account for.
    /// @return Sum of amplitude weights across all octaves (used for normalisation).
    static constexpr TValue weight(std::int32_t octaves) noexcept
    {
        TValue amp = 1;
        TValue value = 0;

        for (std::int32_t i = 0; i < octaves; ++i)
        {
            value += amp;
            amp /= 2;
        }

        return value;
    }

public:

    /// @brief Constructs a PerlinNoise generator and seeds the permutation table with the given integer seed.
    /// @param seed Random seed value (defaults to the standard engine's default seed).
    explicit PerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed)
    {
        reseed(seed);
    }

    /// @brief Constructs a PerlinNoise generator and seeds the permutation table using a uniform random number generator.
    /// @tparam URNG Uniform random number generator type (must not be an arithmetic type).
    /// @param urng  Random number generator instance forwarded to std::shuffle.
    template <class URNG, std::enable_if_t<!std::is_arithmetic_v<URNG>>* = nullptr>
    explicit PerlinNoise(URNG&& urng)
    {
        reseed(std::forward<URNG>(urng));
    }

    /// @brief Returns the permutation table size used by this noise generator.
    /// @return The SIZE template parameter value.
    constexpr size_t size() const
    {
        return SIZE;
    }

    /// @brief Re-seeds the permutation table using an integer seed value.
    /// @param seed New seed value for the permutation shuffle.
    void reseed(std::uint32_t seed)
    {
        for (size_t i = 0; i < SIZE; ++i)
            p[i] = static_cast<std::uint8_t>(i);

        std::shuffle(std::begin(p), std::begin(p) + SIZE, std::default_random_engine(seed));

        for (size_t i = 0; i < SIZE; ++i)
            p[SIZE + i] = p[i];
    }

    /// @brief Re-seeds the permutation table using a uniform random number generator.
    /// @tparam URNG Uniform random number generator type (must not be an arithmetic type).
    /// @param urng  Random number generator instance forwarded to std::shuffle.
    template <class URNG, std::enable_if_t<!std::is_arithmetic_v<URNG>>* = nullptr>
    void reseed(URNG&& urng)
    {
        for (size_t i = 0; i < SIZE; ++i)
            p[i] = static_cast<std::uint8_t>(i);

        std::shuffle(std::begin(p), std::begin(p) + SIZE, std::forward<URNG>(urng));

        for (size_t i = 0; i < SIZE; ++i)
            p[SIZE + i] = p[i];
    }

    ///////////////////////////////////////
    //
    //	Noise [-1, 1]
    //
    ///////////////////////////////////////
    /// @brief Evaluates 1D Perlin noise at position x.
    /// @param x Input coordinate.
    /// @return Noise value in the range [-1, 1].
    TValue noise1D(TValue x) const noexcept
    {
        return noise3D(x, 0, 0);
    }

    /// @brief Evaluates 2D Perlin noise at position (x, y).
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @return Noise value in the range [-1, 1].
    TValue noise2D(TValue x, TValue y) const noexcept
    {
        return noise3D(x, y, 0);
    }

    /// @brief Evaluates 3D Perlin noise at position (x, y, z).
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param z Z coordinate.
    /// @return Noise value in the range [-1, 1].
    TValue noise3D(TValue x, TValue y, TValue z) const noexcept
    {
        const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & (SIZE-1);
        const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & (SIZE-1);
        const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & (SIZE-1);

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        const TValue u = fade(x);
        const TValue v = fade(y);
        const TValue w = fade(z);

        const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                    grad(p[BA], x - 1, y, z)),
                            lerp(u, grad(p[AB], x, y - 1, z),
                                 grad(p[BB], x - 1, y - 1, z))),
                    lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                 grad(p[BA + 1], x - 1, y, z - 1)),
                         lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                              grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

    ///////////////////////////////////////
    //
    //	Noise [0, 1]
    //
    ///////////////////////////////////////
    /// @brief Evaluates 1D Perlin noise at x, remapped to the range [0, 1].
    /// @param x Input coordinate.
    /// @return Noise value in the range [0, 1].
    TValue noise1D_0_1(TValue x) const noexcept
    {
        return noise1D(x) * TValue(0.5) + TValue(0.5);
    }

    /// @brief Evaluates 2D Perlin noise at (x, y), remapped to the range [0, 1].
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @return Noise value in the range [0, 1].
    TValue noise2D_0_1(TValue x, TValue y) const noexcept
    {
        return noise2D(x, y) * TValue(0.5) + TValue(0.5);
    }

    /// @brief Evaluates 3D Perlin noise at (x, y, z), remapped to the range [0, 1].
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param z Z coordinate.
    /// @return Noise value in the range [0, 1].
    TValue noise3D_0_1(TValue x, TValue y, TValue z) const noexcept
    {
        return noise3D(x, y, z) * TValue(0.5) + TValue(0.5);
    }

    ///////////////////////////////////////
    //
    //	Accumulated octave noise
    //	* Return value can be outside the range [-1, 1]
    //
    ///////////////////////////////////////
    /// @brief Accumulates 1D Perlin noise over multiple octaves without normalisation.
    /// @param x       Input coordinate.
    /// @param octaves Number of octave layers to accumulate.
    /// @return Unnormalized accumulated noise value (may exceed [-1, 1]).
    TValue accumulatedOctaveNoise1D(TValue x, std::int32_t octaves) const noexcept
    {
        TValue result = 0;
        TValue amp = 1;

        for (std::int32_t i = 0; i < octaves; ++i)
        {
            result += noise(x) * amp;
            x *= 2;

            amp /= 2;
        }

        return result; // unnormalized
    }

    /// @brief Accumulates 2D Perlin noise over multiple octaves without normalisation.
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param octaves Number of octave layers to accumulate.
    /// @return Unnormalized accumulated noise value (may exceed [-1, 1]).
    TValue accumulatedOctaveNoise2D(TValue x, TValue y, std::int32_t octaves) const noexcept
    {
        TValue result = 0;
        TValue amp = 1;

        for (std::int32_t i = 0; i < octaves; ++i)
        {
            result += noise2D(x, y) * amp;
            x *= 2;
            y *= 2;
            amp /= 2;
        }

        return result; // unnormalized
    }

    /// @brief Accumulates 3D Perlin noise over multiple octaves without normalisation.
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param z       Z coordinate.
    /// @param octaves Number of octave layers to accumulate.
    /// @return Unnormalized accumulated noise value (may exceed [-1, 1]).
    TValue accumulatedOctaveNoise3D(TValue x, TValue y, TValue z, std::int32_t octaves) const noexcept
    {
        TValue result = 0;
        TValue amp = 1;

        for (std::int32_t i = 0; i < octaves; ++i)
        {
            result += noise3D(x, y, z) * amp;
            x *= 2;
            y *= 2;
            z *= 2;
            amp /= 2;
        }

        return result; // unnormalized
    }

    ///////////////////////////////////////
    //
    //	Normalized octave noise [-1, 1]
    //
    ///////////////////////////////////////
    /// @brief Accumulates and normalises 1D Perlin noise to the range [-1, 1].
    /// @param x       Input coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [-1, 1].
    TValue normalizedOctaveNoise1D(TValue x, std::int32_t octaves) const noexcept
    {
        return accumulatedOctaveNoise1D(x, octaves) / weight(octaves);
    }

    /// @brief Accumulates and normalises 2D Perlin noise to the range [-1, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [-1, 1].
    TValue normalizedOctaveNoise2D(TValue x, TValue y, std::int32_t octaves) const noexcept
    {
        return accumulatedOctaveNoise2D(x, y, octaves) / weight(octaves);
    }

    /// @brief Accumulates and normalises 3D Perlin noise to the range [-1, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param z       Z coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [-1, 1].
    TValue normalizedOctaveNoise3D(TValue x, TValue y, TValue z, std::int32_t octaves) const noexcept
    {
        return accumulatedOctaveNoise3D(x, y, z, octaves) / weight(octaves);
    }

    ///////////////////////////////////////
    //
    //	Accumulated octave noise clamped within the range [0, 1]
    //
    ///////////////////////////////////////
    /// @brief Accumulates 1D octave noise, remaps and clamps the result to [0, 1].
    /// @param x       Input coordinate.
    /// @param octaves Number of octave layers.
    /// @return Clamped noise value in [0, 1].
    TValue accumulatedOctaveNoise1D_0_1(TValue x, std::int32_t octaves) const noexcept
    {
        return std::clamp<TValue>(accumulatedOctaveNoise1D(x, octaves) * TValue(0.5) + TValue(0.5), 0, 1);
    }

    /// @brief Accumulates 2D octave noise, remaps and clamps the result to [0, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param octaves Number of octave layers.
    /// @return Clamped noise value in [0, 1].
    TValue accumulatedOctaveNoise2D_0_1(TValue x, TValue y, std::int32_t octaves) const noexcept
    {
        return std::clamp<TValue>(accumulatedOctaveNoise2D(x, y, octaves) * TValue(0.5) + TValue(0.5), 0, 1);
    }

    /// @brief Accumulates 3D octave noise, remaps and clamps the result to [0, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param z       Z coordinate.
    /// @param octaves Number of octave layers.
    /// @return Clamped noise value in [0, 1].
    TValue accumulatedOctaveNoise3D_0_1(TValue x, TValue y, TValue z, std::int32_t octaves) const noexcept
    {
        return std::clamp<TValue>(accumulatedOctaveNoise3D(x, y, z, octaves) * TValue(0.5) + TValue(0.5), 0, 1);
    }

    ///////////////////////////////////////
    //
    //	Normalized octave noise [0, 1]
    //
    ///////////////////////////////////////
    /// @brief Accumulates, normalises, and remaps 1D octave noise to [0, 1].
    /// @param x       Input coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [0, 1].
    TValue normalizedOctaveNoise1D_0_1(TValue x, std::int32_t octaves) const noexcept
    {
        return normalizedOctaveNoise1D(x, octaves) * TValue(0.5) + TValue(0.5);
    }

    /// @brief Accumulates, normalises, and remaps 2D octave noise to [0, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [0, 1].
    TValue normalizedOctaveNoise2D_0_1(TValue x, TValue y, std::int32_t octaves) const noexcept
    {
        return normalizedOctaveNoise2D(x, y, octaves) * TValue(0.5) + TValue(0.5);
    }

    /// @brief Accumulates, normalises, and remaps 3D octave noise to [0, 1].
    /// @param x       X coordinate.
    /// @param y       Y coordinate.
    /// @param z       Z coordinate.
    /// @param octaves Number of octave layers.
    /// @return Normalized noise value in [0, 1].
    TValue normalizedOctaveNoise3D_0_1(TValue x, TValue y, TValue z, std::int32_t octaves) const noexcept
    {
        return normalizedOctaveNoise3D(x, y, z, octaves) * TValue(0.5) + TValue(0.5);
    }

    ///////////////////////////////////////
    //
    //	Serialization
    //
    ///////////////////////////////////////
    /// @brief Copies the first SIZE entries of the permutation table into a std::array for serialization.
    /// @param s Output array that receives the permutation table snapshot.
    void toArray(std::array<std::uint8_t, SIZE>& s) const noexcept
    {
        for (std::size_t i = 0; i < SIZE; ++i)
            s[i] = p[i];
    }

    /// @brief Restores the permutation table from a previously serialized std::array.
    /// @param s Input array containing the permutation table snapshot.
    void fromArray(const std::array<std::uint8_t, SIZE>& s) noexcept
    {
        for (std::size_t i = 0; i < SIZE; ++i)
            p[SIZE + i] = p[i] = s[i];
    }
};

/// @brief Convenience alias for PerlinNoise specialised on float with the default table size of 256.
using Perlin = PerlinNoise<float>;

} //namespace rnd
