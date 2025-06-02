set(RADAR2_NAME Radar2)				#Naziv prvog projekta u solution-u

file(GLOB RADAR2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB RADAR2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(RADAR2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB RADAR2_INC_TD  ${MY_INC}/td/*.h)
file(GLOB RADAR2_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB RADAR2_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${RADAR2_NAME} ${RADAR2_INCS} ${RADAR2_SOURCES} ${RADAR2_INC_TD} ${RADAR2_INC_GUI} ${RADAR2_INC_GUI_GL})

source_group("inc"            FILES ${RADAR2_INCS})
source_group("src"            FILES ${RADAR2_SOURCES})
source_group("inc\\td"        FILES ${RADAR2_INC_TD})
source_group("inc\\gui"        FILES ${RADAR2_INC_GUI})
source_group("inc\\gui"        FILES ${RADAR2_INC_GUI})
source_group("inc\\gl"        FILES ${RADAR2_INC_GUI_GL})

target_link_libraries(${RADAR2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${RADAR2_NAME})

setTargetPropertiesForGUIApp(${RADAR2_NAME} ${RADAR2_PLIST})

setIDEPropertiesForGUIExecutable(${RADAR2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${RADAR2_NAME})

