# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MY_BIN "${HOME_ROOT}/other_bin")
set(MY_LIB "${MY_BIN}/myLib")

if (WIN32)
    add_compile_options(/MP)
	set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY True)
	set(CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY True)
endif()

set(CMAKE_SUPPRESS_REGENERATION True)

if (WIN32)
    #output folder - RamDisk
	set(RAM_DISK C:/Projects)
else()
    #output folder - RamDisk
	set(RAM_DISK ${HOME_ROOT}/Projects)
endif()

#compile folder is on RAMDisk
set(CMAKE_BINARY_DIR ${RAM_DISK}/Out)

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set_property(TARGET MyFile PROPERTY FOLDER "Scripts")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configuration types" FORCE)
if (APPLE)
    #set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configuration types" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Mac OS X build architectures" FORCE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.3" CACHE STRING "Minimum OS X deployment version" FORCE)
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    #set(CMAKE_INSTALL_PREFIX "${MY_BIN}" CACHE STRING "Install location of my project" FORCE)
elseif (WIN32)
	#dodati opcije za MS Visual Studio
else()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

#sepcify C++ warning and optimization flags 
#set(CMAKE_CXX_FLAGS "-Wall -mavx2 -mfma")


#Output folder-i za executables i dlls
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)

set(MY_INC ${WORK_ROOT}/Common/Include)

if (WIN32)
	set(PLATFORM_INC "${MY_INC}/Platforms/win")
	if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		set(MU_LIB_DEBUG "${MY_LIB}/mainUtils32D.lib")
		set(MU_LIB_RELEASE "${MY_LIB}/mainUtils32.lib")
	else()
		set(MU_LIB_DEBUG "${MY_LIB}/mainUtilsD.lib")
		set(MU_LIB_RELEASE "${MY_LIB}/mainUtils.lib")
	endif()
elseif(APPLE)
	set(PLATFORM_INC "${MY_INC}/Platforms/mac")
    set(MU_LIB_DEBUG "${MY_LIB}/mainUtilsD.dylib")
    set(MU_LIB_RELEASE "${MY_LIB}/mainUtils.dylib")
else()
	set(PLATFORM_INC "${MY_INC}/Platforms/linux")
    set(MU_LIB_DEBUG "${MY_LIB}/mainUtilsD.so")
    set(MU_LIB_RELEASE "${MY_LIB}/mainUtils.so")
endif()

include_directories(${MY_INC})

if (WIN32)
	add_compile_definitions(MU_WINDOWS)
elseif(APPLE)
	add_compile_definitions(MU_MACOS)
else()
	add_compile_definitions(MU_LINUX)
endif()

if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
	add_compile_definitions(MU_32BIT)
else()
	add_compile_definitions(MU_64BIT)
endif()

add_compile_definitions("$<$<CONFIG:Release>:MU_RELEASE>$<$<CONFIG:Debug>:MU_DEBUG>")

function(setIDEPropertiesForExecutable targetID)

	if (WIN32)
	elseif (APPLE)
	  set_target_properties(${targetID} PROPERTIES
	   	XCODE_GENERATE_SCHEME TRUE
	   	XCODE_SCHEME_WORKING_DIRECTORY ${SOURCE_ROOT}
	   	XCODE_SCHEME_ENVIRONMENT "DYLD_LIBRARY_PATH=${MY_LIB}"
		COMPILE_FLAGS "-fobjc-arc")
	else()
		if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json")

			file(READ "${HOME_ROOT}/Work/DevEnv/VSCodeDebugger/settings.json" VSCODE_SETTINGS_FILE)
			string(REGEX REPLACE "(\"cmake.buildDirectory\":)[^\n]*," "\\1\"${CMAKE_CURRENT_BINARY_DIR}\"," VSCODE_SETTINGS_FILE "${VSCODE_SETTINGS_FILE}")
			file(WRITE "${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json" "${VSCODE_SETTINGS_FILE}")
			execute_process(
				COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json
			)

	endif()

endif()
endfunction()


function(setIDEPropertiesForGUIExecutable targetID devResPath)
	
	setIDEPropertiesForExecutable("${targetID}") # poziva osnovnu setIDEprops funkciju da se ne moraju obje pozivati

	if (WIN32)
		message(STATUS "Set devResPath for ${targetID} -> -devResPath=${devResPath}")
		set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "-devResPath=${devResPath}")
	elseif (APPLE)
		message(STATUS "Set devResPath for ${targetID} -> -devResPath=${devResPath}")
		set_target_properties(${targetID} PROPERTIES
	   			XCODE_GENERATE_SCHEME TRUE
				XCODE_SCHEME_WORKING_DIRECTORY ${SOURCE_ROOT}
				XCODE_SCHEME_ENVIRONMENT "DYLD_LIBRARY_PATH=${MY_LIB}"
				XCODE_SCHEME_ARGUMENTS "-devResPath=${devResPath}"
				COMPILE_FLAGS "-fobjc-arc")
	else()
	#linux
	endif()

	set(DevResPaths_PATH "${HOME_ROOT}/Work/DevEnv/DevResPaths.txt")
	string(REGEX REPLACE ".*${HOME_ROOT}(.*)" "$HOME\\1" devResPath "${devResPath}")

	if (NOT EXISTS "${DevResPaths_PATH}") #update-ovanje devResPaths.txt fajla
		file(WRITE "${DevResPaths_PATH}" "${targetID}=${devResPath}")
	else()
		file(READ "${DevResPaths_PATH}" DevResPathsContent)
	
		set(REGEX_DEVENV_PATHMATCH "(\n|^)${targetID}=([^\n]*)(\n|$)")

		string(REGEX MATCH  "${REGEX_DEVENV_PATHMATCH}" output_var "${DevResPathsContent}")
		string(REGEX REPLACE "${REGEX_DEVENV_PATHMATCH}" "\\2" current_devres_path "${output_var}")

		if(NOT "${current_devres_path}" STREQUAL "${devResPath}")
			string(REGEX MATCH "(\n|^)${targetID}=" target_exists_in_devres "${DevResPathsContent}")

			if(DEFINED target_exists_in_devres AND NOT "${target_exists_in_devres}" STREQUAL "")
				string(REGEX REPLACE "${REGEX_DEVENV_PATHMATCH}" "\\1${targetID}=${devResPath}\\3" DevResPathsContent "${DevResPathsContent}")
			else()
				set(DevResPathsContent "${DevResPathsContent}\n${targetID}=${devResPath}")
			endif()

			file(WRITE "${DevResPaths_PATH}" "${DevResPathsContent}")
		endif()
	endif()

endfunction()


function(executePostConfigProgram executablePath args)

	if(WIN32)
		string(REGEX MATCH ".exe$" match_found "${executablePath}")
		if(NOT match_found)
			set(executablePath "${executablePath}.exe")
		endif()
	else()
		string(REGEX REPLACE ".exe$" "" executablePath "${executablePath}")
	endif()

	execute_process(
		COMMAND "${executablePath}" "${args}"
		OUTPUT_VARIABLE output_exe
		ERROR_VARIABLE error_result
		RESULT_VARIABLE exit_status
	)

	if(output_exe)
		message(${output_exe})
	endif()
	message("${executablePath} returned ${exit_status}")
	
	if(error_result)
		message(WARNING "Executable ${executablePath} encountered error: ${error_result}")
	endif()


endfunction()

function(setTargetPropertiesForApp targetID plistFileName)
	if (WIN32)
	elseif (APPLE)
    	set_target_properties( ${targetID} PROPERTIES
				MACOSX_BUNDLE TRUE
				MACOSX_BUNDLE_INFO_PLIST ${plistFileName})
	endif()
endfunction()

function(setTargetPropertiesForGUIApp targetID plistFileName)
	if (WIN32)
		set_target_properties(${targetID} PROPERTIES 
				WIN32_EXECUTABLE TRUE)						
	elseif (APPLE)
    	set_target_properties( ${targetID} PROPERTIES
				MACOSX_BUNDLE TRUE
				MACOSX_BUNDLE_INFO_PLIST ${plistFileName})
	endif()
endfunction()

function (addMUCompileDefinitions targetID)
    set(MU_DEF "$<$<CONFIG:Release>:MU_RELEASE>$<$<CONFIG:Debug>:MU_DEBUG>")
	#target_compile_definitions(${targetID} PUBLIC ${MU_DEF})
	
	if (WIN32)
		target_compile_definitions(${targetID} PUBLIC ${MU_DEF} "MU_WINDOWS")
	elseif(APPLE)
		target_compile_definitions(${targetID} PUBLIC ${MU_DEF} "MU_MACOS")
	else()
		target_compile_definitions(${targetID} PUBLIC ${MU_DEF} "MU_LINUX")
	endif()

	if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		target_compile_definitions(${targetID} PUBLIC ${MU_DEF} "MU_32BIT")
	else()
		target_compile_definitions(${targetID} PUBLIC ${MU_DEF} "MU_64BIT")
	endif()
endfunction()

function(setPlatformDLLPath targetID)
	if (WIN32)
		if(NOT DEFINED GTK_BINP)
			if(NOT DEFINED GTK_BINP_LOCAL)
				set(GTK_BINP_LOCAL "${MY_BIN}/GTK")
			endif()
			set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${MY_BIN};${GTK_BINP_LOCAL};%PATH%;")
		else()
			set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${MY_BIN};${GTK_BINP};%PATH%;")
		endif()
	endif()
endfunction()

function(addOpenGLLibraries targetID)
	if (APPLE)
		#target_compile_options(${targetID} PRIVATE "-Wno-deprecated-declarations")
		target_link_libraries(${targetID} "-framework OpenGL")
	elseif(WIN32)
		set(GL_EPOXY_INCS  "${MY_BIN}/GTK/release/include")	
		target_include_directories(${targetID} PRIVATE ${GL_EPOXY_INCS})
		set(GL_EPOXY_LIB  "${MY_BIN}/GTK/release/lib/epoxy.lib")
		target_link_libraries(${targetID} debug ${GL_EPOXY_LIB} optimized ${GL_EPOXY_LIB})
		#message("EpoxyLib=${EPOXY_LIB}. Psth=${GTK_LIBP}")
	else()
		#Linux

		find_package(OpenGL REQUIRED)

		find_path(EPOXY_INCLUDE_DIR
		NAMES epoxy/gl.h
		PATHS /usr/include /usr/local/include
		)

		find_library(EPOXY_LIBRARY
		NAMES epoxy
		PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
		)

		if (EPOXY_LIBRARY AND EPOXY_INCLUDE_DIR)
		else()
			message(FATAL_ERROR "Epoxy not found!")
		endif()
		
		include_directories(${OPENGL_INCLUDE_DIRS})
		include_directories(${EPOXY_INCLUDE_DIR})
		target_link_libraries(${targetID} ${OPENGL_LIBRARIES} ${EPOXY_LIBRARY})

	endif()
endfunction()

function(setTargetPropertiesForGUIAppWithBundleName targetID plistFileName bundleName)
	if (WIN32)
		set_target_properties(${targetID} PROPERTIES 
				WIN32_EXECUTABLE TRUE)						
	elseif (APPLE)
    	set_target_properties( ${targetID} PROPERTIES
				MACOSX_BUNDLE TRUE
				MACOSX_BUNDLE_INFO_PLIST ${plistFileName}
				MACOSX_BUNDLE_GUI_IDENTIFIER "ba.natID.${targetID}"
    			MACOSX_BUNDLE_BUNDLE_NAME ${bundleName})
	else()
		#Linux
	endif()
endfunction()


function(setAppIcon targetID currPath)
	if (APPLE)
		# Specify the location of the icon file
		set(APP_ICON_MACOS "${currPath}/res/appIcon/macApp.icns")

		# Set the MACOSX_BUNDLE_ICON_FILE property to the icon file
		set_target_properties(${targetID} PROPERTIES MACOSX_BUNDLE_ICON_FILE macApp.icns)

		# Add the icon file to the bundle
		set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
		target_sources(${targetID}  PRIVATE ${APP_ICON_MACOS})
	endif()
endfunction()

