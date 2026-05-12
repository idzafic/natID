set(TESTSPARSE_NAME TestSparse)				#Naziv ovog projekta

#collect source files
file(GLOB TESTSPARSE_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTSPARSE_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TESTSPARSE_INC_SPARSE  ${NATID_SDK_INC}/sparse/*.h)
file(GLOB TESTSPARSE_INC_SPARSE_PRIV ${NATID_SDK_INC}/sparse/priv/*.h)

# add shared library
add_executable(${TESTSPARSE_NAME} ${TESTSPARSE_INCS} ${TESTSPARSE_SOURCES} ${TESTSPARSE_INC_SPARSE} ${TESTSPARSE_INC_SPARSE_PRIV})

source_group("inc"            FILES ${TESTSPARSE_INCS})
source_group("inc\\sparse"        FILES ${TESTSPARSE_INC_SPARSE})
source_group("inc\\sparse\\priv"        FILES ${TESTSPARSE_INC_SPARSE_PRIV})
source_group("src"            FILES ${TESTSPARSE_SOURCES})

target_compile_definitions(${TESTSPARSE_NAME} PUBLIC SER_RESULTS MU_USETIMER)

target_link_libraries(${TESTSPARSE_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}
                                    debug ${MATRIX_LIB_DEBUG} optimized ${MATRIX_LIB_RELEASE})

setIDEPropertiesForExecutable(${TESTSPARSE_NAME})
