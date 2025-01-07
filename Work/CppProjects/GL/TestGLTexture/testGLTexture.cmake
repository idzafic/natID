set(TESTGLTEXTURE_NAME testGLTexture)				#Naziv prvog projekta u solution-u

file(GLOB TESTGLTEXTURE_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGLTEXTURE_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGLTEXTURE_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGLTEXTURE_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGLTEXTURE_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTGLTEXTURE_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTGLTEXTURE_NAME} ${TESTGLTEXTURE_INCS} ${TESTGLTEXTURE_SOURCES} ${TESTGLTEXTURE_INC_TD} ${TESTGLTEXTURE_INC_GUI} ${TESTGLTEXTURE_INC_GUI_GL})

source_group("inc"            FILES ${TESTGLTEXTURE_INCS})
source_group("src"            FILES ${TESTGLTEXTURE_SOURCES})
source_group("inc\\td"        FILES ${TESTGLTEXTURE_INC_TD})
source_group("inc\\gui"        FILES ${TESTGLTEXTURE_INC_GUI})
source_group("inc\\gl"        FILES ${TESTGLTEXTURE_INC_GUI_GL})

target_link_libraries(${TESTGLTEXTURE_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTGLTEXTURE_NAME})

setTargetPropertiesForGUIApp(${TESTGLTEXTURE_NAME} ${TESTGLTEXTURE_PLIST})

setIDEPropertiesForGUIExecutable(${TESTGLTEXTURE_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTGLTEXTURE_NAME})

