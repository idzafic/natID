set(PRODAJA_NAME prodaja)	

file(GLOB PRODAJA_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB PRODAJA_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB PRODAJA_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/../common/include/*.h)

file(GLOB PRODAJA_TD  ${MY_INC}/td/*.h)
file(GLOB PRODAJA_CNT  ${MY_INC}/cnt/*.h)
file(GLOB PRODAJA_MU  ${MY_INC}/mu/*.h)
file(GLOB PRODAJA_FO ${MY_INC}/fo/*.h)
file(GLOB PRODAJA_DP ${MY_INC}/dp/*.h)
file(GLOB PRODAJA_XML ${MY_INC}/xml/*.h)
file(GLOB PRODAJA_ARCH ${MY_INC}/arch/*.h)

add_executable(${PRODAJA_NAME} ${PRODAJA_SOURCES} ${PRODAJA_INCS} ${PRODAJA_COMMON_INCS} ${PRODAJA_TD} 
										${PRODAJA_CNT} ${PRODAJA_MU} ${PRODAJA_FO} ${PRODAJA_DP} ${PRODAJA_XML}
										${PRODAJA_ARCH}) 

#formatirati foldere u IDE.u
source_group("inc"            		FILES ${PRODAJA_INCS})
source_group("inc\\common"  		FILES ${PRODAJA_COMMON_INCS})

source_group("inc\\td"            	FILES ${PRODAJA_TD})
source_group("inc\\cnt"            	FILES ${PRODAJA_CNT})
source_group("inc\\mu"            	FILES ${PRODAJA_MU})
source_group("inc\\fo"            	FILES ${PRODAJA_FO})
source_group("inc\\dp"            	FILES ${PRODAJA_DP})
source_group("inc\\xml"            	FILES ${PRODAJA_XML})
source_group("inc\\arch"           	FILES ${PRODAJA_ARCH})

source_group("src"   				FILES ${PRODAJA_SOURCES})

#target_link_libraries(${PRODAJA_NAME} ${MU_LIB_DEBUG}) 

target_link_libraries(${PRODAJA_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE}) 
