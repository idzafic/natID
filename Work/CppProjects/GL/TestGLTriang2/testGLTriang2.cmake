set(TESTGLTRIANG2_NAME testGLTriang2)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLTRIANG2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLTRIANG2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLTRIANG2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLTRIANG2_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLTRIANG2_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLTRIANG2_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLTRIANG2_NAME} ${TESTGLTRIANG2_INCS} ${TESTGLTRIANG2_SOURCES} ${TESTGLTRIANG2_INC_TD} ${TESTGLTRIANG2_INC_GUI} ${TESTGLTRIANG2_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLTRIANG2_INCS})
source_group("src"            FILES ${TESTGLTRIANG2_SOURCES})
source_group("inc\\td"        FILES ${TESTGLTRIANG2_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLTRIANG2_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLTRIANG2_INC_GUI_GL})

target_link_libraries(${TESTGLTRIANG2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLTRIANG2_NAME})

setTargetPropertiesForGUIApp(${TESTGLTRIANG2_NAME} ${TESTGLTRIANG2_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLTRIANG2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLTRIANG2_NAME})

