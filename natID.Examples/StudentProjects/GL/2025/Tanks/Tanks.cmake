set(TANKS_NAME Tanks)				#Naziv prvog projekta u solution-u

file(GLOB TANKS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TANKS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TANKS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${TANKS_NAME} ${TANKS_INCS} ${TANKS_SOURCES})

source_group("inc"            FILES ${TANKS_INCS})
source_group("src"            FILES ${TANKS_SOURCES})

target_link_libraries(${TANKS_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TANKS_NAME})

setTargetPropertiesForGUIApp(${TANKS_NAME} ${TANKS_PLIST})

setIDEPropertiesForGUIExecutable(${TANKS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TANKS_NAME})



