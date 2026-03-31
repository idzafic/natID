set(TEST_FONT_NAME testFontGLX)	

file(GLOB TESTGLX_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLX_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLX_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLX_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLX_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLX_INC_GLX ${MY_INC}/glx/*.h)


add_executable(${TEST_FONT_NAME} ${TESTGLX_INCS} ${TESTGLX_SOURCES} ${TESTGLX_INC_TD} ${TESTGLX_INC_GUI} ${TESTGLX_INC_GLX})

source_group("inc"            FILES ${TESTGLX_INCS})
source_group("src"            FILES ${TESTGLX_SOURCES})
source_group("inc\\td"        FILES ${TESTGLX_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLX_INC_GUI})
source_group("inc\\glx"        FILES ${TESTGLX_INC_GLX})


target_link_libraries(${TEST_FONT_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})


setTargetPropertiesForGUIApp(${TEST_FONT_NAME} ${TESTGLX_PLIST})

setIDEPropertiesForGUIExecutable(${TEST_FONT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEST_FONT_NAME})

registerShaderFolder(${TEST_FONT_NAME} ${NATID_SDK_ROOT}/Common/Shaders/Font  "-e" "mainVS,mainFS")
