set(HORIZONTAL_NAVIGATOR_NAME testNavigatorHorizontal)				#Naziv prvog projekta u solution-u

file(GLOB HORIZONTAL_NAVIGATOR_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB HORIZONTAL_NAVIGATOR_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(HORIZONTAL_NAVIGATOR_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB HORIZONTAL_NAVIGATOR_INC_TD  ${MY_INC}/td/*.h)
file(GLOB HORIZONTAL_NAVIGATOR_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${HORIZONTAL_NAVIGATOR_NAME} ${HORIZONTAL_NAVIGATOR_INCS} ${HORIZONTAL_NAVIGATOR_SOURCES} ${HORIZONTAL_NAVIGATOR_INC_TD}  ${HORIZONTAL_NAVIGATOR_INC_GUI})

source_group("inc"            FILES ${HORIZONTAL_NAVIGATOR_INCS})
source_group("src"            FILES ${HORIZONTAL_NAVIGATOR_SOURCES})
source_group("inc\\td"        FILES ${HORIZONTAL_NAVIGATOR_INC_TD})
source_group("inc\\gui"        FILES ${HORIZONTAL_NAVIGATOR_INC_GUI})


target_link_libraries(${HORIZONTAL_NAVIGATOR_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
								optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${HORIZONTAL_NAVIGATOR_NAME} ${HORIZONTAL_NAVIGATOR_PLIST})

setIDEPropertiesForGUIExecutable(${HORIZONTAL_NAVIGATOR_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${HORIZONTAL_NAVIGATOR_NAME})
