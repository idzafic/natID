set(MINESWEEPER_NAME mineSweeper)				#Naziv prvog projekta u solution-u


file(GLOB MINESWEEPER_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MINESWEEPER_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
#Application icon
set(MINESWEEPER_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(MINESWEEPER_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(MINESWEEPER_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${MINESWEEPER_NAME} ${MINESWEEPER_INCS} ${MINESWEEPER_SOURCES} ${MINESWEEPER_WINAPP_ICON})

source_group("inc"            FILES ${MINESWEEPER_INCS})
source_group("src"            FILES ${MINESWEEPER_SOURCES})

target_link_libraries(${MINESWEEPER_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${MINESWEEPER_NAME} ${MINESWEEPER_PLIST})

setAppIcon(${MINESWEEPER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${MINESWEEPER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${MINESWEEPER_NAME})

