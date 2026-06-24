set(SOLARSYSTEM_NAME SolarSystem)				#Naziv prvog projekta u solution-u

file(GLOB SOLARSYSTEM_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SOLARSYSTEM_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(SOLARSYSTEM_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB SOLARSYSTEM_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB SOLARSYSTEM_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB SOLARSYSTEM_INC_GUI_GL ${NATID_SDK_INC}/gui/gl/*.h)

# add executable
add_executable(${SOLARSYSTEM_NAME} ${SOLARSYSTEM_INCS} ${SOLARSYSTEM_SOURCES} ${SOLARSYSTEM_INC_TD} ${SOLARSYSTEM_INC_GUI} ${SOLARSYSTEM_INC_GUI_GL})

source_group("inc"            FILES ${SOLARSYSTEM_INCS})
source_group("src"            FILES ${SOLARSYSTEM_SOURCES})
source_group("inc\\td"        FILES ${SOLARSYSTEM_INC_TD})
source_group("inc\\gui"        FILES ${SOLARSYSTEM_INC_GUI})
source_group("inc\\gl"        FILES ${SOLARSYSTEM_INC_GUI_GL})

target_link_libraries(${SOLARSYSTEM_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${SOLARSYSTEM_NAME})

# CGAssociateMouseAndMouseCursorPosition (used by the free-cam mouselook to
# cancel macOS's post-warp movement-suppression) lives in CoreGraphics, pulled
# in via the ApplicationServices umbrella framework.
if(APPLE)
    target_link_libraries(${SOLARSYSTEM_NAME} "-framework ApplicationServices")
endif()

setTargetPropertiesForGUIApp(${SOLARSYSTEM_NAME} ${SOLARSYSTEM_PLIST})

setIDEPropertiesForGUIExecutable(${SOLARSYSTEM_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${SOLARSYSTEM_NAME})



