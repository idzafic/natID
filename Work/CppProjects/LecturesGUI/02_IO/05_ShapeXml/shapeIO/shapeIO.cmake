set(SHAPE_IO_NAME shapeIO)	

file(GLOB SHAPE_IO_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SHAPE_IO_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB SHAPE_IO_COMMON_INCS  ${CMAKE_CURRENT_LIST_DIR}/../common/include/*.h)

file(GLOB SHAPE_IO_TD  ${MY_INC}/td/*.h)
file(GLOB SHAPE_IO_CNT  ${MY_INC}/cnt/*.h)
file(GLOB SHAPE_IO_MU  ${MY_INC}/mu/*.h)
file(GLOB SHAPE_IO_FO ${MY_INC}/fo/*.h)
file(GLOB SHAPE_IO_DP ${MY_INC}/dp/*.h)
file(GLOB SHAPE_IO_XML ${MY_INC}/xml/*.h)


add_library(${SHAPE_IO_NAME} SHARED ${SHAPE_IO_SOURCES} ${SHAPE_IO_INCS} ${SHAPE_IO_COMMON_INCS} ${SHAPE_IO_TD} 
										${SHAPE_IO_CNT} ${SHAPE_IO_MU} ${SHAPE_IO_FO} ${SHAPE_IO_DP} ${SHAPE_IO_XML})
										
#formatirati foldere u IDE.u
source_group("inc"            		FILES ${SHAPE_IO_INCS})
source_group("inc\\common"  		FILES ${SHAPE_IO_COMMON_INCS})

source_group("inc\\td"            	FILES ${SHAPE_IO_TD})
source_group("inc\\cnt"            	FILES ${SHAPE_IO_CNT})
source_group("inc\\mu"            	FILES ${SHAPE_IO_MU})
source_group("inc\\fo"            	FILES ${SHAPE_IO_FO})
source_group("inc\\dp"            	FILES ${SHAPE_IO_DP})
source_group("inc\\xml"            	FILES ${SHAPE_IO_XML})

source_group("src"   				FILES ${SHAPE_IO_SOURCES})

target_link_libraries(${SHAPE_IO_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}) 
#Shared library (dll, so, dylib) ekportuje simbole iz ovog projekta
#Napomena: svaki DLL mora ibazi svoju unikatnu _EXPORTS definiciju
#Ovo ide samo u SHARED library subprojects
target_compile_definitions(${SHAPE_IO_NAME} PUBLIC SHAPE_IO_EXPORTS)
