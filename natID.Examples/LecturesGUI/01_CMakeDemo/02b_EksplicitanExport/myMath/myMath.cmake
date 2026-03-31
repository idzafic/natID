set(MYMATH_SHLIB_NAME myMath)	

add_library(${MYMATH_SHLIB_NAME} SHARED ${CMAKE_CURRENT_LIST_DIR}/src/Operacije.cpp 
										${CMAKE_CURRENT_LIST_DIR}/src/MyMath.cpp 
										${CMAKE_CURRENT_LIST_DIR}/../common/include/MyMath.h
										${CMAKE_CURRENT_LIST_DIR}/../common/include/MyMathLib.h)
							


#postaviti za ovu dinamicku biblioteku da sakrije simbole koje ne zelimo da vidimo
set_target_properties(${MYMATH_SHLIB_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

#CMake detektuje OS 
if (WIN32)
	target_compile_definitions(${MYMATH_SHLIB_NAME} PUBLIC MYMATH_SHLIB_EXPORTS MU_WINDOWS)
elseif(APPLE)
	target_compile_definitions(${MYMATH_SHLIB_NAME} PUBLIC MYMATH_SHLIB_EXPORTS MU_MACOS)
else()
	target_compile_definitions(${MYMATH_SHLIB_NAME} PUBLIC MYMATH_SHLIB_EXPORTS MU_LINUX)
endif()

