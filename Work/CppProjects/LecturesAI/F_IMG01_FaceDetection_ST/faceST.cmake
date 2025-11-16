set(FACEDETECTION_NAME FaceDetectionST)				#Naziv prvog projekta u solution-u

file(GLOB FACEDETECTION_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB FACEDETECTION_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB FACEDETECTION_INC_TD  ${MY_INC}/td/*.h)
file(GLOB FACEDETECTION_INC_GUI ${MY_INC}/gui/*.h)

file(GLOB FACEDETECTION_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB FACEDETECTION_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB FACEDETECTION_INC_RND  ${MY_INC}/rnd/*.h)
file(GLOB FACEDETECTION_INC_FO  ${MY_INC}/fo/*.h)
file(GLOB FACEDETECTION_INC_XML  ${MY_INC}/xml/*.h)

#Application icon
set(FACEDETECTION_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(FACEDETECTION_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(FACEDETECTION_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${FACEDETECTION_NAME} ${FACEDETECTION_INCS} ${FACEDETECTION_SOURCES} ${FACEDETECTION_INC_TD} ${FACEDETECTION_INC_THREAD} 
				${FACEDETECTION_INC_CNT} ${FACEDETECTION_INC_FO} ${FACEDETECTION_INC_GUI} ${FACEDETECTION_INC_RND} 
				${FACEDETECTION_INC_XML} ${FACEDETECTION_WINAPP_ICON})

source_group("inc"            FILES ${FACEDETECTION_INCS})
source_group("inc\\td"        FILES ${FACEDETECTION_INC_TD})
source_group("inc\\cnt"        FILES ${FACEDETECTION_INC_CNT})
source_group("inc\\fo"        FILES ${FACEDETECTION_INC_FO})
source_group("inc\\gui"        FILES ${FACEDETECTION_INC_GUI})
source_group("inc\\rnd"        FILES ${FACEDETECTION_INC_RND})
source_group("inc\\thread"        FILES ${FACEDETECTION_INC_THREAD})
source_group("inc\\xml"        FILES ${FACEDETECTION_INC_XML})
source_group("src"            FILES ${FACEDETECTION_SOURCES})

target_link_libraries(${FACEDETECTION_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${FACEDETECTION_NAME} ${FACEDETECTION_PLIST})

setAppIcon(${FACEDETECTION_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${FACEDETECTION_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${FACEDETECTION_NAME})
