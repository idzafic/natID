set(TABLEREPTEST_NAME testTableRep)				#Naziv prvog projekta u solution-u

file(GLOB TABLEREPTEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TABLEREPTEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TABLEREPTEST_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTALLREPS_INC_RPT  ${MY_INC}/rpt/*.h)
file(GLOB TABLEREPTEST_INC_GUI ${MY_INC}/gui/*.h)
#Application icon
set(TABLEREPTEST_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TABLEREPTEST_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TABLEREPTEST_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TABLEREPTEST_NAME} ${TABLEREPTEST_INCS} ${TABLEREPTEST_SOURCES} ${TABLEREPTEST_INC_TD}  ${TABLEREPTEST_INC_GUI}  ${TESTALLREPS_INC_RPT} ${TABLEREPTEST_WINAPP_ICON})

source_group("inc"            FILES ${TABLEREPTEST_INCS})
source_group("src"            FILES ${TABLEREPTEST_SOURCES})
source_group("inc\\td"        FILES ${TABLEREPTEST_INC_TD})
source_group("inc\\gui"        FILES ${TABLEREPTEST_INC_GUI})
source_group("inc\\rpt"        FILES ${TESTALLREPS_INC_RPT})

target_link_libraries(${TABLEREPTEST_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} 
						debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE} debug ${NATRPT_LIB_DEBUG} optimized ${NATRPT_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TABLEREPTEST_NAME} ${TABLEREPTEST_PLIST})

setAppIcon(${TABLEREPTEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TABLEREPTEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TABLEREPTEST_NAME})
