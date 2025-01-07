set(TABLEEDITTEST_NAME testTableEdit)				#Naziv prvog projekta u solution-u

file(GLOB TABLEEDITTEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TABLEEDITTEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TABLEEDITTEST_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TABLEEDITTEST_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TABLEEDITTEST_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TABLEEDITTEST_NAME} ${TABLEEDITTEST_INCS} ${TABLEEDITTEST_SOURCES} ${TABLEEDITTEST_INC_TD}  ${TABLEEDITTEST_INC_GUI})

source_group("inc"            FILES ${TABLEEDITTEST_INCS})
source_group("src"            FILES ${TABLEEDITTEST_SOURCES})
source_group("inc\\td"        FILES ${TABLEEDITTEST_INC_TD})
source_group("inc\\gui"        FILES ${TABLEEDITTEST_INC_GUI})

target_link_libraries(${TABLEEDITTEST_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TABLEEDITTEST_NAME} ${TABLEEDITTEST_PLIST})

setIDEPropertiesForGUIExecutable(${TABLEEDITTEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TABLEEDITTEST_NAME})
