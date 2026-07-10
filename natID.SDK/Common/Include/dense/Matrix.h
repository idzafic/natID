// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Matrix.h
 @brief General dense matrix class with support for BLAS operations, inversion, and serialization. */
// interface for the General Matrix class
#pragma once
#include <matrix/MatrixLib.h>
#include <dense/IOManip.h>
#include <cassert>
#include <iostream>
#include <complex>
#include <tuple>
#include <sparse/Format.h>

namespace dense
{

/// @brief Forward declaration of the diagonal matrix template.
template <typename T>
class DiagMatrix;

/// @brief Forward declaration of the matrix memory manager template.
template <typename T>
class MatrixMgr;

/// @brief General dense matrix class stored in column-major order.
/// @tparam T Element type (e.g., double or std::complex<double>).
template <typename T>
class Matrix
{
    /// @brief Internal descriptor holding dimension and reference-count metadata.
    class Desc
    {
    public:
        td::UINT4 nRows = 0;    ///< Number of rows.
        td::UINT4 nCols = 0;    ///< Number of columns.
        td::UINT4 nRefs = 0;    ///< Reference count for copy-on-write.
        td::UINT4 flags = 0;    ///< Internal flags.
        td::UINT4 maxCols = 0;  ///< Maximum allocated columns.
        td::UINT4 maxRows = 0;  ///< Maximum allocated rows.
        union
        {
            MatrixMgr<T>* pMgr; ///< Optional memory manager pointer.
            char _padding[8];   ///< Padding to ensure consistent union size.
        };
    };

protected:
    Desc* _pDesc = nullptr; ///< Pointer to the shared descriptor block.

protected:

    /// @brief Returns the byte size of the Desc structure.
    /// @return Size in bytes of the internal descriptor.
    constexpr td::UINT4 getDescSize() const
    {
        return sizeof(Desc);
    }

    /// @brief Returns the current reference count.
    /// @return Number of Matrix objects sharing this data.
    td::UINT4 getNoOfRefs() const;

    /// @brief Increments the reference count.
    void addRef();

    /// @brief Decrements the reference count and frees data if it reaches zero.
    void relRef();

    /// @brief Returns the associated memory manager, if any.
    /// @return Pointer to the MatrixMgr, or nullptr if none is set.
    MatrixMgr<T>* getMgr() const;

    /// @brief Allocates internal data for a matrix of the specified dimensions.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    /// @param pMgr Optional memory manager.
    /// @param initWithZero If true, initializes all elements to zero.
    void allocData(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr, bool initWithZero = false);

    /// @brief Creates a private copy of the data if it is shared (copy-on-write).
    void duplicate();

    /// @brief Returns the conjugate of a double value (identity for real types).
    /// @param d Input value.
    /// @return The value unchanged.
    inline double conjugate(double d) const;

    /// @brief Returns a reference to the complex conjugate of the given value.
    /// @param d Input complex value.
    /// @return Reference to the conjugated value.
    inline std::complex<double>& conjugate(std::complex<double>& d) const;

    /// @brief Internal 1x1 matrix inversion helper.
    /// @return Tuple of (inverse matrix, success flag).
    std::tuple<Matrix, bool> invert1() const;

    /// @brief Internal 2x2 matrix inversion helper.
    /// @return Tuple of (inverse matrix, success flag).
    std::tuple<Matrix, bool> invert2() const;

    /// @brief Internal general matrix inversion helper.
    /// @return Tuple of (inverse matrix, success flag).
    std::tuple<Matrix, bool> invert3() const;

    /// @brief Returns the maximum number of rows that can be stored without reallocation.
    /// @return Maximum row capacity.
    td::UINT4 getMaxNoOfRows() const;

    /// @brief Returns the maximum number of columns that can be stored without reallocation.
    /// @return Maximum column capacity.
    td::UINT4 getMaxNoOfCols() const;

public:
    /// @brief Default constructor creating an empty matrix.
    Matrix();

    /// @brief Copy constructor; increments the reference count of the source data.
    /// @param A Source matrix to copy from.
    Matrix(const Matrix<T> &A);

    /// @brief Constructs a matrix with the given dimensions, optionally with a memory manager.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    /// @param pMgr Optional memory manager.
    /// @param initZero If true, initializes all elements to zero.
    Matrix(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr = nullptr, bool initZero = false);

    /// @brief Destructor; releases reference to the shared data.
    ~Matrix();

    //C and C++ interface (0-based index)
    /// @brief Returns a read-only accessor for the matrix using zero-based indexing.
    /// @return MatrixReader providing const element access.
    MatrixReader<T> getManipulator() const;

    /// @brief Returns a read-write accessor for the matrix using zero-based indexing.
    /// @return MatrixIO providing mutable element access.
    MatrixIO<T> getManipulator();

    /// @brief Returns a read-only column accessor starting at the specified column (zero-based).
    /// @param colPos Zero-based column position (default 0).
    /// @return ColumnReader for the given column.
    ColumnReader<T> getColumnManipulator(td::UINT4 colPos=0) const;

    /// @brief Returns a read-write column accessor starting at the specified column (zero-based).
    /// @param colPos Zero-based column position (default 0).
    /// @return ColumnIO for the given column.
    ColumnIO<T> getColumnManipulator(td::UINT4 colPos=0);

    /// @brief Returns a read-only accessor for the first column (zero-based).
    /// @return FirstColumnReader for column 0.
    FirstColumnReader<T> getFirstColumnManipulator() const;

    /// @brief Returns a read-write accessor for the first column (zero-based).
    /// @return FirstColumnIO for column 0.
    FirstColumnIO<T> getFirstColumnManipulator();

    //to simulate Matlab and Fortran (1-based index)
    /// @brief Returns a read-only accessor for the matrix using one-based indexing.
    /// @return MatrixReader1 providing const element access.
    MatrixReader1<T> getManipulator1() const;

    /// @brief Returns a read-write accessor for the matrix using one-based indexing.
    /// @return MatrixIO1 providing mutable element access.
    MatrixIO1<T> getManipulator1();

    /// @brief Returns a read-only column accessor using one-based indexing.
    /// @param colPos One-based column position (default 1).
    /// @return ColumnReader1 for the given column.
    ColumnReader1<T> getColumnManipulator1(td::UINT4 colPos=1) const;

    /// @brief Returns a read-write column accessor using one-based indexing.
    /// @param colPos One-based column position (default 1).
    /// @return ColumnIO1 for the given column.
    ColumnIO1<T> getColumnManipulator1(td::UINT4 colPos=1);

    /// @brief Returns a read-only accessor for the first column using one-based indexing.
    /// @return FirstColumnReader1 for column 1.
    FirstColumnReader1<T> getFirstColumnManipulator1() const;

    /// @brief Returns a read-write accessor for the first column using one-based indexing.
    /// @return FirstColumnIO1 for column 1.
    FirstColumnIO1<T> getFirstColumnManipulator1();

    /// @brief Releases the matrix data and resets to an empty state.
    void clean();

    /// @brief Loads matrix contents from an input stream.
    /// @param ar Input stream to read from.
    void load(std::istream& ar);

    /// @brief Saves matrix contents to an output stream with optional formatting.
    /// @param ar Output stream to write to.
    /// @param minVal Values with absolute value below this threshold are written as zero.
    /// @param width Field width for each element (-1 for default).
    /// @param precision Decimal precision for each element (-1 for default).
    void save(std::ostream& ar, double minVal = 0, int width = -1, int precision = -1) const;

    /// @brief Allocates storage for a matrix of the given size, optionally zeroing it.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    /// @param pMgr Optional memory manager.
    /// @param initZeros If true, initializes all elements to zero.
    void reserve(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr = nullptr, bool initZeros = false);

    /// @brief Copy assignment operator (reference-counted, copy-on-write).
    /// @param A Source matrix.
    void operator = (const Matrix& A);

    /// @brief Returns the number of rows.
    /// @return Row count.
    td::UINT4 getNoOfRows() const;

    /// @brief Returns the number of columns.
    /// @return Column count.
    td::UINT4 getNoOfCols() const;

    /// @brief Sets this matrix to the identity matrix (ones on the diagonal, zeros elsewhere).
    void diagOnes();

    /// @brief Sets all elements to zero.
    void zeros();

    /// @brief Loads matrix contents from a file.
    /// @param fileName Path to the input file.
    /// @return True if the file was opened and read successfully.
    bool load(const char *fileName);

    /// @brief Saves matrix contents to a file.
    /// @param fileName Path to the output file.
    /// @return True if the file was created and written successfully.
    bool save(const char *fileName);

    /// @brief Displays the matrix to an output stream with a label and optional formatting.
    /// @param o Output stream.
    /// @param name Label to print before the matrix.
    /// @param minVal Values below this threshold are displayed as zero.
    /// @param width Field width (-1 for default).
    /// @param precision Decimal precision (-1 for default).
    void show(std::ostream& o, const char* name, double minVal = 0, int width = -1, int precision = -1) const;

    ///this(mxn) <- A(mxk) x B(kxm)
    /// @brief Sets this matrix to the product A * B (or transposed variants).
    /// @param A Left-hand matrix operand.
    /// @param B Right-hand matrix operand.
    /// @param transpA Transposition flag for A ("N" = none, "T" = transpose, "C" = conjugate transpose).
    /// @param transpB Transposition flag for B ("N" = none, "T" = transpose, "C" = conjugate transpose).
    void fromMult(const Matrix& A, const Matrix& B, const char* transpA = "N", const char* transpB = "N");

    /// @brief Adds another matrix to this matrix in place.
    /// @param A Matrix to add.
    /// @return Updated matrix reference.
    Matrix<T> operator +=(const Matrix& A);

    /// @brief Subtracts another matrix from this matrix in place.
    /// @param A Matrix to subtract.
    /// @return Updated matrix reference.
    Matrix<T> operator -=(const Matrix &A);

    /// @brief Multiplies all elements of this matrix by a complex scalar in place.
    /// @param val Complex scalar multiplier.
    /// @return Updated matrix reference.
    Matrix<T> operator *= (const std::complex<double>& val);

    /// @brief Multiplies all elements of this matrix by a real scalar in place.
    /// @param c Real scalar multiplier.
    /// @return Updated matrix reference.
    Matrix<T> operator *= (double c);

    /// @brief Returns the transpose of this matrix as a new matrix.
    /// @return Transposed matrix.
    Matrix<T> transpose() const;

    /// @brief Computes the matrix inverse.
    /// @return Tuple of (inverse matrix, success flag).
    std::tuple<Matrix, bool> invert() const;

    /// @brief Returns a deep copy of this matrix.
    /// @return New independent matrix with the same data.
    Matrix<T> makeCopy() const;

    /// @brief Returns a mutable pointer to the raw element data.
    /// @return Pointer to the first element of the column-major data array.
    T* getData();

    /// @brief Returns a const pointer to the raw element data.
    /// @return Const pointer to the first element of the column-major data array.
    const T* getData() const;

    // y = this * x
    /// @brief Computes the matrix-vector product y = this * x.
    /// @param x Input column vector (matrix with one column).
    /// @param y Output column vector receiving the result.
    void gemv(const Matrix<T>& x, Matrix<T>& y) const;

    // Y = this * X
    /// @brief Computes the matrix-matrix product Y = this * X.
    /// @param X Input matrix.
    /// @param Y Output matrix receiving the result.
    void gemm(const Matrix<T>& X, Matrix<T>& Y) const;

    /// @brief Serializes the matrix to an output stream in the specified format.
    /// @param name Label for the matrix in the output.
    /// @param s Output stream to write to.
    /// @param vf Output format (default: Matlab).
    void serialize(const char* name, std::ostream& s, sparse::Format vf = sparse::Format::Matlab) const;

    //[U,S,V] = this->svd();
    //std::tuple<Matrix, DiagMatrix<double>, Matrix, bool> svd() const;

    //bool eig(Matrix& V, DiagMatrix<INDEX, T>& D, Matrix& W) const;

    ///companion construction for polynomial roots calculation
    ///companion of x^p+a(p-1)*x^(p-1)+a(p-2)*x^(p-2)+....+a(1)*x+a(0)
    //bool buildCompanion(const Matrix& a, bool zeroFirst = true);

    //matrix is modified, b is unchanged, new matrix created for output
    //std::tuple<Matrix, bool> solve(const Matrix& b);

    //in place solution (Fastest method)
    //inBoutX:  on input  -> b vector (matrix) of linear equation Ax=b
    //          on outuput-> x vector (matrix) of linear equation Ax=b
    /// @brief Solves the linear system Ax = b in place (fastest method).
    /// @param inBoutX On input, the right-hand side b; on output, the solution x.
    /// @return True if the system was solved successfully.
    bool solve(Matrix& inBoutX);

    /// @brief Changes the active dimensions of the matrix without reallocating storage.
    /// @param newNoRows New number of rows (must not exceed reserved capacity).
    /// @param newNoCols New number of columns (must not exceed reserved capacity).
    void setDimension(td::UINT4 newNoRows, td::UINT4 newNoCols);

    /// @brief Computes the L1 (column-sum) norm of the matrix.
    /// @return L1 norm value.
    double getL1Norm() const;

    //utility methods
    //static std::tuple<Matrix, bool> pseudoSolve(const Matrix& U, const DiagMatrix<double>& S, const Matrix& V, const Matrix& b, int rank = -1);
};

//#ifdef MU_WINDOWS
//    MATRIX_EXTERN template class MATRIX_API Matrix<double>;
//    MATRIX_EXTERN template class MATRIX_API Matrix<std::complex<double>>;
//#endif

    /// @brief Convenience typedef for a double-precision dense matrix.
    typedef Matrix<double> DblMatrix;
    /// @brief Convenience typedef for a complex double-precision dense matrix.
    typedef Matrix<std::complex<double>> CmplxMatrix;

}
