set(SNAKE_NAME Snake)				#Naziv prvog projekta u solution-u

file(GLOB SNAKE_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SNAKE_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB SNAKE_INC_TD  ${MY_INC}/td/*.h)
file(GLOB SNAKE_INC_GUI ${MY_INC}/gui/*.h)
#Application icon
set(SNAKE_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(SNAKE_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(SNAKE_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${SNAKE_NAME} ${SNAKE_INCS} ${SNAKE_SOURCES} ${SNAKE_INC_TD} ${SNAKE_INC_GUI} ${SNAKE_WINAPP_ICON})

source_group("inc"            FILES ${SNAKE_INCS})
source_group("inc\\td"            FILES ${SNAKE_INC_TD})
source_group("inc\\gui"            FILES ${SNAKE_INC_GUI})
source_group("src"            FILES ${SNAKE_SOURCES})

target_link_libraries(${SNAKE_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${SNAKE_NAME} ${SNAKE_PLIST})

setAppIcon(${SNAKE_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${SNAKE_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${SNAKE_NAME})
