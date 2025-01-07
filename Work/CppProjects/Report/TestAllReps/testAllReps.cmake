set(TESTALLREPS_NAME testAllReps)				#Naziv prvog projekta u solution-u

file(GLOB TESTALLREPS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTALLREPS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTALLREPS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTALLREPS_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTALLREPS_INC_RPT  ${MY_INC}/rpt/*.h)
file(GLOB TESTALLREPS_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TESTALLREPS_NAME} ${TESTALLREPS_INCS} ${TESTALLREPS_SOURCES} ${TESTALLREPS_INC_TD}  ${TESTALLREPS_INC_GUI} ${TESTALLREPS_INC_RPT})

source_group("inc"            FILES ${TESTALLREPS_INCS})
source_group("src"            FILES ${TESTALLREPS_SOURCES})
source_group("inc\\td"        FILES ${TESTALLREPS_INC_TD})
source_group("inc\\gui"        FILES ${TESTALLREPS_INC_GUI})
source_group("inc\\rpt"        FILES ${TESTALLREPS_INC_RPT})

target_link_libraries(${TESTALLREPS_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} 
						debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE} debug ${NATRPT_LIB_DEBUG} optimized ${NATRPT_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTALLREPS_NAME} ${TESTALLREPS_PLIST})

setIDEPropertiesForGUIExecutable(${TESTALLREPS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTALLREPS_NAME})

