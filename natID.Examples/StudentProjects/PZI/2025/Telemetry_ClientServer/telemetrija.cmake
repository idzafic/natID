set(TELEMETRIJA_NAME telemetrija)				#Naziv prvog projekta u solution-u

file(GLOB TELEMETRIJA_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TELEMETRIJA_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TELEMETRIJA_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TELEMETRIJA_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TELEMETRIJA_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TELEMETRIJA_INC_NO ${MY_INC}/no/*.h)
file(GLOB TELEMETRIJA_INC_DP ${MY_INC}/dp/*.h)

# add executable
add_executable(${TELEMETRIJA_NAME} ${TELEMETRIJA_INCS} ${TELEMETRIJA_SOURCES} ${TELEMETRIJA_INC_TD}  ${TELEMETRIJA_INC_GUI} ${TELEMETRIJA_INC_NO} ${TELEMETRIJA_INC_DP})

source_group("inc"            FILES ${TELEMETRIJA_INCS})
source_group("src"            FILES ${TELEMETRIJA_SOURCES})
source_group("inc\\td"        FILES ${TELEMETRIJA_INC_TD})
source_group("inc\\gui"        FILES ${TELEMETRIJA_INC_GUI})
source_group("inc\\no"        FILES ${TELEMETRIJA_INC_NO})
source_group("inc\\dp"         FILES ${TELEMETRIJA_INC_DP})

target_link_libraries(${TELEMETRIJA_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} debug ${NETOP_LIB_DEBUG} optimized ${NETOP_LIB_RELEASE} debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE})

setTargetPropertiesForGUIApp(${TELEMETRIJA_NAME} ${TELEMETRIJA_PLIST})

setIDEPropertiesForGUIExecutable(${TELEMETRIJA_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TELEMETRIJA_NAME})
