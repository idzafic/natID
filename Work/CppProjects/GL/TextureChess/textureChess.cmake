set(TEXTURECHESS_NAME textureChess)				#Naziv prvog projekta u solution-u

file(GLOB TEXTURECHESS_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TEXTURECHESS_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TEXTURECHESS_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TEXTURECHESS_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TEXTURECHESS_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TEXTURECHESS_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TEXTURECHESS_NAME} ${TEXTURECHESS_INCS} ${TEXTURECHESS_SOURCES} ${TEXTURECHESS_INC_TD} ${TEXTURECHESS_INC_GUI} ${TEXTURECHESS_INC_GUI_GL})

source_group("inc"            FILES ${TEXTURECHESS_INCS})
source_group("src"            FILES ${TEXTURECHESS_SOURCES})
source_group("inc\\td"        FILES ${TEXTURECHESS_INC_TD})
source_group("inc\\gui"        FILES ${TEXTURECHESS_INC_GUI})
source_group("inc\\gl"        FILES ${TEXTURECHESS_INC_GUI_GL})

target_link_libraries(${TEXTURECHESS_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TEXTURECHESS_NAME})

setTargetPropertiesForGUIApp(${TEXTURECHESS_NAME} ${TEXTURECHESS_PLIST})

setIDEPropertiesForGUIExecutable(${TEXTURECHESS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TEXTURECHESS_NAME})

