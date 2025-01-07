set(MYMATH_SHLIB_TEST_NAME myMathTestApp)	

file(GLOB MYMATH_SHLIB_TEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MYMATH_SHLIB_TEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB MYMATH_SHLIB_TEST_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/../common/include/*.h)

file(GLOB MYMATH_SHLIB_TEST_TD  ${MY_INC}/td/*.h)
file(GLOB MYMATH_SHLIB_TEST_CNT  ${MY_INC}/cnt/*.h)
file(GLOB MYMATH_SHLIB_TEST_MU  ${MY_INC}/mu/*.h)
file(GLOB MYMATH_SHLIB_TEST_FO ${MY_INC}/fo/*.h)
file(GLOB MYMATH_SHLIB_TEST_DP ${MY_INC}/dp/*.h)
file(GLOB MYMATH_SHLIB_TEST_XML ${MY_INC}/xml/*.h)

add_executable(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_TEST_SOURCES} ${MYMATH_SHLIB_TEST_INCS} ${MYMATH_SHLIB_TEST_COMMON_INCS} ${MYMATH_SHLIB_TEST_TD} 
										${MYMATH_SHLIB_TEST_CNT} ${MYMATH_SHLIB_TEST_MU} ${MYMATH_SHLIB_TEST_FO} ${MYMATH_SHLIB_TEST_DP} ${MYMATH_SHLIB_TEST_XML}) 

#formatirati foldere u IDE.u
source_group("inc"            		FILES ${MYMATH_SHLIB_TEST_INCS})
source_group("inc\\common"  		FILES ${MYMATH_SHLIB_TEST_COMMON_INCS})

source_group("inc\\td"            	FILES ${MYMATH_SHLIB_TEST_TD})
source_group("inc\\cnt"            	FILES ${MYMATH_SHLIB_TEST_CNT})
source_group("inc\\mu"            	FILES ${MYMATH_SHLIB_TEST_MU})
source_group("inc\\fo"            	FILES ${MYMATH_SHLIB_TEST_FO})
source_group("inc\\dp"            	FILES ${MYMATH_SHLIB_TEST_DP})
source_group("inc\\xml"            	FILES ${MYMATH_SHLIB_TEST_XML})

source_group("src"   				FILES ${MYMATH_SHLIB_TEST_SOURCES})

#target_link_libraries(${MYMATH_SHLIB_TEST_NAME} ${MU_LIB_DEBUG}) 

target_link_libraries(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}) 
