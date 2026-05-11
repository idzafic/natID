set(TESTEQPLUGIN_NAME testEQPlugin)		

file(GLOB TESTEQPLUGIN_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTEQPLUGIN_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTEQPLUGIN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTEQPLUGIN_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB TESTEQPLUGIN_INC_GUI ${NATID_SDK_INC}/gui/*.h)
file(GLOB TESTEQPLUGIN_INC_SYST ${NATID_SDK_INC}/syst/*.h)
file(GLOB TESTEQPLUGIN_INC_SC ${NATID_SDK_INC}/sc/*.h)
file(GLOB TESTEQPLUGIN_INC_FO ${NATID_SDK_INC}/fo/*.h)
file(GLOB TESTEQPLUGIN_INC_ARCH ${NATID_SDK_INC}/arch/*.h)

# add executable
add_executable(${TESTEQPLUGIN_NAME} ${TESTEQPLUGIN_INCS} ${TESTEQPLUGIN_SOURCES} ${TESTEQPLUGIN_INC_TD} 
			${TESTEQPLUGIN_INC_GUI} ${TESTEQPLUGIN_INC_SYST} ${TESTEQPLUGIN_INC_SC} ${TESTEQPLUGIN_INC_FO} ${TESTEQPLUGIN_INC_ARCH} )

source_group("inc"            FILES ${TESTEQPLUGIN_INCS})
source_group("inc\\td"        FILES ${TESTEQPLUGIN_INC_TD})
source_group("inc\\gui"        FILES ${TESTEQPLUGIN_INC_GUI})
source_group("inc\\arch"        FILES ${TESTEQPLUGIN_INC_ARCH})
source_group("inc\\fo"        FILES ${TESTEQPLUGIN_INC_FO})
source_group("inc\\sc"        FILES ${TESTEQPLUGIN_INC_SC})
source_group("inc\\syst"        FILES ${TESTEQPLUGIN_INC_SYST})

source_group("src"            FILES ${TESTEQPLUGIN_SOURCES})

target_link_libraries(${TESTEQPLUGIN_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

set(PLUGIN_DIR_LOG "$<$<CONFIG:Release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>$<$<CONFIG:Debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}>")

# Pass it as a macro definition to the application code
target_compile_definitions(${TESTEQPLUGIN_NAME} PRIVATE 
    PLUGIN_DIR="${PLUGIN_DIR_LOG}"
)

setTargetPropertiesForGUIApp(${TESTEQPLUGIN_NAME} ${TESTEQPLUGIN_PLIST})

setIDEPropertiesForGUIExecutable(${TESTEQPLUGIN_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTEQPLUGIN_NAME})
