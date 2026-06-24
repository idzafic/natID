set(SOLARSYSTEM2_NAME SolarSystem2)				#Naziv prvog projekta u solution-u

file(GLOB SOLARSYSTEM2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SOLARSYSTEM2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(SOLARSYSTEM2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB SOLARSYSTEM2_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB SOLARSYSTEM2_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB SOLARSYSTEM2_INC_GUI_GL ${NATID_SDK_INC}/gui/gl/*.h)

# add executable
add_executable(${SOLARSYSTEM2_NAME} ${SOLARSYSTEM2_INCS} ${SOLARSYSTEM2_SOURCES} ${SOLARSYSTEM2_INC_TD} ${SOLARSYSTEM2_INC_GUI} ${SOLARSYSTEM2_INC_GUI_GL})

source_group("inc"            FILES ${SOLARSYSTEM2_INCS})
source_group("src"            FILES ${SOLARSYSTEM2_SOURCES})
source_group("inc\\td"        FILES ${SOLARSYSTEM2_INC_TD})
source_group("inc\\gui"        FILES ${SOLARSYSTEM2_INC_GUI})
source_group("inc\\gl"        FILES ${SOLARSYSTEM2_INC_GUI_GL})

target_link_libraries(${SOLARSYSTEM2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${SOLARSYSTEM2_NAME})

if(APPLE)
    target_link_libraries(${SOLARSYSTEM2_NAME} "-framework ApplicationServices")
endif()

setTargetPropertiesForGUIApp(${SOLARSYSTEM2_NAME} ${SOLARSYSTEM2_PLIST})

setIDEPropertiesForGUIExecutable(${SOLARSYSTEM2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${SOLARSYSTEM2_NAME})



