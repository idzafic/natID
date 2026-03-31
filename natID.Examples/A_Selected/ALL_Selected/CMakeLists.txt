cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME natGUIAll)	#Unijeti naziv solution-a

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

include(../TestSplitAndPropEdit/testSplitAndPropEdit.cmake)

include(../TestSimpleDynTab/testSimpleDynTab.cmake)

include(../TestAnimation/testAnimation.cmake)

include(../TextEditor/textEditor.cmake)

include(../MineSweeper/mineSweeper.cmake)

include(../Snake/Snake.cmake)

include(../TestNavigator/testNavigator.cmake)

#GL
include(${NATID_SDK_ROOT}/DevEnv/GtkGL.cmake)
include(${NATID_SDK_ROOT}/DevEnv/natGL.cmake)

include(../TestGLOrtho/testGLOrtho.cmake)
include(../TestGLAnimation/testGLAnimation.cmake)
include(../TestGLCubeMap/testCubeMap.cmake)

#Report
include(${NATID_SDK_ROOT}/DevEnv/natRpt.cmake)
include(../TestTableRep/testTableRep.cmake)

include(${NATID_SDK_ROOT}/DevEnv/netOp.cmake)
include(../ChatServer/chatServer.cmake)
include(../ChatClient/chatClient.cmake)

include(${NATID_SDK_ROOT}/DevEnv/MatrixLib.cmake)
include(${NATID_SDK_ROOT}/DevEnv/SymbComp.cmake)
include(../DigitTwinTest/digitTwinTest.cmake)

