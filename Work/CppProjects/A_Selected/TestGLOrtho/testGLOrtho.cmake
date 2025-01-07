set(TESTGLORTHO_NAME testGLOrtho)				#Naziv prvog projekta u solution-u
set(TESTGLORTHO_BUNDLE_NAME TestGLOrthogonal)

file(GLOB TESTGLORTHO_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLORTHO_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTGLORTHO_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLORTHO_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLORTHO_INC_GUI_GL ${MY_INC}/gui/gl/*.h)
#Application icon
set(TESTGLORTHO_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TESTGLORTHO_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TESTGLORTHO_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TESTGLORTHO_NAME} ${TESTGLORTHO_INCS} ${TESTGLORTHO_SOURCES} ${TESTGLORTHO_INC_TD} 
				${TESTGLORTHO_INC_GUI} ${TESTGLORTHO_INC_GUI_GL} ${TESTGLORTHO_ICON})

source_group("inc"            FILES ${TESTGLORTHO_INCS})
source_group("src"            FILES ${TESTGLORTHO_SOURCES})
source_group("inc\\td"        FILES ${TESTGLORTHO_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLORTHO_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLORTHO_INC_GUI_GL})

target_link_libraries(${TESTGLORTHO_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLORTHO_NAME})

#setTargetPropertiesForGUIApp(${TESTGLORTHO_NAME} ${TESTGLORTHO_PLIST})

setTargetPropertiesForGUIAppWithBundleName(${TESTGLORTHO_NAME} ${TESTGLORTHO_PLIST} ${TESTGLORTHO_BUNDLE_NAME})

setAppIcon(${TESTGLORTHO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TESTGLORTHO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLORTHO_NAME})

