set(TESTGLTRIANG_NAME testGLTriang)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLTRIANG_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLTRIANG_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLTRIANG_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLTRIANG_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLTRIANG_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLTRIANG_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLTRIANG_NAME} ${TESTGLTRIANG_INCS} ${TESTGLTRIANG_SOURCES} ${TESTGLTRIANG_INC_TD} ${TESTGLTRIANG_INC_GUI} ${TESTGLTRIANG_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLTRIANG_INCS})
source_group("src"            FILES ${TESTGLTRIANG_SOURCES})
source_group("inc\\td"        FILES ${TESTGLTRIANG_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLTRIANG_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLTRIANG_INC_GUI_GL})

target_link_libraries(${TESTGLTRIANG_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLTRIANG_NAME})

setTargetPropertiesForGUIApp(${TESTGLTRIANG_NAME} ${TESTGLTRIANG_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLTRIANG_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLTRIANG_NAME})

