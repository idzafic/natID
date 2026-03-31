cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME PZIGUISol)	#Unijeti naziv solution-a

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



include(${NATID_SDK_ROOT}/DevEnv/natGUI.cmake)
include(${NATID_SDK_ROOT}/DevEnv/DataProvider.cmake)

include(../05_GUI/01_AppWnd/appWnd.cmake)

include(../05_GUI/02_AppWndMenuTB/appWndMenuTB.cmake)

include(../06_GUILayouts/01_DemoVert/appDemoVert.cmake)

include(../06_GUILayouts/02_DemoGrid/appDemoGrid.cmake)

include(../07_TableEditDB/01_DemoTableEdit/appDemoTableEdit.cmake)

include(../08_Canvas/01_Test2DTransf/test2DTransf.cmake)

include(../08_Canvas/02_TestAnimation/testAnimation.cmake)

include(../08_Canvas/03_TestAnimationWithSplitter/testAnimationWithSplitter.cmake)
