set(TESTGL_NAME testGL)				#Naziv prvog projekta u solution-u

file(GLOB TESTGL_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGL_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGL_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGL_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGL_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGL_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGL_NAME} ${TESTGL_INCS} ${TESTGL_SOURCES} ${TESTGL_INC_TD} ${TESTGL_INC_GUI} ${TESTGL_INC_GUI_GL})

source_group("inc"            FILES ${TESTGL_INCS})
source_group("src"            FILES ${TESTGL_SOURCES})
source_group("inc\\td"        FILES ${TESTGL_INC_TD})
source_group("inc\\gui"        FILES ${TESTGL_INC_GUI})
source_group("inc\\gui"        FILES ${TESTGL_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGL_INC_GUI_GL})

target_link_libraries(${TESTGL_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGL_NAME})

setTargetPropertiesForGUIApp(${TESTGL_NAME} ${TESTGL_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGL_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGL_NAME})

