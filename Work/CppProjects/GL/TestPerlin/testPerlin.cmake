set(TESTPERLIN_NAME TestPerlin)  #Naziv projekta u solution-u

file(GLOB TESTPERLIN_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTPERLIN_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTPERLIN_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTPERLIN_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB TESTPERLIN_INC_FO ${MY_INC}/fo/*.h)
file(GLOB TESTPERLIN_INC_RND ${MY_INC}/rnd/*.h)
file(GLOB TESTPERLIN_INC_SYST ${MY_INC}/syst/*.h)

# add executable
add_executable(${TESTPERLIN_NAME} ${TESTPERLIN_INCS} ${TESTPERLIN_SOURCES} ${TESTPERLIN_INC_TD} 
                ${TESTPERLIN_INC_CNT}  ${TESTPERLIN_INC_FO} ${TESTPERLIN_INC_RND} ${TESTPERLIN_INC_SYST})

source_group("inc"            FILES ${TESTPERLIN_INCS})
source_group("inc\\td"        FILES ${TESTPERLIN_INC_TD})
source_group("inc\\cnt"        FILES ${TESTPERLIN_INC_CNT})
source_group("inc\\fo"        FILES ${TESTPERLIN_INC_FO})
source_group("inc\\rnd"        FILES ${TESTPERLIN_INC_RND})
source_group("inc\\syst"        FILES ${TESTPERLIN_INC_SYST})
source_group("src"            FILES ${TESTPERLIN_SOURCES})

target_link_libraries(${TESTPERLIN_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE})

setIDEPropertiesForExecutable(${TESTPERLIN_NAME})
