#pragma once
#include <vector>
#include <fstream>
#include <sparse/ISolver.h>
#include <mu/Timer.h>

template <typename T>
class Tripple
{
public:
	int i;
	int j;
	T val;
};

template <typename T>
bool loadMatrixData(const char* fn, std::vector<Tripple<T>>& matrixData, int& n)
{
	matrixData.clear();

	std::ifstream f(fn);
	if (!f.is_open())
	{
		std::cout << "Cannot open: " << fn << std::endl;
		return false;
	}

	int nz = 0;

	// matrix dimension n and number of nonzeros
	f >> n >> nz;

	if (n <= 0 || nz <= 0)
	{
		std::cout << "Wrong matrix darta in: " << fn << std::endl;
		return false;
	}

	matrixData.reserve(nz);
	for (int i = 0; i < nz; ++i)
	{
		Tripple<T> entry;
		f >> entry.i >> entry.j >> entry.val;
		//input values are 1-based, my solver requires 0-based
		--entry.i;
		--entry.j;
		matrixData.push_back(entry);
	}

	return true;
};

char tmp[1024];
template <class TSOLV, typename T>
void createMatlabMtx(TSOLV& solver, T val, int n)
{
	char tmp[1024];
	char ch = 'r';
	if (typeid(T) != typeid(double))
		ch = 'c';

    SNPRINTF(tmp, 1024, _TRUNCATE, "R:/Matrice/MAT_%d%c.m", n/2, ch);
//    SPRINTF(tmp, "R:/Matrice/MAT_%d%c.m", n/2, ch);
	std::ofstream fm(&tmp[0]);
	solver.serialize("A", fm);
}

template <class TSOLV, typename T>
bool solve(TSOLV& solver, const std::vector<Tripple<T>>& matrixData, const char* solFn)
{
	//populate sparse matrix
	int nz = (int) matrixData.size();
	for (int i = 0; i < nz; ++i)
	{
		solver.addTriple(matrixData[i].i, matrixData[i].j, matrixData[i].val);
	}		

	int n = solver.size();

	{
		T val = 0;
		createMatlabMtx(solver, val, n);
	}
	
	
	//factorize
	mu::Timer<3> timer("Total");
	timer.setName(1, "Factorize");	

	if (!solver.factorize())
	{
		return false;
	}
	timer.pause(1);
	//set RHS
	
	for (int i = 0; i < n; ++i)
	{
		T rhsVal = i + 1;
		solver.setRHS(i, rhsVal);
	}

	timer.setName(2, "TriangSolve");
	//solve triangular
	if (!solver.solve())
		return false;
	timer.pause(2);
	timer.pauseAll();

	//write results
	std::ofstream f(solFn);	
	timer.showAllTimesInMillisec(f);
	solver.showResults("x", f);
	return true;
}


sparse::DblSolver* createSparseSolver(double, int n, int nz)
{
	sparse::DblSolver* pSolver = sparse::createDblSolver(n, nz, sparse::Symmetry::SymmetricPosDef, sparse::SolverType::LU, sparse::Pivoting::No, sparse::Ordering::Own);
	return pSolver;
}

sparse::CmplxSolver* createSparseSolver(std::complex<double>, int n, int nz)
{
	sparse::CmplxSolver* pSolver = sparse::createCmplxSolver(n, nz, sparse::Symmetry::HermitianPosDef, sparse::SolverType::LU, sparse::Pivoting::No, sparse::Ordering::Own);
	return pSolver;
}


template <class TSOLV, typename T>
bool testSolver(TSOLV* pSolv, T, const char* fnInput, const char* fnRes)
{
	int n = 0;
	std::vector<Tripple<T>> matrixData;
	if (!loadMatrixData(fnInput, matrixData, n))
	{
		std::cout << "Test file: " << fnInput << " failed!" << std::endl;
		return false;
	}
		
	
	int nz = (int)matrixData.size();
	T dummy = 0;
	pSolv = createSparseSolver(dummy, n, nz);
	if (!solve(*pSolv, matrixData, fnRes))
	{
		pSolv->release();		
		std::cout << "Test file: " << fnInput << " failed!" << std::endl;		
		return false;
	}
		
	pSolv->release();

	
	std::cout << "Test file: " << fnInput << " OK!" << std::endl;	
	return true;
}

inline void testCholeskyReal()
{
	sparse::DblSolver* pSolver = nullptr;
	double dummy=0;
	testSolver(pSolver, dummy, "R:/Matrice/Real/4node.txt", "R:/Matrice/Real/4node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Real/300node.txt", "R:/Matrice/Real/300node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Real/1888node.txt", "R:/Matrice/Real/1888node_sol.txt");
	//testSolver(pSolver, dummy, "R:/Matrice/Real/9241node.txt", "R:/Matrice/Real/9241node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Real/9241a.txt", "R:/Matrice/Real/9241a_sol.txt");
}

inline void testCholeskyComplex()
{
	sparse::CmplxSolver* pSolver = nullptr;
	std::complex<double> dummy=0;
	testSolver(pSolver, dummy, "R:/Matrice/Cmplx/4node.txt", "R:/Matrice/Cmplx/4node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Cmplx/300node.txt", "R:/Matrice/Cmplx/300node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Cmplx/1888node.txt", "R:/Matrice/Cmplx/1888node_sol.txt");
	testSolver(pSolver, dummy, "R:/Matrice/Cmplx/9241node.txt", "R:/Matrice/Cmplx/9241node_sol.txt");
}

inline void testCholeskyMain()
{
	testCholeskyReal();
	testCholeskyComplex();
}

