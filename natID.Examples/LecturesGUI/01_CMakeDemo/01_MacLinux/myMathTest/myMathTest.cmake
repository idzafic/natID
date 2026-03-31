set(MYMATH_SHLIB_TEST_NAME myMathTestApp)	

add_executable(${MYMATH_SHLIB_TEST_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp 
										${CMAKE_CURRENT_LIST_DIR}/../common/include/MyMath.h) 

target_link_libraries(${MYMATH_SHLIB_TEST_NAME} ${MYMATH_SHLIB_NAME}) 
