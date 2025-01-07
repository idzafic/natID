set(CHAT_SERVER_NAME chatServer)				#Naziv prvog projekta u solution-u

file(GLOB CHAT_SERVER_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB CHAT_SERVER_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB CHAT_SERVER_INC_TD  ${MY_INC}/td/*.h)
file(GLOB CHAT_SERVER_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB CHAT_SERVER_INC_GUI_NO ${MY_INC}/gui/no/*.h)
file(GLOB CHAT_SERVER_INC_NO ${MY_INC}/no/*.h)

#Application icon
set(CHAT_SERVER_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(CHAT_SERVER_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(CHAT_SERVER_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${CHAT_SERVER_NAME} ${CHAT_SERVER_INCS} ${CHAT_SERVER_SOURCES} ${CHAT_SERVER_INC_TD}  
				${CHAT_SERVER_INC_GUI} ${CHAT_SERVER_INC_GUI_NO} ${CHAT_SERVER_INC_NO} ${CHAT_SERVER_APP_ICON})

source_group("inc"            FILES ${CHAT_SERVER_INCS})
source_group("inc\\td"        FILES ${CHAT_SERVER_INC_TD})
source_group("inc\\gui"        FILES ${CHAT_SERVER_INC_GUI})
source_group("inc\\gui\\no"        FILES ${CHAT_SERVER_INC_GUI_NO})
source_group("inc\\no"        FILES ${CHAT_SERVER_INC_NO})
source_group("src"            FILES ${CHAT_SERVER_SOURCES})

target_link_libraries(${CHAT_SERVER_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  
										debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE}
										debug ${NETOP_LIB_DEBUG} optimized ${NETOP_LIB_RELEASE})

setTargetPropertiesForGUIApp(${CHAT_SERVER_NAME} ${CHAT_SERVER_PLIST})

setAppIcon(${CHAT_SERVER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${CHAT_SERVER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${CHAT_SERVER_NAME})
