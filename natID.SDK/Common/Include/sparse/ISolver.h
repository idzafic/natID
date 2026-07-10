// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISolver.h
 @brief Abstract interface for sparse linear system solvers with zero- and one-based indexing. */
#pragma once
#include <sparse/IMatrix.h>
#include <mem/PointerReleaser.h>

namespace sparse
{
    /// @brief Enumeration of supported sparse solver algorithms.
    enum class SolverType : unsigned char {LU = 0,  ///< LU factorization (general)
                                           LLT,     ///< Cholesky (symmetric positive definite)
                                           LDLT,    ///< LDLT factorization (symmetric indefinite)
                                           Ctrl,    ///< Control-based solver
                                           Pardiso  ///< Intel Pardiso direct solver
                                          };

    /// @brief Abstract interface for a sparse linear system solver Ax = b.
    /// @tparam TVAL Numeric value type (e.g., double or complex<double>).
    /// @tparam INDEX Integer index type (e.g., td::INT4).
	template <typename TVAL, typename INDEX >
	class ISolver
	{
	public:
        //C, C++ interface (i,j- zero based ---> 0,1,2,....)
        /// @brief Adds a triplet (i, j, val) to the sparse matrix using zero-based indices.
        /// @param i Zero-based row index.
        /// @param j Zero-based column index.
        /// @param val Value to add at position (i, j).
		virtual void addTriple(INDEX i, INDEX j, const  TVAL& val) = 0; //index base 0

        /// @brief Sets the right-hand side vector element at zero-based index i.
        /// @param i Zero-based index into the RHS vector.
        /// @param val Value to assign.
        virtual void setRHS(INDEX i, const TVAL& val) = 0; //base

        /// @brief Returns a mutable reference to the RHS element at zero-based index i.
        /// @param i Zero-based index.
        /// @return Reference to the RHS element.
        virtual TVAL& RHS(INDEX i) = 0; //get or set RHS element

        /// @brief Returns a const reference to the RHS element at zero-based index i.
        /// @param i Zero-based index.
        /// @return Const reference to the RHS element.
        virtual const TVAL& RHS(INDEX i) const = 0; //get RHS element

        /// @brief Returns the solution element at zero-based index i after solve.
        /// @param i Zero-based index.
        /// @return Const reference to the solution element.
        virtual const TVAL& x(INDEX i) const = 0; //get solution element

        //Matlab, Fortran interface (i,j - zero based ->1,2,3,...)
        /// @brief Adds a triplet (I, J, val) to the sparse matrix using one-based indices.
        /// @param I One-based row index.
        /// @param J One-based column index.
        /// @param val Value to add at position (I, J).
		virtual void addTriple1(INDEX I, INDEX J, const TVAL& val) = 0; //index base 1

        /// @brief Sets the right-hand side vector element at one-based index I.
        /// @param I One-based index into the RHS vector.
        /// @param val Value to assign.
        virtual void setRHS1(INDEX I, const TVAL& val) = 0; //base 1

        /// @brief Returns a mutable reference to the RHS element at one-based index I.
        /// @param I One-based index.
        /// @return Reference to the RHS element.
        virtual TVAL& RHS1(INDEX I) = 0; //get or set RHS element

        /// @brief Returns a const reference to the RHS element at one-based index I.
        /// @param I One-based index.
        /// @return Const reference to the RHS element.
        virtual const TVAL& RHS1(INDEX I) const = 0; //get RHS element

        /// @brief Returns the solution element at one-based index I after solve.
        /// @param I One-based index.
        /// @return Const reference to the solution element.
        virtual const TVAL& x1(INDEX I) const = 0; //get solution element

        //below is interface independent on indexing or 0-based
        /// @brief Fills all diagonal entries of the matrix with the given value.
        /// @param val Value to assign to each diagonal element.
        virtual void populateDiagonals(const TVAL& val) = 0;

		/// @brief Clears (zeros) the entire right-hand side vector.
		virtual void clearRHS() = 0; //clean free vector

		/// @brief Returns the dimension (number of rows/columns) of the square system.
		/// @return Size of the linear system.
		virtual INDEX size() const = 0;

		//RHS
        //setRHS: C, C++ interface (i,j- zero based ---> 0,1,2,....)
        /// @brief Sets the entire RHS vector from a SafeFullVector (zero-based).
        /// @param rhs Vector containing the right-hand side values.
		virtual void setRHS(const cnt::SafeFullVector<TVAL>& rhs) = 0;

        /// @brief Retrieves the current RHS vector into a SafeFullVector.
        /// @param rhs Output vector to be populated with RHS values.
        virtual void getRHS(cnt::SafeFullVector<TVAL>& rhs) const = 0;

		//RHS max abs value
        /// @brief Sets the starting position from which the maximum absolute RHS value is tracked.
        /// @param i Starting index; pass -1 to disable RHS max tracking.
		virtual void setRHSMaxValueStartingPos(INDEX i) = 0; //-1, RHS max abs value will not be tracked

        /// @brief Returns the maximum absolute value in the RHS vector starting from the configured position.
        /// @return Maximum absolute RHS value.
		virtual double getRHSMaxValue() = 0;

        /// @brief Computes the maximum absolute RHS value over elements in [startPos, endPos).
        /// @param startPos Starting index (inclusive).
        /// @param endPos Ending index (exclusive).
        /// @return Maximum absolute value found in the specified range.
		virtual double calcMaxRHSValue(INDEX startPos, INDEX endPos) = 0;

		//solution
		/// @brief Performs matrix factorization in preparation for solving.
		/// @return True if factorization succeeded, false otherwise.
		virtual bool factorize() = 0;

        /// @brief Returns the last error message from the solver.
        /// @return C-string describing the last error, or empty string if none.
		virtual const char* getLastError() const  = 0;

        /// @brief Solves the linear system Ax = b using the previously factored matrix.
        /// @return True if the solve succeeded, false otherwise.
		virtual bool solve() = 0;

        /// @brief Solves the system using externally supplied RHS and solution buffers.
        /// @param rhs Pointer to the right-hand side array.
        /// @param x Pointer to the output solution array.
        /// @return True if the solve succeeded, false otherwise.
		virtual bool solveExt(const TVAL* rhs, TVAL* x) = 0;

        /// @brief Releases all resources held by this solver object.
		virtual void release() = 0;

        /// @brief Serializes the sparse matrix to an output stream in the specified format.
        /// @param name Label for the matrix in the output.
        /// @param s Output stream to write to.
        /// @param vf Output format (default: Matlab).
		virtual void serialize(const char* name, std::ostream& s, Format vf = sparse::Format::Matlab) const = 0;

        /// @brief Writes the solution vector to an output stream in the specified format.
        /// @param resName Label for the result in the output.
        /// @param s Output stream to write to.
        /// @param fmt Output format (default: Matlab).
		virtual void showResults(const char* resName, std::ostream& s, sparse::Format fmt = sparse::Format::Matlab) const = 0;

        /// @brief Writes the RHS vector to an output stream in the specified format.
        /// @param rhsName Label for the RHS in the output.
        /// @param s Output stream to write to.
        /// @param fmt Output format (default: Matlab).
		virtual void showRHS(const char* rhsName, std::ostream& s, sparse::Format fmt = sparse::Format::Matlab) const = 0;

        /// @brief Returns the number of non-zero entries in the sparse matrix.
        /// @return Count of non-zero elements.
		virtual int getNoOfNonZero() const = 0;
	};

    /// @brief Convenience typedef for a double-precision sparse solver with INT4 indexing.
	typedef ISolver<double, td::INT4> DblSolver;
    /// @brief Convenience typedef for a complex double-precision sparse solver with INT4 indexing.
	typedef ISolver< std::complex<double>, td::INT4> CmplxSolver;

    /// @brief RAII wrapper that auto-releases a DblSolver on destruction.
    typedef mem::PointerReleaser<DblSolver> DblSolverReleaser;
    /// @brief RAII wrapper that auto-releases a CmplxSolver on destruction.
    typedef mem::PointerReleaser<CmplxSolver> CmplxSolverReleaser;

    /// @brief Sets the BLAS platform to use for matrix operations.
    /// @param blasPlatform The BLAS platform identifier.
	MATRIX_API void setBLASPlatform(BLAS::Platform blasPlatform);

    /// @brief Sets the number of threads available for parallel solver operations.
    /// @param noThreads Number of threads to use.
	MATRIX_API void setNoThreads(int noThreads);

    /// @brief Releases any dynamically loaded solver libraries.
	MATRIX_API void releaseSolverLibraries();

    /// @brief Creates a double-precision sparse solver with the given dimensions and options.
    /// @param NMAT Number of rows/columns in the square system.
    /// @param NZ Estimated number of non-zero entries.
    /// @param symmetry Symmetry type of the matrix (default: NonSymmetric).
    /// @param solverType Algorithm to use for factorization (default: LU).
    /// @param pivoting Pivoting strategy (default: DiagonalSinglePass).
    /// @param rowOrdering Row reordering strategy (default: Own).
    /// @return Pointer to the created solver; caller must call release() when done.
    MATRIX_API sparse::DblSolver* createDblSolver(int NMAT, int NZ, sparse::Symmetry = sparse::Symmetry::NonSymmetric, sparse::SolverType solverType = sparse::SolverType::LU, sparse::Pivoting pivoting = sparse::Pivoting::DiagonalSinglePass, sparse::Ordering rowOrdering=sparse::Ordering::Own);

    /// @brief Creates a complex double-precision sparse solver with the given dimensions and options.
    /// @param NMAT Number of rows/columns in the square system.
    /// @param NZ Estimated number of non-zero entries.
    /// @param symmetry Symmetry type of the matrix (default: NonSymmetric).
    /// @param solverType Algorithm to use for factorization (default: LU).
    /// @param pivoting Pivoting strategy (default: DiagonalSinglePass).
    /// @param rowOrdering Row reordering strategy (default: Own).
    /// @return Pointer to the created solver; caller must call release() when done.
	MATRIX_API sparse::CmplxSolver* createCmplxSolver(int NMAT, int NZ, sparse::Symmetry = sparse::Symmetry::NonSymmetric, sparse::SolverType solverType = sparse::SolverType::LU, sparse::Pivoting pivoting = sparse::Pivoting::DiagonalSinglePass, sparse::Ordering rowOrdering=sparse::Ordering::Own);
}
