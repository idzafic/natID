set(RADAR_NAME testRadar)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLX_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLX_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLX_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLX_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB TESTGLX_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB TESTGLX_INC_GLX ${NATID_SDK_INC}/glx/*.h)
file(GLOB RADAR_NAME_SHADERS  ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.frag ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*.vert)

# add executable
# add executable
add_executable(${RADAR_NAME} ${TESTGLX_INCS} ${TESTGLX_SOURCES} ${TESTGLX_INC_TD} ${TESTGLX_INC_GUI} ${TESTGLX_INC_GLX} ${RADAR_NAME_SHADERS})

source_group("inc"            FILES ${TESTGLX_INCS})
source_group("src"            FILES ${TESTGLX_SOURCES})
source_group("inc\\td"        FILES ${TESTGLX_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLX_INC_GUI})
source_group("inc\\glx"        FILES ${TESTGLX_INC_GLX})
source_group("shaders"        FILES ${RADAR_NAME_SHADERS})

#if(APPLE)
	target_link_libraries(${RADAR_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})
#lse()
	#Windows and linux later
#	target_link_libraries(${RADAR_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
#									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
#								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})
								
#endif()

#addOpenGLLibraries(${RADAR_NAME})

target_compile_definitions(${RADAR_NAME} PUBLIC SHADER_LOCATION="${CMAKE_CURRENT_LIST_DIR}/src/shaders")

target_compile_definitions(${RADAR_NAME} PUBLIC VULKAN_VALIDATION="1")

setTargetPropertiesForGUIApp(${RADAR_NAME} ${TESTGLX_PLIST})

setIDEPropertiesForGUIExecutable(${RADAR_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${RADAR_NAME})

registerShaderFolder(${RADAR_NAME} ${CMAKE_CURRENT_LIST_DIR}/res/shaders  "-e" "mainVS,mainFS")

#setStartupProject(${CMAKE_CURRENT_SOURCE_DIR} ${RADAR_NAME})