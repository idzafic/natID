set(TEXTURECHESS2_NAME textureChess2)				#Naziv prvog projekta u solution-u

file(GLOB TEXTURECHESS2_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TEXTURECHESS2_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TEXTURECHESS2_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TEXTURECHESS2_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TEXTURECHESS2_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TEXTURECHESS2_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TEXTURECHESS2_NAME} ${TEXTURECHESS2_INCS} ${TEXTURECHESS2_SOURCES} ${TEXTURECHESS2_INC_TD} ${TEXTURECHESS2_INC_GUI} ${TEXTURECHESS2_INC_GUI_GL})

source_group("inc"            FILES ${TEXTURECHESS2_INCS})
source_group("src"            FILES ${TEXTURECHESS2_SOURCES})
source_group("inc\\td"        FILES ${TEXTURECHESS2_INC_TD})
source_group("inc\\gui"        FILES ${TEXTURECHESS2_INC_GUI})
source_group("inc\\gl"        FILES ${TEXTURECHESS2_INC_GUI_GL})

target_link_libraries(${TEXTURECHESS2_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TEXTURECHESS2_NAME})

setTargetPropertiesForGUIApp(${TEXTURECHESS2_NAME} ${TEXTURECHESS2_PLIST})

setIDEPropertiesForGUIExecutable(${TEXTURECHESS2_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEXTURECHESS2_NAME})

