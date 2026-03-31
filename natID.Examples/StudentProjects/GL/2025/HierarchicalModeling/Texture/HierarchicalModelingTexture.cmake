set(HTEXTUREMODEL_NAME HierarchModelTexture)				#Naziv prvog projekta u solution-u

file(GLOB HTEXTUREMODEL_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB HTEXTUREMODEL_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(HTEXTUREMODEL_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${HTEXTUREMODEL_NAME} ${HTEXTUREMODEL_INCS} ${HTEXTUREMODEL_SOURCES})

source_group("inc"            FILES ${HTEXTUREMODEL_INCS})
source_group("src"            FILES ${HTEXTUREMODEL_SOURCES})

target_link_libraries(${HTEXTUREMODEL_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})


addOpenGLLibraries(${HTEXTUREMODEL_NAME})

setTargetPropertiesForGUIApp(${HTEXTUREMODEL_NAME} ${HTEXTUREMODEL_PLIST})

setIDEPropertiesForGUIExecutable(${HTEXTUREMODEL_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${HTEXTUREMODEL_NAME})



