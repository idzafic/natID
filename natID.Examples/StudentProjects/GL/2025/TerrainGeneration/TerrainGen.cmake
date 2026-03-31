set(TERRAIN_GEN_NAME TerrainGeneration)				#Naziv prvog projekta u solution-u

file(GLOB TERRAIN_GEN_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TERRAIN_GEN_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TERRAIN_GEN_SHADERS  ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*)
set(TERRAIN_GEN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TERRAIN_GEN_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TERRAIN_GEN_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB TERRAIN_GEN_INC_FO ${MY_INC}/fo/*.h)
file(GLOB TERRAIN_GEN_INC_RND ${MY_INC}/rnd/*.h)
file(GLOB TERRAIN_GEN_INC_SYST ${MY_INC}/syst/*.h)

# add executable
add_executable(${TERRAIN_GEN_NAME} ${TERRAIN_GEN_INCS} ${TERRAIN_GEN_SOURCES}  ${TERRAIN_GEN_SHADERS} ${TERRAIN_GEN_INC_TD} 
                ${TERRAIN_GEN_INC_CNT}  ${TERRAIN_GEN_INC_FO} ${TERRAIN_GEN_INC_RND} ${TERRAIN_GEN_INC_SYST})

source_group("inc\\td"        FILES ${TERRAIN_GEN_INC_TD})
source_group("inc\\cnt"        FILES ${TERRAIN_GEN_INC_CNT})
source_group("inc\\fo"        FILES ${TERRAIN_GEN_INC_FO})
source_group("inc\\rnd"        FILES ${TERRAIN_GEN_INC_RND})
source_group("inc\\syst"        FILES ${TERRAIN_GEN_INC_SYST})
source_group("inc"            FILES ${TERRAIN_GEN_INCS})
source_group("src"            FILES ${TERRAIN_GEN_SOURCES})
source_group("shaders"            FILES ${TERRAIN_GEN_SHADERS})

target_link_libraries(${TERRAIN_GEN_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TERRAIN_GEN_NAME})

setTargetPropertiesForGUIApp(${TERRAIN_GEN_NAME} ${TERRAIN_GEN_PLIST})

setIDEPropertiesForGUIExecutable(${TERRAIN_GEN_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TERRAIN_GEN_NAME})



