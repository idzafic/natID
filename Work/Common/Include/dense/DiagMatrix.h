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

	template <typename T>
	class DiagMatrix
	{
        class Desc
        {
        public:
            td::UINT4 size = 0;
            td::UINT4 nRefs = 0;
            td::UINT4 flags = 0;
            td::UINT4 maxSize = 0;
            union
            {
                MatrixMgr<T>* pMgr;
                char _padding[16];
            };
        };
        
		template <typename M>
		friend class Matrix;
	protected:
        Desc* _pDesc;
	protected:

        constexpr td::UINT4 getDescSize() const
        {
            return sizeof(Desc);
        }
        
        td::UINT4 getNoOfRefs() const;
        void addRef();
        void relRef();
        void duplicate();
        MatrixMgr<T>* getMgr() const;
        td::UINT4 getMaxSize() const;
        
        T* getData();

        const T* getData() const;
		
        void allocData(td::UINT4 size, MatrixMgr<T>* pMgr, bool initWithZero = false);
		
	public:
        DiagMatrix(const DiagMatrix& A);

        DiagMatrix();

        DiagMatrix(int size, MatrixMgr<T>* pMgr = nullptr, bool initZero = false);

        ~DiagMatrix();

        void clean();

        td::UINT4 getDiagLen() const;

        void load(std::istream& ar);

        void save(std::ostream& ar, double minVal = 0, int width = -1, int precision = -1) const;

//        T& operator () (td::UINT4 diag);
//        
//        const T& operator () (td::UINT4 diag) const;

        void reserve(td::UINT4 size,  MatrixMgr<T>* pMgr, bool initZeros);

        void operator = (const DiagMatrix& A);

        td::UINT4 getNoOfRows() const;

        td::UINT4 getNoOfCols() const;

        void ones();

        void zeros();

        bool load(const char* fileName);

        bool save(const char* fileName);

        void show(std::ostream& o, const char* name, double minVal = 0, int width = -1, int precision = -1) const;

        td::UINT4 rank(double eps) const;
        
        DiagReader<T> getManipulator() const;
        DiagIO<T> getManipulator();
        
        DiagReader1<T> getManipulator1() const;
        DiagIO1<T> getManipulator1();
        
        void setDimension(td::UINT4 newSize);
        
        void serialize(const char* name, std::ostream& s, sparse::Format vf = sparse::Format::Matlab) const;

        bool invert();
	};

    typedef DiagMatrix<double> DblDiagMatrix;
    typedef DiagMatrix<std::complex<double>> CmplxDiagMatrix;

//#ifdef MU_WINDOWS
//    template class MATRIX_API DiagMatrix<double>;
//    template class MATRIX_API DiagMatrix<std::complex<double>>;
//#endif

    
}

