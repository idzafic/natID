set(CHESS_NAME Chess)				#Naziv prvog projekta u solution-u

file(GLOB CHESS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB CHESS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB CHESS_INC_TD  ${MY_INC}/td/*.h)
file(GLOB CHESS_INC_GUI ${MY_INC}/gui/*.h)

file(GLOB CHESS_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB CHESS_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB CHESS_INC_FO  ${MY_INC}/fo/*.h)
file(GLOB CHESS_INC_XML  ${MY_INC}/xml/*.h)

#Application icon
set(CHESS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(CHESS_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(CHESS_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${CHESS_NAME} ${CHESS_INCS} ${CHESS_SOURCES} ${CHESS_INC_TD} ${CHESS_INC_THREAD} 
				${CHESS_INC_CNT} ${CHESS_INC_FO} ${CHESS_INC_GUI} ${CHESS_INC_XML} ${CHESS_WINAPP_ICON})

source_group("inc"            FILES ${CHESS_INCS})
source_group("inc\\td"        FILES ${CHESS_INC_TD})
source_group("inc\\cnt"        FILES ${CHESS_INC_CNT})
source_group("inc\\fo"        FILES ${CHESS_INC_FO})
source_group("inc\\gui"        FILES ${CHESS_INC_GUI})
source_group("inc\\thread"        FILES ${CHESS_INC_THREAD})
source_group("inc\\xml"        FILES ${CHESS_INC_XML})
source_group("src"            FILES ${CHESS_SOURCES})

target_link_libraries(${CHESS_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${CHESS_NAME} ${CHESS_PLIST})

setAppIcon(${CHESS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${CHESS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${CHESS_NAME})
