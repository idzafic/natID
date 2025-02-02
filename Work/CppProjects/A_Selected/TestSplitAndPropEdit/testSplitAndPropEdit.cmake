set(TESTSPLITANDPROPEDIT_NAME testSplitAndPropEdit)				#Naziv prvog projekta u solution-u

file(GLOB TESTSPLITANDPROPEDIT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTSPLITANDPROPEDIT_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTSPLITANDPROPEDIT_INC_MODEL  ${CMAKE_CURRENT_LIST_DIR}/src/model/*.h)
file(GLOB TESTSPLITANDPROPEDIT_INC_PROPERTY  ${CMAKE_CURRENT_LIST_DIR}/src/property/*.h)
file(GLOB TESTSPLITANDPROPEDIT_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTSPLITANDPROPEDIT_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(TESTSPLITANDPROPEDIT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(TESTSPLITANDPROPEDIT_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(TESTSPLITANDPROPEDIT_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${TESTSPLITANDPROPEDIT_NAME} ${TESTSPLITANDPROPEDIT_INCS} ${TESTSPLITANDPROPEDIT_INC_MODEL}  ${TESTSPLITANDPROPEDIT_INC_PROPERTY}  ${TESTSPLITANDPROPEDIT_SOURCES}
			${TESTSPLITANDPROPEDIT_INC_TD}  ${TESTSPLITANDPROPEDIT_INC_GUI}  ${TESTSPLITANDPROPEDIT_WINAPP_ICON})

source_group("inc"            FILES ${TESTSPLITANDPROPEDIT_INCS})
source_group("inc\\model"            FILES ${TESTSPLITANDPROPEDIT_INC_MODEL})
source_group("inc\\property"            FILES ${TESTSPLITANDPROPEDIT_INC_PROPERTY})
source_group("inc\\td"        FILES ${TESTSPLITANDPROPEDIT_INC_TD})
source_group("inc\\gui"        FILES ${TESTSPLITANDPROPEDIT_INC_GUI})
source_group("src"            FILES ${TESTSPLITANDPROPEDIT_SOURCES})

target_link_libraries(${TESTSPLITANDPROPEDIT_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TESTSPLITANDPROPEDIT_NAME} ${TESTSPLITANDPROPEDIT_PLIST})

setAppIcon(${TESTSPLITANDPROPEDIT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${TESTSPLITANDPROPEDIT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTSPLITANDPROPEDIT_NAME})
