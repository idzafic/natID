cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME PZIGUISol)	#Unijeti naziv solution-a

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

include(${WORK_ROOT}/DevEnv/natGUI.cmake)
include(${WORK_ROOT}/DevEnv/DataProvider.cmake)

include(../05_GUI/01_AppWnd/appWnd.cmake)

include(../05_GUI/02_AppWndMenuTB/appWndMenuTB.cmake)

include(../06_GUILayouts/01_DemoVert/appDemoVert.cmake)

include(../06_GUILayouts/02_DemoGrid/appDemoGrid.cmake)

include(../07_TableEditDB/01_DemoTableEdit/appDemoTableEdit.cmake)

include(../08_Canvas/01_Test2DTransf/test2DTransf.cmake)

include(../08_Canvas/02_TestAnimation/testAnimation.cmake)

include(../08_Canvas/03_TestAnimationWithSplitter/testAnimationWithSplitter.cmake)
