set(APPDEMOVERT_NAME appDemoVert)				#Naziv prvog projekta u solution-u

file(GLOB APPDEMOVERT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB APPDEMOVERT_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(APPDEMOVERT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB APPDEMOVERT_INC_TD  ${MY_INC}/td/*.h)
file(GLOB APPDEMOVERT_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${APPDEMOVERT_NAME} ${APPDEMOVERT_INCS} ${APPDEMOVERT_SOURCES} ${APPDEMOVERT_INC_TD}  ${APPDEMOVERT_INC_GUI})

source_group("inc"            FILES ${APPDEMOVERT_INCS})
source_group("inc\\td"        FILES ${APPDEMOVERT_INC_TD})
source_group("inc\\gui"        FILES ${APPDEMOVERT_INC_GUI})
source_group("src"            FILES ${APPDEMOVERT_SOURCES})

target_link_libraries(${APPDEMOVERT_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${APPDEMOVERT_NAME} ${APPDEMOVERT_PLIST})

setIDEPropertiesForGUIExecutable(${APPDEMOVERT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${APPDEMOVERT_NAME})
