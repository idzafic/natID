cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME LISArray)	#Unijeti naziv solution-a

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


#doaj sve cpp fajlove potrebne za implmentaciju
add_executable(${SOLUTION_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp) 

target_link_libraries(${SOLUTION_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}) 