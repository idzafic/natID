set(HMODEL_NAME HierarchicalModeling)				#Naziv prvog projekta u solution-u

file(GLOB HMODEL_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB HMODEL_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(HMODEL_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${HMODEL_NAME} ${HMODEL_INCS} ${HMODEL_SOURCES})

source_group("inc"            FILES ${HMODEL_INCS})
source_group("src"            FILES ${HMODEL_SOURCES})

target_link_libraries(${HMODEL_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})


addOpenGLLibraries(${HMODEL_NAME})

setTargetPropertiesForGUIApp(${HMODEL_NAME} ${HMODEL_PLIST})

setIDEPropertiesForGUIExecutable(${HMODEL_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${HMODEL_NAME})



