cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME myMathTestMacLinWin)	#Unijeti naziv solution-a

project(${SOLUTION_NAME})

#ukljuci prvi subproject - generisanje shared libraray (dll)
include(../myMath/myMath.cmake)

#ukljuci drugi subroject - executable koji korisi myMath shared library
include(myMathTest.cmake)

#dodaj ovisnost (ona garantuje da se prvo kompajlira shared library pa onda executable)
add_dependencies(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_NAME})
