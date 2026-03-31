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
