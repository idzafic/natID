set(MYMATH_SHLIB_NAME myMath)	

file(GLOB MYMATH_SHLIB_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MYMATH_SHLIB_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB MYMATH_SHLIB_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/../common/include/*.h)

file(GLOB MYMATH_SHLIB_TD  ${MY_INC}/td/*.h)
file(GLOB MYMATH_SHLIB_CNT  ${MY_INC}/cnt/*.h)
file(GLOB MYMATH_SHLIB_MU  ${MY_INC}/mu/*.h)
file(GLOB MYMATH_SHLIB_FO ${MY_INC}/fo/*.h)
file(GLOB MYMATH_SHLIB_DP ${MY_INC}/dp/*.h)
file(GLOB MYMATH_SHLIB_XML ${MY_INC}/xml/*.h)


add_library(${MYMATH_SHLIB_NAME} SHARED ${MYMATH_SHLIB_SOURCES} ${MYMATH_SHLIB_INCS} ${MYMATH_SHLIB_COMMON_INCS} ${MYMATH_SHLIB_TD} 
										${MYMATH_SHLIB_CNT} ${MYMATH_SHLIB_MU} ${MYMATH_SHLIB_FO} ${MYMATH_SHLIB_DP} ${MYMATH_SHLIB_XML})
										
#formatirati foldere u IDE.u
source_group("inc"            		FILES ${MYMATH_SHLIB_INCS})
source_group("inc\\common"  		FILES ${MYMATH_SHLIB_COMMON_INCS})

source_group("inc\\td"            	FILES ${MYMATH_SHLIB_TD})
source_group("inc\\cnt"            	FILES ${MYMATH_SHLIB_CNT})
source_group("inc\\mu"            	FILES ${MYMATH_SHLIB_MU})
source_group("inc\\fo"            	FILES ${MYMATH_SHLIB_FO})
source_group("inc\\dp"            	FILES ${MYMATH_SHLIB_DP})
source_group("inc\\xml"            	FILES ${MYMATH_SHLIB_XML})

source_group("src"   				FILES ${MYMATH_SHLIB_SOURCES})

target_link_libraries(${MYMATH_SHLIB_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}) 
#Shared library (dll, so, dylib) ekportuje simbole iz ovog projekta
#Napomena: svaki DLL mora ibazi svoju unikatnu _EXPORTS definiciju
#Ovo ide samo u SHARED library subprojects
target_compile_definitions(${MYMATH_SHLIB_NAME} PUBLIC MYMATH_SHLIB_EXPORTS)
