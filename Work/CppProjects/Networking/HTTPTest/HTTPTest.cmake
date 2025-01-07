set(HTTPTEST_NAME HTTPTest)				#Naziv programa

file(GLOB HTTPTEST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB HTTPTEST_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
#set(HTTPTEST_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB HTTPTEST_INC_TD  ${MY_INC}/td/*.h)
file(GLOB HTTPTEST_INC_NO ${MY_INC}/no/*.h)

# add executable
add_executable(${HTTPTEST_NAME} ${HTTPTEST_INCS} ${HTTPTEST_SOURCES} ${HTTPTEST_INC_TD}  ${HTTPTEST_INC_NO})

source_group("inc"            FILES ${HTTPTEST_INCS})
source_group("src"            FILES ${HTTPTEST_SOURCES})
source_group("inc\\td"        FILES ${HTTPTEST_INC_TD})
source_group("inc\\no"        FILES ${HTTPTEST_INC_NO})

target_link_libraries(${HTTPTEST_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
								debug ${NETOP_LIB_DEBUG} optimized ${NETOP_LIB_RELEASE})

setIDEPropertiesForExecutable(${HTTPTEST_NAME})

