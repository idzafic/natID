set(TESTDENSE_NAME TestDense)				#Naziv prvog projekta u solution-u

#file(GLOB EXE1_SOURCES ${SOURCE_ROOT}/src/*.cpp ${SOURCE_ROOT}/inc/*.h)
file(GLOB TESTDENSE_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTDENSE_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTDENSE_INC_SPARSE  ${NATID_SDK_INC}/dense/*.h)
file(GLOB TESTDENSE_INC_SPARSE_PRIV ${NATID_SDK_INC}/dense/priv/*.h)
file(GLOB TESTDENSE_INC_SIMD ${NATID_SDK_INC}/simd/*.h)

# add executable
add_executable(${TESTDENSE_NAME} ${TESTDENSE_INCS} ${TESTDENSE_SOURCES} ${TESTDENSE_INC_SPARSE} ${TESTDENSE_INC_SPARSE_PRIV} ${TESTDENSE_INC_SIMD})

source_group("inc"            FILES ${TESTDENSE_INCS})
source_group("inc\\simd"        FILES ${TESTDENSE_INC_SIMD})
source_group("inc\\sparse"        FILES ${TESTDENSE_INC_SPARSE})
source_group("inc\\sparse\\priv"        FILES ${TESTDENSE_INC_SPARSE_PRIV})

source_group("src"            FILES ${TESTDENSE_SOURCES})

target_compile_definitions(${TESTDENSE_NAME} PUBLIC SER_RESULTS MU_USETIMER)


target_link_libraries(${TESTDENSE_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}
                    debug ${MATRIX_LIB_DEBUG} optimized ${MATRIX_LIB_RELEASE})

setIDEPropertiesForExecutable(${TESTDENSE_NAME})

