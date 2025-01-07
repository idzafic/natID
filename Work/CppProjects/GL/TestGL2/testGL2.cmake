set(TESTGL2_NAME testGL2)				#Naziv prvog projekta u solution-u

file(GLOB TESTGL2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGL2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGL2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGL2_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGL2_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGL2_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGL2_NAME} ${TESTGL2_INCS} ${TESTGL2_SOURCES} ${TESTGL2_INC_TD} ${TESTGL2_INC_GUI} ${TESTGL2_INC_GUI_GL})

source_group("inc"            FILES ${TESTGL2_INCS})
source_group("src"            FILES ${TESTGL2_SOURCES})
source_group("inc\\td"        FILES ${TESTGL2_INC_TD})
source_group("inc\\gui"        FILES ${TESTGL2_INC_GUI})
source_group("inc\\gui"        FILES ${TESTGL2_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGL2_INC_GUI_GL})

target_link_libraries(${TESTGL2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGL2_NAME})

setTargetPropertiesForGUIApp(${TESTGL2_NAME} ${TESTGL2_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGL2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGL2_NAME})

