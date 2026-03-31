cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME natGLAll_GTK)	#Unijeti naziv solution-a

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
include(${NATID_SDK_ROOT}/DevEnv/GtkGL.cmake)
include(${NATID_SDK_ROOT}/DevEnv/natGL.cmake)

include(../TestGL/testGL.cmake)

include(../TestGL2/testGL2.cmake)

include(../TestGLTriang/testGLTriang.cmake)

include(../TestGLTriang2/testGLTriang2.cmake)

include(../TestGLTexture/testGLTexture.cmake)

include(../TextureChess/textureChess.cmake)

include(../TextureChess2/textureChess2.cmake)

include(../TestGLAnimationFlat/testGLAnimationFlat.cmake)

include(../TestGLAnimation3DTexture/testGLAnimation3DTexture.cmake)

include(../TestGLAnimation3DTexture2/testGLAnimation3DTexture2.cmake)

include(../TestGLCameraAndInstancing/testCameraAndInstancing.cmake)

include(../TestGLLight/testLight.cmake)

include(../TestStencil/testStencil.cmake)

include(../Radar2/Radar2.cmake)

include(../TestGLTess2/testGLTess2.cmake)

include(${NATID_SDK_ROOT}/DevEnv/ZipperLib.cmake)

include(../TestPerlin/testPerlin.cmake)

include(../Terrain/terrain.cmake)

