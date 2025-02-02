set(TESTGLCAMERAANDINSTANCING_NAME testCameraAndInstancing)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLCAMERAANDINSTANCING_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLCAMERAANDINSTANCING_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLCAMERAANDINSTANCING_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLCAMERAANDINSTANCING_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLCAMERAANDINSTANCING_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLCAMERAANDINSTANCING_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLCAMERAANDINSTANCING_NAME} ${TESTGLCAMERAANDINSTANCING_INCS} ${TESTGLCAMERAANDINSTANCING_SOURCES} ${TESTGLCAMERAANDINSTANCING_INC_TD} ${TESTGLCAMERAANDINSTANCING_INC_GUI} ${TESTGLCAMERAANDINSTANCING_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLCAMERAANDINSTANCING_INCS})
source_group("src"            FILES ${TESTGLCAMERAANDINSTANCING_SOURCES})
source_group("inc\\td"        FILES ${TESTGLCAMERAANDINSTANCING_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLCAMERAANDINSTANCING_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLCAMERAANDINSTANCING_INC_GUI_GL})

target_link_libraries(${TESTGLCAMERAANDINSTANCING_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLCAMERAANDINSTANCING_NAME})

setTargetPropertiesForGUIApp(${TESTGLCAMERAANDINSTANCING_NAME} ${TESTGLCAMERAANDINSTANCING_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLCAMERAANDINSTANCING_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLCAMERAANDINSTANCING_NAME})



