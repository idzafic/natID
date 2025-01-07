set(APPDEMOGRID_NAME appDemoGrid)				#Naziv prvog projekta u solution-u

file(GLOB APPDEMOGRID_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB APPDEMOGRID_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(APPDEMOGRID_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB APPDEMOGRID_INC_TD  ${MY_INC}/td/*.h)
file(GLOB APPDEMOGRID_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${APPDEMOGRID_NAME} ${APPDEMOGRID_INCS} ${APPDEMOGRID_SOURCES} ${APPDEMOGRID_INC_TD}  ${APPDEMOGRID_INC_GUI})

source_group("inc"            FILES ${APPDEMOGRID_INCS})
source_group("inc\\td"        FILES ${APPDEMOGRID_INC_TD})
source_group("inc\\gui"        FILES ${APPDEMOGRID_INC_GUI})
source_group("src"            FILES ${APPDEMOGRID_SOURCES})

target_link_libraries(${APPDEMOGRID_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${APPDEMOGRID_NAME} ${APPDEMOGRID_PLIST})

setIDEPropertiesForGUIExecutable(${APPDEMOGRID_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${APPDEMOGRID_NAME})
