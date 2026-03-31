cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME testGLX)	#Unijeti naziv solution-a

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

include(${NATID_SDK_ROOT}/DevEnv/DataProvider.cmake)
include(${NATID_SDK_ROOT}/DevEnv/natGUI.cmake)
include(${NATID_SDK_ROOT}/DevEnv/natGLX.cmake)

include(TestSkybox.cmake)
