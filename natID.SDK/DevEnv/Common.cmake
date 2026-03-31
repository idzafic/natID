# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    add_compile_options(/MP)
endif()

set(CMAKE_SUPPRESS_REGENERATION True)

set(NATID_SDK_BIN ${NATID_SDK_ROOT}/bin)
set(NATID_SDK_LIB ${NATID_SDK_BIN}/lib)

set(RAM_DISK ${HOME_ROOT}/natID.RAMDisk)

#compile folder is on RAMDisk
set(CMAKE_BINARY_DIR ${RAM_DISK}/Out)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configuration types" FORCE)
if (APPLE)
	set(CMAKE_OSX_ARCHITECTURES "${CMAKE_HOST_SYSTEM_PROCESSOR}" CACHE STRING "Mac OS X build architectures" FORCE)
	# Set deployment target depending on architecture
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "14.3" CACHE STRING "Minimum macOS deployment version" FORCE)
        message(STATUS "ARM64 detected → setting macOS deployment target to 14.3")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET "12.3" CACHE STRING "Minimum macOS deployment version" FORCE)
        message(STATUS "Intel/x86_64 detected → setting macOS deployment target to 12.3")
    endif()
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_VERBOSE_MAKEFILE ON)
elseif (WIN32)
	#dodati opcije za MS Visual Studio
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ot /Ob2")
	set(CMAKE_C_FLAGS_RELEASE "/O2")
	# Define minimum supported Windows version (Windows 10, later)
	add_compile_definitions(_WIN32_WINNT=0x0A00)
	add_compile_definitions(WINVER=0x0A00)
	add_compile_definitions(NTDDI_VERSION=0x0A000008)  #NTDDI_WIN10_19H2

	#QtCreator:
	if(CMAKE_VERSION VERSION_GREATER "3.12")
    	include(ProcessorCount)
    	ProcessorCount(NCores)
    	if(NOT NCores EQUAL 0)
        	set(CMAKE_BUILD_PARALLEL_LEVEL ${NCores})
			message(STATUS "Detected ${NCores} CPU cores which will be used for compilation..")
			if (MSVC)
				set(CMAKE_BUILD_OPTIONS "/maxcpucount:${NCores}")
				message(STATUS "Building with ${NCores} parallel threads on Visual Studio....")
			endif()
		else()
			message(WARNING "WARNING! Unable to detect number of CPU cores, defaulting to 8 cores.")
			set(CMAKE_BUILD_PARALLEL_LEVEL 8)
			if (MSVC)
				set(CMAKE_BUILD_OPTIONS "/maxcpucount:8")
				message(STATUS "Building with 8 parallel threads on Visual Studio")
			endif()
    	endif()
	endif()
else()
	#Linux
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

	set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")
	set(ENV{LD_LIBRARY_PATH} "/lib/x86_64-linux-gnu/:$ENV{LD_LIBRARY_PATH}")

	if(CMAKE_VERSION VERSION_GREATER "3.12")
		include(ProcessorCount)
		ProcessorCount(NCores)
		if(NOT NCores EQUAL 0)
			set(CMAKE_BUILD_PARALLEL_LEVEL ${NCores})
			message(STATUS "Detected ${NCores} CPU cores which will be used for compilation..")
		else()
			message(WARNING "Unable to detect number of CPU cores, defaulting to 4 cores.")
			set(CMAKE_BUILD_PARALLEL_LEVEL 4)
		endif()
	endif()

endif()

#Output folder-i za executables i dlls
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Release)

set(NATID_SDK_INC ${NATID_SDK_ROOT}/Common/Include)

if (WIN32)
	set(PLATFORM_INC "${NATID_SDK_INC}/Platforms/win")
	if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		set(MU_LIB_DEBUG "${NATID_SDK_LIB}/mainUtils32D.lib")
		set(MU_LIB_RELEASE "${NATID_SDK_LIB}/mainUtils32.lib")
	else()
		set(MU_LIB_DEBUG "${NATID_SDK_LIB}/mainUtilsD.lib")
		set(MU_LIB_RELEASE "${NATID_SDK_LIB}/mainUtils.lib")
	endif()
elseif(APPLE)
	set(PLATFORM_INC "${NATID_SDK_INC}/Platforms/mac")
    set(MU_LIB_DEBUG "${NATID_SDK_LIB}/mainUtilsD.dylib")
    set(MU_LIB_RELEASE "${NATID_SDK_LIB}/mainUtils.dylib")
else()
	set(PLATFORM_INC "${NATID_SDK_INC}/Platforms/linux")
    set(MU_LIB_DEBUG "${NATID_SDK_LIB}/mainUtilsD.so")
    set(MU_LIB_RELEASE "${NATID_SDK_LIB}/mainUtils.so")
endif()

include_directories(${NATID_SDK_INC})

#MU_WINDOWS, MU_MACOS, MU_LINUX, MU_64BIT, MU_32BIT
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

# Detect ARM architecture
if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm" OR 
   "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "aarch64" OR
   "${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "arm" OR
   "${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "aarch64")
    message(STATUS "ARM Architecture Detected!!")
    add_compile_definitions(MU_ARM)
else()
    message(STATUS "x86 Architecture Detected!!")
    add_compile_definitions(MU_X86)
endif()


add_compile_definitions("$<$<CONFIG:Release>:MU_RELEASE>$<$<CONFIG:Debug>:MU_DEBUG>")


function(setIDEPropertiesForExecutable targetID)

	if (WIN32)
		#set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "-devResPath=nekiFolder")
		set_target_properties(${targetID} PROPERTIES
    		VS_DEBUGGER_ENVIRONMENT "PATH=${NATID_SDK_BIN};${PATH}")
		
	elseif (APPLE)
	  set_target_properties(${targetID} PROPERTIES
	   	XCODE_GENERATE_SCHEME TRUE
	   	XCODE_SCHEME_WORKING_DIRECTORY ${SOURCE_ROOT}

		XCODE_ATTRIBUTE_CODE_SIGN_STYLE Manual
		XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"

		XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
		XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER ""
		XCODE_ATTRIBUTE_PROVISIONING_PROFILE ""

		XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED YES
		XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO

	   	XCODE_SCHEME_ENVIRONMENT "DYLD_LIBRARY_PATH=${NATID_SDK_LIB}"
		COMPILE_FLAGS "-fobjc-arc")
	else()
		# 1. Match the path where CMake is actually putting the exe
        # Based on your global settings: ${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug
        set(ACTUAL_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${SOLUTION_NAME}/Debug")
        
        # 2. Ensure .vscode directory exists
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/.vscode")

        # 3. settings.json - Pointing VS Code to the correct build root
        if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json")
            # We point this to the root of your RAM_DISK Out folder so CMake Tools knows where to look
            set(VSCODE_SETTINGS_FILE
"{
    \"cmake.buildDirectory\": \"${RAM_DISK}/build/${targetID}\",
    \"cmake.debugConfig\": {
        \"setupCommands\": [
            {
                \"text\": \"source ${NATID_SDK_ROOT}/DevEnv/VSCodeDebugger/debugHelper_unix.py\"
            },
            {
                \"text\": \"-enable-pretty-printing\"
            }
        ]
    }
}")
            file(WRITE "${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json" "${VSCODE_SETTINGS_FILE}")
        endif()

        # 4. launch.json - Pointing to the specific Debug binary
        if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/.vscode/launch.json")
            
            set(BINARY_PATH "${ACTUAL_OUTPUT_DIR}/${targetID}")

            set(VSCODE_LAUNCH_FILE
"{
    \"version\": \"0.2.0\",
    \"configurations\": [
        {
            \"name\": \"Debug ${targetID}\",
            \"type\": \"cppdbg\",
            \"request\": \"launch\",
            \"program\": \"${BINARY_PATH}\",
            \"args\": [],
            \"cwd\": \"${SOURCE_ROOT}\",
            \"environment\": [
                {
                    \"name\": \"LD_LIBRARY_PATH\",
                    \"value\": \"${NATID_SDK_LIB}:\$LD_LIBRARY_PATH\"
                }
            ],
            \"MIMode\": \"gdb\",
            \"setupCommands\": [
                {
                    \"description\": \"Enable pretty-printing for gdb\",
                    \"text\": \"-enable-pretty-printing\",
                    \"ignoreFailures\": true
                }
            ]
        }
    ]
}")
            file(WRITE "${CMAKE_CURRENT_LIST_DIR}/.vscode/launch.json" "${VSCODE_LAUNCH_FILE}")
            
            # Use Shell to touch files (ensures file system sync)
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_LIST_DIR}/.vscode/settings.json")
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_LIST_DIR}/.vscode/launch.json")
        endif()
	endif()
endfunction()

function(setIDEPropertiesForGUIExecutable targetID devResPath)
	
	setIDEPropertiesForExecutable("${targetID}") # poziva osnovnu setIDEprops funkciju da se ne moraju obje pozivati

	if (WIN32)
		message(STATUS "Set devResPath for ${targetID} -> -devResPath=${devResPath}")
		set_target_properties(${targetID} PROPERTIES 
			VS_DEBUGGER_COMMAND_ARGUMENTS "-devResPath=${DevResPathLoc}"
    		VS_DEBUGGER_ENVIRONMENT "PATH=${NATID_SDK_BIN};${PATH}"
			WIN32_EXECUTABLE TRUE)
	elseif (APPLE)
		message(STATUS "Set devResPath for ${targetID} -> -devResPath=${devResPath}")
		set_target_properties(${targetID} PROPERTIES
	   			XCODE_GENERATE_SCHEME TRUE
				XCODE_SCHEME_WORKING_DIRECTORY ${SOURCE_ROOT}

				XCODE_ATTRIBUTE_CODE_SIGN_STYLE Manual
				XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"

				XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
				XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER ""
				XCODE_ATTRIBUTE_PROVISIONING_PROFILE ""

				XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED YES
				XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO

				XCODE_SCHEME_ENVIRONMENT "DYLD_LIBRARY_PATH=${NATID_SDK_LIB}"
				XCODE_SCHEME_ARGUMENTS "-devResPath=${devResPath}"
				COMPILE_FLAGS "-fobjc-arc")
	else()
		#linux
		target_link_directories(${targetID} PRIVATE ${NATID_SDK_LIB})
		target_link_options(${targetID} PRIVATE "-Wl,--no-undefined")
		set_target_properties(${targetID} PROPERTIES
        		BUILD_RPATH "${NATID_SDK_LIB}"
        		INSTALL_RPATH "${NATID_SDK_LIB}")
	endif()

	set(DevResPaths_PATH "${NATID_SDK_ROOT}/DevEnv/DevResPaths.txt")
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
				set(GTK_BINP_LOCAL "${NATID_SDK_BIN}/GTK")
			endif()
			set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${NATID_SDK_BIN};${GTK_BINP_LOCAL};%PATH%;")
		else()
			set_target_properties(${targetID} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${NATID_SDK_BIN};${GTK_BINP};%PATH%;")
		endif()
	endif()
endfunction()

function(addOpenGLLibraries targetID)
	if (APPLE)
		#target_compile_options(${targetID} PRIVATE "-Wno-deprecated-declarations")
		target_link_libraries(${targetID} "-framework OpenGL")
	elseif(WIN32)
		set(GL_EPOXY_INCS  "${NATID_SDK_BIN}/GTK/release/include")	
		target_include_directories(${targetID} PRIVATE ${GL_EPOXY_INCS})
		set(GL_EPOXY_LIB  "${NATID_SDK_BIN}/GTK/release/lib/epoxy.lib")
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

function(register_shader_folder target_name shader_folder_path)
	# This function registers a shader folder to be compiled automatically after building a target
	# 
	# Parameters:
	#   target_name: The name of the CMake target (executable/library) that this shader compilation should be attached to
	#                This must be a target that was already created with add_executable() or add_library()
	#   shader_folder_path: Path to the folder containing shader files (.vert, .frag, .comp, etc.)


	get_filename_component(SCRIPT_DIR "${CMAKE_CURRENT_FUNCTION_LIST_FILE}" DIRECTORY)
	set(SHADER_SCRIPT_PATH "${SCRIPT_DIR}/GLSL2Metal.py")
	
	# Convert relative paths to absolute for consistent behavior
	get_filename_component(SHADER_FOLDER_ABS "${shader_folder_path}" ABSOLUTE)
	
	# Determine output base name (default to target name)
	set(SHADER_OUTPUT_BASE "${target_name}_shaders")
	
	# Determine Python executable based on platform
	if(WIN32)
		set(PYTHON_CMD "python")
	else()
		set(PYTHON_CMD "python3")
	endif()
	
	# Build script arguments
	set(SCRIPT_ARGS 
		-i "${SHADER_FOLDER_ABS}"
		-o "${target_name}"
	)
	
	# Add any additional arguments from ARGN (optional arguments)
	if(ARGN)
		list(APPEND SCRIPT_ARGS ${ARGN})
	endif()
	
	# Build the full command line for display
	set(FULL_COMMAND "${PYTHON_CMD}" "${SHADER_SCRIPT_PATH}" ${SCRIPT_ARGS})
	string(REPLACE ";" " " COMMAND_LINE "${FULL_COMMAND}")
	
	# Add POST_BUILD command to compile shaders with simple error handling
	add_custom_command(
		TARGET ${target_name}
		POST_BUILD
		
		COMMAND ${CMAKE_COMMAND} -E echo "Running shader compiler: ${COMMAND_LINE}"
		
		COMMAND ${PYTHON_CMD} "${SHADER_SCRIPT_PATH}" ${SCRIPT_ARGS}
		
		WORKING_DIRECTORY "${SHADER_FOLDER_ABS}"
		COMMENT "Compiling shaders for ${target_name}"
		VERBATIM
	)
	
	message(STATUS "Registered shader folder '${SHADER_FOLDER_ABS}' for target '${target_name}'")
	if(ARGN)
		message(STATUS "Additional script arguments: ${ARGN}")
	endif()
endfunction()

function(registerShaderFolder targetName shaderFolderPath)
	# This function registers a shader folder to be compiled automatically after building a target
	# 
	# Parameters:
	#   targetName: The name of the CMake target (executable/library) that this shader compilation should be attached to
	#                This must be a target that was already created with add_executable() or add_library()
	#   shaderFolderPath: Path to the folder containing shader files (.vert, .frag, .comp, etc.)


	get_filename_component(SCRIPT_DIR "${CMAKE_CURRENT_FUNCTION_LIST_FILE}" DIRECTORY)
	set(SHADER_SCRIPT_PATH "${SCRIPT_DIR}/GLSL2Metal.py")
	
	# Convert relative paths to absolute for consistent behavior
	get_filename_component(SHADER_FOLDER_ABS "${shaderFolderPath}" ABSOLUTE)
	
	# Determine output base name
	set(SHADER_OUTPUT_BASE "${targetName}")
	
	# Determine Python executable based on platform
	if(WIN32)
		set(PYTHON_CMD "python")
	else()
		set(PYTHON_CMD "python3")
	endif()
	
	# Build script arguments
	set(SCRIPT_ARGS 
		-i "${SHADER_FOLDER_ABS}"
		-o "${SHADER_OUTPUT_BASE}"
	)
	
	# Add any additional arguments from ARGN (optional arguments)
	if(ARGN)
		list(APPEND SCRIPT_ARGS ${ARGN})
	endif()
	
	# Build the full command line for display
	set(FULL_COMMAND "${PYTHON_CMD}" "${SHADER_SCRIPT_PATH}" ${SCRIPT_ARGS})
	string(REPLACE ";" " " COMMAND_LINE "${FULL_COMMAND}")
	
	# Discover shader input files
	set(SHADER_EXTENSIONS .vert .frag .comp .geom .tesc .tese .rgen .rint .rahit .rchit .rmiss .rcall .mesh .task)
	set(SHADER_INPUT_FILES)
	foreach(EXT ${SHADER_EXTENSIONS})
		file(GLOB FOUND_SHADERS "${SHADER_FOLDER_ABS}/*${EXT}")
		if(FOUND_SHADERS)
			list(APPEND SHADER_INPUT_FILES ${FOUND_SHADERS})
		endif()
	endforeach()
	
	# Determine output extension based on platform
	if(APPLE)
		set(OUTPUT_EXT ".metallib")
	else()
		# Windows and Linux use SPIR-V
		set(OUTPUT_EXT ".spv")
	endif()
	
	# Generate output file paths
	set(SHADER_OUTPUT_FILES "${SHADER_FOLDER_ABS}/${SHADER_OUTPUT_BASE}${OUTPUT_EXT}")

	
	# Add POST_BUILD command to compile shaders with proper dependencies
	add_custom_command(
		COMMAND ${CMAKE_COMMAND} -E echo "Running shader compiler: ${COMMAND_LINE}"
		
		COMMAND ${PYTHON_CMD} "${SHADER_SCRIPT_PATH}" ${SCRIPT_ARGS}
		
		DEPENDS ${SHADER_INPUT_FILES}
		OUTPUT ${SHADER_OUTPUT_FILES}
		
		WORKING_DIRECTORY "${SHADER_FOLDER_ABS}"
		COMMENT "Compiling shaders for ${SHADER_OUTPUT_BASE}; Input files: ${SHADER_INPUT_FILES}"
		VERBATIM
	)
	
	# Add output files as sources to the target (marked as HEADER_FILE_ONLY so they won't be compiled)
	if(SHADER_OUTPUT_FILES)
		target_sources(${SHADER_OUTPUT_BASE} PRIVATE ${SHADER_OUTPUT_FILES})
		set_source_files_properties(${SHADER_OUTPUT_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
	endif()
	
	message(STATUS "Registered shader folder '${SHADER_FOLDER_ABS}' for target '${SHADER_OUTPUT_BASE}'")
	if(ARGN)
		message(STATUS "Additional script arguments: ${ARGN}")
	endif()
endfunction()