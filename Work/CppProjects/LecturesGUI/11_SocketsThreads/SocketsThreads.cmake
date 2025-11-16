set(WS_NAME SocketsThreads)				#Naziv prvog projekta u solution-u

file(GLOB WS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB WS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(WS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${WS_NAME} ${WS_INCS} ${WS_SOURCES})

source_group("inc"            FILES ${WS_INCS})
source_group("src"            FILES ${WS_SOURCES})

target_link_libraries(${WS_NAME} debug ${MU_LIB_DEBUG}   optimized ${MU_LIB_RELEASE})

setIDEPropertiesForExecutable(${WS_NAME})
