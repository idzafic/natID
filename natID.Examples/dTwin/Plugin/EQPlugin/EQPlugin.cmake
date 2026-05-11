set(EQPLUGIN_NAME eq1d)				#Naziv prvog projekta u solution-u

file(GLOB EQPLUGIN_CPP_COMMON_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB EQPLUGIN_CPP_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB EQPLUGIN_INC_GUI  ${NATID_SDK_INC}/gui/*.h)
file(GLOB EQPLUGIN_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB EQPLUGIN_INC_CNT  ${NATID_SDK_INC}/cnt/*.h)
file(GLOB EQPLUGIN_INC_MU  ${NATID_SDK_INC}/mu/*.h)
file(GLOB EQPLUGIN_INC_MEM  ${NATID_SDK_INC}/mem/*.h)
file(GLOB EQPLUGIN_INC_FO ${NATID_SDK_INC}/fo/*.h)
file(GLOB EQPLUGIN_INC_SC ${NATID_SDK_INC}/sc/*.h)
file(GLOB EQPLUGIN_INC_SYST ${NATID_SDK_INC}/syst/*.h)
file(GLOB EQPLUGIN_INC_DENSE ${NATID_SDK_INC}/dense/*.h)
file(GLOB EQPLUGIN_INC_SPARSE ${NATID_SDK_INC}/sparse/*.h)

# add shared library (plugin is a shared executatable binary file)
add_library(${EQPLUGIN_NAME} SHARED ${EQPLUGIN_CPP_COMMON_SOURCES} ${EQPLUGIN_INC_GUI} ${EQPLUGIN_CPP_COMMON_INCS} 
							${EQPLUGIN_INC_TD} ${EQPLUGIN_INC_SYST} 
							${EQPLUGIN_INC_CNT} ${EQPLUGIN_INC_MU} ${EQPLUGIN_INC_MEM} ${EQPLUGIN_INC_FO}
							${EQPLUGIN_INC_SC} ${EQPLUGIN_INC_DENSE} ${EQPLUGIN_INC_SPARSE})

source_group("inc\\inc"        FILES ${EQPLUGIN_CPP_COMMON_INCS})
source_group("inc\\gui"        FILES ${EQPLUGIN_INC_GUI})
source_group("inc\\td"        FILES ${EQPLUGIN_INC_TD})
source_group("inc\\cnt"        FILES ${EQPLUGIN_INC_CNT})
source_group("inc\\dense"        FILES ${EQPLUGIN_INC_DENSE})
source_group("inc\\mu"        FILES ${EQPLUGIN_INC_MU})
source_group("inc\\mem"        FILES ${EQPLUGIN_INC_MEM})
source_group("inc\\fo"        FILES ${EQPLUGIN_INC_FO})
source_group("inc\\sc"        FILES ${EQPLUGIN_INC_SC})
source_group("inc\\sparse"        FILES ${EQPLUGIN_INC_SPARSE})
source_group("inc\\syst"        FILES ${EQPLUGIN_INC_SYST})

source_group("src\\cpp"			FILES ${EQPLUGIN_CPP_COMMON_SOURCES})

target_link_libraries(${EQPLUGIN_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
										debug ${MATRIX_LIB_DEBUG} optimized ${MATRIX_LIB_RELEASE}
									  debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE})
									
target_compile_definitions(${EQPLUGIN_NAME} PUBLIC PLUGIN_EXPORTS)

setIDEPropertiesForLib(${EQPLUGIN_NAME})
