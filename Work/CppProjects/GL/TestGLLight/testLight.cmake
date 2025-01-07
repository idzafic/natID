set(TESTGLLIGHT_NAME testLight)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLLIGHT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLLIGHT_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLLIGHT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLLIGHT_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLLIGHT_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLLIGHT_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLLIGHT_NAME} ${TESTGLLIGHT_INCS} ${TESTGLLIGHT_SOURCES} ${TESTGLLIGHT_INC_TD} ${TESTGLLIGHT_INC_GUI} ${TESTGLLIGHT_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLLIGHT_INCS})
source_group("src"            FILES ${TESTGLLIGHT_SOURCES})
source_group("inc\\td"        FILES ${TESTGLLIGHT_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLLIGHT_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLLIGHT_INC_GUI_GL})

target_link_libraries(${TESTGLLIGHT_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLLIGHT_NAME})

setTargetPropertiesForGUIApp(${TESTGLLIGHT_NAME} ${TESTGLLIGHT_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLLIGHT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLLIGHT_NAME})



