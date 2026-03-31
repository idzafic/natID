set(TEST_COMPUTE_NAME testComputeGLX)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLX_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLX_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLX_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLX_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB TESTGLX_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB TESTGLX_INC_GLX ${NATID_SDK_INC}/glx/*.h)



# add executable
add_executable(${TEST_COMPUTE_NAME} ${TESTGLX_INCS} ${TESTGLX_SOURCES} ${TESTGLX_INC_TD} ${TESTGLX_INC_GUI} ${TESTGLX_INC_GLX})
target_include_directories(${TEST_COMPUTE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/common/stb_image)

source_group("inc"            FILES ${TESTGLX_INCS})
source_group("src"            FILES ${TESTGLX_SOURCES})
source_group("inc\\td"        FILES ${TESTGLX_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLX_INC_GUI})
source_group("inc\\glx"        FILES ${TESTGLX_INC_GLX})

if(APPLE)
	target_link_libraries(${TEST_COMPUTE_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})
else()
	#Windows and linux later
	target_include_directories(${TEST_COMPUTE_NAME} PUBLIC ${VULKAN_INCS})
	target_link_libraries(${TEST_COMPUTE_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGLX_LIB_DEBUG} optimized ${NATGLX_LIB_RELEASE})
	target_compile_definitions(${TEST_COMPUTE_NAME} PUBLIC GLX_USE_VULKAN)
								
endif()

#addOpenGLLibraries(${TEST_COMPUTE_NAME})

target_compile_definitions(${TEST_COMPUTE_NAME} PUBLIC SHADER_LOCATION="${CMAKE_CURRENT_LIST_DIR}/src/shaders")

target_compile_definitions(${TEST_COMPUTE_NAME} PUBLIC VULKAN_VALIDATION="1")

setTargetPropertiesForGUIApp(${TEST_COMPUTE_NAME} ${TESTGLX_PLIST})

setIDEPropertiesForGUIExecutable(${TEST_COMPUTE_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEST_COMPUTE_NAME})

registerShaderFolder(${TEST_COMPUTE_NAME} "${CMAKE_CURRENT_LIST_DIR}/src/shaders")

#setStartupProject(${CMAKE_CURRENT_SOURCE_DIR} ${TEST_COMPUTE_NAME})
