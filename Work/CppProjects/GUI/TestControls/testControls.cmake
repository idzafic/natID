set(TEST_CONTROLS_NAME testControls)				#Naziv prvog projekta u solution-u

file(GLOB TEST_CONTROLS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TEST_CONTROLS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TEST_CONTROLS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TEST_CONTROLS_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TEST_CONTROLS_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TEST_CONTROLS_NAME} ${TEST_CONTROLS_INCS} ${TEST_CONTROLS_SOURCES} ${TEST_CONTROLS_INC_TD}  ${TEST_CONTROLS_INC_GUI})

source_group("inc"            FILES ${TEST_CONTROLS_INCS})
source_group("src"            FILES ${TEST_CONTROLS_SOURCES})
source_group("inc\\td"        FILES ${TEST_CONTROLS_INC_TD})
source_group("inc\\gui"        FILES ${TEST_CONTROLS_INC_GUI})


target_link_libraries(${TEST_CONTROLS_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TEST_CONTROLS_NAME} ${TEST_CONTROLS_PLIST})

setIDEPropertiesForGUIExecutable(${TEST_CONTROLS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEST_CONTROLS_NAME})
