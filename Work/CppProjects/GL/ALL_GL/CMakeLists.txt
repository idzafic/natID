cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME natGLAll_GTK)	#Unijeti naziv solution-a

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

include(${WORK_ROOT}/DevEnv/DataProvider.cmake) 
include(${WORK_ROOT}/DevEnv/natGUI.cmake)
include(${WORK_ROOT}/DevEnv/GtkGL.cmake)
include(${WORK_ROOT}/DevEnv/natGL.cmake)

include(../TestGL/testGL.cmake)

include(../TestGL2/testGL2.cmake)

include(../TestGLTriang/testGLTriang.cmake)

include(../TestGLTriang2/testGLTriang2.cmake)

include(../TestGLTexture/testGLTexture.cmake)

include(../TestGLAnimationFlat/testGLAnimationFlat.cmake)

include(../TestGLAnimation3DTexture/testGLAnimation3DTexture.cmake)

include(../TestGLAnimation3DTexture2/testGLAnimation3DTexture2.cmake)

include(../TestGLCameraAndInstancing/testCameraAndInstancing.cmake)

include(../TestGLLight/testLight.cmake)

include(../TestStencil/testStencil.cmake)

include(../TestPerlin/testPerlin.cmake)

include(../TestGLTess2/testGLTess2.cmake)

