// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <sparse/IMatrix.h>
#include <mem/PointerReleaser.h>

namespace sparse
{
    enum class SolverType : unsigned char {LU = 0, LLT, LDLT, Ctrl, Pardiso};

	template <typename TVAL, typename INDEX >
	class ISolver
	{
	public:
        //C, C++ interface (i,j- zero based ---> 0,1,2,....)
		virtual void addTriple(INDEX i, INDEX j, const  TVAL& val) = 0; //index base 0
        virtual void setRHS(INDEX i, const TVAL& val) = 0; //base
        virtual TVAL& RHS(INDEX i) = 0; //get or set RHS element
        virtual const TVAL& RHS(INDEX i) const = 0; //get RHS element
        virtual const TVAL& x(INDEX i) const = 0; //get solution element
        
        //Matlab, Fortran interface (i,j - zero based ->1,2,3,...)
		virtual void addTriple1(INDEX I, INDEX J, const TVAL& val) = 0; //index base 1
        virtual void setRHS1(INDEX I, const TVAL& val) = 0; //base 1
        virtual TVAL& RHS1(INDEX I) = 0; //get or set RHS element
        virtual const TVAL& RHS1(INDEX I) const = 0; //get RHS element
        virtual const TVAL& x1(INDEX I) const = 0; //get solution element
    
        //below is interface independent on indexing or 0-based
        virtual void populateDiagonals(const TVAL& val) = 0;
		virtual void clearRHS() = 0; //clean free vector
		
		//virtual TVAL* getX() = 0; //get whole vector of unknown elements
		virtual INDEX size() const = 0;
		//RHS
        //setRHS: C, C++ interface (i,j- zero based ---> 0,1,2,....)
		virtual void setRHS(const cnt::SafeFullVector<TVAL>& rhs) = 0;
        virtual void getRHS(cnt::SafeFullVector<TVAL>& rhs) const = 0;
        
		//RHS max abs value
		virtual void setRHSMaxValueStartingPos(INDEX i) = 0; //-1, RHS max abs value will not be tracked
		virtual double getRHSMaxValue() = 0;
		virtual double calcMaxRHSValue(INDEX startPos, INDEX endPos) = 0;
		//solution		
		virtual bool factorize() = 0;
		virtual const char* getLastError() const  = 0;
		virtual bool solve() = 0;
		virtual bool solveExt(const TVAL* rhs, TVAL* x) = 0;
		virtual void release() = 0;
		//virtual virtual double normRHS(int startPos=0, int length=-1) = 0; //startPos is starting element of RHS
		virtual void serialize(const char* name, std::ostream& s, Format vf = sparse::Format::Matlab) const = 0;
		virtual void showResults(const char* resName, std::ostream& s, sparse::Format fmt = sparse::Format::Matlab) const = 0;
		virtual void showRHS(const char* rhsName, std::ostream& s, sparse::Format fmt = sparse::Format::Matlab) const = 0;
		virtual int getNoOfNonZero() const = 0;
	};

	typedef ISolver<double, td::INT4> DblSolver;
	typedef ISolver< std::complex<double>, td::INT4> CmplxSolver;

    typedef mem::PointerReleaser<DblSolver> DblSolverReleaser;
    typedef mem::PointerReleaser<CmplxSolver> CmplxSolverReleaser;

	MATRIX_API void setBLASPlatform(BLAS::Platform blasPlatform);

	MATRIX_API void setNoThreads(int noThreads);

	MATRIX_API void releaseSolverLibraries();

    MATRIX_API sparse::DblSolver* createDblSolver(int NMAT, int NZ, sparse::Symmetry = sparse::Symmetry::NonSymmetric, sparse::SolverType solverType = sparse::SolverType::LU, sparse::Pivoting pivoting = sparse::Pivoting::DiagonalSinglePass, sparse::Ordering rowOrdering=sparse::Ordering::Own);

	MATRIX_API sparse::CmplxSolver* createCmplxSolver(int NMAT, int NZ, sparse::Symmetry = sparse::Symmetry::NonSymmetric, sparse::SolverType solverType = sparse::SolverType::LU, sparse::Pivoting pivoting = sparse::Pivoting::DiagonalSinglePass, sparse::Ordering rowOrdering=sparse::Ordering::Own);
}
