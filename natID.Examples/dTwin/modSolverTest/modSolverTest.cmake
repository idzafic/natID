set(MODSOLVER_TEST_NAME modSolverTest)				#Naziv prvog projekta u solution-u

file(GLOB MODSOLVER_TEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MODSOLVER_TEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB MODSOLVER_TEST_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB MODSOLVER_TEST_INC_CNT  ${NATID_SDK_INC}/cnt/*.h)
file(GLOB MODSOLVER_TEST_INC_MU  ${NATID_SDK_INC}/mu/*.h)
file(GLOB MODSOLVER_TEST_INC_SC  ${NATID_SDK_INC}/sc/*.h)
file(GLOB MODSOLVER_TEST_INC_SC_PRIV  ${NATID_SDK_INC}/sc/priv/*.h)
file(GLOB MODSOLVER_TEST_INC_MATH  ${NATID_SDK_INC}/math/*.h)
file(GLOB MODSOLVER_TEST_INC_MEM  ${NATID_SDK_INC}/mem/*.h)
file(GLOB MODSOLVER_TEST_INC_RND  ${NATID_SDK_INC}/rnd/*.h)
file(GLOB MODSOLVER_TEST_INC_RND_PRIV  ${NATID_SDK_INC}/rnd/priv/*.h)

file(GLOB MODSOLVER_TEST_INC_SYST ${NATID_SDK_INC}/syst/*.h)

# add executable
add_executable(${MODSOLVER_TEST_NAME} ${MODSOLVER_TEST_INCS} ${MODSOLVER_TEST_SOURCES} 
				${MODSOLVER_TEST_INC_TD} ${MODSOLVER_TEST_INC_CNT} ${MODSOLVER_TEST_INC_MU} 
				${MODSOLVER_TEST_INC_MATH} ${MODSOLVER_TEST_INC_MEM} ${MODSOLVER_TEST_INC_RND} ${MODSOLVER_TEST_INC_RND_PRIV}
				${MODSOLVER_TEST_INC_SC} ${MODSOLVER_TEST_INC_SC_PRIV} ${MODSOLVER_TEST_INC_SYST})

source_group("inc"             FILES ${MODSOLVER_TEST_INCS})
source_group("inc\\td"         FILES ${MODSOLVER_TEST_INC_TD})
source_group("inc\\cnt"        FILES ${MODSOLVER_TEST_INC_CNT})
source_group("inc\\mu"         FILES ${MODSOLVER_TEST_INC_MU})
source_group("inc\\sc"         FILES ${MODSOLVER_TEST_INC_SC})
source_group("inc\\sc\\priv"         FILES ${MODSOLVER_TEST_INC_SC_PRIV})
source_group("inc\\math"        FILES ${MODSOLVER_TEST_INC_MATH})
source_group("inc\\mem"        FILES ${MODSOLVER_TEST_INC_MEM})
source_group("inc\\rnd"        FILES ${MODSOLVER_TEST_INC_RND})
source_group("inc\\rnd\\priv"        FILES ${MODSOLVER_TEST_INC_RND_PRIV})
source_group("inc\\syst"       FILES ${MODSOLVER_TEST_INC_SYST})
source_group("src"             FILES ${MODSOLVER_TEST_SOURCES})

target_link_libraries(${MODSOLVER_TEST_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}
											debug ${SYMBCOMP_LIB_DEBUG} optimized ${SYMBCOMP_LIB_RELEASE}
											debug ${MODSOLVER_LIB_DEBUG} optimized ${MODSOLVER_LIB_RELEASE})

setIDEPropertiesForExecutable(${MODSOLVER_TEST_NAME})
