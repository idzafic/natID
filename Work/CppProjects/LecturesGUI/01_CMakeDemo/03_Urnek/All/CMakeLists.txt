cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME myUrnekSolution)	#Unijeti naziv solution-a

#BEGIN dio koji ide u sve solutions
project(${SOLUTION_NAME})
set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(HOME_ROOT $ENV{HOME})
if (WIN32)
	string(REPLACE "\\" "/" HOME_ROOT "${HOME_ROOT}")
endif()
set(WORK_ROOT ${HOME_ROOT}/Work)
include(${WORK_ROOT}/DevEnv/Common.cmake) 
#END dio koji ide u sve solutions


#ukljuci prvi subproject - generisanje shared libraray (dll)
include(../myMath/myMath.cmake)

#ukljuci drugi subroject - executable koji korisi myMath shared library
include(../myMathTest/myMathTest.cmake)

#dodaj ovisnost (ona garantuje da se prvo kompajlira shared library pa onda executable)
add_dependencies(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_NAME})
