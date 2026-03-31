set(TEST_STENCIL_NAME testStencilGLX)

file(GLOB TESTGLX_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLX_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLX_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLX_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB TESTGLX_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB TESTGLX_INC_GLX ${NATID_SDK_INC}/glx/*.h)

file(GLOB TESTGLX_SHADERS  ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.frag ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.vert)



# add executable
add_executable(${TEST_STENCIL_NAME} ${TESTGLX_INCS} ${TESTGLX_SOURCES} ${TESTGLX_INC_TD} ${TESTGLX_INC_GUI} ${TESTGLX_INC_GLX} ${TESTGLX_SHADERS})

set_source_files_properties(${TESTGLX_SHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("inc"            FILES ${TESTGLX_INCS})
source_group("src"            FILES ${TESTGLX_SOURCES})
source_group("inc\\td"        FILES ${TESTGLX_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLX_INC_GUI})
source_group("inc\\glx"        FILES ${TESTGLX_INC_GLX})
source_group("shaders"        FILES ${TESTGLX_SHADERS})


target_link_libraries(${TEST_STENCIL_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								   debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})


setTargetPropertiesForGUIApp(${TEST_STENCIL_NAME} ${TESTGLX_PLIST})

setIDEPropertiesForGUIExecutable(${TEST_STENCIL_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEST_STENCIL_NAME})

registerShaderFolder(${TEST_STENCIL_NAME} ${CMAKE_CURRENT_LIST_DIR}/res/shaders "-e" "mainVS,mainFS")

