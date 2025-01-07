set(TESTTIMER_NAME testTimer)				#Naziv prvog projekta u solution-u

file(GLOB TESTTIMER_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTTIMER_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTTIMER_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTTIMER_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTTIMER_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TESTTIMER_NAME} ${TESTTIMER_INCS} ${TESTTIMER_SOURCES} ${TESTTIMER_INC_TD}  ${TESTTIMER_INC_GUI})

source_group("inc"            FILES ${TESTTIMER_INCS})
source_group("inc\\td"        FILES ${TESTTIMER_INC_TD})
source_group("inc\\gui"        FILES ${TESTTIMER_INC_GUI})
source_group("src"            FILES ${TESTTIMER_SOURCES})

target_link_libraries(${TESTTIMER_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTTIMER_NAME} ${TESTTIMER_PLIST})

setIDEPropertiesForGUIExecutable(${TESTTIMER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTTIMER_NAME})
