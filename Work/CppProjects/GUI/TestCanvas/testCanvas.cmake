set(CANVASTEST_NAME testCanvas)				#Naziv prvog projekta u solution-u

file(GLOB CANVASTEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB CANVASTEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(CANVASTEST_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB CANVASTEST_INC_TD  ${MY_INC}/td/*.h)
file(GLOB CANVASTEST_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${CANVASTEST_NAME} ${CANVASTEST_INCS} ${CANVASTEST_SOURCES} ${CANVASTEST_INC_TD}  ${CANVASTEST_INC_GUI})

source_group("inc"            FILES ${CANVASTEST_INCS})
source_group("src"            FILES ${CANVASTEST_SOURCES})
source_group("inc\\td"        FILES ${CANVASTEST_INC_TD})
source_group("inc\\gui"        FILES ${CANVASTEST_INC_GUI})

target_link_libraries(${CANVASTEST_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${CANVASTEST_NAME} ${CANVASTEST_PLIST})

setIDEPropertiesForGUIExecutable(${CANVASTEST_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${CANVASTEST_NAME})
