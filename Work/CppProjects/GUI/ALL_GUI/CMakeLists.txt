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

include(../TestGUI/testGUI.cmake)

include(../TestDynTab/testDynTab.cmake)

include(../TestControls/testControls.cmake)

include(../TestCanvas/testCanvas.cmake)

include(../TestScrolledCanvas/testScrolledCanvas.cmake)

include(../TestNavigator/testNavigator.cmake)

include(../TestTableEdit/testTableEdit.cmake)

include(../TestThread/testThread.cmake)

include(../TestTimer/testTimer.cmake)

include(../TestWindowsAndDialogs/testWindowsAndDialogs.cmake)

include(../Test2DTransf/test2DTransf.cmake)

include(../TestAnimationWithSplitter/testAnimationWithSplitter.cmake)

include(../Test2DETFTransf/test2DETFTransf.cmake)

include(../Test2DETFTransfWSplitter/test2DETFTransfWSplitter.cmake)
