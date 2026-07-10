// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Transient.h @brief Prony-method transient decomposition: data structures and API. */
#pragma once
#include <BLAS/BLASInterface.h>
#include <matrix/General.h>
#include "Signal.h"
#include <vector>

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the TRANSIENT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// TRANSIENT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TRANSIENT_EXPORTS
#define TRANSIENT_API __declspec(dllexport)
#else
#define TRANSIENT_API __declspec(dllimport)
#endif

// This class is exported from the Transient.dll

namespace trans
{
	/// @brief POD structure describing a single exponentially-damped sinusoidal component.
	typedef struct _TR
	{
		double magnitude;  ///< Amplitude of the component
		double frequency;  ///< Frequency in Hz
		double thau;       ///< Damping coefficient (time constant)
		double phiRad;     ///< Phase shift in radians
		double phiDeg;     ///< Phase shift in degrees
	}Transient;

	/// @brief Container of Transient components with formatted display support.
	class Transients : public std::vector<Transient>
	{
	public:
		/// @brief Default constructor — creates an empty transients container.
		Transients()
			: std::vector<Transient>()
		{}

		/// @brief Prints all transient components in a formatted table to the given stream.
		/// @param o Output stream (defaults to std::cout).
		void show(std::ostream& o = std::cout) const
		{
			std::streamsize ss = o.precision();
			o << "----------------------------------------------------------------" << std::endl;
			o << "   Magnitude      Damping    Frequency   PhaseShift   PhaseShift" << std::endl;
			o << "                                  [Hz]        [rad]        [deg]" << std::endl;
			o << "----------------------------------------------------------------" << std::endl;
			auto it = begin();
			auto itEnd = cend();
			while (it != itEnd)
			{
				o << std::fixed << std::setw(12) << std::setprecision(4) << it->magnitude << " ";
				o << std::fixed << std::setw(12) << std::setprecision(4) << it->thau << " ";
				o << std::fixed << std::setw(12) << std::setprecision(4) << it->frequency << " ";
				o << std::fixed << std::setw(12) << std::setprecision(4) << it->phiRad << " ";
				o << std::fixed << std::setw(12) << std::setprecision(4) << it->phiDeg << std::endl;
				++it;
			}
			o << "----------------------------------------------------------------" << std::endl;
			o << std::setprecision(ss); //set it back
		}
		/// @brief Prints all transient components to the stream pointed to by pO.
		/// @param pO Pointer to the output stream; does nothing if nullptr.
		void show(std::ostream* pO) const
		{
			if (pO != nullptr)
				show(*pO);
		}
	};

	/// @brief Computes Prony decomposition in a MATLAB-compatible manner.
	/// @param fs Sampling frequency in Hz.
	/// @param nRows Number of rows (samples) in the signal matrix.
	/// @param nComponents Maximum number of complex exponential components.
	/// @param signal Input signal matrix.
	/// @param res Output result matrix.
	/// @param eps Relative tolerance for rank determination.
	/// @return 0 on success, non-zero error code on failure.
	TRANSIENT_API int calcPronyMATLABLike(double fs, int nRows, int nComponents, const matrix::General<double>& signal, matrix::General<double>& res, double eps = 1e-5);

	/// @brief Computes Prony decomposition of a real-valued signal.
	/// @param nSamplesToUse Number of signal samples to use.
	/// @param nMaxExpectedNonNegFreqComponents Maximum number of non-negative frequency components expected.
	/// @param signal Input real-valued signal.
	/// @param res Output container of identified Transient components.
	/// @param eps Relative tolerance for rank determination.
	/// @return Number of identified components on success, negative on failure.
	TRANSIENT_API int calcProny(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<double>& signal, Transients& res, double eps = 1e-5);

	/// @brief Estimates the number of harmonics present in a real-valued signal.
	/// @param nSamplesToUse Number of signal samples to use.
	/// @param nMaxExpectedNonNegFreqComponents Upper bound on the number of harmonics.
	/// @param signal Input real-valued signal.
	/// @param eps Relative tolerance for rank determination.
	/// @return Estimated number of harmonics.
	TRANSIENT_API int estimateNumberOfHarmonics(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<double>& signal, double eps = 1e-5);

	/// @brief Computes Prony decomposition of a complex-valued signal.
	/// @param nSamplesToUse Number of signal samples to use.
	/// @param nMaxExpectedNonNegFreqComponents Maximum number of non-negative frequency components expected.
	/// @param signal Input complex-valued signal.
	/// @param res Output container of identified Transient components.
	/// @param eps Relative tolerance for rank determination.
	/// @return Number of identified components on success, negative on failure.
	TRANSIENT_API int calcProny(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<std::complex<double>>& signal, Transients& res, double eps = 1e-5);

	/// @brief Estimates the number of harmonics present in a complex-valued signal.
	/// @param nSamplesToUse Number of signal samples to use.
	/// @param nMaxExpectedNonNegFreqComponents Upper bound on the number of harmonics.
	/// @param signal Input complex-valued signal.
	/// @param eps Relative tolerance for rank determination.
	/// @return Estimated number of harmonics.
	TRANSIENT_API int estimateNumberOfHarmonics(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<std::complex<double>>& signal, double eps = 1e-5);

	//for debug
	/// @brief Sets the debug output stream used by the transient analysis library.
	/// @param pDbgOutStream Pointer to an output stream for diagnostic messages; nullptr disables output.
	TRANSIENT_API void setTransientDebugStream(std::ostream* pDbgOutStream);
}
