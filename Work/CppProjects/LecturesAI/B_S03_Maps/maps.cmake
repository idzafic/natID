set(MAPS_NAME maps)				#Naziv prvog projekta u solution-u

file(GLOB MAPS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MAPS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB MAPS_INC_TD  ${MY_INC}/td/*.h)

file(GLOB MAPS_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB MAPS_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB MAPS_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB MAPS_INC_FO  ${MY_INC}/fo/*.h)
file(GLOB MAPS_INC_XML  ${MY_INC}/xml/*.h)

#Application icon
set(MAPS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(MAPS_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(MAPS_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${MAPS_NAME} ${MAPS_INCS} ${MAPS_SOURCES} ${MAPS_INC_TD} ${MAPS_INC_THREAD} 
				${MAPS_INC_CNT} ${MAPS_INC_FO} ${MAPS_INC_GUI} ${MAPS_INC_XML} ${MAPS_WINAPP_ICON})

source_group("inc"            FILES ${MAPS_INCS})
source_group("inc\\td"        FILES ${MAPS_INC_TD})
source_group("inc\\cnt"        FILES ${MAPS_INC_CNT})
source_group("inc\\fo"        FILES ${MAPS_INC_FO})
source_group("inc\\gui"        FILES ${MAPS_INC_GUI})
source_group("inc\\thread"        FILES ${MAPS_INC_THREAD})
source_group("inc\\xml"        FILES ${MAPS_INC_XML})
source_group("src"            FILES ${MAPS_SOURCES})

target_link_libraries(${MAPS_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${MAPS_NAME} ${MAPS_PLIST})

setAppIcon(${MAPS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${MAPS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${MAPS_NAME})
