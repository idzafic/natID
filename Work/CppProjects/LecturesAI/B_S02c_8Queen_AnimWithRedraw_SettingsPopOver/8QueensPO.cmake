set(THE8QEENS_PO_NAME eightQueensPO)				#Naziv prvog projekta u solution-u

file(GLOB THE8QEENS_PO_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB THE8QEENS_PO_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB THE8QEENS_PO_INC_TD  ${MY_INC}/td/*.h)
file(GLOB THE8QEENS_PO_INC_GUI ${MY_INC}/gui/*.h)

file(GLOB THE8QEENS_PO_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB THE8QEENS_PO_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB THE8QEENS_PO_INC_FO  ${MY_INC}/fo/*.h)
file(GLOB THE8QEENS_PO_INC_XML  ${MY_INC}/xml/*.h)

#Application icon
set(THE8QEENS_PO_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(THE8QEENS_PO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(THE8QEENS_PO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${THE8QEENS_PO_NAME} ${THE8QEENS_PO_INCS} ${THE8QEENS_PO_SOURCES} ${THE8QEENS_PO_INC_TD} ${THE8QEENS_PO_INC_THREAD} 
				${THE8QEENS_PO_INC_CNT} ${THE8QEENS_PO_INC_FO} ${THE8QEENS_PO_INC_GUI} ${THE8QEENS_PO_INC_XML} ${THE8QEENS_PO_WINAPP_ICON})

source_group("inc"            FILES ${THE8QEENS_PO_INCS})
source_group("inc\\td"        FILES ${THE8QEENS_PO_INC_TD})
source_group("inc\\cnt"        FILES ${THE8QEENS_PO_INC_CNT})
source_group("inc\\fo"        FILES ${THE8QEENS_PO_INC_FO})
source_group("inc\\gui"        FILES ${THE8QEENS_PO_INC_GUI})
source_group("inc\\thread"        FILES ${THE8QEENS_PO_INC_THREAD})
source_group("inc\\xml"        FILES ${THE8QEENS_PO_INC_XML})
source_group("src"            FILES ${THE8QEENS_PO_SOURCES})

target_link_libraries(${THE8QEENS_PO_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${THE8QEENS_PO_NAME} ${THE8QEENS_PO_PLIST})

setAppIcon(${THE8QEENS_PO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${THE8QEENS_PO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${THE8QEENS_PO_NAME})
