set(MYMATH_SHLIB_NAME myMath)	

add_library(${MYMATH_SHLIB_NAME} SHARED ${CMAKE_CURRENT_LIST_DIR}/src/Operacije.cpp 
										${CMAKE_CURRENT_LIST_DIR}/src/MyMath.cpp 
										${CMAKE_CURRENT_LIST_DIR}/../common/include/MyMath.h)