set(SUPERMARIO_NAME superMario)				#Naziv prvog projekta u solution-u

file(GLOB SUPERMARIO_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SUPERMARIO_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

file(GLOB SUPERMARIO_INC_TD  ${MY_INC}/td/*.h)
file(GLOB SUPERMARIO_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(SUPERMARIO_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(SUPERMARIO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(SUPERMARIO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${SUPERMARIO_NAME} ${SUPERMARIO_INCS} ${SUPERMARIO_SOURCES} ${SUPERMARIO_INC_TD}  
                        ${SUPERMARIO_INC_GUI} ${SUPERMARIO_WINAPP_ICON})

source_group("inc"            FILES ${SUPERMARIO_INCS})
source_group("src"            FILES ${SUPERMARIO_SOURCES})
source_group("inc\\td"        FILES ${SUPERMARIO_INC_TD})
source_group("inc\\gui"        FILES ${SUPERMARIO_INC_GUI})

target_link_libraries(${SUPERMARIO_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${SUPERMARIO_NAME} ${SUPERMARIO_PLIST})

setAppIcon(${SUPERMARIO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${SUPERMARIO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${SUPERMARIO_NAME})
