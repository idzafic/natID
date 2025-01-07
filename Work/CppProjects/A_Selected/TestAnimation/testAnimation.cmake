set(TESTANIMATION_NAME testAnimation)				#Naziv prvog projekta u solution-u

file(GLOB TESTANIMATION_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTANIMATION_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTANIMATION_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTANIMATION_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(TESTANIMATION_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TESTANIMATION_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TESTANIMATION_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TESTANIMATION_NAME} ${TESTANIMATION_INCS} ${TESTANIMATION_SOURCES} ${TESTANIMATION_INC_TD} 
				${TESTANIMATION_INC_GUI} ${TESTANIMATION_WINAPP_ICON})

source_group("inc"            FILES ${TESTANIMATION_INCS})
source_group("inc\\td"        FILES ${TESTANIMATION_INC_TD})
source_group("inc\\gui"        FILES ${TESTANIMATION_INC_GUI})
source_group("src"            FILES ${TESTANIMATION_SOURCES})

target_link_libraries(${TESTANIMATION_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTANIMATION_NAME} ${TESTANIMATION_PLIST})

setAppIcon(${TESTANIMATION_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TESTANIMATION_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTANIMATION_NAME})
