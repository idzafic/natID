set(TESTGLCUBEMAP_NAME testCubeMap)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLCUBEMAP_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLCUBEMAP_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTGLCUBEMAP_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLCUBEMAP_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLCUBEMAP_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

#Application icon
set(TESTGLCUBEMAP_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TESTGLCUBEMAP_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TESTGLCUBEMAP_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TESTGLCUBEMAP_NAME} ${TESTGLCUBEMAP_INCS} ${TESTGLCUBEMAP_SOURCES} ${TESTGLCUBEMAP_INC_TD} ${TESTGLCUBEMAP_INC_GUI} 
				${TESTGLCUBEMAP_INC_GUI_GL} ${TESTGLCUBEMAP_WINAPP_ICON})

source_group("inc"            FILES ${TESTGLCUBEMAP_INCS})
source_group("src"            FILES ${TESTGLCUBEMAP_SOURCES})
source_group("inc\\td"        FILES ${TESTGLCUBEMAP_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLCUBEMAP_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLCUBEMAP_INC_GUI_GL})

target_link_libraries(${TESTGLCUBEMAP_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLCUBEMAP_NAME})

setTargetPropertiesForGUIApp(${TESTGLCUBEMAP_NAME} ${TESTGLCUBEMAP_PLIST})

setAppIcon(${TESTGLCUBEMAP_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TESTGLCUBEMAP_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLCUBEMAP_NAME})



