set(TEST_GLX_EXTRUSION testExtrusion)

file(GLOB TESTSKYBOX_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTSKYBOX_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTSKYBOX_SHADERS  ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.frag ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.vert)
set(TESTSKYBOX_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)


# add executable
add_executable(${TEST_GLX_EXTRUSION} ${TESTSKYBOX_INCS} ${TESTSKYBOX_SOURCES} ${TESTSKYBOX_SHADERS})

set_source_files_properties(${TESTSKYBOX_SHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("inc"            FILES ${TESTSKYBOX_INCS})
source_group("src"            FILES ${TESTSKYBOX_SOURCES})
source_group("shaders"        FILES ${TESTSKYBOX_SHADERS})


target_link_libraries(${TEST_GLX_EXTRUSION} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})



setTargetPropertiesForGUIApp(${TEST_GLX_EXTRUSION} ${TESTSKYBOX_PLIST})

setIDEPropertiesForGUIExecutable(${TEST_GLX_EXTRUSION} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEST_GLX_EXTRUSION})

registerShaderFolder(${TEST_GLX_EXTRUSION} ${CMAKE_CURRENT_LIST_DIR}/res/shaders "-e" "mainVS,mainFS")

