// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <sparse/PivotAndOrder.h>
#include <BLAS/Platform.h>
#include <complex>
#include <td/Types.h>
#include <cnt/SafeFullVector.h>
#include <sparse/SerializerMatrixEntry.h>
#include <iostream>
#include <matrix/MatrixLib.h>
#include <dense/DiagMatrix.h>
#include <tuple>
#include <mem/PointerReleaser.h>

namespace sparse
{

    template <typename INDEX, typename TVAL>
    class ISolver;

	template <typename INDEX, typename TVAL>
	class IMatrix
	{
	public:
		virtual void addTriple(INDEX i, INDEX j, const  TVAL& val) = 0; //index base 0
        virtual void addTriple1(INDEX I, INDEX J, const  TVAL& val) = 0; //index base 1
        
		virtual INDEX getNoOfRows() const = 0;
        virtual INDEX getNoOfCols() const = 0;
        virtual size_t getNoOfNonZero() const = 0;
		virtual void serialize(const char* name, std::ostream& s, Format vf = sparse::Format::Matlab) const = 0;
        
        virtual std::tuple<INDEX, INDEX> getMaxDimension() const = 0;
        virtual void setDimension(INDEX N, INDEX M) = 0;
        virtual void reset() = 0;
        
        //calcuate this' * w * this --> solver, this' * w * b -> solverRHS
        virtual void calcGainAndRHS(const dense::DblDiagMatrix& W, dense::Matrix<TVAL>& B, ISolver<TVAL, INDEX>* pToSolver) = 0;
        //this is matrix E transfered to solver in form
        //[G E'; E 0]
        virtual bool addAsEqualityConstraint(int fromPosition, dense::Matrix<TVAL>& b,  ISolver<TVAL, INDEX>* pToSolver) const = 0;
        
        virtual void release() = 0;
	};

	typedef IMatrix<td::INT4, double> IDblMatrix;
	typedef IMatrix<td::INT4, std::complex<double> > ICmplxMatrix;

    MATRIX_API sparse::IDblMatrix* createDblMatrix(int n, int m, int NZ, sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric);
	MATRIX_API sparse::ICmplxMatrix* createCmplxMatrix(int n, int m, int NZ, sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric);
	 
    //export (when method show was called)
	MATRIX_API void showDblVector(const char* lbl, const char* vecName, std::ostream& s, const cnt::SafeFullVector<double>& res, sparse::Format fmt = sparse::Format::Matlab);
	MATRIX_API void showCmplxVector(const char* lbl, const char* vecName, std::ostream& s, const cnt::SafeFullVector<td::cmplx>& res, sparse::Format fmt = sparse::Format::Matlab);

	MATRIX_API void showDblMatrix(const char* lbl, const char* matName, std::ostream& s, const cnt::SafeFullVector<sparse::SerializerDblMatrixEntry>& mat, int NMAT, int nCols, sparse::Format fmt = sparse::Format::Matlab);
	MATRIX_API void showCmplxMatrix(const char* lbl, const char* matName, std::ostream& s, const cnt::SafeFullVector<SerializerCmplxMatrixEntry>& mat, int NMAT, int nCols, sparse::Format fmt = sparse::Format::Matlab);

	MATRIX_API void setNumberOfDigitsForOutput(int nDigits);

    typedef mem::PointerReleaser<IDblMatrix> DblMatrixReleaser;
    typedef mem::PointerReleaser<ICmplxMatrix> CmplxMatrixReleaser;
}
