set(APPWND_NAME appWnd)				#Naziv prvog projekta u solution-u

file(GLOB APPWND_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB APPWND_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(APPWND_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB APPWND_INC_TD  ${MY_INC}/td/*.h)
file(GLOB APPWND_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${APPWND_NAME} ${APPWND_SOURCES} ${APPWND_INCS} ${APPWND_INC_MODEL} ${APPWND_INC_TD} ${APPWND_INC_GUI})

source_group("inc"            FILES ${APPWND_INCS})
source_group("inc\\td"        FILES ${APPWND_INC_TD})
source_group("inc\\gui"        FILES ${APPWND_INC_GUI})
source_group("src"            FILES ${APPWND_SOURCES})

target_link_libraries(${APPWND_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
								optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${APPWND_NAME} ${APPWND_PLIST})

setIDEPropertiesForGUIExecutable(${APPWND_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${APPWND_NAME})
