set(DIGIT_TWIN_TEST_NAME digitTwinTest)				#Naziv prvog projekta u solution-u

file(GLOB DIGIT_TWIN_TEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB DIGIT_TWIN_TEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB DIGIT_TWIN_TEST_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB DIGIT_TWIN_TEST_INC_SC  ${MY_INC}/sc/*.h)
file(GLOB DIGIT_TWIN_TEST_INC_TD  ${MY_INC}/td/*.h)
file(GLOB DIGIT_TWIN_TEST_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB DIGIT_TWIN_TEST_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(DIGIT_TWIN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${DIGIT_TWIN_TEST_NAME} ${DIGIT_TWIN_TEST_INCS} ${DIGIT_TWIN_TEST_SOURCES} ${DIGIT_TWIN_TEST_INC_TD}  
	${DIGIT_TWIN_TEST_INC_GUI} ${DIGIT_TWIN_TEST_INC_CNT} ${DIGIT_TWIN_TEST_INC_SC} ${DIGIT_TWIN_TEST_INC_THREAD}
	${DIGIT_TWIN_WINAPP_ICON})

source_group("inc"            FILES ${DIGIT_TWIN_TEST_INCS})
source_group("inc\\cnt"        FILES ${DIGIT_TWIN_TEST_INC_CNT})
source_group("inc\\sc"        FILES ${DIGIT_TWIN_TEST_INC_SC})
source_group("inc\\td"        FILES ${DIGIT_TWIN_TEST_INC_TD})
source_group("inc\\thread"    FILES ${DIGIT_TWIN_TEST_INC_THREAD})
source_group("inc\\gui"        FILES ${DIGIT_TWIN_TEST_INC_GUI})
source_group("src"            FILES ${DIGIT_TWIN_TEST_SOURCES})

target_link_libraries(${DIGIT_TWIN_TEST_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} debug ${SYMBCOMP_LIB_DEBUG}
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE} optimized ${SYMBCOMP_LIB_RELEASE})


target_compile_definitions(${DIGIT_TWIN_TEST_NAME} PUBLIC MU_SHOW_MEM_LEAKS)

setTargetPropertiesForGUIApp(${DIGIT_TWIN_TEST_NAME} ${DIGIT_TWIN_PLIST})

setAppIcon(${DIGIT_TWIN_TEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${DIGIT_TWIN_TEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${DIGIT_TWIN_TEST_NAME})
