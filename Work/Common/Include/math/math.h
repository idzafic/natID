// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <math/Constants.h>
#include <complex>
#include <compiler/MinMaxFix.h>
#include <td/Point.h>

namespace math
{
//Note: Velika slova zbog glupog min max macro problema na Visual Studio-uflow
//Ne mijenjati u mala slova
template <typename TNUM>
constexpr inline TNUM Max(TNUM a, TNUM b)
{
    return ((a > b) ? a : b);
}

//Note: Velika slova zbog glupog min max macro problema na Visual Studio-uflow
//Ne mijenjati u mala slova
template <typename TNUM>
constexpr inline TNUM Min(TNUM a, TNUM b)
{
    return ((a < b) ? a : b);
}

template <typename TVAL>
constexpr inline TVAL square(const std::complex<TVAL>& x)
{
    return x.real()*x.real() + x.imag()*x.imag();
}

template <typename TVAL>
constexpr inline TVAL abs(const std::complex<TVAL>& x)
{
    return std::sqrt(math::square(x));
}

template <typename TVAL>
constexpr inline TVAL magnitude(const std::complex<TVAL>& x)
{
    return math::abs(x);
}

template <typename TVAL>
constexpr inline TVAL angle(const std::complex<TVAL>& x)
{
    return std::atan2(x.imag(), x.real());
}

template <typename TVAL>
constexpr inline TVAL angleDeg(const std::complex<TVAL>& x)
{
    return math::angle(x) * 180 / (TVAL) math::DBL_PI;
}

template <typename TVAL>
constexpr inline TVAL abs(TVAL x)
{
    return ((x > 0) ? x : -x);
}

template <typename TVAL>
constexpr inline TVAL cmplxMaxAbs(const std::complex<TVAL>& val)
{
    return math::Max(math::abs(val.real()), math::abs(val.imag()));
}

template <typename TVAL>
inline void conj(std::complex<TVAL>& x)
{
    TVAL imag = x.imag();
	if (imag != 0)
	{
		x.imag(-imag);
	}
}

template <typename T>
constexpr inline std::complex<T> toRect(T mag, T angle)
{
    return std::complex<T>(mag * std::cos(angle), mag * std::sin(angle));
}

template <typename T>
inline void toPolar(std::complex<T>& val)
{
    T angle = std::atan2(val.imag(), val.real());
	T mag = std::abs(val);
	val.real(mag);
	val.imag(angle);
}

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

template<typename VALUE>
constexpr inline VALUE radians(const VALUE degrees)
{
    return degrees * (VALUE)math::DBL_PI / (VALUE)180.;
}

template<typename VALUE>
constexpr inline VALUE degrees(const VALUE radians)
{
    return radians * (VALUE)180. / (VALUE)math::DBL_PI;
}

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

template<typename T, typename T2>
void translate(td::Point<T>* pts, size_t nPoints, T2 dx, T2 dy)
{
    for (size_t i=0; i<nPoints; ++i)
    {
        pts[i].x += dx;
        pts[i].y += dy;
    }
}

template<typename T, typename T2>
void scale(td::Point<T>* pts, size_t nPoints, T2 sx, T2 sy)
{
    for (size_t i=0; i<nPoints; ++i)
    {
        pts[i].x *= sx;
        pts[i].y *= sy;
    }
}

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
