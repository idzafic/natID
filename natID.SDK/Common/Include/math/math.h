// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file math.h
    @brief Collection of template math utility functions for scalar and complex arithmetic, coordinate transforms, and point operations. */
#pragma once
#include <td/Types.h>
#include <math/Constants.h>
#include <complex>
#include <compiler/MinMaxFix.h>
#include <td/Point.h>
#include <cmath>

namespace math
{
//Note: Velika slova zbog glupog min max macro problema na Visual Studio-uflow
//Ne mijenjati u mala slova
/// @brief Returns the larger of two values.
/// @tparam TNUM Numeric type supporting operator>.
/// @param a First value.
/// @param b Second value.
/// @return The greater of a and b.
template <typename TNUM>
constexpr inline TNUM Max(TNUM a, TNUM b)
{
    return ((a > b) ? a : b);
}

//Note: Velika slova zbog glupog min max macro problema na Visual Studio-uflow
//Ne mijenjati u mala slova
/// @brief Returns the smaller of two values.
/// @tparam TNUM Numeric type supporting operator<.
/// @param a First value.
/// @param b Second value.
/// @return The lesser of a and b.
template <typename TNUM>
constexpr inline TNUM Min(TNUM a, TNUM b)
{
    return ((a < b) ? a : b);
}

/// @brief Computes the squared magnitude of a complex number (real^2 + imag^2).
/// @tparam TVAL Floating-point component type.
/// @param x Complex input value.
/// @return Squared magnitude as TVAL.
template <typename TVAL>
constexpr inline TVAL square(const std::complex<TVAL>& x)
{
    return x.real()*x.real() + x.imag()*x.imag();
}

/// @brief Computes the absolute value (magnitude) of a complex number.
/// @tparam TVAL Floating-point component type.
/// @param x Complex input value.
/// @return Magnitude as TVAL.
template <typename TVAL>
constexpr inline TVAL abs(const std::complex<TVAL>& x)
{
    return std::sqrt(math::square(x));
}

/// @brief Returns the magnitude of a complex number (alias for math::abs).
/// @tparam TVAL Floating-point component type.
/// @param x Complex input value.
/// @return Magnitude as TVAL.
template <typename TVAL>
constexpr inline TVAL magnitude(const std::complex<TVAL>& x)
{
    return math::abs(x);
}

/// @brief Computes the phase angle of a complex number in radians.
/// @tparam TVAL Floating-point component type.
/// @param x Complex input value.
/// @return Phase angle in radians as TVAL.
template <typename TVAL>
constexpr inline TVAL angle(const std::complex<TVAL>& x)
{
    return std::atan2(x.imag(), x.real());
}

/// @brief Computes the phase angle of a complex number in degrees.
/// @tparam TVAL Floating-point component type.
/// @param x Complex input value.
/// @return Phase angle in degrees as TVAL.
template <typename TVAL>
constexpr inline TVAL angleDeg(const std::complex<TVAL>& x)
{
    return math::angle(x) * 180 / (TVAL) math::DBL_PI;
}

/// @brief Computes the absolute value of a scalar.
/// @tparam TVAL Numeric type.
/// @param x Input scalar value.
/// @return Absolute value of x.
template <typename TVAL>
constexpr inline TVAL abs(TVAL x)
{
    return ((x > 0) ? x : -x);
}

/// @brief Returns the maximum of the absolute values of the real and imaginary parts of a complex number.
/// @tparam TVAL Floating-point component type.
/// @param val Complex input value.
/// @return Max of |real| and |imag| as TVAL.
template <typename TVAL>
constexpr inline TVAL cmplxMaxAbs(const std::complex<TVAL>& val)
{
    return math::Max(math::abs(val.real()), math::abs(val.imag()));
}

/// @brief Conjugates a complex number in-place by negating the imaginary part.
/// @tparam TVAL Floating-point component type.
/// @param x Complex value to conjugate in place.
template <typename TVAL>
inline void conj(std::complex<TVAL>& x)
{
    TVAL imag = x.imag();
	if (imag != 0)
	{
		x.imag(-imag);
	}
}

/// @brief Converts polar form (magnitude, angle) to rectangular complex form.
/// @tparam T Floating-point type.
/// @param mag Magnitude of the complex number.
/// @param angle Phase angle in radians.
/// @return Complex number in rectangular form.
template <typename T>
constexpr inline std::complex<T> toRect(T mag, T angle)
{
    return std::complex<T>(mag * std::cos(angle), mag * std::sin(angle));
}

/// @brief Converts a complex number from rectangular to polar form in-place (real = magnitude, imag = angle).
/// @tparam T Floating-point type.
/// @param val Complex value to convert; on return real holds magnitude and imag holds angle in radians.
template <typename T>
inline void toPolar(std::complex<T>& val)
{
    T angle = std::atan2(val.imag(), val.real());
	T mag = std::abs(val);
	val.real(mag);
	val.imag(angle);
}

/// @brief Converts a complex number from polar form (real = magnitude, imag = angle) to rectangular form in-place.
/// @tparam T Floating-point type.
/// @param val Complex value where real is magnitude and imag is angle in radians; on return holds rectangular form.
template <typename T>
inline void toRect(std::complex<T>& val)
{
    T angle = val.imag();
	T mag = val.real();
	if (mag > 0)
	{
		val.real(mag*std::cos(angle));
		val.imag(mag*std::sin(angle));
	}
	else
	{
		val.real(0);
		val.imag(0);
	}
}

/// @brief Converts an angle from degrees to radians.
/// @tparam VALUE Floating-point type.
/// @param degrees Angle in degrees.
/// @return Angle in radians.
template<typename VALUE>
constexpr inline VALUE radians(const VALUE degrees)
{
    return degrees * (VALUE)math::DBL_PI / (VALUE)180.;
}

/// @brief Converts an angle from radians to degrees.
/// @tparam VALUE Floating-point type.
/// @param radians Angle in radians.
/// @return Angle in degrees.
template<typename VALUE>
constexpr inline VALUE degrees(const VALUE radians)
{
    return radians * (VALUE)180. / (VALUE)math::DBL_PI;
}

/// @brief Rotates an array of 2-D points by a given angle in degrees.
/// @tparam T Coordinate value type.
/// @tparam T2 Rotation angle type (convertible to T).
/// @param pts Pointer to the array of points to rotate in-place.
/// @param nPoints Number of points in the array.
/// @param rotationDeg Rotation angle in degrees (counter-clockwise).
template<typename T, typename T2>
void rotateDeg(td::Point<T>* pts, size_t nPoints, T2 rotationDeg)
{
    T rotationRad = rotationDeg * math::DBL_PI / 180.;
    std::complex<T> rot(std::cos(rotationRad), std::sin(rotationRad));
    for (size_t i=0; i<nPoints; ++i)
    {
        std::complex<T> pti(pts[i].x, pts[i].y);
        pti *= rot;
        pts[i].x = pti.real();
        pts[i].y = pti.imag();
    }
}

/// @brief Translates an array of 2-D points by (dx, dy).
/// @tparam T Coordinate value type.
/// @tparam T2 Translation offset type (convertible to T).
/// @param pts Pointer to the array of points to translate in-place.
/// @param nPoints Number of points in the array.
/// @param dx Translation offset along the x-axis.
/// @param dy Translation offset along the y-axis.
template<typename T, typename T2>
void translate(td::Point<T>* pts, size_t nPoints, T2 dx, T2 dy)
{
    for (size_t i=0; i<nPoints; ++i)
    {
        pts[i].x += dx;
        pts[i].y += dy;
    }
}

/// @brief Scales an array of 2-D points by (sx, sy).
/// @tparam T Coordinate value type.
/// @tparam T2 Scale factor type (convertible to T).
/// @param pts Pointer to the array of points to scale in-place.
/// @param nPoints Number of points in the array.
/// @param sx Scale factor along the x-axis.
/// @param sy Scale factor along the y-axis.
template<typename T, typename T2>
void scale(td::Point<T>* pts, size_t nPoints, T2 sx, T2 sy)
{
    for (size_t i=0; i<nPoints; ++i)
    {
        pts[i].x *= sx;
        pts[i].y *= sy;
    }
}

/// @brief Clamps a value to the inclusive range [minVal, maxVal].
/// @tparam T Numeric type supporting comparison operators.
/// @param val Value to clamp.
/// @param minVal Lower bound of the range.
/// @param maxVal Upper bound of the range.
/// @return val clamped to [minVal, maxVal].
template<typename T>
T clip(T val, T minVal, T maxVal)
{
	if (val < minVal)
		return minVal;
	if (val > maxVal)
		return maxVal;
	return val;
}


//    template <typename T>
//    inline T magnitude(std::complex<T> val)
//    {
//        return std::abs(val);
//    }
//
//    template <typename T>
//    inline T angle(std::complex<T> val)
//    {
//        return std::arg(val);
//    }

}
