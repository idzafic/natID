cmake_minimum_required(VERSION 3.18)

set(SOLUTION_NAME natGUIAll)	#Unijeti naziv solution-a

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

include(../TestSplitAndPropEdit/testSplitAndPropEdit.cmake)

include(../TestSimpleDynTab/testSimpleDynTab.cmake)

include(../TestAnimation/testAnimation.cmake)

include(../TextEditor/textEditor.cmake)

include(../MineSweeper/mineSweeper.cmake)

include(../Snake/Snake.cmake)

include(../TestNavigator/testNavigator.cmake)

#GL
include(${WORK_ROOT}/DevEnv/GtkGL.cmake)
include(${WORK_ROOT}/DevEnv/natGL.cmake)

include(../TestGLOrtho/testGLOrtho.cmake)
include(../TestGLAnimation/testGLAnimation.cmake)
include(../TestGLCubeMap/testCubeMap.cmake)

#Report
include(${WORK_ROOT}/DevEnv/natRpt.cmake)
include(../TestTableRep/testTableRep.cmake)

include(${WORK_ROOT}/DevEnv/netOp.cmake)
include(../ChatServer/chatServer.cmake)
include(../ChatClient/chatClient.cmake)

include(${WORK_ROOT}/DevEnv/MatrixLib.cmake)
include(${WORK_ROOT}/DevEnv/SymbComp.cmake)
include(../DigitTwinTest/digitTwinTest.cmake)

