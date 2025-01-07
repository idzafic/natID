set(TESTTHREAD_NAME testThread)				#Naziv prvog projekta u solution-u

file(GLOB TESTTHREAD_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTTHREAD_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTTHREAD_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTTHREAD_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTTHREAD_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${TESTTHREAD_NAME} ${TESTTHREAD_INCS} ${TESTTHREAD_SOURCES} ${TESTTHREAD_INC_TD}  ${TESTTHREAD_INC_GUI})

source_group("inc"            FILES ${TESTTHREAD_INCS})
source_group("inc\\td"        FILES ${TESTTHREAD_INC_TD})
source_group("inc\\gui"        FILES ${TESTTHREAD_INC_GUI})
source_group("src"            FILES ${TESTTHREAD_SOURCES})

target_link_libraries(${TESTTHREAD_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTTHREAD_NAME} ${TESTTHREAD_PLIST})

setIDEPropertiesForGUIExecutable(${TESTTHREAD_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTTHREAD_NAME})
