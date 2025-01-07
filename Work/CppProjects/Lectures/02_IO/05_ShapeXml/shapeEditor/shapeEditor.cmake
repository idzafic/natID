set(SHAPE_EDITOR_NAME shapeEditor)	

file(GLOB SHAPE_EDITOR_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SHAPE_EDITOR_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB SHAPE_EDITOR_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/../common/include/*.h)

file(GLOB SHAPE_EDITOR_TD  ${MY_INC}/td/*.h)
file(GLOB SHAPE_EDITOR_CNT  ${MY_INC}/cnt/*.h)
file(GLOB SHAPE_EDITOR_MU  ${MY_INC}/mu/*.h)
file(GLOB SHAPE_EDITOR_FO ${MY_INC}/fo/*.h)
file(GLOB SHAPE_EDITOR_DP ${MY_INC}/dp/*.h)
file(GLOB SHAPE_EDITOR_XML ${MY_INC}/xml/*.h)
file(GLOB SHAPE_EDITOR_ARCH ${MY_INC}/arch/*.h)

add_executable(${SHAPE_EDITOR_NAME} ${SHAPE_EDITOR_SOURCES} ${SHAPE_EDITOR_INCS} ${SHAPE_EDITOR_COMMON_INCS} ${SHAPE_EDITOR_TD} 
										${SHAPE_EDITOR_CNT} ${SHAPE_EDITOR_MU} ${SHAPE_EDITOR_FO} ${SHAPE_EDITOR_DP} ${SHAPE_EDITOR_XML}
										${SHAPE_EDITOR_ARCH}) 

#formatirati foldere u IDE.u
source_group("inc"            		FILES ${SHAPE_EDITOR_INCS})
source_group("inc\\common"  		FILES ${SHAPE_EDITOR_COMMON_INCS})

source_group("inc\\td"            	FILES ${SHAPE_EDITOR_TD})
source_group("inc\\cnt"            	FILES ${SHAPE_EDITOR_CNT})
source_group("inc\\mu"            	FILES ${SHAPE_EDITOR_MU})
source_group("inc\\fo"            	FILES ${SHAPE_EDITOR_FO})
source_group("inc\\dp"            	FILES ${SHAPE_EDITOR_DP})
source_group("inc\\xml"            	FILES ${SHAPE_EDITOR_XML})
source_group("inc\\arch"           	FILES ${SHAPE_EDITOR_ARCH})

source_group("src"   				FILES ${SHAPE_EDITOR_SOURCES})

#target_link_libraries(${SHAPE_EDITOR_NAME} ${MU_LIB_DEBUG}) 

target_link_libraries(${SHAPE_EDITOR_NAME} ${SHAPE_IO_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}) 
