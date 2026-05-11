// TestISparseSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sparse/ISolver.h>
#include <iostream>
#include <fstream>
#include <fo/FileOperations.h>
#include <mu/Timer.h>
#include <mem/PointerReleaser.h>
// #include "TestCholesky.h"
#include "TestPath.h"
#include <mu/Application.h>
#include <sparse/Format.h>
#include <dense/Matrix.h>

#define SOLVERTYPE sparse::SolverType::LU
#define SHOW_STATISTICS

static fo::fs::path natIDSDKPath;

using namespace std;

void testFromMemory();
//void testFromMemorySimIndef();

template <class ISOLVER>
inline void setCmplxRHS(ISOLVER& pSolver, int N)
{
    for (int i = 0; i<N; ++i)
    {
        //td::cmplx val(i+1, 0);
        td::cmplx val(i+1, i);
        pSolver->setRHS(i, val);
    }
}

template <class ISOLVER>
inline void setRealRHS(ISOLVER& pSolver, int N)
{
    for (int i = 0; i<N; ++i)
    {
        //td::cmplx val(i+1, 0);
        double val = 1;
        pSolver->setRHS(i, val);
    }
}

void testFromFile()
{
    sparse::CmplxSolverReleaser pSolver(sparse::createCmplxSolver(3, 9, sparse::Symmetry::NonSymmetric, sparse::SolverType::LU));
	td::cmplx zeroVal(0,0);
	pSolver->populateDiagonals(zeroVal);
	std::ifstream iFile;
    fo::fs::path filePath = natIDSDKPath / "SimpleComplex.txt";
	if (!fo::openExistingBinaryFile(iFile, filePath))
    {
        std::cout << "ERROR! Cannot open file: " << filePath << td::endl;
        return;
    }

    int nRows, nCols, nNonzero;
    iFile >> nRows >> nCols >> nNonzero;
    if (nRows != 3 || nCols != 3)
    {
        std::cout << "ERROR! Wrong matrix size in: " << filePath << td::endl;
        return;
    }
	for (int nE=1; nE<=nNonzero; ++nE)
	{
		int i, j;
		td::cmplx val;
		iFile >> i >> j >> val;
		pSolver->addTriple1(i, j, val);
	}
	std::cout << "ALL OK\n";
}

void testMyExample()
{
	sparse::DblSolverReleaser pSolver(sparse::createDblSolver(5, 12, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzSinglePass));
	sparse::DblSolver& s2 = *(pSolver.ptr());
	double val = 0;
	s2.populateDiagonals(val);	
	s2.addTriple(1, 0, 3);
	s2.addTriple(0, 1, 3);
	s2.addTriple(2, 1, -1);
	s2.addTriple(4, 1, 4);
	s2.addTriple(1, 2, 4);
	s2.addTriple(2, 2, -3);
	s2.addTriple(3, 2, 1);
	s2.addTriple(4, 2, 2);
	s2.addTriple(2, 3, 2);
	s2.addTriple(1, 4, 6);
	s2.addTriple(4, 4, 1);
	s2.addTriple(0, 0, 2);

	if (!s2.factorize())
	{
		std::cout << "Factorization failed!\n";
		assert(false);
		return;
	}

	s2.setRHS(0, 8);
	s2.setRHS(1, 45);
	s2.setRHS(2, -3);
	s2.setRHS(3, 3);
	s2.setRHS(4, 19);

	s2.solve();

	for (int i=0; i<5;++i)
	{
		std::cout << "x(" << i << ")=" << s2.x(i) << std::endl;		
	}
}

void testUMFPackExample()
{
	
	//int Ti[] =    { 0, 1, 0,  2, 4, 1,  2, 3, 4, 2, 1, 4 };
	//int Tj[] =    { 0, 0, 1,  1, 1, 2,  2, 2, 2, 3, 4, 4 };
	//double Tx[] = { 2, 3, 3, -1, 4, 4, -3, 1, 2, 2, 6, 1 };	
	//double b[] = { 8, 45, -3, 3, 19 };
	sparse::DblSolverReleaser pSolver(sparse::createDblSolver(5, 12, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzSinglePass));
	sparse::DblSolver& s2 = *(pSolver.ptr());
	s2.addTriple(0, 0, 2);
	s2.addTriple(1, 0, 3);
	s2.addTriple(0, 1, 3);
	s2.addTriple(2, 1,-1);
	s2.addTriple(4, 1, 4);
	s2.addTriple(1, 2, 4);
	s2.addTriple(2, 2, -3);
	s2.addTriple(3, 2, 1);
	s2.addTriple(4, 2, 2);
	s2.addTriple(2, 3, 2);
	s2.addTriple(1, 4, 6);
	s2.addTriple(4, 4, 1);

	if (!s2.factorize())
	{
		std::cout << "Factorization failed!\n";
		assert(false);
		return;
	}

	s2.setRHS(0, 8);
	s2.setRHS(1, 45);
	s2.setRHS(2, -3);
	s2.setRHS(3, 3);
	s2.setRHS(4, 19);

	s2.solve();

	for (int i=0; i<5;++i)
	{
		std::cout << "x(" << i << ")=" << s2.x(i) << std::endl;		
	}
}

void testSC_PID_Example()
{
    
    sparse::DblSolverReleaser pSolver(sparse::createDblSolver(8, 64, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzSinglePass));
    sparse::DblSolver& s2 = *(pSolver.ptr());
    s2.addTriple1(1, 1, 5.000000000000000e-04); //J(1,1)=5.000000000000000e-04;
    s2.addTriple1(2, 1, -1.000000000000000e+00); //J(2,1)=-1.000000000000000e+00;
    s2.addTriple1(3, 1, 3.700000000000000e+02); //J(3,1)=3.700000000000000e+02;
    s2.addTriple1(8, 1, 1.000000000000000e+00); //J(8,1)=1.000000000000000e+00;
    
    s2.addTriple1(3, 2, -1.000000000000000e+00); //J(3,2)=-1.000000000000000e+00;
    s2.addTriple1(6, 2, 5.000000000000000e-04); //J(6,2)=5.000000000000000e-04;
    
    s2.addTriple1(7, 3, -1.000000000000000e+00); //J(7,3)=-1.000000000000000e+00;
    s2.addTriple1(8, 3, 1.000000000000000e+00); //J(8,3)=1.000000000000000e+00;
    
    s2.addTriple1(1, 4, -1.000000000000000e+00); //J(1,4)=-1.000000000000000e+00;
    s2.addTriple1(3, 4, 1.000000000000000e+02); //J(3,4)=1.000000000000000e+02;
    
    s2.addTriple1(2, 5, 8.333333333333334e-06); // J(2,5)=8.333333333333334e-06;
    s2.addTriple1(3, 5, 1.000000000000000e+00); //J(3,5)=1.000000000000000e+00;
    
    s2.addTriple1(4, 6, -1.000000000000000e+00); //J(4,6)=-1.000000000000000e+00;
    s2.addTriple1(7, 6, 1.000000000000000e+00); //J(7,6)=1.000000000000000e+00;
    
    s2.addTriple1(4, 7, 5.000000000000000e-04); //J(4,7)=5.000000000000000e-04;
    s2.addTriple1(5, 7, -1.000000000000000e+00); //J(5,7)=-1.000000000000000e+00;
    s2.addTriple1(6, 7, -2.828000000000000e-02); //J(6,7)=-2.828000000000000e-02;
    
    s2.addTriple1(5, 8, 5.000000000000000e-04); //J(5,8)=5.000000000000000e-04;
    s2.addTriple1(6, 8, -1.007828000000000e+00); //J(6,8)=-1.007828000000000e+00
    

    if (!s2.factorize())
    {
        std::cout << "Factorization failed!\n";
        assert(false);
        return;
    }

    s2.setRHS1(1, 0);
    s2.setRHS1(2, 0);
    s2.setRHS1(3, 0);
    s2.setRHS1(4, 0);
    s2.setRHS1(5, 0);
    s2.setRHS1(6, 0);
    s2.setRHS1(7, 0);
    s2.setRHS1(8, 1);

    s2.solve();

    for (int i=0; i<8;++i)
    {
        std::cout << "x(" << i << ")=" << s2.x(i) << std::endl;
    }
    
    sparse::DblSolverReleaser pSolver2(sparse::createDblSolver(8, 64, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzMultiPass));
    sparse::DblSolver& s3 = *(pSolver2.ptr());
    s3.addTriple1(1, 1, 5.000000000000000e-04); //J(1,1)=5.000000000000000e-04;
    s3.addTriple1(2, 1, -1.000000000000000e+00); //J(2,1)=-1.000000000000000e+00;
    s3.addTriple1(3, 1, 3.700000000000000e+02); //J(3,1)=3.700000000000000e+02;
    s3.addTriple1(8, 1, 1.000000000000000e+00); //J(8,1)=1.000000000000000e+00;
    
    s3.addTriple1(3, 2, -1.000000000000000e+00); //J(3,2)=-1.000000000000000e+00;
    s3.addTriple1(6, 2, 5.000000000000000e-04); //J(6,2)=5.000000000000000e-04;
    
    s3.addTriple1(7, 3, -1.000000000000000e+00); //J(7,3)=-1.000000000000000e+00;
    s3.addTriple1(8, 3, 1.000000000000000e+00); //J(8,3)=1.000000000000000e+00;
    
    s3.addTriple1(1, 4, -1.000000000000000e+00); //J(1,4)=-1.000000000000000e+00;
    s3.addTriple1(3, 4, 1.000000000000000e+02); //J(3,4)=1.000000000000000e+02;
    
    s3.addTriple1(2, 5, 8.333333333333334e-06); // J(2,5)=8.333333333333334e-06;
    s3.addTriple1(3, 5, 1.000000000000000e+00); //J(3,5)=1.000000000000000e+00;
    
    s3.addTriple1(4, 6, -1.000000000000000e+00); //J(4,6)=-1.000000000000000e+00;
    s3.addTriple1(7, 6, 1.000000000000000e+00); //J(7,6)=1.000000000000000e+00;
    
    s3.addTriple1(4, 7, 5.000000000000000e-04); //J(4,7)=5.000000000000000e-04;
    s3.addTriple1(5, 7, -1.000000000000000e+00); //J(5,7)=-1.000000000000000e+00;
    s3.addTriple1(6, 7, -2.828000000000000e-02); //J(6,7)=-2.828000000000000e-02;
    
    s3.addTriple1(5, 8, 5.000000000000000e-04); //J(5,8)=5.000000000000000e-04;
    s3.addTriple1(6, 8, -1.007828000000000e+00); //J(6,8)=-1.007828000000000e+00
    

    if (!s3.factorize())
    {
        std::cout << "Factorization failed!\n";
        assert(false);
        return;
    }

    s3.setRHS1(1, 0);
    s3.setRHS1(2, 0);
    s3.setRHS1(3, 0);
    s3.setRHS1(4, 0);
    s3.setRHS1(5, 0);
    s3.setRHS1(6, 0);
    s3.setRHS1(7, 0);
    s3.setRHS1(8, 1);

    s3.solve();

    for (int i=0; i<8;++i)
    {
        std::cout << "x(" << i << ")=" << s3.x(i) << std::endl;
    }
    
    std::cout << "Deltas:" << std::endl;
    for (int i=0; i<8;++i)
    {
        std::cout << "dx(" << i << ")=" << s2.x(i) - s3.x(i) << std::endl;
    }
}

void showSolverSymmetryAndPivoting(sparse::SolverType usedSolver, sparse::Symmetry symmetry, sparse::Pivoting pivoting)
{
    switch (usedSolver)
    {
        case sparse::SolverType::LU:
            std::cout << "LU";
            break;
        case sparse::SolverType::Pardiso:
            std::cout << "PARD";
            break;
        case sparse::SolverType::LDLT:
            std::cout << "LDLT";
            break;
        case sparse::SolverType::Ctrl:
            std::cout << "CTRL";
            break;
        default:
            std::cout << "XXXX";
    }
    std::cout << "(";
    switch (symmetry)
    {
        case sparse::Symmetry::NonSymmetric:
            std::cout << "NnS";
            break;
        case sparse::Symmetry::SymmetricPosDef:
            std::cout << "SPD";
            break;
        case sparse::Symmetry::SymmetricIndef:
            std::cout << "SIN";
            break;
        case sparse::Symmetry::HermitianPosDef:
            std::cout << "HPD";
            break;
        case sparse::Symmetry::HermitianIndef:
            std::cout << "HIN";
            break;
        default:
            std::cout << "SXX";
    }
    
    std::cout << ",";
    switch (pivoting)
    {
        case sparse::Pivoting::No:
            std::cout << "NoPv";
            break;
        case sparse::Pivoting::DiagonalSinglePass:
            std::cout << "DPSP";
            break;
        case sparse::Pivoting::DiagonalMultiPass:
            std::cout << "DPMP";
            break;
        default:
            std::cout << "PvXX";
    }
    std::cout << "):";
}

template <class T>
void createOffStreams(T& offStream, T& offRes, fo::fs::path& outFolder, const char* strMatrixName, bool complex, bool hermitian)
{
    td::String strOut;
    strOut.format("%s/%s.txt", outFolder.string().c_str(), strMatrixName);
    const char* fileNameOut = strOut.c_str();
    if (!fo::openFile(offStream, fileNameOut))
    {
        std::cout << "Cannot open out file: " << fileNameOut << std::endl;
        return;
    }

    if (complex)
    {
        if (hermitian)
            offStream << "Testing complex Hermitian matrix: " << strMatrixName << endl;
        else
            offStream << "Testing complex matrix: " << strMatrixName << endl;
    }
    else
    {
        offStream << "Testing real matrix: " << strMatrixName << endl;
    }

    td::String strRes;
    strRes.format("%s/%s_Res.txt", outFolder.string().c_str(), strMatrixName);
    if (!fo::openFile(offRes, strRes))
    {
        std::cout << "Cannot open out file: " << strRes << std::endl;
        return;
    }
}

const char* getFolderName(sparse::SolverType usedSolver)
{
    const char* solverType = "LU";
    if (usedSolver == sparse::SolverType::Pardiso)
        solverType = "Pardiso";
    else if (usedSolver == sparse::SolverType::LDLT)
        solverType = "LDLT";
    else if (usedSolver == sparse::SolverType::Ctrl)
        solverType = "CTRL";
    return solverType;
}

void testRealMatrix(const fo::fs::path& inPath, const char* strMatrixName,  sparse::SolverType usedSolver, bool PosDef)
{
    if (usedSolver == sparse::SolverType::Pardiso)
        return;
    if (usedSolver == sparse::SolverType::Ctrl)
        return;
    
    
    td::String matrixFileName;
    matrixFileName.format("%s.mtx", strMatrixName);
    auto matrixIn = inPath / matrixFileName.c_str();
    const char* matrixName = matrixIn.c_str();

    auto outPath = getResultPath(natIDSDKPath);
    td::String strOut;
    
    fo::createFolder(outPath);
    
    td::String strMatrixType;
    strMatrixType.format("Real_%s", (PosDef)?"PosDef":"Indef");
    
    const char* solverType = getFolderName(usedSolver);
    fo::fs::path realFolder(outPath / strMatrixType.c_str());
    fo::createFolder(realFolder);
    
    fo::fs::path outFolder(realFolder / solverType);
    fo::createFolder(outFolder);
    
    //, sparse::Pivoting usedPivoting, sparse::Ordering usedOrdering
    //Hermit_1280b
#ifdef SER_RESULTS
    std::ofstream offStream;
    std::ofstream offRes;
    createOffStreams(offStream, offRes, outFolder, strMatrixName, false, false);
#endif
    ifstream f;
    if (!fo::openFile(f, matrixName))
    {
        std::cout << "Cannot open file: " << matrixName << endl;
    }

    string str;
    f >> str;
    if ((str.length() == 0) ||
        (str.at(0) != 'r'))
    {
        std::cout << "Expected \'real\' in " << matrixName << endl;
        return;
    }
    f >> str;
    if (str.length() == 0)
    {
        std::cout << "Expected \'symmetric\' in " << matrixName << endl;
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
        std::cout << "Expected square matrix in " << matrixName << endl;
        return;
    }


    if (NZ <= 0)
    {
        std::cout << "Number of lines in file is negative: " << NZ << endl;
        return;
    }

    double* pVal = new double[NZ];
    int* pI = new int[NZ];
    int* pJ = new int[NZ];

    mem::PointerDeleter<double> apVal(pVal);
    mem::PointerDeleter<int> apI(pI);
    mem::PointerDeleter<int> apJ(pJ);

    //iTimer = timer.add("Loading matrix");
    //load matrix
    for (int k = 0; k<NZ; ++k)
    {
        int i;
        int j;
        double re;
        f >> i >> j >> re;
        if (i <= 0 || j <= 0)
        {
            std::cout << "Negative indices detected at line " << k + 1 << endl;
            return;
        }

        //if (j > i)
        //{
        //    offStream << "Upper part of matrix detected at line " << k + 1 << endl;
        //    return;
        //}

        pI[k] = i - 1;
        pJ[k] = j - 1;
        pVal[k] = re;

        ++realNonZero;

        if ((usedSolver == sparse::SolverType::LU) && (i != j))
        {
            ++realNonZero;
        }
    }

    //timer.stop(iTimer);
    //cout << "*";

    mu::Timer<5> timer("Total time");
    timer.measure(0);
    int iTimer = 1; // timer->add("Creating matrix");
    timer.setName(iTimer, "Creating matrix");
    timer.measure(iTimer);
    //myUtils::SparseComplexSolver<double> m(N);
    int nRealNonZero = realNonZero;
    sparse::Symmetry symmetry = sparse::Symmetry::SymmetricIndef;
    if (usedSolver == sparse::SolverType::LU)
        symmetry = sparse::Symmetry::NonSymmetric;
    else
    {
        if (PosDef)
            symmetry = sparse::Symmetry::SymmetricPosDef;
    }

    sparse::Pivoting pivoting = sparse::Pivoting::No;
    
    if (symmetry != sparse::Symmetry::SymmetricPosDef)
        pivoting = sparse::Pivoting::DiagonalMultiPass;

    showSolverSymmetryAndPivoting(usedSolver, symmetry, pivoting);
    
    //if (usedSolver == sparse::LU)
        //pivoting = sparse::DiagonalPivotingMultiPass;

    sparse::DblSolverReleaser pSolver(sparse::createDblSolver(N, nRealNonZero, symmetry, usedSolver, pivoting, sparse::Ordering::Own));

    timer.pause(iTimer);
    
    //testHermitianSolver(offStream, matrixName, pSolver.ref(), N, NZ, pI, pJ, pVal, symmetric, *timer, realNonZero, false);

    timer.setName(++iTimer, "Populating matrix");
    double zero = 0;
    pSolver->populateDiagonals(zero);
    
    //int nAddedForSymmetry = 0;

    for (int k = 0; k<NZ; ++k)
    {
        pSolver->addTriple(pI[k], pJ[k], pVal[k]);

        if ((usedSolver == sparse::SolverType::LU) && (pI[k] != pJ[k]))
        {
            pSolver->addTriple(pJ[k], pI[k], pVal[k]);
        }
    }

    timer.pause(iTimer);

    setRealRHS(pSolver, N);
    cout << "*";

    //m.save("InputMatrix:", offStream, true);
#ifdef SER_RESULTS
    offStream << "\nMatrix:\n--------------\n";
    pSolver->serialize("A", offStream, sparse::Format::Fortran);
    offStream << "\nEND Matrix:\n--------------\n";
#endif
    timer.setName(++iTimer, "Factorize");
    bool bFact = pSolver->factorize();
    timer.pause(iTimer);
    
    if (!bFact)
    {
#ifdef SER_RESULTS
        offStream << "ERROR: Cannot factorize matrix!!" << endl;
#endif
        const char* pLastErr = pSolver->getLastError();
        if (pLastErr)
            cout << pLastErr << " ";
        
        cout << "x" << std::endl;
        return;
    }
    //else
    //{
    //    offStream << "Factorization OK!" << endl;
    //}

    if (bFact)
    {
        //set some free values
        
//        for (int i = 0; i<N; ++i)
//        {
//            //td::cmplx val(i+1, 0);
//            double val = 1;
//            pSolver->setRHS(i, val);
//        }
        
#ifdef SER_RESULTS
        pSolver->showRHS("b", offStream, sparse::Format::Fortran);
#endif
        //m.showResults("Free Vector:", offStream, true);
        timer.setName(++iTimer, "Solver");
        pSolver->solve();
        timer.pauseAll();
#ifdef SER_RESULTS
        offStream << std::endl << "Results:" << std::endl;
        pSolver->showResults("x", offStream, sparse::Format::Fortran);
        pSolver->showResults("x", offRes, sparse::Format::Fortran);
#endif
    }
        
    timer.pauseAll();
    //timer.calcTotal();
    cout << "*" << std::endl;
#ifdef SER_RESULTS
    if (matrixName)
    {
        offStream << "Results for " << matrixName << endl;
    }

    if (symmetric)
        offStream << "Matrix is symmetric\n";
    else
        offStream << "Matrix is not symmetric (general)\n";

    offStream << "Matrix dimension: " << N << " input file rows: " << NZ << " nonzero elements: " << realNonZero << endl;
#endif

#ifdef SHOW_STATISTICS
    //m.showStatistics(offStream);
    timer.showAllTimesInMillisec(offStream);
    timer.showAllTimesInMillisec(offRes);
    offStream << "---------\n";
#else
    timer.showAllTimesInMillisec(std::cout);
#endif
}

void testComplexMatrix(const fo::fs::path& inPath, const char* strMatrixName,  sparse::SolverType usedSolver, bool PosDef, bool hermitian = true)
{
    if (usedSolver == sparse::SolverType::Pardiso)
        return;
    if (usedSolver == sparse::SolverType::Ctrl)
        return;
    
    td::String matrixFileName;
    matrixFileName.format("%s.mtx", strMatrixName);
    auto matrixIn = inPath / matrixFileName.c_str();
    const char* matrixName = matrixIn.c_str();

    auto outPath = getResultPath(natIDSDKPath);

    fo::createFolder(outPath);
    
    td::String strMatrixType;
    strMatrixType.format("Cmplx_%s", (PosDef)?"PosDef":"Indef");
    fo::fs::path realFolder(outPath / strMatrixType.c_str());
    fo::createFolder(realFolder);
    
    const char* solverType = getFolderName(usedSolver);
    
    fo::fs::path outFolder(realFolder / solverType);

    fo::createFolder(outFolder);
    
	//, sparse::Pivoting usedPivoting, sparse::Ordering usedOrdering
	//Hermit_1280b	
#ifdef SER_RESULTS
    std::ofstream offStream;
    std::ofstream offRes;
    createOffStreams(offStream, offRes, outFolder, strMatrixName, false, false);
#endif
    
	ifstream f;
	if (!fo::openFile(f, matrixName))
	{
		std::cout << "Cannot open file: " << matrixName << endl;
	}

	string str;
	f >> str;
	if ((str.length() == 0) ||
		(str.at(0) != 'c'))
	{
		std::cout << "Expected \'complex\' in " << matrixName << endl;
		return;
	}
	f >> str;
	if (str.length() == 0)
	{
		std::cout << "Expected \'symmetric\' in " << matrixName << endl;
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
		std::cout << "Expected square matrix in " << matrixName << endl;
		return;
	}


	if (NZ <= 0)
	{
		std::cout << "Number of lines in file is negative: " << NZ << endl;
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
	for (int k = 0; k<NZ; ++k)
	{
		int i;
		int j;
		double re, im;
		f >> i >> j >> re >> im;
		if (i <= 0 || j <= 0)
		{
			std::cout << "Negative indices detected at line " << k + 1 << endl;
			return;
		}

		//if (j > i)
		//{
		//	offStream << "Upper part of matrix detected at line " << k + 1 << endl;
		//	return;
		//}

		pI[k] = i - 1;
		pJ[k] = j - 1;
		pVal[k].real(re);
		pVal[k].imag(im);

		++realNonZero;

		if ((usedSolver == sparse::SolverType::LU) && (i != j))
		{
			++realNonZero;
		}
	}

	//timer.stop(iTimer);	
	//cout << "*";

	mu::Timer<5> timer("Total time");
	timer.measure(0);
	int iTimer = 1; // timer->add("Creating matrix");
	timer.setName(iTimer, "Creating matrix");
	timer.measure(iTimer);
	//myUtils::SparseComplexSolver<double> m(N);
	int nRealNonZero = realNonZero;
	sparse::Symmetry symmetry = sparse::Symmetry::SymmetricIndef;
	if (usedSolver == sparse::SolverType::LU)
		symmetry = sparse::Symmetry::NonSymmetric;
	else
	{
		if (hermitian)
        {
            if (PosDef)
                symmetry = sparse::Symmetry::HermitianPosDef;
            else
                symmetry = sparse::Symmetry::HermitianIndef;
        }
        else
        {
            if (PosDef)
                symmetry = sparse::Symmetry::SymmetricPosDef;
            else
                symmetry = sparse::Symmetry::SymmetricIndef;
        }
	}

	sparse::Pivoting pivoting = sparse::Pivoting::No;
    
	if (usedSolver == sparse::SolverType::LU)
		pivoting = sparse::Pivoting::DiagonalMultiPass;
    else
    {
        if (PosDef)
            pivoting = sparse::Pivoting::No;
        else
            pivoting = sparse::Pivoting::DiagonalMultiPass;
    }
    
    if (usedSolver == sparse::SolverType::Pardiso)
        symmetry = sparse::Symmetry::SymmetricIndef;
    
    //Ne radi sa NoPivoting
    pivoting = sparse::Pivoting::DiagonalMultiPass;
    
    showSolverSymmetryAndPivoting(usedSolver, symmetry, pivoting);

	sparse::CmplxSolverReleaser pSolver(sparse::createCmplxSolver(N, nRealNonZero, symmetry, usedSolver, pivoting, sparse::Ordering::Own));

	timer.pause(iTimer);
	
	//testHermitianSolver(offStream, matrixName, pSolver.ref(), N, NZ, pI, pJ, pVal, symmetric, *timer, realNonZero, false);

	timer.setName(++iTimer, "Populating matrix");	
	//int nAddedForSymmetry = 0;
    td::cmplx zero(0,0);
    pSolver->populateDiagonals(zero);

	for (int k = 0; k<NZ; ++k)
	{
		pSolver->addTriple(pI[k], pJ[k], pVal[k]);

		if ((usedSolver == sparse::SolverType::LU) && (pI[k] != pJ[k]))
		{
			if (hermitian)
				pSolver->addTriple(pJ[k], pI[k], std::conj(pVal[k]));
			else
				pSolver->addTriple(pJ[k], pI[k], pVal[k]);
		}
	}

	timer.pause(iTimer);

    setCmplxRHS(pSolver, N);
    
	cout << "*";

	//m.save("InputMatrix:", offStream, true);
#ifdef SER_RESULTS
	offStream << "\nMatrix:\n--------------\n";
	pSolver->serialize("A", offStream, sparse::Format::Matlab);
	offStream << "\nEND Matrix:\n--------------\n";
#endif
	timer.setName(++iTimer, "Factorize");
	bool bFact = pSolver->factorize();
	timer.pause(iTimer);
	
	if (!bFact)
	{
#ifdef SER_RESULTS
		offStream << "ERROR: Cannot factorize matrix!!" << endl;
#endif
        const char* pLastErr = pSolver->getLastError();
        if (pLastErr)
            cout << pLastErr << " ";
        
		cout << "x" << std::endl;
		return;
	}		
	//else
	//{
	//	offStream << "Factorization OK!" << endl;
	//}	

	

	if (bFact)
	{
		//set some free values
		
//		for (int i = 0; i<N; ++i)
//		{
//			//td::cmplx val(i+1, 0);
//			td::cmplx val(1, 0);
//			pSolver->setRHS(i, val);
//		}
		
#ifdef SER_RESULTS
		pSolver->showRHS("b", offStream, sparse::Format::Fortran);
#endif
		//m.showResults("Free Vector:", offStream, true);
		timer.setName(++iTimer, "Solver");		
		pSolver->solve();
		timer.pauseAll(); 
#ifdef SER_RESULTS
		offStream << std::endl << "Results:" << std::endl;
		pSolver->showResults("x", offStream, sparse::Format::Fortran);
        pSolver->showResults("x", offRes, sparse::Format::Fortran);
#endif
	}
		
	timer.pauseAll();
    //timer.calcTotal();
	cout << "*" << std::endl;	
#ifdef SER_RESULTS
	if (matrixName)
	{
		offStream << "Results for " << matrixName << endl;
	}

	if (symmetric)
		offStream << "Matrix is symmetric\n";
	else
		offStream << "Matrix is not symmetric (general)\n";

	offStream << "Matrix dimension: " << N << " input file rows: " << NZ << " nonzero elements: " << realNonZero << endl;
#endif

#ifdef SHOW_STATISTICS
	//m.showStatistics(offStream);
    timer.showAllTimesInMillisec(offStream);
    timer.showAllTimesInMillisec(offRes);
	offStream << "---------\n";
#else
	timer.showAllTimesInMillisec(std::cout);
#endif
}

inline void createComplexMatrix(const char* inFileName, const char* outFileName, bool hermitian, double reImRatio)
{

	ifstream f;
	if (!fo::openFile(f, inFileName))
	{
		std::cout << "Cannot open inut matrix file: " << inFileName << std::endl;
	}

	std::ofstream offStream;
	if (!fo::openFile(offStream, outFileName))
	{
		std::cout << "Cannot open out matrix file: " << outFileName << std::endl;
		return;
	}	

	

	string str;
	f >> str;
	if ((str.length() == 0) ||
		(str.at(0) != 'r'))
	{
		offStream << "Expected \'real\' in " << inFileName << endl;
		return;
	}
	offStream << "complex ";
	f >> str;
	if (str.length() == 0)
	{
		offStream << "Expected \'symmetric or real\' in " << inFileName << endl;
		return;
	}
	
	offStream << str;
	if (hermitian)
		offStream <<"Hermitian" << std::endl;
	else
		offStream << std::endl;

	bool symmetric = false;
	if (str.at(0) == 's')
	{
		symmetric = true;
	}

	int N = -1;
	int NN = 0;
	int NZ = 0;
	//int realNonZero = 0;
	f >> N >> NN >> NZ;
	if (N != NN)
	{
		offStream << "Expected square matrix in " << inFileName << endl;
		return;
	}

	if (NZ <= 0)
	{
		offStream << "Number of lines in file is negative: " << NZ << endl;
		return;
	}

	offStream << N << " " << NN << " " << NZ << std::endl;


	//iTimer = timer.add("Loading matrix");
	//load matrix
	for (int k = 0; k<NZ; ++k)
	{
		int i;
		int j;
		double re;
		f >> i >> j >> re;
		if (i <= 0 || j <= 0)
		{
			offStream << "Negative indices detected at line " << k + 1 << endl;
			return;
		}

		if (j > i)
		{
			offStream << "Upper part of matrix detected at line " << k + 1 << endl;
			return;
		}
		if (i == j)
		{
			if (!hermitian)
				offStream << i << " " << j << " " << re << " " << re / reImRatio << std::endl;
			else
				offStream << i << " " << j << " " << re << " " << 0 << std::endl;
		}
		else
			offStream << i << " " << j << " " << re << " " << re / reImRatio << std::endl;	

	}

	std::cout << "All ok" << std::endl;
}

void testRealCholesky()
{
    auto inPath = getRealCholleskyPath(natIDSDKPath);
    std::cout << std::endl << "Testing Real Chollesky...." << std::endl;
    std::cout << "4node (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "4node", sparse::SolverType::LDLT, true);
    testRealMatrix(inPath, "4node", sparse::SolverType::LU, true);
    
    std::cout << "case300 (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "case300", sparse::SolverType::LDLT, true);
    testRealMatrix(inPath, "case300", sparse::SolverType::LU, true);
    
    std::cout << "1k5 (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "1k5", sparse::SolverType::LDLT, true);
    testRealMatrix(inPath, "1k5", sparse::SolverType::LU, true);
    
    std::cout << "1888 (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "1888", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "1888", sparse::SolverType::LU, false);
    
    std::cout << "3k (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "3k", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "3k", sparse::SolverType::LU, false);
    
    std::cout << "9241 (Real_PosDef)" << std::endl;
    testRealMatrix(inPath, "9241", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "9241", sparse::SolverType::LU, false);
}

void testRealLDLT()
{
    auto inPath = getRealLDLTPath(natIDSDKPath);
    std::cout << std::endl << "Testing Real LDLT...." << std::endl;
    std::cout << "4node (Real_Indef)" << std::endl;
    
    testRealMatrix(inPath, "4node", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "4node", sparse::SolverType::LU, false);
    
    std::cout << "case300 (Real_Indef)" << std::endl;
    testRealMatrix(inPath, "case300", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "case300", sparse::SolverType::LU, false);
    
    std::cout << "1k5 (Real_Indef)" << std::endl;
    testRealMatrix(inPath, "1k5", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "1k5", sparse::SolverType::LU, false);
    
    std::cout << "1888 (Real_Indef)" << std::endl;
    testRealMatrix(inPath, "1888", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "1888", sparse::SolverType::LU, false);
    
    std::cout << "3k (Real_Indef)" << std::endl;
    testRealMatrix(inPath, "3k", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "3k", sparse::SolverType::LU, false);
    
    std::cout << "9241 (Real_Indef)" << std::endl;
    testRealMatrix(inPath, "9241", sparse::SolverType::LDLT, false);
    testRealMatrix(inPath, "9241", sparse::SolverType::LU, false);
}

void testCmplxCholesky()
{
    auto inPath = getCmplxCholleskyPath(natIDSDKPath);
    std::cout << std::endl << "Testing Complex Chollesky...." << std::endl;
    std::cout << "4node (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "4node", sparse::SolverType::LU, true, true);
    testComplexMatrix(inPath, "4node", sparse::SolverType::LDLT, true, true);
    
    std::cout << "case300 (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "case300", sparse::SolverType::LDLT, true, true);
    testComplexMatrix(inPath, "case300", sparse::SolverType::LU, true, true);;
    
    std::cout << "1k5 (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "1k5", sparse::SolverType::LDLT, true, true);
    testComplexMatrix(inPath, "1k5", sparse::SolverType::LU, true, true);
    
    std::cout << "1888 (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "1888", sparse::SolverType::LDLT, true, true);
    testComplexMatrix(inPath, "1888", sparse::SolverType::LU, true, true);
    
    std::cout << "3k (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "3k", sparse::SolverType::LDLT, true, true);
    testComplexMatrix(inPath, "3k", sparse::SolverType::LU, true, true);
    
    std::cout << "9241 (Complex_PosDef)" << std::endl;
    testComplexMatrix(inPath, "9241", sparse::SolverType::LDLT, true, true);
    testComplexMatrix(inPath, "9241", sparse::SolverType::LU, true, true);
}

void testCmplxLDLT()
{
    auto inPath = getCmplxLDLTPath(natIDSDKPath);
    std::cout << std::endl << "Testing Complex LDLT...." << std::endl;
    std::cout << "4node (Complex_Indef)" << std::endl;

    testComplexMatrix(inPath, "4node", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "4node", sparse::SolverType::LU, false, true);
    
    std::cout << "case300 (Complex_Indef)" << std::endl;
    testComplexMatrix(inPath, "case300", sparse::SolverType::Ctrl, false, true);
    testComplexMatrix(inPath, "case300", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "case300", sparse::SolverType::LU, false, true);
    testComplexMatrix(inPath, "case300", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "1k5 (Complex_Indef)" << std::endl;
    testComplexMatrix(inPath, "1k5", sparse::SolverType::Ctrl, false, true);
    testComplexMatrix(inPath, "1k5", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "1k5", sparse::SolverType::LU, false, true);
    testComplexMatrix(inPath, "1k5", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "1888 (Complex_Indef)" << std::endl;
    testComplexMatrix(inPath, "1888", sparse::SolverType::Ctrl, false, true);
    testComplexMatrix(inPath, "1888", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "1888", sparse::SolverType::LU, false, true);
    testComplexMatrix(inPath, "1888", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "3k (Complex_Indef)" << std::endl;
    testComplexMatrix(inPath, "3k", sparse::SolverType::Ctrl, false, true);
    testComplexMatrix(inPath, "3k", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "3k", sparse::SolverType::LU, false, true);
    testComplexMatrix(inPath, "3k", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "9241 (Complex_Indef)" << std::endl;
    testComplexMatrix(inPath, "9241", sparse::SolverType::Ctrl, false, true);
    testComplexMatrix(inPath, "9241", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "9241", sparse::SolverType::LU, false, true);
    testComplexMatrix(inPath, "9241", sparse::SolverType::Pardiso, false, true);
}

void testCmplxLDLTDbg()
{
    auto inPath = getCmplxLDLTPath(natIDSDKPath);
    std::cout << std::endl << "Testing Complex LDLT...." << std::endl;
    std::cout << "1k5 (Complex_Indef)" << std::endl;

    testComplexMatrix(inPath, "1k5", sparse::SolverType::LU, true, true);
}

inline void convertSomeMatrices()
{
	createComplexMatrix("D:/DropBox/Work/TestData/TestMatrices/bcsstk24.mtx", "F:/bcsstk24C.mtx", true, 5.);
}

template<class SOLVER>
void loadH(SOLVER solver)
{
    solver->addTriple(0, 0, std::complex<double>(5.501870e+01, -2.373150e+02));
    solver->addTriple(0, 1, std::complex<double>(-1.681610e+01, 4.223570e+01));
    solver->addTriple(0, 2, std::complex<double>(-9.208620e+00, 3.157290e+01));
    solver->addTriple(0, 6, std::complex<double>(-3.963400e+01, 1.142380e+02));
    solver->addTriple(0, 7, std::complex<double>(1.681610e+01, -4.223570e+01));
    solver->addTriple(0, 8, std::complex<double>(9.208620e+00, -3.157290e+01));
    solver->addTriple(0, 58, std::complex<double>(-1.538460e+01, 1.230770e+02));
    solver->addTriple(1, 0, std::complex<double>(-1.681610e+01, 4.223570e+01));
    solver->addTriple(1, 1, std::complex<double>(5.008970e+01, -2.313280e+02));
    solver->addTriple(1, 2, std::complex<double>(-4.371560e+00, 2.411360e+01));
    solver->addTriple(1, 6, std::complex<double>(1.681610e+01, -4.223570e+01));
    solver->addTriple(1, 7, std::complex<double>(-3.470500e+01, 1.082520e+02));
    solver->addTriple(1, 8, std::complex<double>(4.371560e+00, -2.411360e+01));
    solver->addTriple(1, 59, std::complex<double>(1.142800e+02, -4.821500e+01));
    solver->addTriple(2, 0, std::complex<double>(-9.208620e+00, 3.157290e+01));
    solver->addTriple(2, 1, std::complex<double>(-4.371560e+00, 2.411360e+01));
    solver->addTriple(2, 2, std::complex<double>(4.607500e+01, -2.252000e+02));
    solver->addTriple(2, 6, std::complex<double>(9.208620e+00, -3.157290e+01));
    solver->addTriple(2, 7, std::complex<double>(4.371560e+00, -2.411360e+01));
    solver->addTriple(2, 8, std::complex<double>(-3.069040e+01, 1.021240e+02));
    solver->addTriple(2, 60, std::complex<double>(-9.889540e+01, -7.486190e+01));
    solver->addTriple(3, 3, std::complex<double>(5.501870e+01, 2.373150e+02));
    solver->addTriple(3, 4, std::complex<double>(-1.681610e+01, -4.223570e+01));
    solver->addTriple(3, 5, std::complex<double>(-9.208620e+00, -3.157290e+01));
    solver->addTriple(3, 9, std::complex<double>(-3.963400e+01, -1.142380e+02));
    solver->addTriple(3, 10, std::complex<double>(1.681610e+01, 4.223570e+01));
    solver->addTriple(3, 11, std::complex<double>(9.208620e+00, 3.157290e+01));
    solver->addTriple(3, 61, std::complex<double>(-1.538460e+01, -1.230770e+02));
    solver->addTriple(4, 3, std::complex<double>(-1.681610e+01, -4.223570e+01));
    solver->addTriple(4, 4, std::complex<double>(5.008970e+01, 2.313280e+02));
    solver->addTriple(4, 5, std::complex<double>(-4.371560e+00, -2.411360e+01));
    solver->addTriple(4, 9, std::complex<double>(1.681610e+01, 4.223570e+01));
    solver->addTriple(4, 10, std::complex<double>(-3.470500e+01, -1.082520e+02));
    solver->addTriple(4, 11, std::complex<double>(4.371560e+00, 2.411360e+01));
    solver->addTriple(4, 62, std::complex<double>(1.142800e+02, 4.821500e+01));
    solver->addTriple(5, 3, std::complex<double>(-9.208620e+00, -3.157290e+01));
    solver->addTriple(5, 4, std::complex<double>(-4.371560e+00, -2.411360e+01));
    solver->addTriple(5, 5, std::complex<double>(4.607500e+01, 2.252000e+02));
    solver->addTriple(5, 9, std::complex<double>(9.208620e+00, 3.157290e+01));
    solver->addTriple(5, 10, std::complex<double>(4.371560e+00, 2.411360e+01));
    solver->addTriple(5, 11, std::complex<double>(-3.069040e+01, -1.021240e+02));
    solver->addTriple(5, 63, std::complex<double>(-9.889540e+01, 7.486190e+01));
    solver->addTriple(6, 3, std::complex<double>(-3.963400e+01, -1.142380e+02));
    solver->addTriple(6, 4, std::complex<double>(1.681610e+01, 4.223570e+01));
    solver->addTriple(6, 5, std::complex<double>(9.208620e+00, 3.157290e+01));
    solver->addTriple(6, 6, std::complex<double>(-1.499160e-05, -2.310840e-04));
    solver->addTriple(6, 9, std::complex<double>(2.718660e+02, 4.753850e+02));
    solver->addTriple(6, 10, std::complex<double>(-8.538140e+01, -1.365930e+02));
    solver->addTriple(6, 11, std::complex<double>(-9.246720e+01, -1.216430e+02));
    solver->addTriple(6, 15, std::complex<double>(-1.925980e+02, -2.469100e+02));
    solver->addTriple(6, 16, std::complex<double>(5.174910e+01, 5.212130e+01));
    solver->addTriple(6, 17, std::complex<double>(7.405000e+01, 5.849700e+01));
    solver->addTriple(6, 35, std::complex<double>(-3.963400e+01, -1.142380e+02));
    solver->addTriple(6, 36, std::complex<double>(1.681610e+01, 4.223570e+01));
    solver->addTriple(6, 37, std::complex<double>(9.208620e+00, 3.157290e+01));
    solver->addTriple(7, 3, std::complex<double>(2.816920e+01, -3.568110e+01));
    solver->addTriple(7, 4, std::complex<double>(-7.639610e+01, 8.418120e+01));
    solver->addTriple(7, 5, std::complex<double>(1.869720e+01, -1.584270e+01));
    solver->addTriple(7, 7, std::complex<double>(1.846680e-04, 1.379380e-04));
    solver->addTriple(7, 9, std::complex<double>(-7.560210e+01, 1.422390e+02));
    solver->addTriple(7, 10, std::complex<double>(3.240540e+02, -6.366620e+02));
    solver->addTriple(7, 11, std::complex<double>(-5.297410e+01, 1.419000e+02));
    solver->addTriple(7, 15, std::complex<double>(1.926380e+01, -7.087670e+01));
    solver->addTriple(7, 16, std::complex<double>(-1.267750e+02, 2.693660e+02));
    solver->addTriple(7, 17, std::complex<double>(2.260730e+01, -5.653580e+01));
    solver->addTriple(7, 26, std::complex<double>(-4.448690e+01, 1.989340e+02));
    solver->addTriple(7, 27, std::complex<double>(-7.027680e+00, -5.367840e+01));
    solver->addTriple(7, 35, std::complex<double>(2.816920e+01, -3.568110e+01));
    solver->addTriple(7, 36, std::complex<double>(-7.639610e+01, 8.418120e+01));
    solver->addTriple(7, 37, std::complex<double>(1.869720e+01, -1.584270e+01));
    solver->addTriple(8, 3, std::complex<double>(-3.194730e+01, -7.811560e+00));
    solver->addTriple(8, 4, std::complex<double>(-2.306880e+01, -8.270930e+00));
    solver->addTriple(8, 5, std::complex<double>(1.037870e+02, 2.448310e+01));
    solver->addTriple(8, 8, std::complex<double>(-1.779480e-04, 1.167470e-04));
    solver->addTriple(8, 9, std::complex<double>(1.515790e+02, -1.925760e+01));
    solver->addTriple(8, 10, std::complex<double>(1.493760e+02, -2.507290e+01));
    solver->addTriple(8, 11, std::complex<double>(-7.043170e+02, 4.368190e+01));
    solver->addTriple(8, 15, std::complex<double>(-8.768490e+01, 3.488070e+01));
    solver->addTriple(8, 16, std::complex<double>(-6.026510e+01, 8.689420e+00));
    solver->addTriple(8, 17, std::complex<double>(3.021880e+02, -3.292130e+01));
    solver->addTriple(8, 26, std::complex<double>(-4.297300e+01, 3.292540e+01));
    solver->addTriple(8, 27, std::complex<double>(1.945550e+02, -5.972680e+01));
    solver->addTriple(8, 35, std::complex<double>(-3.194730e+01, -7.811560e+00));
    solver->addTriple(8, 36, std::complex<double>(-2.306880e+01, -8.270930e+00));
    solver->addTriple(8, 37, std::complex<double>(1.037870e+02, 2.448310e+01));
    solver->addTriple(9, 0, std::complex<double>(-3.963400e+01, 1.142380e+02));
    solver->addTriple(9, 1, std::complex<double>(1.681610e+01, -4.223570e+01));
    solver->addTriple(9, 2, std::complex<double>(9.208620e+00, -3.157290e+01));
    solver->addTriple(9, 6, std::complex<double>(2.718660e+02, -4.753850e+02));
    solver->addTriple(9, 7, std::complex<double>(-8.538140e+01, 1.365930e+02));
    solver->addTriple(9, 8, std::complex<double>(-9.246720e+01, 1.216430e+02));
    solver->addTriple(9, 9, std::complex<double>(-1.499160e-05, 2.310840e-04));
    solver->addTriple(9, 12, std::complex<double>(-1.925980e+02, 2.469100e+02));
    solver->addTriple(9, 13, std::complex<double>(5.174910e+01, -5.212130e+01));
    solver->addTriple(9, 14, std::complex<double>(7.405000e+01, -5.849700e+01));
    solver->addTriple(9, 32, std::complex<double>(-3.963400e+01, 1.142380e+02));
    solver->addTriple(9, 33, std::complex<double>(1.681610e+01, -4.223570e+01));
    solver->addTriple(9, 34, std::complex<double>(9.208620e+00, -3.157290e+01));
    solver->addTriple(10, 0, std::complex<double>(2.816920e+01, 3.568110e+01));
    solver->addTriple(10, 1, std::complex<double>(-7.639610e+01, -8.418120e+01));
    solver->addTriple(10, 2, std::complex<double>(1.869720e+01, 1.584270e+01));
    solver->addTriple(10, 6, std::complex<double>(-7.560210e+01, -1.422390e+02));
    solver->addTriple(10, 7, std::complex<double>(3.240540e+02, 6.366620e+02));
    solver->addTriple(10, 8, std::complex<double>(-5.297410e+01, -1.419000e+02));
    solver->addTriple(10, 10, std::complex<double>(1.846680e-04, -1.379380e-04));
    solver->addTriple(10, 12, std::complex<double>(1.926380e+01, 7.087670e+01));
    solver->addTriple(10, 13, std::complex<double>(-1.267750e+02, -2.693660e+02));
    solver->addTriple(10, 14, std::complex<double>(2.260730e+01, 5.653580e+01));
    solver->addTriple(10, 24, std::complex<double>(-4.448690e+01, -1.989340e+02));
    solver->addTriple(10, 25, std::complex<double>(-7.027680e+00, 5.367840e+01));
    solver->addTriple(10, 32, std::complex<double>(2.816920e+01, 3.568110e+01));
    solver->addTriple(10, 33, std::complex<double>(-7.639610e+01, -8.418120e+01));
    solver->addTriple(10, 34, std::complex<double>(1.869720e+01, 1.584270e+01));
    solver->addTriple(11, 0, std::complex<double>(-3.194730e+01, 7.811560e+00));
    solver->addTriple(11, 1, std::complex<double>(-2.306880e+01, 8.270930e+00));
    solver->addTriple(11, 2, std::complex<double>(1.037870e+02, -2.448310e+01));
    solver->addTriple(11, 6, std::complex<double>(1.515790e+02, 1.925760e+01));
    solver->addTriple(11, 7, std::complex<double>(1.493760e+02, 2.507290e+01));
    solver->addTriple(11, 8, std::complex<double>(-7.043170e+02, -4.368190e+01));
    solver->addTriple(11, 11, std::complex<double>(-1.779480e-04, -1.167470e-04));
    solver->addTriple(11, 12, std::complex<double>(-8.768490e+01, -3.488070e+01));
    solver->addTriple(11, 13, std::complex<double>(-6.026510e+01, -8.689420e+00));
    solver->addTriple(11, 14, std::complex<double>(3.021880e+02, 3.292130e+01));
    solver->addTriple(11, 24, std::complex<double>(-4.297300e+01, -3.292540e+01));
    solver->addTriple(11, 25, std::complex<double>(1.945550e+02, 5.972680e+01));
    solver->addTriple(11, 32, std::complex<double>(-3.194730e+01, 7.811560e+00));
    solver->addTriple(11, 33, std::complex<double>(-2.306880e+01, 8.270930e+00));
    solver->addTriple(11, 34, std::complex<double>(1.037870e+02, -2.448310e+01));
    solver->addTriple(12, 6, std::complex<double>(-1.925980e+02, 2.469100e+02));
    solver->addTriple(12, 7, std::complex<double>(5.174910e+01, -5.212130e+01));
    solver->addTriple(12, 8, std::complex<double>(7.405000e+01, -5.849700e+01));
    solver->addTriple(12, 12, std::complex<double>(2.137120e+02, -2.852980e+02));
    solver->addTriple(12, 13, std::complex<double>(-5.174910e+01, 5.212130e+01));
    solver->addTriple(12, 14, std::complex<double>(-7.405000e+01, 5.849700e+01));
    solver->addTriple(12, 18, std::complex<double>(-2.111320e+01, 3.838770e+01));
    solver->addTriple(13, 6, std::complex<double>(5.174910e+01, -5.212130e+01));
    solver->addTriple(13, 7, std::complex<double>(-1.698910e+02, 2.444740e+02));
    solver->addTriple(13, 8, std::complex<double>(3.765780e+01, -4.784640e+01));
    solver->addTriple(13, 12, std::complex<double>(-5.174910e+01, 5.212130e+01));
    solver->addTriple(13, 13, std::complex<double>(1.910040e+02, -2.828610e+02));
    solver->addTriple(13, 14, std::complex<double>(-3.765780e+01, 4.784640e+01));
    solver->addTriple(13, 19, std::complex<double>(-2.111320e+01, 3.838770e+01));
    solver->addTriple(14, 6, std::complex<double>(7.405000e+01, -5.849700e+01));
    solver->addTriple(14, 7, std::complex<double>(3.765780e+01, -4.784640e+01));
    solver->addTriple(14, 8, std::complex<double>(-1.796050e+02, 2.452420e+02));
    solver->addTriple(14, 12, std::complex<double>(-7.405000e+01, 5.849700e+01));
    solver->addTriple(14, 13, std::complex<double>(-3.765780e+01, 4.784640e+01));
    solver->addTriple(14, 14, std::complex<double>(2.007180e+02, -2.836290e+02));
    solver->addTriple(14, 20, std::complex<double>(-2.111320e+01, 3.838770e+01));
    solver->addTriple(15, 9, std::complex<double>(-1.925980e+02, -2.469100e+02));
    solver->addTriple(15, 10, std::complex<double>(5.174910e+01, 5.212130e+01));
    solver->addTriple(15, 11, std::complex<double>(7.405000e+01, 5.849700e+01));
    solver->addTriple(15, 15, std::complex<double>(2.137120e+02, 2.852980e+02));
    solver->addTriple(15, 16, std::complex<double>(-5.174910e+01, -5.212130e+01));
    solver->addTriple(15, 17, std::complex<double>(-7.405000e+01, -5.849700e+01));
    solver->addTriple(15, 21, std::complex<double>(-2.111320e+01, -3.838770e+01));
    solver->addTriple(16, 9, std::complex<double>(5.174910e+01, 5.212130e+01));
    solver->addTriple(16, 10, std::complex<double>(-1.698910e+02, -2.444740e+02));
    solver->addTriple(16, 11, std::complex<double>(3.765780e+01, 4.784640e+01));
    solver->addTriple(16, 15, std::complex<double>(-5.174910e+01, -5.212130e+01));
    solver->addTriple(16, 16, std::complex<double>(1.910040e+02, 2.828610e+02));
    solver->addTriple(16, 17, std::complex<double>(-3.765780e+01, -4.784640e+01));
    solver->addTriple(16, 22, std::complex<double>(-2.111320e+01, -3.838770e+01));
    solver->addTriple(17, 9, std::complex<double>(7.405000e+01, 5.849700e+01));
    solver->addTriple(17, 10, std::complex<double>(3.765780e+01, 4.784640e+01));
    solver->addTriple(17, 11, std::complex<double>(-1.796050e+02, -2.452420e+02));
    solver->addTriple(17, 15, std::complex<double>(-7.405000e+01, -5.849700e+01));
    solver->addTriple(17, 16, std::complex<double>(-3.765780e+01, -4.784640e+01));
    solver->addTriple(17, 17, std::complex<double>(2.007180e+02, 2.836290e+02));
    solver->addTriple(17, 23, std::complex<double>(-2.111320e+01, -3.838770e+01));
    solver->addTriple(18, 15, std::complex<double>(-2.111320e+01, -3.838770e+01));
    solver->addTriple(18, 21, std::complex<double>(2.111320e+01, 3.838770e+01));
    solver->addTriple(19, 16, std::complex<double>(-2.268810e+01, 3.747850e+01));
    solver->addTriple(19, 22, std::complex<double>(2.268810e+01, -3.747850e+01));
    solver->addTriple(20, 17, std::complex<double>(4.380140e+01, 9.092530e-01));
    solver->addTriple(20, 23, std::complex<double>(-4.380140e+01, -9.092530e-01));
    solver->addTriple(21, 12, std::complex<double>(-2.111320e+01, 3.838770e+01));
    solver->addTriple(21, 18, std::complex<double>(2.111320e+01, -3.838770e+01));
    solver->addTriple(22, 13, std::complex<double>(-2.268810e+01, -3.747850e+01));
    solver->addTriple(22, 19, std::complex<double>(2.268810e+01, 3.747850e+01));
    solver->addTriple(23, 14, std::complex<double>(4.380140e+01, -9.092530e-01));
    solver->addTriple(23, 20, std::complex<double>(-4.380140e+01, 9.092530e-01));
    solver->addTriple(24, 10, std::complex<double>(-4.448690e+01, 1.989340e+02));
    solver->addTriple(24, 11, std::complex<double>(-7.027680e+00, -5.367840e+01));
    solver->addTriple(24, 24, std::complex<double>(2.547620e-05, 9.989530e-06));
    solver->addTriple(24, 26, std::complex<double>(1.186320e+02, -5.304900e+02));
    solver->addTriple(24, 27, std::complex<double>(1.874050e+01, 1.431420e+02));
    solver->addTriple(24, 30, std::complex<double>(-7.414480e+01, 3.315560e+02));
    solver->addTriple(24, 31, std::complex<double>(-1.171280e+01, -8.946410e+01));
    solver->addTriple(25, 7, std::complex<double>(5.000070e+01, -2.075310e+01));
    solver->addTriple(25, 8, std::complex<double>(-1.490030e+02, 1.386270e+02));
    solver->addTriple(25, 24, std::complex<double>(-1.333350e+02, 5.534150e+01));
    solver->addTriple(25, 25, std::complex<double>(3.973400e+02, -3.696710e+02));
    solver->addTriple(25, 28, std::complex<double>(8.333450e+01, -3.458850e+01));
    solver->addTriple(25, 29, std::complex<double>(-2.483380e+02, 2.310440e+02));
    solver->addTriple(26, 7, std::complex<double>(-4.448690e+01, -1.989340e+02));
    solver->addTriple(26, 8, std::complex<double>(-7.027680e+00, 5.367840e+01));
    solver->addTriple(26, 24, std::complex<double>(1.186320e+02, 5.304900e+02));
    solver->addTriple(26, 25, std::complex<double>(1.874050e+01, -1.431420e+02));
    solver->addTriple(26, 26, std::complex<double>(2.547620e-05, -9.989530e-06));
    solver->addTriple(26, 28, std::complex<double>(-7.414480e+01, -3.315560e+02));
    solver->addTriple(26, 29, std::complex<double>(-1.171280e+01, 8.946410e+01));
    solver->addTriple(27, 10, std::complex<double>(5.000070e+01, 2.075310e+01));
    solver->addTriple(27, 11, std::complex<double>(-1.490030e+02, -1.386270e+02));
    solver->addTriple(27, 26, std::complex<double>(-1.333350e+02, -5.534150e+01));
    solver->addTriple(27, 27, std::complex<double>(3.973400e+02, 3.696710e+02));
    solver->addTriple(27, 30, std::complex<double>(8.333450e+01, 3.458850e+01));
    solver->addTriple(27, 31, std::complex<double>(-2.483380e+02, -2.310440e+02));
    solver->addTriple(28, 26, std::complex<double>(5.990900e+01, -1.443400e+02));
    solver->addTriple(28, 27, std::complex<double>(-4.001810e+02, 4.301340e+02));
    solver->addTriple(28, 28, std::complex<double>(-9.478820e-06, 3.702910e-06));
    solver->addTriple(28, 29, std::complex<double>(9.478820e-06, -3.702910e-06));
    solver->addTriple(28, 30, std::complex<double>(-5.990890e+01, 1.443400e+02));
    solver->addTriple(28, 31, std::complex<double>(4.001810e+02, -4.301340e+02));
    solver->addTriple(29, 24, std::complex<double>(-1.667290e+02, 1.954010e+02));
    solver->addTriple(29, 25, std::complex<double>(-1.650030e+02, 1.964560e+02));
    solver->addTriple(29, 28, std::complex<double>(1.667290e+02, -1.954010e+02));
    solver->addTriple(29, 29, std::complex<double>(1.650030e+02, -1.964560e+02));
    solver->addTriple(30, 24, std::complex<double>(5.990900e+01, 1.443400e+02));
    solver->addTriple(30, 25, std::complex<double>(-4.001810e+02, -4.301340e+02));
    solver->addTriple(30, 28, std::complex<double>(-5.990890e+01, -1.443400e+02));
    solver->addTriple(30, 29, std::complex<double>(4.001810e+02, 4.301340e+02));
    solver->addTriple(30, 30, std::complex<double>(-9.478820e-06, -3.702910e-06));
    solver->addTriple(30, 31, std::complex<double>(9.478820e-06, 3.702910e-06));
    solver->addTriple(31, 26, std::complex<double>(-1.667290e+02, -1.954010e+02));
    solver->addTriple(31, 27, std::complex<double>(-1.650030e+02, -1.964560e+02));
    solver->addTriple(31, 30, std::complex<double>(1.667290e+02, 1.954010e+02));
    solver->addTriple(31, 31, std::complex<double>(1.650030e+02, 1.964560e+02));
    solver->addTriple(32, 6, std::complex<double>(1.189020e+02, -3.427130e+02));
    solver->addTriple(32, 7, std::complex<double>(-5.044830e+01, 1.267070e+02));
    solver->addTriple(32, 8, std::complex<double>(-2.762590e+01, 9.471880e+01));
    solver->addTriple(32, 32, std::complex<double>(-2.173140e+03, 2.527830e+03));
    solver->addTriple(32, 33, std::complex<double>(4.027010e+02, -6.919600e+02));
    solver->addTriple(32, 34, std::complex<double>(4.447880e+02, -6.431960e+02));
    solver->addTriple(32, 35, std::complex<double>(-5.021970e+00, 2.872550e+00));
    solver->addTriple(32, 36, std::complex<double>(2.660560e+00, -1.523280e+00));
    solver->addTriple(32, 37, std::complex<double>(2.361400e+00, -1.349270e+00));
    solver->addTriple(32, 38, std::complex<double>(1.071420e+03, -8.065610e+02));
    solver->addTriple(32, 39, std::complex<double>(-2.513560e+02, 3.118380e+02));
    solver->addTriple(32, 40, std::complex<double>(-1.119060e+02, 2.552740e+02));
    solver->addTriple(32, 46, std::complex<double>(7.450130e+02, -6.931330e+02));
    solver->addTriple(32, 47, std::complex<double>(-2.500040e+02, 1.037650e+02));
    solver->addTriple(32, 52, std::complex<double>(2.378040e+02, -6.854270e+02));
    solver->addTriple(32, 53, std::complex<double>(-1.008970e+02, 2.534140e+02));
    solver->addTriple(32, 54, std::complex<double>(-5.525170e+01, 1.894380e+02));
    solver->addTriple(33, 6, std::complex<double>(1.349560e+02, -1.966410e+01));
    solver->addTriple(33, 7, std::complex<double>(-3.333030e+02, 7.221100e+01));
    solver->addTriple(33, 8, std::complex<double>(6.920640e+01, -2.481280e+01));
    solver->addTriple(33, 32, std::complex<double>(-8.006050e+02, -2.769420e+00));
    solver->addTriple(33, 33, std::complex<double>(2.321740e+03, 3.926930e+02));
    solver->addTriple(33, 34, std::complex<double>(-6.033570e+02, 1.267680e+01));
    solver->addTriple(33, 35, std::complex<double>(2.508430e+00, -1.433330e+00));
    solver->addTriple(33, 36, std::complex<double>(-4.869060e+00, 2.782610e+00));
    solver->addTriple(33, 37, std::complex<double>(2.360630e+00, -1.349290e+00));
    solver->addTriple(33, 38, std::complex<double>(3.957380e+02, 6.176160e+01));
    solver->addTriple(33, 39, std::complex<double>(-1.321830e+03, -6.093260e+02));
    solver->addTriple(33, 40, std::complex<double>(3.957380e+02, 6.176160e+01));
    solver->addTriple(33, 52, std::complex<double>(2.699120e+02, -3.932820e+01));
    solver->addTriple(33, 53, std::complex<double>(-6.666070e+02, 1.444220e+02));
    solver->addTriple(33, 54, std::complex<double>(1.384130e+02, -4.962560e+01));
    solver->addTriple(34, 6, std::complex<double>(-1.360930e+01, 4.042910e+01));
    solver->addTriple(34, 7, std::complex<double>(-1.351740e+01, 4.190220e+01));
    solver->addTriple(34, 8, std::complex<double>(-1.711020e+01, 4.643700e+01));
    solver->addTriple(34, 32, std::complex<double>(4.418840e+02, -3.975590e+02));
    solver->addTriple(34, 33, std::complex<double>(2.691840e+02, -1.978400e+02));
    solver->addTriple(34, 34, std::complex<double>(4.541120e+02, -4.145280e+02));
    solver->addTriple(34, 38, std::complex<double>(-2.360530e+02, 7.981610e+01));
    solver->addTriple(34, 39, std::complex<double>(-2.286320e+02, 7.213350e+01));
    solver->addTriple(34, 40, std::complex<double>(-2.360530e+02, 7.981610e+01));
    solver->addTriple(34, 46, std::complex<double>(-1.650030e+02, 1.964560e+02));
    solver->addTriple(34, 47, std::complex<double>(-1.667290e+02, 1.954010e+02));
    solver->addTriple(34, 52, std::complex<double>(-2.721860e+01, 8.085820e+01));
    solver->addTriple(34, 53, std::complex<double>(-2.703470e+01, 8.380430e+01));
    solver->addTriple(34, 54, std::complex<double>(-3.422040e+01, 9.287410e+01));
    solver->addTriple(35, 9, std::complex<double>(1.189020e+02, 3.427130e+02));
    solver->addTriple(35, 10, std::complex<double>(-5.044830e+01, -1.267070e+02));
    solver->addTriple(35, 11, std::complex<double>(-2.762590e+01, -9.471880e+01));
    solver->addTriple(35, 32, std::complex<double>(-5.021970e+00, -2.872550e+00));
    solver->addTriple(35, 33, std::complex<double>(2.660560e+00, 1.523280e+00));
    solver->addTriple(35, 34, std::complex<double>(2.361400e+00, 1.349270e+00));
    solver->addTriple(35, 35, std::complex<double>(-2.173140e+03, -2.527830e+03));
    solver->addTriple(35, 36, std::complex<double>(4.027010e+02, 6.919600e+02));
    solver->addTriple(35, 37, std::complex<double>(4.447880e+02, 6.431960e+02));
    solver->addTriple(35, 41, std::complex<double>(1.071420e+03, 8.065610e+02));
    solver->addTriple(35, 42, std::complex<double>(-2.513560e+02, -3.118380e+02));
    solver->addTriple(35, 43, std::complex<double>(-1.119060e+02, -2.552740e+02));
    solver->addTriple(35, 48, std::complex<double>(7.450130e+02, 6.931330e+02));
    solver->addTriple(35, 49, std::complex<double>(-2.500040e+02, -1.037650e+02));
    solver->addTriple(35, 55, std::complex<double>(2.378040e+02, 6.854270e+02));
    solver->addTriple(35, 56, std::complex<double>(-1.008970e+02, -2.534140e+02));
    solver->addTriple(35, 57, std::complex<double>(-5.525170e+01, -1.894380e+02));
    solver->addTriple(36, 9, std::complex<double>(1.349560e+02, 1.966410e+01));
    solver->addTriple(36, 10, std::complex<double>(-3.333030e+02, -7.221100e+01));
    solver->addTriple(36, 11, std::complex<double>(6.920640e+01, 2.481280e+01));
    solver->addTriple(36, 32, std::complex<double>(2.508430e+00, 1.433330e+00));
    solver->addTriple(36, 33, std::complex<double>(-4.869060e+00, -2.782610e+00));
    solver->addTriple(36, 34, std::complex<double>(2.360630e+00, 1.349290e+00));
    solver->addTriple(36, 35, std::complex<double>(-8.006050e+02, 2.769420e+00));
    solver->addTriple(36, 36, std::complex<double>(2.321740e+03, -3.926930e+02));
    solver->addTriple(36, 37, std::complex<double>(-6.033570e+02, -1.267680e+01));
    solver->addTriple(36, 41, std::complex<double>(3.957380e+02, -6.176160e+01));
    solver->addTriple(36, 42, std::complex<double>(-1.321830e+03, 6.093260e+02));
    solver->addTriple(36, 43, std::complex<double>(3.957380e+02, -6.176160e+01));
    solver->addTriple(36, 55, std::complex<double>(2.699120e+02, 3.932820e+01));
    solver->addTriple(36, 56, std::complex<double>(-6.666070e+02, -1.444220e+02));
    solver->addTriple(36, 57, std::complex<double>(1.384130e+02, 4.962560e+01));
    solver->addTriple(37, 9, std::complex<double>(-1.360930e+01, -4.042910e+01));
    solver->addTriple(37, 10, std::complex<double>(-1.351740e+01, -4.190220e+01));
    solver->addTriple(37, 11, std::complex<double>(-1.711020e+01, -4.643700e+01));
    solver->addTriple(37, 35, std::complex<double>(4.418840e+02, 3.975590e+02));
    solver->addTriple(37, 36, std::complex<double>(2.691840e+02, 1.978400e+02));
    solver->addTriple(37, 37, std::complex<double>(4.541120e+02, 4.145280e+02));
    solver->addTriple(37, 41, std::complex<double>(-2.360530e+02, -7.981610e+01));
    solver->addTriple(37, 42, std::complex<double>(-2.286320e+02, -7.213350e+01));
    solver->addTriple(37, 43, std::complex<double>(-2.360530e+02, -7.981610e+01));
    solver->addTriple(37, 48, std::complex<double>(-1.650030e+02, -1.964560e+02));
    solver->addTriple(37, 49, std::complex<double>(-1.667290e+02, -1.954010e+02));
    solver->addTriple(37, 55, std::complex<double>(-2.721860e+01, -8.085820e+01));
    solver->addTriple(37, 56, std::complex<double>(-2.703470e+01, -8.380430e+01));
    solver->addTriple(37, 57, std::complex<double>(-3.422040e+01, -9.287410e+01));
    solver->addTriple(38, 35, std::complex<double>(-3.571400e+02, -2.688540e+02));
    solver->addTriple(38, 36, std::complex<double>(8.378540e+01, 1.039460e+02));
    solver->addTriple(38, 37, std::complex<double>(3.730210e+01, 8.509150e+01));
    solver->addTriple(38, 38, std::complex<double>(0, -1.200320e+00));
    solver->addTriple(38, 41, std::complex<double>(3.571400e+02, 2.680530e+02));
    solver->addTriple(38, 42, std::complex<double>(-8.378540e+01, -1.035460e+02));
    solver->addTriple(38, 43, std::complex<double>(-3.730210e+01, -8.469150e+01));
    solver->addTriple(39, 35, std::complex<double>(4.812730e+01, -1.245330e+02));
    solver->addTriple(39, 36, std::complex<double>(-4.440820e+01, 4.831340e+02));
    solver->addTriple(39, 37, std::complex<double>(4.812730e+01, -1.245330e+02));
    solver->addTriple(39, 39, std::complex<double>(1.039510e+00, 6.001590e-01));
    solver->addTriple(39, 41, std::complex<double>(-4.778090e+01, 1.243330e+02));
    solver->addTriple(39, 42, std::complex<double>(4.371510e+01, -4.827340e+02));
    solver->addTriple(39, 43, std::complex<double>(-4.778090e+01, 1.243330e+02));
    solver->addTriple(40, 35, std::complex<double>(-9.234240e+01, -1.024120e+01));
    solver->addTriple(40, 36, std::complex<double>(-1.319130e+02, 2.058720e+01));
    solver->addTriple(40, 37, std::complex<double>(4.114040e+02, -1.748650e+02));
    solver->addTriple(40, 40, std::complex<double>(-1.039510e+00, 6.001590e-01));
    solver->addTriple(40, 41, std::complex<double>(9.199600e+01, 1.004120e+01));
    solver->addTriple(40, 42, std::complex<double>(1.315660e+02, -2.078720e+01));
    solver->addTriple(40, 43, std::complex<double>(-4.107110e+02, 1.752660e+02));
    solver->addTriple(41, 32, std::complex<double>(-3.571400e+02, 2.688540e+02));
    solver->addTriple(41, 33, std::complex<double>(8.378540e+01, -1.039460e+02));
    solver->addTriple(41, 34, std::complex<double>(3.730210e+01, -8.509150e+01));
    solver->addTriple(41, 38, std::complex<double>(3.571400e+02, -2.680530e+02));
    solver->addTriple(41, 39, std::complex<double>(-8.378540e+01, 1.035460e+02));
    solver->addTriple(41, 40, std::complex<double>(-3.730210e+01, 8.469150e+01));
    solver->addTriple(41, 41, std::complex<double>(0, 1.200320e+00));
    solver->addTriple(42, 32, std::complex<double>(4.812730e+01, 1.245330e+02));
    solver->addTriple(42, 33, std::complex<double>(-4.440820e+01, -4.831340e+02));
    solver->addTriple(42, 34, std::complex<double>(4.812730e+01, 1.245330e+02));
    solver->addTriple(42, 38, std::complex<double>(-4.778090e+01, -1.243330e+02));
    solver->addTriple(42, 39, std::complex<double>(4.371510e+01, 4.827340e+02));
    solver->addTriple(42, 40, std::complex<double>(-4.778090e+01, -1.243330e+02));
    solver->addTriple(42, 42, std::complex<double>(1.039510e+00, -6.001590e-01));
    solver->addTriple(43, 32, std::complex<double>(-9.234240e+01, 1.024120e+01));
    solver->addTriple(43, 33, std::complex<double>(-1.319130e+02, -2.058720e+01));
    solver->addTriple(43, 34, std::complex<double>(4.114040e+02, 1.748650e+02));
    solver->addTriple(43, 38, std::complex<double>(9.199600e+01, -1.004120e+01));
    solver->addTriple(43, 39, std::complex<double>(1.315660e+02, 2.078720e+01));
    solver->addTriple(43, 40, std::complex<double>(-4.107110e+02, -1.752660e+02));
    solver->addTriple(43, 43, std::complex<double>(-1.039510e+00, -6.001590e-01));
    solver->addTriple(44, 44, std::complex<double>(-5.196230e-01, 3.000040e-01));
    solver->addTriple(44, 45, std::complex<double>(-3.109120e+02, 8.147000e+01));
    solver->addTriple(44, 49, std::complex<double>(3.114320e+02, -8.117000e+01));
    solver->addTriple(45, 44, std::complex<double>(-3.109120e+02, -8.147000e+01));
    solver->addTriple(45, 45, std::complex<double>(-5.196230e-01, -3.000040e-01));
    solver->addTriple(45, 47, std::complex<double>(3.114320e+02, 8.117000e+01));
    solver->addTriple(46, 32, std::complex<double>(-2.483380e+02, 2.310440e+02));
    solver->addTriple(46, 34, std::complex<double>(8.333450e+01, -3.458850e+01));
    solver->addTriple(46, 46, std::complex<double>(3.968570e+02, -2.877300e+02));
    solver->addTriple(46, 47, std::complex<double>(-8.333450e+01, 3.458840e+01));
    solver->addTriple(46, 50, std::complex<double>(-1.485200e+02, 5.668640e+01));
    solver->addTriple(47, 32, std::complex<double>(8.333450e+01, -3.458850e+01));
    solver->addTriple(47, 34, std::complex<double>(-2.500640e+02, 2.299890e+02));
    solver->addTriple(47, 44, std::complex<double>(-2.260110e+02, 2.291230e+02));
    solver->addTriple(47, 46, std::complex<double>(-8.333450e+01, 3.458840e+01));
    solver->addTriple(47, 47, std::complex<double>(4.760750e+02, -4.591120e+02));
    solver->addTriple(48, 35, std::complex<double>(-2.483380e+02, -2.310440e+02));
    solver->addTriple(48, 37, std::complex<double>(8.333450e+01, 3.458850e+01));
    solver->addTriple(48, 48, std::complex<double>(3.968570e+02, 2.877300e+02));
    solver->addTriple(48, 49, std::complex<double>(-8.333450e+01, -3.458840e+01));
    solver->addTriple(48, 51, std::complex<double>(-1.485200e+02, -5.668640e+01));
    solver->addTriple(49, 35, std::complex<double>(8.333450e+01, 3.458850e+01));
    solver->addTriple(49, 37, std::complex<double>(-2.500640e+02, -2.299890e+02));
    solver->addTriple(49, 45, std::complex<double>(-2.260110e+02, -2.291230e+02));
    solver->addTriple(49, 48, std::complex<double>(-8.333450e+01, -3.458840e+01));
    solver->addTriple(49, 49, std::complex<double>(4.760750e+02, 4.591120e+02));
    solver->addTriple(50, 48, std::complex<double>(-1.485200e+02, -5.668640e+01));
    solver->addTriple(50, 50, std::complex<double>(0, -4.666810e-04));
    solver->addTriple(50, 51, std::complex<double>(1.485200e+02, 5.668600e+01));
    solver->addTriple(51, 46, std::complex<double>(-1.485200e+02, 5.668640e+01));
    solver->addTriple(51, 50, std::complex<double>(1.485200e+02, -5.668600e+01));
    solver->addTriple(51, 51, std::complex<double>(0, 4.666810e-04));
    solver->addTriple(52, 32, std::complex<double>(-7.926810e+01, 2.284760e+02));
    solver->addTriple(52, 33, std::complex<double>(3.363220e+01, -8.447150e+01));
    solver->addTriple(52, 34, std::complex<double>(1.841720e+01, -6.314580e+01));
    solver->addTriple(52, 52, std::complex<double>(7.926810e+01, -2.284750e+02));
    solver->addTriple(52, 53, std::complex<double>(-3.363220e+01, 8.447150e+01));
    solver->addTriple(52, 54, std::complex<double>(-1.841720e+01, 6.314580e+01));
    solver->addTriple(53, 32, std::complex<double>(3.363220e+01, -8.447150e+01));
    solver->addTriple(53, 33, std::complex<double>(-6.941010e+01, 2.165030e+02));
    solver->addTriple(53, 34, std::complex<double>(8.743120e+00, -4.822730e+01));
    solver->addTriple(53, 52, std::complex<double>(-3.363220e+01, 8.447150e+01));
    solver->addTriple(53, 53, std::complex<double>(6.941010e+01, -2.165030e+02));
    solver->addTriple(53, 54, std::complex<double>(-8.743120e+00, 4.822730e+01));
    solver->addTriple(54, 32, std::complex<double>(1.841720e+01, -6.314580e+01));
    solver->addTriple(54, 33, std::complex<double>(8.743120e+00, -4.822730e+01));
    solver->addTriple(54, 34, std::complex<double>(-6.138080e+01, 2.042470e+02));
    solver->addTriple(54, 52, std::complex<double>(-1.841720e+01, 6.314580e+01));
    solver->addTriple(54, 53, std::complex<double>(-8.743120e+00, 4.822730e+01));
    solver->addTriple(54, 54, std::complex<double>(6.138080e+01, -2.042470e+02));
    solver->addTriple(55, 35, std::complex<double>(-7.926810e+01, -2.284760e+02));
    solver->addTriple(55, 36, std::complex<double>(3.363220e+01, 8.447150e+01));
    solver->addTriple(55, 37, std::complex<double>(1.841720e+01, 6.314580e+01));
    solver->addTriple(55, 55, std::complex<double>(7.926810e+01, 2.284750e+02));
    solver->addTriple(55, 56, std::complex<double>(-3.363220e+01, -8.447150e+01));
    solver->addTriple(55, 57, std::complex<double>(-1.841720e+01, -6.314580e+01));
    solver->addTriple(56, 35, std::complex<double>(3.363220e+01, 8.447150e+01));
    solver->addTriple(56, 36, std::complex<double>(-6.941010e+01, -2.165030e+02));
    solver->addTriple(56, 37, std::complex<double>(8.743120e+00, 4.822730e+01));
    solver->addTriple(56, 55, std::complex<double>(-3.363220e+01, -8.447150e+01));
    solver->addTriple(56, 56, std::complex<double>(6.941010e+01, 2.165030e+02));
    solver->addTriple(56, 57, std::complex<double>(-8.743120e+00, -4.822730e+01));
    solver->addTriple(57, 35, std::complex<double>(1.841720e+01, 6.314580e+01));
    solver->addTriple(57, 36, std::complex<double>(8.743120e+00, 4.822730e+01));
    solver->addTriple(57, 37, std::complex<double>(-6.138080e+01, -2.042470e+02));
    solver->addTriple(57, 55, std::complex<double>(-1.841720e+01, -6.314580e+01));
    solver->addTriple(57, 56, std::complex<double>(-8.743120e+00, -4.822730e+01));
    solver->addTriple(57, 57, std::complex<double>(6.138080e+01, 2.042470e+02));
    solver->addTriple(58, 0, std::complex<double>(1.940650e+00, -1.916900e-01));
    solver->addTriple(58, 3, std::complex<double>(1.940650e+00, 1.916900e-01));
    solver->addTriple(58, 58, std::complex<double>(-9.465990e-01, 1.904990e-01));
    solver->addTriple(58, 61, std::complex<double>(-9.465990e-01, -1.904990e-01));
    solver->addTriple(59, 1, std::complex<double>(-8.043150e-01, 1.776490e+00));
    solver->addTriple(59, 4, std::complex<double>(-8.043150e-01, -1.776490e+00));
    solver->addTriple(59, 59, std::complex<double>(-9.465990e-01, 1.904990e-01));
    solver->addTriple(59, 62, std::complex<double>(-9.465990e-01, -1.904990e-01));
    solver->addTriple(60, 2, std::complex<double>(-1.136330e+00, -1.584800e+00));
    solver->addTriple(60, 5, std::complex<double>(-1.136330e+00, 1.584800e+00));
    solver->addTriple(60, 60, std::complex<double>(-9.465990e-01, 1.904990e-01));
    solver->addTriple(60, 63, std::complex<double>(-9.465990e-01, -1.904990e-01));
    solver->addTriple(61, 58, std::complex<double>(1, 0));
    solver->addTriple(61, 61, std::complex<double>(-1, 0));
    solver->addTriple(62, 59, std::complex<double>(1, 0));
    solver->addTriple(62, 62, std::complex<double>(-1, 0));
    solver->addTriple(63, 60, std::complex<double>(1, 0));
    solver->addTriple(63, 63, std::complex<double>(-1, 0));
}

template<class SOLVER>
void loadRHS(SOLVER solver)
{
    solver->setRHS(0, std::complex<double>(-9.616220e-02, -7.693350e-01));
    solver->setRHS(1, std::complex<double>(7.143260e-01, 3.014040e-01));
    solver->setRHS(2, std::complex<double>(-6.181690e-01, 4.679360e-01));
    solver->setRHS(3, std::complex<double>(-9.616220e-02, 7.693350e-01));
    solver->setRHS(4, std::complex<double>(7.143260e-01, -3.014040e-01));
    solver->setRHS(5, std::complex<double>(-6.181690e-01, -4.679360e-01));
    solver->setRHS(6, std::complex<double>(5.098500e-02, -2.976890e-02));
    solver->setRHS(7, std::complex<double>(1.980270e-01, -1.137710e-01));
    solver->setRHS(8, std::complex<double>(3.509880e-01, -2.037880e-01));
    solver->setRHS(9, std::complex<double>(5.098500e-02, 2.976890e-02));
    solver->setRHS(10, std::complex<double>(1.980270e-01, 1.137710e-01));
    solver->setRHS(11, std::complex<double>(3.509880e-01, 2.037880e-01));
    solver->setRHS(12, std::complex<double>(1.728060e-06, -2.322200e-05));
    solver->setRHS(13, std::complex<double>(1.657180e-05, 1.095380e-05));
    solver->setRHS(14, std::complex<double>(-1.726750e-05, 1.335090e-05));
    solver->setRHS(15, std::complex<double>(1.728060e-06, 2.322200e-05));
    solver->setRHS(16, std::complex<double>(1.657180e-05, -1.095380e-05));
    solver->setRHS(17, std::complex<double>(-1.726750e-05, -1.335090e-05));
    solver->setRHS(18, std::complex<double>(9.600000e-01, -6.600000e-01));
    solver->setRHS(19, std::complex<double>(7.200000e-01, -5.400000e-01));
    solver->setRHS(20, std::complex<double>(7.200000e-01, -5.400000e-01));
    solver->setRHS(21, std::complex<double>(9.600000e-01, 6.600000e-01));
    solver->setRHS(22, std::complex<double>(7.200000e-01, 5.400000e-01));
    solver->setRHS(23, std::complex<double>(7.200000e-01, 5.400000e-01));
    solver->setRHS(24, std::complex<double>(-2.527690e-05, 9.874420e-06));
    solver->setRHS(25, std::complex<double>(0, 0));
    solver->setRHS(26, std::complex<double>(-2.527690e-05, -9.874420e-06));
    solver->setRHS(27, std::complex<double>(0, 0));
    solver->setRHS(28, std::complex<double>(-1.379990e+00, 7.919840e-01));
    solver->setRHS(29, std::complex<double>(7.476510e-08, 7.448980e-06));
    solver->setRHS(30, std::complex<double>(-1.379990e+00, -7.919840e-01));
    solver->setRHS(31, std::complex<double>(7.476510e-08, -7.448980e-06));
    solver->setRHS(32, std::complex<double>(-7.382280e+00, -4.569360e+00));
    solver->setRHS(33, std::complex<double>(1.830820e-01, 8.411730e+00));
    solver->setRHS(34, std::complex<double>(-1.355960e-05, 3.917360e-06));
    solver->setRHS(35, std::complex<double>(-7.382280e+00, 4.569360e+00));
    solver->setRHS(36, std::complex<double>(1.830820e-01, -8.411730e+00));
    solver->setRHS(37, std::complex<double>(-1.355960e-05, -3.917360e-06));
    solver->setRHS(38, std::complex<double>(2.910000e+00, 6.031760e-02));
    solver->setRHS(39, std::complex<double>(4.080000e-01, 8.403180e-01));
    solver->setRHS(40, std::complex<double>(1.740000e+00, -7.168240e-02));
    solver->setRHS(41, std::complex<double>(2.910000e+00, -6.031760e-02));
    solver->setRHS(42, std::complex<double>(4.080000e-01, -8.403180e-01));
    solver->setRHS(43, std::complex<double>(1.740000e+00, 7.168240e-02));
    solver->setRHS(44, std::complex<double>(1.020000e+00, 1.200090e-01));
    solver->setRHS(45, std::complex<double>(1.020000e+00, -1.200090e-01));
    solver->setRHS(46, std::complex<double>(1.532600e-06, -4.767410e-04));
    solver->setRHS(47, std::complex<double>(-1.571770e-05, 1.084430e-05));
    solver->setRHS(48, std::complex<double>(1.532600e-06, 4.767410e-04));
    solver->setRHS(49, std::complex<double>(-1.571770e-05, -1.084430e-05));
    solver->setRHS(50, std::complex<double>(7.680000e-01, -5.155330e-01));
    solver->setRHS(51, std::complex<double>(7.680000e-01, 5.155330e-01));
    solver->setRHS(52, std::complex<double>(-4.179920e-06, -5.196560e-05));
    solver->setRHS(53, std::complex<double>(3.804340e-05, 3.018520e-05));
    solver->setRHS(54, std::complex<double>(-3.622050e-05, 2.430620e-05));
    solver->setRHS(55, std::complex<double>(-4.179920e-06, 5.196560e-05));
    solver->setRHS(56, std::complex<double>(3.804340e-05, -3.018520e-05));
    solver->setRHS(57, std::complex<double>(-3.622050e-05, -2.430620e-05));
    solver->setRHS(58, std::complex<double>(-4.395140e-02, 0));
    solver->setRHS(59, std::complex<double>(-4.395140e-02, 0));
    solver->setRHS(60, std::complex<double>(-4.395140e-02, 0));
    solver->setRHS(61, std::complex<double>(0, 0));
    solver->setRHS(62, std::complex<double>(0, 0));
    solver->setRHS(63, std::complex<double>(0, 0));
}

void printMatlabRes(const char* filename)
{
    std::ofstream f(filename);

    f << "X(0) = (-2.312272e-02,5.948967e-02)" << std::endl;
    f << "X(1) = (-4.943361e-03,2.795692e-03)" << std::endl;
    f << "X(2) = (-5.141878e-02,-9.516303e-03)" << std::endl;
    f << "X(3) = (-2.312272e-02,-5.948967e-02)" << std::endl;
    f << "X(4) = (-4.943361e-03,-2.795692e-03)" << std::endl;
    f << "X(5) = (-5.141878e-02,9.516303e-03)" << std::endl;
    f << "X(6) = (-3.860961e-02,1.250674e-01)" << std::endl;
    f << "X(7) = (-9.315333e-03,3.557058e-02)" << std::endl;
    f << "X(8) = (-8.315096e-02,-5.353875e-02)" << std::endl;
    f << "X(9) = (-3.860961e-02,-1.250674e-01)" << std::endl;
    f << "X(10) = (-9.315333e-03,-3.557058e-02)" << std::endl;
    f << "X(11) = (-8.315096e-02,5.353875e-02)" << std::endl;
    f << "X(12) = (-3.852230e-02,1.282825e-01)" << std::endl;
    f << "X(13) = (-7.235409e-03,3.477096e-02)" << std::endl;
    f << "X(14) = (-8.551659e-02,-5.422477e-02)" << std::endl;
    f << "X(15) = (-3.852230e-02,-1.282825e-01)" << std::endl;
    f << "X(16) = (-7.235409e-03,-3.477096e-02)" << std::endl;
    f << "X(17) = (-8.551659e-02,5.422477e-02)" << std::endl;
    f << "X(18) = (-4.116232e-02,1.547425e-01)" << std::endl;
    f << "X(19) = (1.181953e-02,2.709511e-02)" << std::endl;
    f << "X(20) = (-1.016915e-01,-6.688891e-02)" << std::endl;
    f << "X(21) = (-4.116232e-02,-1.547425e-01)" << std::endl;
    f << "X(22) = (1.181953e-02,-2.709511e-02)" << std::endl;
    f << "X(23) = (-1.016915e-01,6.688891e-02)" << std::endl;
    f << "X(24) = (-5.974854e-03,3.423409e-02)" << std::endl;
    f << "X(25) = (-8.650585e-02,-5.222706e-02)" << std::endl;
    f << "X(26) = (-5.974854e-03,-3.423409e-02)" << std::endl;
    f << "X(27) = (-8.650585e-02,5.222706e-02)" << std::endl;
    f << "X(28) = (-3.970616e-03,3.343213e-02)" << std::endl;
    f << "X(29) = (-8.851872e-02,-5.143975e-02)" << std::endl;
    f << "X(30) = (-3.970616e-03,-3.343213e-02)" << std::endl;
    f << "X(31) = (-8.851872e-02,5.143975e-02)" << std::endl;
    f << "X(32) = (-5.264539e-02,1.821936e-01)" << std::endl;
    f << "X(33) = (-2.726565e-02,6.685437e-02)" << std::endl;
    f << "X(34) = (-1.010329e-01,-8.910862e-02)" << std::endl;
    f << "X(35) = (-5.264539e-02,-1.821936e-01)" << std::endl;
    f << "X(36) = (-2.726565e-02,-6.685437e-02)" << std::endl;
    f << "X(37) = (-1.010329e-01,8.910862e-02)" << std::endl;
    f << "X(38) = (-4.796445e-02,1.866066e-01)" << std::endl;
    f << "X(39) = (-2.771177e-02,6.707055e-02)" << std::endl;
    f << "X(40) = (-1.035666e-01,-8.765248e-02)" << std::endl;
    f << "X(41) = (-4.796445e-02,-1.866066e-01)" << std::endl;
    f << "X(42) = (-2.771177e-02,-6.707055e-02)" << std::endl;
    f << "X(43) = (-1.035666e-01,8.765248e-02)" << std::endl;
    f << "X(44) = (-1.071459e-01,-8.618632e-02)" << std::endl;
    f << "X(45) = (-1.071459e-01,8.618632e-02)" << std::endl;
    f << "X(46) = (-5.297081e-02,1.850173e-01)" << std::endl;
    f << "X(47) = (-1.041532e-01,-8.727040e-02)" << std::endl;
    f << "X(48) = (-5.297081e-02,-1.850173e-01)" << std::endl;
    f << "X(49) = (-1.041532e-01,8.727040e-02)" << std::endl;
    f << "X(50) = (-4.961385e-02,1.897700e-01)" << std::endl;
    f << "X(51) = (-4.961385e-02,-1.897700e-01)" << std::endl;
    f << "X(52) = (-5.264519e-02,1.821947e-01)" << std::endl;
    f << "X(53) = (-2.726568e-02,6.685508e-02)" << std::endl;
    f << "X(54) = (-1.010330e-01,-8.910826e-02)" << std::endl;
    f << "X(55) = (-5.264519e-02,-1.821947e-01)" << std::endl;
    f << "X(56) = (-2.726568e-02,-6.685508e-02)" << std::endl;
    f << "X(57) = (-1.010330e-01,8.910826e-02)" << std::endl;
    f << "X(58) = (-1.214224e-02,7.150385e-16)" << std::endl;
    f << "X(59) = (2.216905e-02,9.436098e-17)" << std::endl;
    f << "X(60) = (6.900806e-02,1.960836e-16)" << std::endl;
    f << "X(61) = (-1.214224e-02,7.150175e-16)" << std::endl;
    f << "X(62) = (2.216905e-02,9.437540e-17)" << std::endl;
    f << "X(63) = (6.900806e-02,1.962616e-16)" << std::endl;
}

int testLUSolver(bool onlySolve)
{

    auto solver = sparse::createCmplxSolver(64,
        448,
        sparse::Symmetry::NonSymmetric,
        sparse::SolverType::LU,
        sparse::Pivoting::MarkowitzMultiPass,
        sparse::Ordering::Own);
    
    //N, realNZ, nonSymetric, LU, DiagonalMultiPass, OwnOrdering)
    auto resPath = getResultPath(natIDSDKPath);
    
    if (onlySolve)
    {
        std::cout << "Only solve:";
        loadH(solver);

        loadRHS(solver);
        
        auto resH = resPath / "H.m";
        std::ofstream hOutM;
        hOutM.open(resH);
        
        solver->serialize("H", hOutM, sparse::Format::Matlab);
        auto resRHS = resPath / "b.m";
        std::ofstream hOutB;
        hOutB.open(resRHS);
        
        solver->showRHS("b", hOutB, sparse::Format::Matlab);

        
        if (!solver->solve())
        {
            std::cout << "Cannot solve!" << std::endl;
            return 0;
        }

        auto resX = resPath / "x_solve_only.txt";
        std::ofstream hOut;
        hOut.open(resX);
        solver->showResults("x", hOut, sparse::Format::Matlab);
        
//        for (int i = 0; i < 64; ++i)
//        {
//            auto x = solver->x(i);
//            hOut << "X(" << i << ") = " << x << std::endl;
//        }
        std::cout << " Done...\n";
    }
    else
    {
        std::cout << "Factorize + solve:";

        loadH(solver);

        if (!solver->factorize())
        {
            std::cout << "Cannot factorize!" << std::endl;
            return 0;
        }

        loadRHS(solver);

        if (!solver->solve())
        {
            std::cout << "Cannot factorize!" << std::endl;
            return 0;
        }

        auto resX = resPath / "Res_Fact_Solve.tx.txt";
        std::ofstream hOut;
        hOut.open(resX);

        solver->showResults("x", hOut, sparse::Format::Matlab);
        
//        for (int i = 0; i < 64; ++i)
//        {
//            auto x = solver->x(i);
//            hOut << "X(" << i << ") = " << x << std::endl;
//        }
        std::cout << " Done...\n";
    }

    solver->release();


    return 1;
}

void testLUSolveExt()
{
//    clear all
//    Y=sparse(4,4);
//    Y(1,1)=1+0i;
//    Y(2,1)=-1+33i;
//    Y(2,2)=2-66i;
//    Y(2,3)=-1+33i;
//    Y(3,2)=-1+33i;
//    Y(3,3)=2-66i;
//    Y(3,4)=-1+33i;
//    Y(4,3)=-1+33i;
//    Y(4,4)=1-33i;
//
//    RHS=sparse(4,1);
//    RHS(1,1) = 1;
//    RHS(2,1) = -2+0.5i;
//    RHS(3,1) = 0;
//    RHS(4,1) = -1.5+1i;
//
//    V=Y\RHS
//    Solution:
//    V(1,1)      1.0000 + 0.0000i
//    V(2,1)      0.9514 - 0.1046i
//    V(3,1)      0.9197 - 0.1491i
//    V(4,1)      0.8881 - 0.1936i
    
    int N = 4;
    int NZ = 9;
    auto solver = sparse::createCmplxSolver(N, NZ,
        sparse::Symmetry::NonSymmetric,
        sparse::SolverType::LU,
        sparse::Pivoting::DiagonalMultiPass,
        sparse::Ordering::Own);
    td::cmplx val =1;
    
    solver->addTriple(0, 0, val);
    solver->addTriple(1, 0, td::cmplx(-1, 33));
    solver->addTriple(1, 1, td::cmplx(2, -66));
    solver->addTriple(1, 2, td::cmplx(-1, 33));
    solver->addTriple(2, 1, td::cmplx(-1, 33));
    solver->addTriple(2, 2, td::cmplx(2, -66));
    solver->addTriple(3, 2, td::cmplx(-1, 33));
    solver->addTriple(2, 3, td::cmplx(-1, 33));
    solver->addTriple(3, 3, td::cmplx(1, -33));
    
    if (!solver->factorize())
    {
        std::cout << "ERROR! Cannot factorize matrix" << td::endl;
        return;
    }
    
    td::cmplx rhs[4];
    rhs[0] = td::cmplx(1,0);
    rhs[1] = td::cmplx(-2,0.5);
    rhs[2] = td::cmplx(0,0);
    rhs[3] = td::cmplx(-1.5,1);
    td::cmplx x[4];
    
    if (!solver->solveExt(rhs, x))
    {
        std::cout << "ERROR! Cannot solve the problem" << td::endl;
        return;
    }

    for (size_t i=0; i< 4; ++i)
    {
        std::cout << "x(" << i+1 << ")=" << x[i] << td::endl;
    }    
}

void testLUSolveWithRHSVector()
{
//    clear all
//    Y=sparse(4,4);
//    Y(1,1)=1+0i;
//    Y(2,1)=-1+33i;
//    Y(2,2)=2-66i;
//    Y(2,3)=-1+33i;
//    Y(3,2)=-1+33i;
//    Y(3,3)=2-66i;
//    Y(3,4)=-1+33i;
//    Y(4,3)=-1+33i;
//    Y(4,4)=1-33i;
//
//    RHS=sparse(4,1);
//    RHS(1,1) = 1;
//    RHS(2,1) = -2+0.5i;
//    RHS(3,1) = 0;
//    RHS(4,1) = -1.5+1i;
//
//    V=Y\RHS
//    Rjesenje:
//    V(1,1)      1.0000 + 0.0000i
//    V(2,1)      0.9514 - 0.1046i
//    V(3,1)      0.9197 - 0.1491i
//    V(4,1)      0.8881 - 0.1936i
    
    int N = 4;
    int NZ = 9;
    auto solver = sparse::createCmplxSolver(N, NZ,
        sparse::Symmetry::NonSymmetric,
        sparse::SolverType::LU,
        sparse::Pivoting::DiagonalMultiPass,
        sparse::Ordering::Own);
    td::cmplx val =1;
    
    solver->addTriple(0, 0, val);
    solver->addTriple(1, 0, td::cmplx(-1, 33));
    solver->addTriple(1, 1, td::cmplx(2, -66));
    solver->addTriple(1, 2, td::cmplx(-1, 33));
    solver->addTriple(2, 1, td::cmplx(-1, 33));
    solver->addTriple(2, 2, td::cmplx(2, -66));
    solver->addTriple(3, 2, td::cmplx(-1, 33));
    solver->addTriple(2, 3, td::cmplx(-1, 33));
    solver->addTriple(3, 3, td::cmplx(1, -33));
    
    if (!solver->factorize())
    {
        std::cout << "ERROR! Cannot factorize matrix" << td::endl;
        solver->release();
        return;
    }
    
    cnt::SafeFullVector<td::cmplx> rhs;
    rhs.reserve(4);
    rhs[0] = td::cmplx(1,0);
    rhs[1] = td::cmplx(-2,0.5);
    rhs[2] = td::cmplx(0,0);
    rhs[3] = td::cmplx(-1.5,1);
    
    solver->setRHS(rhs);
    
    if (!solver->solve())
    {
        std::cout << "ERROR! Cannot solve the problem" << td::endl;
        solver->release();
        return;
    }

    for (int i=0; i< 4; ++i)
    {
        std::cout << "x(" << i+1 << ")=" << solver->x(i) << td::endl;
    }
    solver->release();
}

void testIMatrix()
{
    sparse::IDblMatrix* pMatrix = sparse::createDblMatrix(5, 4, 10);
    
    pMatrix->addTriple(0, 0, 1);
    pMatrix->addTriple(1, 1, 11);
    pMatrix->addTriple(2, 2, 22);
    pMatrix->addTriple(3, 3, 33);
    
    pMatrix->addTriple(1, 0, 10);
    
    pMatrix->addTriple(1, 2, 12);
    pMatrix->addTriple(2, 1, 21);
    
    pMatrix->addTriple(2, 3, 23);
    pMatrix->addTriple(3, 1, 31);
    
    pMatrix->addTriple(4, 1, 41);
    pMatrix->addTriple(4, 3, 43);
    pMatrix->addTriple(4, 2, 42);
    
    std::ofstream f("/Volumes/RAMDisk/Out/testImatrix.txt");
    pMatrix->serialize("H", f, sparse::Format::Cpp);
    auto nNz = pMatrix->getNoOfNonZero();
    nNz +=1;
    pMatrix->release();
}

void testDblGain()
{
    //H=[1 0 0; 1 2 3; 0 2 4; 0 5 0];
    //W=diag([10 20 30 40]);
    
    std::cout << "Test DBL Gain matrix computation" << td::endl;
    
    sparse::IDblMatrix* pMatrix = sparse::createDblMatrix(4, 3, 7);
    
    pMatrix->addTriple(0, 0, 1);
    
    pMatrix->addTriple(1, 0, 1);
    pMatrix->addTriple(1, 1, 2);
    pMatrix->addTriple(1, 2, 3);
    
    pMatrix->addTriple(2, 1, 2);
    pMatrix->addTriple(2, 2, 4);
    
    pMatrix->addTriple(3, 1, 5);
    
    pMatrix->serialize("H", std::cout);
    
    dense::DblDiagMatrix W(4);
    auto w = W.getManipulator();
    w(0)=10;
    w(1)=20;
    w(2)=30;
    w(3)=40;
    
    dense::DblMatrix B(4,1);
    auto b = B.getFirstColumnManipulator();
    b(0)=2;
    b(1)=4;
    b(2)=6;
    b(3)=8;
    
    sparse::DblSolver* pSolver = sparse::createDblSolver(3, 9);
    
    pMatrix->calcGainAndRHS(W, B, pSolver);
    
    pSolver->serialize("G", std::cout);
    pSolver->showRHS("G_rhs", std::cout);
    pSolver->release();
    pMatrix->release();
    
}

void testCmplxGain4x3(sparse::ICmplxMatrix* pMatrix = nullptr)
{
    std::cout << "Test CMPLX Gain matrix computation (4x3)" << td::endl;
    //W=diag([10 20 30 40]);
    bool bRelease = false;
    if (!pMatrix)
    {
        pMatrix = sparse::createCmplxMatrix(4, 3, 7);
        bRelease = true;
    }
    //H=[1+2i 0 0; 1+1i 2+2i 3+3i; 0 2+2i 4+4i; 0 5+5i 0]

    pMatrix->addTriple(0, 0, td::cmplx(1, 2));
    
    pMatrix->addTriple(1, 0, td::cmplx(1,1));
    pMatrix->addTriple(1, 1, td::cmplx(2,-2));
    pMatrix->addTriple(1, 2, td::cmplx(3,3));
    
    pMatrix->addTriple(2, 1, td::cmplx(2,2));
    pMatrix->addTriple(2, 2, td::cmplx(4,4));
    
    pMatrix->addTriple(3, 1, td::cmplx(5,5));
    
    pMatrix->serialize("H", std::cout);
    
    dense::DblDiagMatrix W(4);
    auto w = W.getManipulator();
    w(0)=10;
    w(1)=20;
    w(2)=30;
    w(3)=40;
    
    //b=[2-3i;4-2i;6-5i;8-3i]
    dense::CmplxMatrix B(4,1);
    auto b = B.getFirstColumnManipulator();
    b(0)=td::cmplx(2,-3);
    b(1)=td::cmplx(4,-2);
    b(2)=td::cmplx(6,-5);
    b(3)=td::cmplx(8,-3);
    
    sparse::CmplxSolver* pSolver = sparse::createCmplxSolver(3, 9);
    
    pMatrix->calcGainAndRHS(W, B, pSolver);
    
    pSolver->serialize("G", std::cout);
    pSolver->showRHS("G_rhs", std::cout);
    pSolver->release();
    if (bRelease)
        pMatrix->release();
}

void testCmplxGain7x5(sparse::ICmplxMatrix* pMatrix = nullptr)
{
    std::cout << "Test CMPLX Gain matrix computation (7x5)" << td::endl;
    //W=diag([10 20 30 40]);
    bool bRelease = false;
    if (!pMatrix)
    {
        pMatrix = sparse::createCmplxMatrix(7, 5, 7);
        bRelease = true;
    }
    //H=[1+2i 0 0; 1+1i 2+2i 3+3i; 0 2+2i 4+4i; 0 5+5i 0]

    pMatrix->addTriple1(1, 1, td::cmplx(1, 1));
    
    pMatrix->addTriple1(1, 5, td::cmplx(1, 5));
    
    pMatrix->addTriple1(2, 1, td::cmplx(1,1));
    pMatrix->addTriple1(2, 2, td::cmplx(2,-2));
    pMatrix->addTriple1(2, 4, td::cmplx(3,3));
    
    pMatrix->addTriple1(3, 2, td::cmplx(2,2));
    pMatrix->addTriple1(3, 5, td::cmplx(2,5));
    pMatrix->addTriple1(3, 4, td::cmplx(3,6));
    
    pMatrix->addTriple1(4, 2, td::cmplx(4,2));
    
    pMatrix->addTriple1(5, 3, td::cmplx(5,3));
    pMatrix->addTriple1(5, 5, td::cmplx(5,5));
    
    pMatrix->addTriple1(6, 3, td::cmplx(5,3));
    pMatrix->addTriple1(6, 5, td::cmplx(5,5));
    
    pMatrix->addTriple1(7, 2, td::cmplx(7,2));
    
    pMatrix->serialize("H", std::cout);
    
    dense::DblDiagMatrix W(7);
    auto w = W.getManipulator1();
    w(1)=10;
    w(2)=20;
    w(3)=30;
    w(4)=40;
    w(5)=50;
    w(6)=60;
    w(7)=70;
    
    //b=[2-3i;4-2i;6-5i;8-3i]
    dense::CmplxMatrix B(7,1);
    auto b = B.getFirstColumnManipulator1();
    b(1)=td::cmplx(2,-3);
    b(2)=td::cmplx(4,-2);
    b(3)=td::cmplx(6,-5);
    b(4)=td::cmplx(8,-3);
    b(5)=td::cmplx(5,-3);
    b(6)=td::cmplx(6,-3);
    b(7)=td::cmplx(7,-3);
    
    sparse::CmplxSolver* pSolver = sparse::createCmplxSolver(5, 20);
    
    pMatrix->calcGainAndRHS(W, B, pSolver);
    
    pSolver->serialize("G", std::cout);
    pSolver->showRHS("G_rhs", std::cout);
    pSolver->release();
    
    pMatrix->setDimension(4, 3);
    testCmplxGain4x3(pMatrix);
    
    if (bRelease)
        pMatrix->release();
}

//ovaj ispod je sa biljeznice
void testCmplxGain5x4(sparse::ICmplxMatrix* pMatrix = nullptr)
{
    std::cout << "Test CMPLX Gain matrix computation (5x4)" << td::endl;
    //W=diag([10 20 30 40]);
    bool bRelease = false;
    if (!pMatrix)
    {
        pMatrix = sparse::createCmplxMatrix(5, 4, 9);
        bRelease = true;
    }
    //H=[1+2i 0 0; 1+1i 2+2i 3+3i; 0 2+2i 4+4i; 0 5+5i 0]

    pMatrix->addTriple1(1, 1, td::cmplx(1, 1));
    pMatrix->addTriple1(4, 1, td::cmplx(4, 1));
    pMatrix->addTriple1(5, 1, td::cmplx(5, 1));
    
    pMatrix->addTriple1(2, 2, td::cmplx(2,2));
    pMatrix->addTriple1(4, 2, td::cmplx(4,2));
    
    pMatrix->addTriple1(1, 3, td::cmplx(1,3));
    pMatrix->addTriple1(3, 3, td::cmplx(3,3));
    pMatrix->addTriple1(5, 3, td::cmplx(5,3));
    
    pMatrix->addTriple1(4, 4, td::cmplx(4,4));

    pMatrix->serialize("H", std::cout);
    
    dense::DblDiagMatrix W(5);
    auto w = W.getManipulator1();
    w(1)=10;
    w(2)=20;
    w(3)=30;
    w(4)=40;
    w(5)=50;
    
    //b=[2-3i;4-2i;6-5i;8-3i]
    dense::CmplxMatrix B(5,1);
    auto b = B.getFirstColumnManipulator1();
    b(1)=td::cmplx(2,-1);
    b(2)=td::cmplx(4,-2);
    b(3)=td::cmplx(6,-3);
    b(4)=td::cmplx(8,-4);
    b(5)=td::cmplx(5,-5);
    
    sparse::CmplxSolver* pSolver = sparse::createCmplxSolver(4, 16);
    
    pMatrix->calcGainAndRHS(W, B, pSolver);
    
    pSolver->serialize("G", std::cout);
    pSolver->showRHS("G_rhs", std::cout);
    pSolver->release();
    
    if (bRelease)
        pMatrix->release();
}

void testEqualityConstraint()
{
    sparse::IDblMatrix* pEC = sparse::createDblMatrix(2, 3, 4);
    pEC->addTriple1(1, 1, 0.1);
    pEC->addTriple1(1, 2, 0.2);
    pEC->addTriple1(2, 2, 0.3);
    pEC->addTriple1(2, 3, 0.4);
    
    sparse::DblSolver* pSolver = sparse::createDblSolver(5, 7, sparse::Symmetry::SymmetricIndef, sparse::SolverType::LDLT);
    pSolver->addTriple1(1, 1, 1);
    pSolver->addTriple1(2, 2, 2);
    pSolver->addTriple1(3, 3, 3);
    pSolver->setRHS1(1, 1);
    pSolver->setRHS1(2, 2);
    pSolver->setRHS1(3, 3);
    
    dense::DblMatrix B(2, 1);
    auto b = B.getFirstColumnManipulator1();
    b(1)= 1;
    b(2) = 2;
    
    pEC->addAsEqualityConstraint(3, B, pSolver);
    pEC->serialize("E", std::cout);
    pSolver->serialize("S", std::cout);
}

int main(int argc, const char* argv[])
{
    mu::Application app(argc, argv);

    fo::fs::path homePath = mu::getHomePath();
    natIDSDKPath = homePath / "natID.SDK/bin/TestData/SparseMatrices";
    
    std::cout << "Sparse Test Started" << std::endl;
    
    testLUSolver(true);
    testLUSolver(false);
    
    testEqualityConstraint();
    
    testCmplxGain5x4();
    testCmplxGain4x3();
    testCmplxGain7x5();
    
//    return 0;
    
    testDblGain();
    
//    return 0;
    testIMatrix();
//    return 0;
    testLUSolveWithRHSVector();
    testLUSolveExt();
   
    
    auto inPath = getTestMatricesPath(natIDSDKPath);
    
//    testComplexMatrix(pInPath, "Matrice_1k5_PF_C", sparse::LU, false, false);
//    testComplexMatrix(pInPath, "Matrice_1k5_PF_C", sparse::Pardiso, false, false);
    
//    return 0;
    //sparse::setNoThreads(1);
    //const char* pInRLDLTPath = getRealLDLTPath();
    //testRealMatrix(pInRLDLTPath, "4node", sparse::Pardiso, false);
    //return;
    //testCmplxLDLTDbg();
    //return 0;
    testCmplxCholesky();

    testRealCholesky();
    testCmplxCholesky();
    testRealLDLT();
    testCmplxLDLT();

    
    std::cout << "Hermit5 (Hermitian)" << std::endl;
    testComplexMatrix(inPath, "Hermit5", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "Hermit5", sparse::SolverType::LU, false, true);
//    testComplexMatrix(pInPath, "Hermit5", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "IEEE6SEPF_Hermit (Hermitian)" << std::endl;
    testComplexMatrix(inPath, "IEEE6SEPF_Hermit",  sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "IEEE6SEPF_Hermit", sparse::SolverType::LU, false, true);
//    testComplexMatrix(pInPath, "IEEE6SEPF_Hermit", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "Testing IEEE14_SEPF_Hermit (Hermitian)" << std::endl;
    testComplexMatrix(inPath, "IEEE14_SEPF_Hermit", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "IEEE14_SEPF_Hermit",  sparse::SolverType::LU, false, true);
//    testComplexMatrix(pInPath, "IEEE14_SEPF_Hermit",  sparse::SolverType::Pardiso, false, true);
	
    std::cout << "Testing Hermit_1280b (Hermitian)" << std::endl;
    testComplexMatrix(inPath, "Hermit_1280b", sparse::SolverType::LDLT, false, true);
    testComplexMatrix(inPath, "Hermit_1280b", sparse::SolverType::LU, false, true);
//    testComplexMatrix(pInPath, "Hermit_1280b", sparse::SolverType::Pardiso, false, true);
    
    std::cout << "Testing Matrice_1k5_PF_C (nesimetricna)" << std::endl;
    //testComplexMatrix("Matrice_1k5_PF_C", sparse::LDLT, false);
    testComplexMatrix(inPath, "Matrice_1k5_PF_C", sparse::SolverType::LU, false, false);
//    testComplexMatrix(pInPath, "Matrice_1k5_PF_C", sparse::SolverType::Pardiso, false, false);
//	return 0;


//	return 0;
//    testFromMemorySimIndef();
//	return 0;
	testFromFile();
//	return 0;
	//testMyExample();
	//return 1;
    testFromMemory();
//	sparse::releaseSolverLibraries();
//	return 0;
    testUMFPackExample();
    //return 1;
    
    testSC_PID_Example();

	//sparse::IDblSparseSolver* pSolver = sparse::createDblSparseSolver(4, 10, sparse::LUIDz, sparse::MarkowitzPivotingSinglePass);
    {
        sparse::DblSolverReleaser pSolver(sparse::createDblSolver(4, 7, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzSinglePass));
        
        cout << "Solver created. Ptr=" << pSolver.ptr() << endl;
        
        sparse::DblSolver& s2 = *(pSolver.ptr());
        s2.addTriple(0, 1, 2);
        s2.addTriple(0, 3, 3);
        s2.addTriple(1, 0, 4);
        //s2.addTriple1(2, 2, std::complex<double>(5,0));
        s2.addTriple(1, 2, 6);
        s2.addTriple(2, 1, 7);
        //s2.addTriple1(3, 3, std::complex<double>(8,0));
        s2.addTriple(2, 3, 9);
        //s2.addTriple1(4, 1, std::complex<double>(10,0));
        s2.addTriple(3, 2, 12);
        
        cout << "Factorazing...\n";
        if (!s2.factorize())
        {
            assert(false);
            return 0;
        }
        
        s2.setRHS(0, 16);
        s2.setRHS(1, 22);
        s2.setRHS(2, 50);
        s2.setRHS(3, 36);
        
        cout << "Solving...\n";
        s2.solve();
        
        for (int i=0; i<4;++i)
        {
            std::cout << "x(" << i << ")=" << s2.x(i) << std::endl;
        }
    }
    {
        sparse::DblSolverReleaser pSolver(sparse::createDblSolver(2, 4, sparse::Symmetry::NonSymmetric, SOLVERTYPE, sparse::Pivoting::MarkowitzSinglePass));
        
        cout << "Solver created. Ptr=" << pSolver.ptr() << endl;
        
        sparse::DblSolver& s2 = *(pSolver.ptr());
        s2.addTriple(0, 0, 1);
        s2.addTriple(1, 1, 2);
        
        cout << "Factorazing...\n";
        if (!s2.factorize())
        {
            assert(false);
            return 0;
        }
        
        s2.setRHS(0, 1);
        s2.setRHS(1, 4);
        
        cout << "Solving...\n";
        if (!s2.solve())
        {
            assert(false);
            return 0;
        }
        
        for (int i=0; i<2;++i)
        {
            std::cout << "x(" << i << ")=" << s2.x(i) << std::endl;
        }
    }
	sparse::releaseSolverLibraries();

	return 0;
}

