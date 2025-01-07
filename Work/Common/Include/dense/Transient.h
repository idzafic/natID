// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
	typedef struct _TR
	{
		double magnitude;
		double frequency;
		double thau;
		double phiRad;
		double phiDeg;
	}Transient;

	class Transients : public std::vector<Transient>
	{
	public:
		Transients()
			: std::vector<Transient>()
		{}

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
		void show(std::ostream* pO) const
		{
			if (pO != nullptr)
				show(*pO);
		}
	};

	TRANSIENT_API int calcPronyMATLABLike(double fs, int nRows, int nComponents, const matrix::General<double>& signal, matrix::General<double>& res, double eps = 1e-5);
	TRANSIENT_API int calcProny(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<double>& signal, Transients& res, double eps = 1e-5);
	TRANSIENT_API int estimateNumberOfHarmonics(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<double>& signal, double eps = 1e-5);

	TRANSIENT_API int calcProny(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<std::complex<double>>& signal, Transients& res, double eps = 1e-5);
	TRANSIENT_API int estimateNumberOfHarmonics(int nSamplesToUse, int nMaxExpectedNonNegFreqComponents, const Signal<std::complex<double>>& signal, double eps = 1e-5);

	//for debug
	TRANSIENT_API void setTransientDebugStream(std::ostream* pDbgOutStream);
}

