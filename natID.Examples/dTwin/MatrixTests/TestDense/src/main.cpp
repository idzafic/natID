// TestISparseSolver.cpp : Defines the entry point for the console application.
//
#include <mu/Application.h>
#include <sparse/Format.h>
#include <dense/Matrix.h>
#include <dense/DiagMatrix.h>


// function to print complex number vector :
template <typename C>
void printcx(const char * text , C a)
{
    auto aa = a. to_vector (); // get elements as real vector
    printf ("\n%s", text ); // print text
    for (int n = 0; n < a. size (); n++)
    { // loop through elements
        printf ("(%.3G ,%.3G) ", aa [2* n], aa [2* n +1]) ;
    }
}

using namespace std;

void testDblSolve2x2(dense::DblMatrix& M, dense::DblMatrix& B)
{
    cout << "Dbl Matrix test 2x2:" << td::endl
         << "-------------------" << td::endl;
    auto m = M.getManipulator();
    
    m(0,0)=1;
    m(0,1)=2;
    m(1,0)=3;
    m(1,1)=4;
    
    cout << "Double matrix M" << td::endl;
    M.show(std::cout, "M");
    
    dense::FirstColumnIO<double> b;
    auto b2 = B.getFirstColumnManipulator();
    b = b2;
    
    b2(0) = 10;
    b2(1) = 20;
    
    //or
    b(0)=1;
    b(1)=2;
    
    bool solveOK = M.solve(B);
    if (solveOK)
    {
        cout << "Solution found" << td::endl;
        auto x = B.getManipulator();
        cout << "x(0)=" << x(0,0) << " x[1]=" << x(1,0) << td::endl;
        cout << "Double matrix X" << td::endl;
        B.show(std::cout, "X");
    }
    else
    {
        cout << "ERROR! Cannot find solution!" << td::endl;
    }
    //delete pm;
}

void testDblSolve1x1(dense::DblMatrix& M, dense::DblMatrix& B)
{
    cout << "Dbl Matrix test 1x1:" << td::endl
         << "-------------------" << td::endl;
    auto m = M.getManipulator();
    
    m(0,0)=10;
    
    cout << "Double matrix M" << td::endl;
    M.show(std::cout, "M");
    
    //dense::DblMatrix B(2, 1);
    auto b = B.getColumnManipulator();
    
    b(0) = 2;
    bool solveOK = M.solve(B);
    if (solveOK)
    {
        cout << "Solution found" << td::endl;
        auto x = B.getColumnManipulator();
        cout << "x(0)=" << x(0) << td::endl;
        cout << "Double matrix X" << td::endl;
        B.show(std::cout, "X");
    }
    else
    {
        cout << "ERROR! Cannot find solution!" << td::endl;
    }
    //delete pm;
}

void testCmplxSolve2x2(dense::CmplxMatrix& M, dense::CmplxMatrix& B)
{
    cout << "Cmplx Matrix test 2x2:" << td::endl
         << "-------------------" << td::endl;
    
    auto m = M.getManipulator();
    m(0, 0) = td::cmplx(1,1);
    m(1, 0) = td::cmplx(3, 3);
    m(0, 1) = td::cmplx(2, 2);
    m(1, 1) = td::cmplx(4, 4);
    
    cout << "Complex matrix M" << td::endl;
    M.show(std::cout, "M");

    //dense::CmplxMatrix B(2,1);
    auto b = B.getColumnManipulator();
    b(0) = td::cmplx(1, 1);
    b(1) = td::cmplx(2, 2);
    
    bool solveOK = M.solve(B);
    if (solveOK)
    {
        cout << "Solution found" << td::endl;
        auto x = B.getColumnManipulator();
        cout << "x(0)=" << x(0) << " x[1]=" << x(1) << td::endl;
        cout << "Complex matrix X" << td::endl;
        B.show(std::cout, "X");
    }
    else
    {
        cout << "ERROR! Cannot find solution!" << td::endl;
    }
    
}

void testCmplxSolve1x1(dense::CmplxMatrix& M, dense::CmplxMatrix& B)
{
    cout << "Cmplx Matrix test 1x1:" << td::endl
         << "-------------------" << td::endl;
    
    auto m = M.getManipulator();
    m(0, 0) = td::cmplx(2, 0);
    
    cout << "Complex matrix M" << td::endl;
    M.show(std::cout, "M");

    //dense::CmplxMatrix B(2,1);
    auto b = B.getColumnManipulator();
    b(0) = td::cmplx(10, 8);
    
    bool solveOK = M.solve(B);
    if (solveOK)
    {
        cout << "Solution found" << td::endl;
        auto x = B.getColumnManipulator();
        cout << "x(0)=" << x(0) << td::endl;
        cout << "Complex matrix X" << td::endl;
        B.show(std::cout, "X");
    }
    else
    {
        cout << "ERROR! Cannot find solution!" << td::endl;
    }
    
}

void testCmplxDiag()
{
    dense::CmplxDiagMatrix W(3);
    auto w = W.getManipulator();
    w(0) = td::cmplx(10,10);
    w(1) = td::cmplx(20, 20);
    w(2) = td::cmplx(30, 30);
    W.show(std::cout, "W");
}

void testDblDiag()
{
    dense::DblDiagMatrix W(3);
    auto w = W.getManipulator();
    w(0) = 10;
    w(1) = 20;
    w(2) = 30;
    W.show(std::cout, "W");
}

int main(int argc, const char* argv[])
{
    mu::Application app(argc, argv);
    
    
    //start with 2x2
    dense::DblMatrix D(2, 2);
    dense::DblMatrix Bd(2, 1);
    testDblSolve2x2(D, Bd);
    
    dense::CmplxMatrix C(2, 2);
    dense::CmplxMatrix Dc(2, 1);
    testCmplxSolve2x2(C, Dc);
    
    //reduce to 2x1 using same matrices
    D.setDimension(1, 1);
    Bd.setDimension(1, 1);
    testDblSolve1x1(D, Bd);
    
    C.setDimension(1, 1);
    Dc.setDimension(1, 1);
    testCmplxSolve1x1(C, Dc);
    
    //increase again to 2x2 using same matrices
    D.setDimension(2, 2);
    Bd.setDimension(2, 1);
    testDblSolve2x2(D, Bd);
    
    C.setDimension(2, 2);
    Dc.setDimension(2, 1);
    testCmplxSolve2x2(C, Dc);
    
    testDblDiag();
    testCmplxDiag();
    
	return 0;
}

