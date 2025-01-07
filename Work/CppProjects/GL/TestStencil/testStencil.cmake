set(TESTSTENCIL_NAME testStencil)				#Naziv prvog projekta u solution-u

file(GLOB TESTSTENCIL_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTSTENCIL_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTSTENCIL_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTSTENCIL_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTSTENCIL_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTSTENCIL_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTSTENCIL_NAME} ${TESTSTENCIL_INCS} ${TESTSTENCIL_SOURCES} ${TESTSTENCIL_INC_TD} ${TESTSTENCIL_INC_GUI} ${TESTSTENCIL_INC_GUI_GL})

source_group("inc"            FILES ${TESTSTENCIL_INCS})
source_group("src"            FILES ${TESTSTENCIL_SOURCES})
source_group("inc\\td"        FILES ${TESTSTENCIL_INC_TD})
source_group("inc\\gui"        FILES ${TESTSTENCIL_INC_GUI})
source_group("inc\\gl"        FILES ${TESTSTENCIL_INC_GUI_GL})

target_link_libraries(${TESTSTENCIL_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTSTENCIL_NAME})

setTargetPropertiesForGUIApp(${TESTSTENCIL_NAME} ${TESTSTENCIL_PLIST})

setIDEPropertiesForGUIExecutable(${TESTSTENCIL_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTSTENCIL_NAME})



