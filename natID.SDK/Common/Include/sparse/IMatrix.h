// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IMatrix.h
    @brief Declares the IMatrix interface for sparse matrix operations and provides factory functions. */
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

    /// @brief Forward declaration of the sparse solver interface.
    /// @tparam INDEX Integer type used for row and column indices.
    /// @tparam TVAL Scalar value type stored in the matrix.
    template <typename INDEX, typename TVAL>
    class ISolver;

    /// @brief Abstract interface for a sparse matrix supporting triplet insertion, querying, and solver integration.
    /// @tparam INDEX Integer type used for row and column indices.
    /// @tparam TVAL Scalar value type stored in the matrix.
	template <typename INDEX, typename TVAL>
	class IMatrix
	{
	public:
        /// @brief Inserts or accumulates a value at position (i, j) using zero-based indexing.
        /// @param i Zero-based row index.
        /// @param j Zero-based column index.
        /// @param val Value to add at (i, j).
		virtual void addTriple(INDEX i, INDEX j, const  TVAL& val) = 0; //index base 0

        /// @brief Inserts or accumulates a value at position (I, J) using one-based indexing.
        /// @param I One-based row index.
        /// @param J One-based column index.
        /// @param val Value to add at (I, J).
        virtual void addTriple1(INDEX I, INDEX J, const  TVAL& val) = 0; //index base 1

        /// @brief Returns the number of rows in the matrix.
        /// @return Row count.
		virtual INDEX getNoOfRows() const = 0;

        /// @brief Returns the number of columns in the matrix.
        /// @return Column count.
        virtual INDEX getNoOfCols() const = 0;

        /// @brief Returns the number of structurally non-zero entries.
        /// @return Non-zero entry count.
        virtual size_t getNoOfNonZero() const = 0;

        /// @brief Serializes the matrix to an output stream in the specified format.
        /// @param name Label string written as a prefix in the output.
        /// @param s Output stream to write to.
        /// @param vf Output format (default: Matlab).
		virtual void serialize(const char* name, std::ostream& s, Format vf = sparse::Format::Matlab) const = 0;

        /// @brief Returns the maximum (rows, cols) dimension pair of the matrix.
        /// @return A tuple containing (max_rows, max_cols).
        virtual std::tuple<INDEX, INDEX> getMaxDimension() const = 0;

        /// @brief Sets the explicit dimensions of the matrix.
        /// @param N Number of rows.
        /// @param M Number of columns.
        virtual void setDimension(INDEX N, INDEX M) = 0;

        /// @brief Resets the matrix to an empty state, clearing all entries.
        virtual void reset() = 0;

        /// @brief Computes the gain matrix (this^T * W * this) and the right-hand side (this^T * W * B) into the solver.
        /// @param W Diagonal weight matrix.
        /// @param B Dense right-hand side matrix.
        /// @param pToSolver Pointer to the solver that receives the gain matrix and RHS.
        virtual void calcGainAndRHS(const dense::DblDiagMatrix& W, dense::Matrix<TVAL>& B, ISolver<TVAL, INDEX>* pToSolver) = 0;

        /// @brief Transfers this matrix as an equality constraint E into the solver in the form [G E'; E 0].
        /// @param fromPosition Starting row/column offset in the solver.
        /// @param b Dense right-hand side vector for the equality constraint.
        /// @param pToSolver Pointer to the target solver.
        /// @return True if the constraint was added successfully, false otherwise.
        virtual bool addAsEqualityConstraint(int fromPosition, dense::Matrix<TVAL>& b,  ISolver<TVAL, INDEX>* pToSolver) const = 0;

        /// @brief Releases this matrix object and frees its resources.
        virtual void release() = 0;
	};

	typedef IMatrix<td::INT4, double> IDblMatrix; ///< Convenience typedef for a real-valued sparse matrix with 32-bit indices.
	typedef IMatrix<td::INT4, std::complex<double> > ICmplxMatrix; ///< Convenience typedef for a complex-valued sparse matrix with 32-bit indices.

    /// @brief Factory function that creates a real-valued sparse matrix.
    /// @param n Number of rows.
    /// @param m Number of columns.
    /// @param NZ Estimated number of non-zero entries.
    /// @param symmetry Symmetry type of the matrix (default: NonSymmetric).
    /// @return Pointer to the newly created IDblMatrix (caller must call release()).
    MATRIX_API sparse::IDblMatrix* createDblMatrix(int n, int m, int NZ, sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric);

    /// @brief Factory function that creates a complex-valued sparse matrix.
    /// @param n Number of rows.
    /// @param m Number of columns.
    /// @param NZ Estimated number of non-zero entries.
    /// @param symmetry Symmetry type of the matrix (default: NonSymmetric).
    /// @return Pointer to the newly created ICmplxMatrix (caller must call release()).
	MATRIX_API sparse::ICmplxMatrix* createCmplxMatrix(int n, int m, int NZ, sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric);

    /// @brief Serializes a real-valued dense vector to the given stream in the specified format.
    /// @param lbl Namespace or label prefix for the output.
    /// @param vecName Variable name written in the output.
    /// @param s Output stream to write to.
    /// @param res The vector of double values to serialize.
    /// @param fmt Output format (default: Matlab).
	MATRIX_API void showDblVector(const char* lbl, const char* vecName, std::ostream& s, const cnt::SafeFullVector<double>& res, sparse::Format fmt = sparse::Format::Matlab);

    /// @brief Serializes a complex-valued dense vector to the given stream in the specified format.
    /// @param lbl Namespace or label prefix for the output.
    /// @param vecName Variable name written in the output.
    /// @param s Output stream to write to.
    /// @param res The vector of complex values to serialize.
    /// @param fmt Output format (default: Matlab).
	MATRIX_API void showCmplxVector(const char* lbl, const char* vecName, std::ostream& s, const cnt::SafeFullVector<td::cmplx>& res, sparse::Format fmt = sparse::Format::Matlab);

    /// @brief Serializes a real-valued sparse matrix (in entry-list form) to the given stream.
    /// @param lbl Namespace or label prefix for the output.
    /// @param matName Variable name written in the output.
    /// @param s Output stream to write to.
    /// @param mat Vector of serialized matrix entries.
    /// @param NMAT Number of rows in the matrix.
    /// @param nCols Number of columns in the matrix.
    /// @param fmt Output format (default: Matlab).
	MATRIX_API void showDblMatrix(const char* lbl, const char* matName, std::ostream& s, const cnt::SafeFullVector<sparse::SerializerDblMatrixEntry>& mat, int NMAT, int nCols, sparse::Format fmt = sparse::Format::Matlab);

    /// @brief Serializes a complex-valued sparse matrix (in entry-list form) to the given stream.
    /// @param lbl Namespace or label prefix for the output.
    /// @param matName Variable name written in the output.
    /// @param s Output stream to write to.
    /// @param mat Vector of serialized complex matrix entries.
    /// @param NMAT Number of rows in the matrix.
    /// @param nCols Number of columns in the matrix.
    /// @param fmt Output format (default: Matlab).
	MATRIX_API void showCmplxMatrix(const char* lbl, const char* matName, std::ostream& s, const cnt::SafeFullVector<SerializerCmplxMatrixEntry>& mat, int NMAT, int nCols, sparse::Format fmt = sparse::Format::Matlab);

    /// @brief Sets the number of significant digits used when writing matrix/vector values to output.
    /// @param nDigits Number of decimal digits to display.
	MATRIX_API void setNumberOfDigitsForOutput(int nDigits);

    typedef mem::PointerReleaser<IDblMatrix> DblMatrixReleaser; ///< RAII wrapper that calls release() on an IDblMatrix pointer.
    typedef mem::PointerReleaser<ICmplxMatrix> CmplxMatrixReleaser; ///< RAII wrapper that calls release() on an ICmplxMatrix pointer.
}
