cmake_minimum_required(VERSION 3.17)

set(SOLUTION_NAME SolTestPerlin)	#Unijeti naziv solution-a

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

include(${WORK_ROOT}/DevEnv/ZipperLib.cmake)

include(testPerlin.cmake)
