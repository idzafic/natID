#include "stdafx.h"
#include <sparse/ISolver.h>
#include <complex>
#include <mem/PointerReleaser.h>
#include <mu/Timer.h>
#include <fo/FileOperations.h>
#include <td/Types.h>
#include "TestPath.h"

#define FOR_COMPARISON

using namespace std;

template <typename TSTREAM>
bool openFile(TSTREAM& f, const char* fileName)
{
	try
	{		
		f.open(fileName);
	}
	catch(...)
	{
		return false;
	}

	return f.is_open();
}

inline void getFreeVal(int i, double& val)
{
	val = i;
}

inline void getFreeVal(int i, std::complex<double>& val)
{
	val = std::complex<double>(i, i*5);
}

template <class TSTREAM, typename TELEM, typename TMAT>
inline void testSolver(TSTREAM& offStream, const char* matrixName, TMAT& m, int N, int NZ, int* pI, int* pJ, TELEM* pVal, bool symmetric, mu::Timer<5>& timer, int realNonZero, bool makeTopologicalSymmetry = false, TELEM* pFree = 0)
{
	int iTimer = 2;
	timer.setName(iTimer, "Populating matrix");
	timer.measure(iTimer);
	//int nAddedForSymmetry = 0;

	for (int k=0; k<NZ; ++k)
	{
		m.addTriple(pI[k], pJ[k], pVal[k]);		
	}	

	timer.pause(iTimer);	

	cout << "*";

	//m.save("InputMatrix:", offStream, true);

	++iTimer;
	timer.setName(iTimer, "Factorize");
	timer.measure(iTimer);
	bool bFact = m.factorize();
	timer.pause(iTimer);	

	cout << "*";

	if (bFact)
	{
		//set some free values
		if (pFree == 0)
		{
			for (int i=0; i<N;++i)
			{
				TELEM val=0;
				getFreeVal(i, val);				
				m.setRHS(i, val);

			}
		}
		else
			for (int i=0; i<N;++i)
				m.setRHS(i, pFree[i]);

		//m.showResults("Free Vector:", offStream, true);		
		++iTimer;
		timer.setName(iTimer, "Solver");
		timer.measure(iTimer);
		m.solve();
		timer.pauseAll();
		m.showResults("x", offStream);
	}
	timer.pauseAll();

	cout << "*\n";

	if (!bFact)
		offStream << "ERROR: Cannot factorize matrix!!" << endl;
	else
	{
		offStream << "Factorization OK!" << endl;
	}

	if (matrixName)
	{
		offStream << "Results for " << matrixName << endl;
	}

	if (symmetric)
		offStream << "Matrix is symmetric\n";
	else
		offStream << "Matrix is not symmetric (general)\n";

	offStream << "Matrix dimension: " << N << " input file rows: " << NZ << " nonzero elements: " << realNonZero << endl;

#ifdef SHOW_STATISTICS
	m.showStatistics(offStream);
#endif
	timer.showAllTimesInMillisec(offStream);	
	offStream << "---------\n";
}

template <class TSTREAM, sparse::SolverType usedSolver, sparse::Pivoting usedPivoting, sparse::Ordering usedOrdering>
void testRealMatrix(TSTREAM& offStream, const char* matrixName)
{
	offStream << "Testing real matrix: " << matrixName << endl;

	ifstream f;
	if (!openFile(f, matrixName))
	{
		offStream << "Cannot open file: " << matrixName << endl;
	}

	string str;
	f >> str;
	if (str.length() == 0)
	{
		offStream << "Unexpected format" << endl;
		return;
	}
	bool bFreeGiven = false;
	if (str.at(0) == 'f')
	{
		bFreeGiven = true;
		f >> str;
	}

	if (str.length() == 0 || str.at(0) != 'r')
	{
		offStream << "Expected \'real\' in " << matrixName << endl;
		return;
	}

	f >> str;
	if ( str.length() == 0)		
	{
		offStream << "Expected \'symmetric\' or \'nonsymmetrix\' in " << matrixName << endl;
		return;
	}

	bool symmetric = false;
	if (str.at(0) == 's')
	{
		symmetric = true;
	}

	int N = -1;
	int NN = 0;
	int NZ = 0;

	f >> N >> NN >> NZ;
	if (N != NN)
	{
		offStream << "Expected square matrix in " << matrixName << endl;
		return;
	}

	if (NZ <= 0)
	{
		offStream << "Number of lines in file is negative: " << NZ << endl;
		return;
	}
	double* pVal = new double[NZ];
	int* pI = new int[NZ];
	int* pJ = new int[NZ];

	mem::PointerDeleter<double> apVal(pVal);
	mem::PointerDeleter<int> apI(pI);
	mem::PointerDeleter<int> apJ(pJ);

	int realNonZero = 0;
	//iTimer = timer.add("Loading matrix");
	//load matrix
	for (int k=0; k<NZ;++k)
	{
		int i;
		int j;
		double val;
		f >> i >> j >> val;
		if (i <= 0 || j <= 0)
		{
			offStream << "Negative indices detected at line " << k+1 << endl;
			return;
		}		
		++realNonZero;
		pI[k] = i-1;
		pJ[k] = j-1;
		pVal[k] = val;		

		//if (symmetric && (i != j) )
		//{			
		//	++realNonZero;
		//}
	}

	double* pFree = 0;
	if (bFreeGiven)
	{
		pFree = new double[N];
		memset(pFree, 0, N * sizeof(double));
		int NFREE = 0;
		f >> NFREE;
		for (int i=0; i<NFREE; ++i)
		{
			int K = 0;
			double val = 0;
			f >> K >> val;
			assert(K > 0);
			pFree[--K] = val;
		}
	}
	//timer.stop(iTimer);	

	cout << "*";

	mu::Timer<5> timer("Total time");
	timer.measure(0);
	int iTimer = 1;
	timer.setName(iTimer, "Creating matrix");
	timer.measure(iTimer);
	int nRealNonZero = NZ;
	sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric;
	if (symmetric)
		symmetry = sparse::Symmetry::SymmetricGeneral;

    sparse::DblSolverReleaser pSolver(sparse::createDblSolver(N, nRealNonZero, symmetry, usedSolver, usedPivoting, usedOrdering ));
	//mem::PointerReleaser<sparse::DblSolver> pSolver(sparse::createDblSolver(N, nRealNonZero, symmetry, usedSolver, usedPivoting, usedOrdering ));
	timer.pause(iTimer);	

	testSolver(offStream, matrixName, pSolver.ref(), N, NZ, pI, pJ, pVal, symmetric, timer, realNonZero, false, pFree);		
}


template <class TSTREAM, sparse::SolverType usedSolver, sparse::Pivoting usedPivoting, sparse::Ordering usedOrdering>
void testComplexMatrix(TSTREAM& offStream, const char* matrixName)
{
	offStream << "Testing complex matrix: " << matrixName << endl;

	ifstream f;
	if (!openFile(f, matrixName))
	{
		offStream << "Cannot open file: " << matrixName << endl;
	}

	string str;
	f >> str;
	if ( (str.length() == 0) ||
		(str.at(0) != 'c') )
	{
		offStream << "Expected \'complex\' in " << matrixName << endl;
		return;
	}
	f >> str;
	if ( str.length() == 0)		
	{
		offStream << "Expected \'symmetric\' or \'nonsymmetrix\' in " << matrixName << endl;
		return;
	}

	bool symmetric = false;
	if (str.at(0) == 's')
	{
		symmetric = true;
	}

	int N = -1;
	int NN = 0;
	int NZ = 0;
	int realNonZero = 0;
	f >> N >> NN >> NZ;
	if (N != NN)
	{
		offStream << "Expected square matrix in " << matrixName << endl;
		return;
	}


	if (NZ <= 0)
	{
		offStream << "Number of lines in file is negative: " << NZ << endl;
		return;
	}
	complex<double>* pVal = new complex<double>[NZ];
	int* pI = new int[NZ];
	int* pJ = new int[NZ];

	mem::PointerDeleter<complex<double> > apVal(pVal);
	mem::PointerDeleter<int> apI(pI);
	mem::PointerDeleter<int> apJ(pJ);

	//iTimer = timer.add("Loading matrix");
	//load matrix
	for (int k=0; k<NZ;++k)
	{
		int i;
		int j;		
		double re, im;
		f >> i >> j >> re >> im;
		if (i <= 0 || j <= 0)
		{
			offStream << "Negative indices detected at line " << k+1 << endl;
			return;
		}

		pI[k] = i-1;
		pJ[k] = j-1;
		pVal[k].real(re);
		pVal[k].imag(im);

		++realNonZero;
	}

	//timer.stop(iTimer);	
	cout << "*";

	mu::Timer<5> timer("Total time");
	timer.measure(0);
	int iTimer = 1;
	timer.setName(iTimer, "Creating matrix");
	timer.measure(iTimer);
	//myUtils::SparseComplexSolver<double> m(N);
	int nRealNonZero = NZ;
	sparse::Symmetry symmetry = sparse::Symmetry::NonSymmetric;
	if (symmetric)
		symmetry = sparse::Symmetry::SymmetricGeneral;
		
	sparse::CmplxSolverReleaser pSolver(sparse::createCmplxSolver(N, nRealNonZero, symmetry, usedSolver, usedPivoting, usedOrdering ));

	timer.pause(iTimer);	

	testSolver(offStream, matrixName, pSolver.ref(), N, NZ, pI, pJ, pVal, symmetric, timer, realNonZero, false);
	
}

template<typename VALUETYPE>
class SElement 
{
public:
	VALUETYPE Value;
	int Row;
	int Col;			
	SElement *RowNext;
	SElement *ColNext;
};


template <sparse::SolverType usedSolver>
int testAllMatrices(const char* strFileName, const char* solverName)
{
	std::ofstream offStream;

	openFile(offStream, strFileName);

	offStream << solverName << endl;

	if (true)
	{		
		offStream << "Trying small real unsymmetric matrix..." << endl;

		sparse::DblSolverReleaser pDblSolver(sparse::createDblSolver(5, 12, sparse::Symmetry::NonSymmetric, usedSolver, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own));
		sparse::DblSolver& s1 = *(pDblSolver.getPtr());
		s1.addTriple(0, 0, 2);
		//s1.addTriple(1, 0, 3);
		s1.addTriple(1, 0, 2);
		s1.addTriple(1, 0, 1);

		s1.addTriple(0, 1, 3);
		s1.addTriple(2, 1,-1);
		s1.addTriple(4, 1, 4);
		s1.addTriple(1, 2, 4);
		s1.addTriple(2, 2, -3);
		s1.addTriple(3, 2, 1);
		s1.addTriple(4, 2, 2);
		s1.addTriple(2, 3, 2);
		s1.addTriple(1, 4, 6);
		s1.addTriple(4, 4, 1);

		if (!s1.factorize())
		{
			offStream << "Factorization of small real matrix failed!\n";
			assert(false);
			//return;
		}

		s1.setRHS(0, 8);
		s1.setRHS(1, 45);
		s1.setRHS(2, -3);
		s1.setRHS(3, 3);
		s1.setRHS(4, 19);

		s1.solve();

		for (int i=0; i<5;++i)
		{
			offStream << "x(" << i << ")=" << s1.x(i) << std::endl;		
		}

		offStream << "Small unsymmetric real matrix OK!" << endl;

		offStream << "Trying small complex unsymmetric matrix..." << endl;

		sparse::CmplxSolverReleaser pCmplxSolver(sparse::createCmplxSolver(4, 7, sparse::Symmetry::NonSymmetric, usedSolver, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own));
		sparse::CmplxSolver& s2 = *(pCmplxSolver.getPtr());
		//typedef sparse::SparseSolverComplex<sparse::MarkowitzPivotingSinglePass, sparse::OwnOrdering> SparseCmplx;
		//SparseCmplx s2(4, 10);
		//s2.addTriple1(1, 1, std::complex<double>(1,0));
		s2.addTriple(0, 1, std::complex<double>(2,0));			
		//s2.addTriple(0, 3, std::complex<double>(3,0));
		s2.addTriple(0, 3, std::complex<double>(2,0));
		s2.addTriple(0, 3, std::complex<double>(1,0));

		s2.addTriple(1, 0, std::complex<double>(4,0));
		//s2.addTriple1(2, 2, std::complex<double>(5,0));
		s2.addTriple(1, 2, std::complex<double>(6,0));		
		s2.addTriple(2, 1, std::complex<double>(7,0));
		//s2.addTriple1(3, 3, std::complex<double>(8,0));
		s2.addTriple(2, 3, std::complex<double>(9,0));
		//s2.addTriple1(4, 1, std::complex<double>(10,0));
		s2.addTriple(3, 2, std::complex<double>(12,0));
		//s2.addTriple1(4, 4, std::complex<double>(11,0));

		//s2.show("s2", std::cout, sparse::VIEW_SCILAB);
		//s2.save("Input", cout, true);
		//s2.saveByColumns("ByColumns", cout);
		if (s2.factorize())
		{			
		//	s2.showOrdering(cout);
			//s2.save("Factorized", cout, true);
			s2.setRHS(0, std::complex<double>(16,0));
			s2.setRHS(1, std::complex<double>(22,5));
			s2.setRHS(2, std::complex<double>(50,0));
			s2.setRHS(3, std::complex<double>(36,0));
			s2.solve();
			//const double * x= s2.getX();

			for (int i=0; i<4;++i)
			{
				//cout  << "x(" << i << ")=" << s2.getX(i) << std::endl;				
				offStream << "x(" << i << ")=" << s2.x(i) << ";" << std::endl;
			}
			offStream << "Complex unsymmetric matrix OK\n";
		}
		else
		{
			offStream << "Cannot factorize matrix\n";
		}		

		offStream << "Trying small real symmetric matrix..." << endl;
		//From PARDISO manual
		sparse::DblSolverReleaser pDblSolver2(sparse::createDblSolver(8, 20, sparse::Symmetry::SymmetricGeneral, usedSolver, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own));
		sparse::DblSolver& s3(pDblSolver2.ref());
		s3.addTriple(0, 0, 7);		
		s3.addTriple(0, 2, 1);
		s3.addTriple(0, 5, 2);
		s3.addTriple(0, 6, 7);

		s3.addTriple(1, 1, -4);
		s3.addTriple(1, 2, 8);
		s3.addTriple(1, 4, 2);

		s3.addTriple(2, 2, 1);
		s3.addTriple(2, 7, 5);

		s3.addTriple(3, 3, 7);
		s3.addTriple(3, 6, 9);

		s3.addTriple(4, 4, 5);
		s3.addTriple(4, 5, -1);
		s3.addTriple(4, 6, 5);

		//s3.addTriple(5, 5, 0);
		s3.addTriple(5, 7, 5);

		s3.addTriple(6, 6, 11);

		s3.addTriple(7, 7, 5);

		if (!s3.factorize())
		{
			offStream << "Factorization of small real matrix failed!\n";
			assert(false);
			//return;
		}

		s3.setRHS(0, 1);
		s3.setRHS(1, 2);
		s3.setRHS(2, 3);
		s3.setRHS(3, 4);
		s3.setRHS(4, 5);
		s3.setRHS(5, 6);
		s3.setRHS(6, 7);
		s3.setRHS(7, 8);	

		if (!s3.solve())
		{
			offStream << "Cannot solve!" << endl;
		}
		else
		{
			for (int i=0; i<8;++i)
			{
				offStream << "x(" << i << ")=" << s3.x(i) << std::endl;		
			}
		}		

		offStream << "Small real symmetric matrix OK!" << endl;
		//return 1;
	}		

#ifdef FOR_COMPARISON
	
    //testRealMatrix<ofstream, usedSolver, sparse::DiagonalPivotingMultiPass, sparse::OwnOrdering>(offStream, "aug3dcqp.mtx");
	
    //testRealMatrix<ofstream, usedSolver, sparse::DiagonalPivotingMultiPass, sparse::ExternOrderingAtMulA>(offStream, "gemat1.mtx");
	
	//return 1;
    //testRealMatrix<ofstream, usedSolver, sparse::NoPivoting, sparse::OwnOrdering>(offStream, "685_bus.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "1138_bus.mtx");

    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "685_bus.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "494_bus.mtx");
    testRealMatrix<ofstream, usedSolver,sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "662_bus.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "qh1484.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "qh882.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "bcsstk26.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "gemat11.mtx");
    //testRealMatrix<ofstream, usedSolver, sparse::MarkowitzPivotingSinglePass, sparse::ExternOrderingAtPlusA>(offStream, "gemat11.mtx");
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "gemat12.mtx");
    testComplexMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalSinglePass, sparse::Ordering::Own>(offStream, "young4c.mtx");
	//

	//testRealMatrix<ofstream, myUtils::NoPivoting, myUtils::OwnOrdering>(offStream, "685_bus.mtx");
	//testRealMatrix<ofstream, myUtils::NoPivoting, myUtils::OwnOrdering>(offStream, "685_bus.mtx");
	//testRealMatrix<ofstream, myUtils::NoPivoting, myUtils::OwnOrdering>(offStream, "494_bus.mtx");
	//testRealMatrix<ofstream,myUtils::NoPivoting, myUtils::OwnOrdering>(offStream, "662_bus.mtx");	
	//testRealMatrix<ofstream, myUtils::NoPivoting, myUtils::OwnOrdering>(offStream, "1138_bus.mtx");
	//testRealMatrix<ofstream, myUtils::MinFillInPivoting, myUtils::OwnOrdering>(offStream, "qh1484.mtx");
	//testRealMatrix<ofstream, myUtils::MinFillInPivoting, myUtils::OwnOrdering>(offStream, "qh882.mtx");	
	//testComplexMatrix<ofstream, myUtils::NoPivoting, myUtils::ExternOrderingSym>(offStream, "young4c.mtx");
    ////testComplexMatrix<ofstream, false, false>(offStream, "conf5_0_00l4x4_2600.mtx");
#else
    //testRealMatrix<ofstream, myUtils::MarkowitzPivotingSinglePass, myUtils::OwnOrdering>(offStream, "test10dbl.mtx");
	//return 1;	
    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "aug3dcqp.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "685_bus.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "685_bus.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::No, sparse::Ordering::Own>(offStream, "494_bus.mtx");
    testRealMatrix<ofstream,sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "662_bus.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "1138_bus.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "qh1484.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "qh882.mtx");

    testComplexMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "test10cmplx.mtx");
    testComplexMatrix<ofstream, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "young4c.mtx");
    //testRealMatrix<ofstream, myUtils::DiagonalPivotingSinglePass, myUtils::ExternOrderingSym>(offStream, "bcsstk26.mtx");
    //testRealMatrix<ofstream, myUtils::DiagonalPivotingSinglePass, myUtils::ExternOrderingSym>(offStream, "bcsstk18.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "gemat11.mtx");
    testRealMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "gemat12.mtx");
    testComplexMatrix<ofstream, sparse::Pivoting::MarkowitzSinglePass, sparse::Ordering::Own>(offStream, "ted_AB.mtx");
    //testComplexMatrix<ofstream, myUtils::DiagonalPivotingMultiPass, myUtils::OwnOrdering>(offStream, "windscreen.mtx");

    //testComplexMatrix<ofstream, myUtils::MarkowitzPivotingSinglePass, myUtils::OwnOrdering>(offStream, "conf5_0_00l4x4_2600.mtx");
#endif

	cout << "Test matrix done\n";

	return 1;
}


void testFromMemory()
{
    //sparse::setBLASPlatform(BLAS::Intel_Nehalem);
	testAllMatrices<sparse::SolverType::LU>("Results_IDz.txt", "FastSolverIDz");
//    testAllMatrices<sparse::SolverType::Pardiso>("Results_IntelSolver.txt", "IntelSolver");
    //testAllMatrices<sparse::MUMPS>("Results_MUMPS.txt", "MUMPS");
}

//
//template <sparse::SolverType usedSolver>
//int testAllSymIndefMatrices(const char* strFileName, const char* solverName)
//{
//	std::ofstream offStream;
//#ifdef MU_WINDOWS
//	td::String fileName("R:/");
//#else
//    td::String fileName("/Volumes/RAMDisk/");
//#endif
//	fileName += strFileName;
//
//	openFile(offStream, fileName.c_str());
//
//	offStream << solverName << endl;
//	
//	
//    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "SmallIndefinite.mtx");
//    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "bcsstk26.mtx");
//
//    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "bcsstk24.mtx");
//    testRealMatrix<ofstream, usedSolver, sparse::Pivoting::DiagonalMultiPass, sparse::Ordering::Own>(offStream, "aug3dcqp.mtx");
//	
//	cout << solverName << " done...\n";
//	
//	return 1;
//}


//void testFromMemorySimIndef()
//{
//	sparse::setBLASPlatform(BLAS::Intel_Nehalem);
//	testAllSymIndefMatrices<sparse::SolverType::LDLT>("FastSymIndefSolver.txt", "FastSymIndefSolver");
//	//testAllSymIndefMatrices<sparse::FastSolver>("Results_IDz.txt", "FastSolverIDz");
//	//testAllSymIndefMatrices<sparse::MUMPS>("Results_MUMPS.txt", "MUMPS");
//	//testAllSymIndefMatrices<sparse::IntelSolver_11>("Results_IntelSolver_11.txt", "IntelSolver_11");
//		
//	
//
//}
