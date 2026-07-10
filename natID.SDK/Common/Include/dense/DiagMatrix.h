// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DiagMatrix.h @brief Diagonal matrix class template for dense linear algebra operations. */
// interface for the General Matrix class
#pragma once
#include <matrix/MatrixLib.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <complex>
#include <dense/IOManip.h>
#include <sparse/Format.h>

namespace dense
{
	template < typename M>
	class Matrix;

    template <typename T>
    class MatrixMgr;

	/// @brief Template class representing a diagonal matrix stored as a compact 1-D array.
	template <typename T>
	class DiagMatrix
	{
        /// @brief Internal descriptor holding metadata for the diagonal matrix storage.
        class Desc
        {
        public:
            td::UINT4 size = 0;     ///< Current number of diagonal elements
            td::UINT4 nRefs = 0;    ///< Reference count for copy-on-write semantics
            td::UINT4 flags = 0;    ///< Miscellaneous flags
            td::UINT4 maxSize = 0;  ///< Maximum allocated capacity
            union
            {
                MatrixMgr<T>* pMgr; ///< Pointer to the matrix memory manager
                char _padding[16];  ///< Padding to keep the union size fixed
            };
        };

		template <typename M>
		friend class Matrix;
	protected:
        Desc* _pDesc; ///< Pointer to the descriptor block holding size and metadata
	protected:

        /// @brief Returns the byte size of the Desc structure.
        /// @return Size of Desc in bytes.
        constexpr td::UINT4 getDescSize() const
        {
            return sizeof(Desc);
        }

        /// @brief Returns the current reference count.
        /// @return Number of active references to this matrix data.
        td::UINT4 getNoOfRefs() const;
        /// @brief Increments the reference count.
        void addRef();
        /// @brief Decrements the reference count and frees data when it reaches zero.
        void relRef();
        /// @brief Creates a private copy of the data (copy-on-write).
        void duplicate();
        /// @brief Returns the associated memory manager.
        /// @return Pointer to the MatrixMgr instance.
        MatrixMgr<T>* getMgr() const;
        /// @brief Returns the maximum allocated diagonal size.
        /// @return Maximum capacity of the diagonal storage.
        td::UINT4 getMaxSize() const;

        /// @brief Returns a mutable pointer to the raw diagonal data.
        /// @return Pointer to the first diagonal element.
        T* getData();

        /// @brief Returns a const pointer to the raw diagonal data.
        /// @return Const pointer to the first diagonal element.
        const T* getData() const;

        /// @brief Allocates storage for the diagonal elements.
        /// @param size Number of diagonal elements to allocate.
        /// @param pMgr Memory manager to use for allocation.
        /// @param initWithZero If true, initializes all elements to zero.
        void allocData(td::UINT4 size, MatrixMgr<T>* pMgr, bool initWithZero = false);

	public:
        /// @brief Copy constructor — creates a shared reference to the source data.
        /// @param A Source diagonal matrix to copy from.
        DiagMatrix(const DiagMatrix& A);

        /// @brief Default constructor — creates an empty diagonal matrix.
        DiagMatrix();

        /// @brief Constructs a diagonal matrix with a given size.
        /// @param size Number of rows/columns (diagonal length).
        /// @param pMgr Optional memory manager; nullptr uses the default allocator.
        /// @param initZero If true, initializes all diagonal elements to zero.
        DiagMatrix(int size, MatrixMgr<T>* pMgr = nullptr, bool initZero = false);

        /// @brief Destructor — releases the matrix data.
        ~DiagMatrix();

        /// @brief Releases the matrix data and resets the object to an empty state.
        void clean();

        /// @brief Returns the length of the diagonal (number of diagonal elements).
        /// @return Number of diagonal elements.
        td::UINT4 getDiagLen() const;

        /// @brief Loads matrix data from a stream.
        /// @param ar Input stream to read from.
        void load(std::istream& ar);

        /// @brief Saves matrix data to a stream.
        /// @param ar Output stream to write to.
        /// @param minVal Values with absolute value below this threshold are written as zero.
        /// @param width Field width for formatted output (-1 for default).
        /// @param precision Number of decimal digits (-1 for default).
        void save(std::ostream& ar, double minVal = 0, int width = -1, int precision = -1) const;

//        T& operator () (td::UINT4 diag);
//
//        const T& operator () (td::UINT4 diag) const;

        /// @brief Reserves storage for up to size diagonal elements.
        /// @param size Desired capacity.
        /// @param pMgr Memory manager to use.
        /// @param initZeros If true, initializes elements to zero.
        void reserve(td::UINT4 size,  MatrixMgr<T>* pMgr, bool initZeros);

        /// @brief Assignment operator — performs a shared copy of matrix data.
        /// @param A Source diagonal matrix.
        void operator = (const DiagMatrix& A);

        /// @brief Returns the number of rows (equals diagonal length).
        /// @return Number of rows.
        td::UINT4 getNoOfRows() const;

        /// @brief Returns the number of columns (equals diagonal length).
        /// @return Number of columns.
        td::UINT4 getNoOfCols() const;

        /// @brief Sets all diagonal elements to one.
        void ones();

        /// @brief Sets all diagonal elements to zero.
        void zeros();

        /// @brief Loads the diagonal matrix from a file.
        /// @param fileName Path to the file.
        /// @return true on success, false on failure.
        bool load(const char* fileName);

        /// @brief Saves the diagonal matrix to a file.
        /// @param fileName Path to the output file.
        /// @return true on success, false on failure.
        bool save(const char* fileName);

        /// @brief Prints the diagonal matrix to an output stream with a label.
        /// @param o Output stream.
        /// @param name Label string printed before the matrix.
        /// @param minVal Values below this threshold are displayed as zero.
        /// @param width Field width for formatted output (-1 for default).
        /// @param precision Number of decimal digits (-1 for default).
        void show(std::ostream& o, const char* name, double minVal = 0, int width = -1, int precision = -1) const;

        /// @brief Computes the numerical rank of the diagonal matrix.
        /// @param eps Threshold below which a diagonal element is considered zero.
        /// @return Numerical rank.
        td::UINT4 rank(double eps) const;

        /// @brief Returns a read-only manipulator for 0-based diagonal access.
        /// @return DiagReader providing const element access.
        DiagReader<T> getManipulator() const;
        /// @brief Returns a read-write manipulator for 0-based diagonal access.
        /// @return DiagIO providing mutable element access.
        DiagIO<T> getManipulator();

        /// @brief Returns a read-only manipulator for 1-based diagonal access.
        /// @return DiagReader1 providing const element access with 1-based indexing.
        DiagReader1<T> getManipulator1() const;
        /// @brief Returns a read-write manipulator for 1-based diagonal access.
        /// @return DiagIO1 providing mutable element access with 1-based indexing.
        DiagIO1<T> getManipulator1();

        /// @brief Resizes the diagonal matrix to a new dimension.
        /// @param newSize New diagonal length.
        void setDimension(td::UINT4 newSize);

        /// @brief Serializes the diagonal matrix to a stream in a given sparse format.
        /// @param name Label used in the serialized output.
        /// @param s Output stream.
        /// @param vf Sparse format selector (default: Matlab).
        void serialize(const char* name, std::ostream& s, sparse::Format vf = sparse::Format::Matlab) const;

        /// @brief Inverts the diagonal matrix in place (reciprocal of each diagonal element).
        /// @return true if inversion succeeded, false if a zero diagonal element was encountered.
        bool invert();
	};

    typedef DiagMatrix<double> DblDiagMatrix;           ///< Convenience alias for a real-valued diagonal matrix.
    typedef DiagMatrix<std::complex<double>> CmplxDiagMatrix; ///< Convenience alias for a complex-valued diagonal matrix.

//#ifdef MU_WINDOWS
//    template class MATRIX_API DiagMatrix<double>;
//    template class MATRIX_API DiagMatrix<std::complex<double>>;
//#endif


}
