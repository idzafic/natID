set(TESTGLSHADOW_NAME testShadow)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLSHADOW_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLSHADOW_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLSHADOW_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLSHADOW_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLSHADOW_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLSHADOW_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLSHADOW_NAME} ${TESTGLSHADOW_INCS} ${TESTGLSHADOW_SOURCES} ${TESTGLSHADOW_INC_TD} ${TESTGLSHADOW_INC_GUI} ${TESTGLSHADOW_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLSHADOW_INCS})
source_group("src"            FILES ${TESTGLSHADOW_SOURCES})
source_group("inc\\td"        FILES ${TESTGLSHADOW_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLSHADOW_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLSHADOW_INC_GUI_GL})

target_link_libraries(${TESTGLSHADOW_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLSHADOW_NAME})

setTargetPropertiesForGUIApp(${TESTGLSHADOW_NAME} ${TESTGLSHADOW_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLSHADOW_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLSHADOW_NAME})



