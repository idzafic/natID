set(TESTGUI_NAME testGUI)				#Naziv prvog projekta u solution-u

file(GLOB TESTGUI_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGUI_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGUI_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGUI_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGUI_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TESTGUI_NAME} ${TESTGUI_INCS} ${TESTGUI_SOURCES} ${TESTGUI_INC_TD}  ${TESTGUI_INC_GUI})

source_group("inc"            FILES ${TESTGUI_INCS})
source_group("src"            FILES ${TESTGUI_SOURCES})
source_group("inc\\td"        FILES ${TESTGUI_INC_TD})
source_group("inc\\gui"        FILES ${TESTGUI_INC_GUI})

target_link_libraries(${TESTGUI_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTGUI_NAME} ${TESTGUI_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGUI_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGUI_NAME})

