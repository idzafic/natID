set(TESTGL_TESS2_NAME testGLTess2)				#Naziv prvog projekta u solution-u

file(GLOB TESTGL_TESS2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGL_TESS2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGL_TESS2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGL_TESS2_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGL_TESS2_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGL_TESS2_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGL_TESS2_NAME} ${TESTGL_TESS2_INCS} ${TESTGL_TESS2_SOURCES} ${TESTGL_TESS2_INC_TD} ${TESTGL_TESS2_INC_GUI} ${TESTGL_TESS2_INC_GUI_GL})

source_group("inc"            FILES ${TESTGL_TESS2_INCS})
source_group("src"            FILES ${TESTGL_TESS2_SOURCES})
source_group("inc\\td"        FILES ${TESTGL_TESS2_INC_TD})
source_group("inc\\gui"        FILES ${TESTGL_TESS2_INC_GUI})
source_group("inc\\gui"        FILES ${TESTGL_TESS2_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGL_TESS2_INC_GUI_GL})

target_link_libraries(${TESTGL_TESS2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGL_TESS2_NAME})

setTargetPropertiesForGUIApp(${TESTGL_TESS2_NAME} ${TESTGL_TESS2_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGL_TESS2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGL_TESS2_NAME})

