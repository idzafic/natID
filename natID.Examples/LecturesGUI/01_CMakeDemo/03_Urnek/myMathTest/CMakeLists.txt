cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME myMathTestMacLinWin)	#Unijeti naziv solution-a

#BEGIN common part for all solutions
project(${SOLUTION_NAME})
set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})
if (WIN32)
	set(HOME_ROOT $ENV{USERPROFILE})
	string(REPLACE "\\" "/" HOME_ROOT "${HOME_ROOT}")
else()
	set(HOME_ROOT $ENV{HOME})
endif()
set(NATID_SDK_ROOT ${HOME_ROOT}/natID.SDK)
include(${NATID_SDK_ROOT}/DevEnv/Common.cmake) 
#END common part for all solutions




#ukljuci prvi subproject - generisanje shared libraray (dll)
include(../myMath/myMath.cmake)

#ukljuci drugi subroject - executable koji korisi myMath shared library
include(myMathTest.cmake)

#dodaj ovisnost (ona garantuje da se prvo kompajlira shared library pa onda executable)
add_dependencies(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_NAME})
