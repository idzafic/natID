// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

template <typename T>
class DiagMatrix;

template <typename T>
class MatrixMgr;

template <typename T>
class Matrix
{
    class Desc
    {
    public:
        td::UINT4 nRows = 0;
        td::UINT4 nCols = 0;
        td::UINT4 nRefs = 0;
        td::UINT4 flags = 0;
        td::UINT4 maxCols = 0;
        td::UINT4 maxRows = 0;
        union
        {
            MatrixMgr<T>* pMgr;
            char _padding[8];
        };
    };
    
protected:
    Desc* _pDesc = nullptr;
    
protected:

    constexpr td::UINT4 getDescSize() const
    {
        return sizeof(Desc);
    }
    
    td::UINT4 getNoOfRefs() const;
    void addRef();
    void relRef();
    MatrixMgr<T>* getMgr() const;
    void allocData(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr, bool initWithZero = false);
    void duplicate();
    inline double conjugate(double d) const;
    inline std::complex<double>& conjugate(std::complex<double>& d) const;
    std::tuple<Matrix, bool> invert1() const;
    std::tuple<Matrix, bool> invert2() const;
    std::tuple<Matrix, bool> invert3() const;
    
    td::UINT4 getMaxNoOfRows() const;
    td::UINT4 getMaxNoOfCols() const;
public:
    Matrix();
    Matrix(const Matrix<T> &A);
    Matrix(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr = nullptr, bool initZero = false);
    ~Matrix();
    
    //C and C++ interface (0-based index)
    MatrixReader<T> getManipulator() const;
    MatrixIO<T> getManipulator();
    ColumnReader<T> getColumnManipulator(td::UINT4 colPos=0) const;
    ColumnIO<T> getColumnManipulator(td::UINT4 colPos=0);
    FirstColumnReader<T> getFirstColumnManipulator() const;
    FirstColumnIO<T> getFirstColumnManipulator();
    
    //to simulate Matlab and Fortran (1-based index)
    MatrixReader1<T> getManipulator1() const;
    MatrixIO1<T> getManipulator1();
    ColumnReader1<T> getColumnManipulator1(td::UINT4 colPos=1) const;
    ColumnIO1<T> getColumnManipulator1(td::UINT4 colPos=1);
    FirstColumnReader1<T> getFirstColumnManipulator1() const;
    FirstColumnIO1<T> getFirstColumnManipulator1();
    
    void clean();
    void load(std::istream& ar);
    void save(std::ostream& ar, double minVal = 0, int width = -1, int precision = -1) const;
    void reserve(td::UINT4 nRows, td::UINT4 nCols, MatrixMgr<T>* pMgr = nullptr, bool initZeros = false);
    void operator = (const Matrix& A);
    td::UINT4 getNoOfRows() const;
    td::UINT4 getNoOfCols() const;
    void diagOnes();
    void zeros();
    bool load(const char *fileName);
    bool save(const char *fileName);
    void show(std::ostream& o, const char* name, double minVal = 0, int width = -1, int precision = -1) const;
    ///this(mxn) <- A(mxk) x B(kxm)
    void fromMult(const Matrix& A, const Matrix& B, const char* transpA = "N", const char* transpB = "N");
    Matrix<T> operator +=(const Matrix& A);
    Matrix<T> operator -=(const Matrix &A);
    Matrix<T> operator *= (const std::complex<double>& val);
    Matrix<T> operator *= (double c);
    Matrix<T> transpose() const;
    std::tuple<Matrix, bool> invert() const;
    Matrix<T> makeCopy() const;
    
    T* getData();
    const T* getData() const;
    
    // y = this * x
    void gemv(const Matrix<T>& x, Matrix<T>& y) const;
    
    // Y = this * X
    void gemm(const Matrix<T>& X, Matrix<T>& Y) const;
    
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
    bool solve(Matrix& inBoutX);
    
    void setDimension(td::UINT4 newNoRows, td::UINT4 newNoCols);
    
    double getL1Norm() const;
    
    //utility methods
    //static std::tuple<Matrix, bool> pseudoSolve(const Matrix& U, const DiagMatrix<double>& S, const Matrix& V, const Matrix& b, int rank = -1);
};

//#ifdef MU_WINDOWS
//    MATRIX_EXTERN template class MATRIX_API Matrix<double>;
//    MATRIX_EXTERN template class MATRIX_API Matrix<std::complex<double>>;
//#endif

    typedef Matrix<double> DblMatrix;
    typedef Matrix<std::complex<double>> CmplxMatrix;

}

