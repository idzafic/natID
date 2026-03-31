cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME natGLX)	#Unijeti naziv solution-a

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

include(../Radar/radar.cmake)
add_dependencies(${RADAR_NAME} ${NATGLX_NAME} ${NATGUI_NAME})

include(../Terrain/terrain.cmake)
add_dependencies(${TERRAIN_NAME} ${NATGLX_NAME} ${NATGUI_NAME})

include(../TestGLX2/testGL2.cmake)
add_dependencies(${TEST_2_NAME} ${NATGLX_NAME} ${NATGUI_NAME})

include(../TestGLXAnimation3DTexture/TestGLXAnimation3DTexture.cmake)
add_dependencies(${TEST_GLX_ANIMATION_NAME} ${NATGLX_NAME} ${NATGUI_NAME})

include(../TestGLXAnimationFlat/TestGLXAnimationFlat.cmake)
add_dependencies(${TEST_GLX_ANIMATION_FLAT_NAME} ${NATGLX_NAME})

include(../TestGLXCameraAndInstancing/TestGLXCameraAndInstancing.cmake)
add_dependencies(${TEST_GLX_INSTANCING_NAME} ${NATGLX_NAME})

include(../TestGLXLight/TestGLXLight.cmake)
add_dependencies(${TESTLIGHT_NAME} ${NATGLX_NAME})

include(../TestGLXShadow/TestGLXShadow.cmake)
add_dependencies(${TESTSHADOW_NAME} ${NATGLX_NAME} ${NATGUI_NAME})

include(../TestGLXTexture/TestGLXTexture.cmake)
add_dependencies(${TEST_TEXTURE_NAME} ${NATGLX_NAME})

include(../TestTextureChessGLX/TestTextureChessGLX.cmake)
add_dependencies(${TEST_TEXTURE_CHESS_NAME} ${NATGLX_NAME})

include(../TestStencilGLX/TestStencilGLX.cmake)
add_dependencies(${TEST_STENCIL_NAME} ${NATGLX_NAME})

include(../TestGLX/testGLX.cmake)
add_dependencies(${TESTGLX_NAME} ${NATGLX_NAME})

include(../TestFontGLX/TestFontGLX.cmake)
add_dependencies(${TEST_FONT_NAME} ${NATGLX_NAME})

include(../TestCompute/TestCompute.cmake)
add_dependencies(${TEST_COMPUTE_NAME} ${NATGLX_NAME})

include(../GameOfLife/GameOfLife.cmake)
add_dependencies(${TEST_GAME_OF_LIFE_NAME} ${NATGLX_NAME})

include(../TestSkybox/TestSkybox.cmake)
add_dependencies(${TEST_SKYBOX_NAME}  ${NATGLX_NAME})

include(../TestGLXOrtho/TestGLXOrtho.cmake)
add_dependencies(${TEST_GLXORTHO}  ${NATGLX_NAME})

include(../TestGLXTriang/TestGLXTriang.cmake)
add_dependencies(${TEST_GLX_TRIANG} ${NATGLX_NAME})

include(../TestExtrusion/TestExtrusion.cmake)
add_dependencies(${TEST_GLX_EXTRUSION} ${NATGLX_NAME})