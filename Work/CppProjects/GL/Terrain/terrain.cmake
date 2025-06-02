set(TERRAIN_NAME terrain)				#Naziv prvog projekta u solution-u

file(GLOB TERRAIN_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TERRAIN_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TERRAIN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TERRAIN_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TERRAIN_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TERRAIN_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TERRAIN_NAME} ${TERRAIN_INCS} ${TERRAIN_SOURCES} ${TERRAIN_INC_TD} ${TERRAIN_INC_GUI} ${TERRAIN_INC_GUI_GL})

source_group("inc"            FILES ${TERRAIN_INCS})
source_group("src"            FILES ${TERRAIN_SOURCES})
source_group("inc\\td"        FILES ${TERRAIN_INC_TD})
source_group("inc\\gui"        FILES ${TERRAIN_INC_GUI})
source_group("inc\\gui"        FILES ${TERRAIN_INC_GUI})
source_group("inc\\gl"        FILES ${TERRAIN_INC_GUI_GL})

target_link_libraries(${TERRAIN_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TERRAIN_NAME})

setTargetPropertiesForGUIApp(${TERRAIN_NAME} ${TERRAIN_PLIST})

setIDEPropertiesForGUIExecutable(${TERRAIN_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TERRAIN_NAME})

