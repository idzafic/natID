set(TEXTEDITOR_NAME textEditor)				#Naziv prvog projekta u solution-u

file(GLOB TEXTEDITOR_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TEXTEDITOR_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TEXTEDITOR_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TEXTEDITOR_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(TEXTEDITOR_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TEXTEDITOR_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TEXTEDITOR_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TEXTEDITOR_NAME} ${TEXTEDITOR_INCS} ${TEXTEDITOR_SOURCES} ${TEXTEDITOR_INC_TD}  ${TEXTEDITOR_INC_GUI} ${TEXTEDITOR_WINAPP_ICON})

source_group("inc"            FILES ${TEXTEDITOR_INCS})
source_group("inc\\td"        FILES ${TEXTEDITOR_INC_TD})
source_group("inc\\gui"        FILES ${TEXTEDITOR_INC_GUI})
source_group("src"            FILES ${TEXTEDITOR_SOURCES})

target_link_libraries(${TEXTEDITOR_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TEXTEDITOR_NAME} ${TEXTEDITOR_PLIST})

setAppIcon(${TEXTEDITOR_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TEXTEDITOR_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEXTEDITOR_NAME})
