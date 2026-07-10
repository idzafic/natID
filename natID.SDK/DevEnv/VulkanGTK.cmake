#Vulkan libs
# Find Vulkan using CMake's built-in FindVulkan module
find_package( Vulkan REQUIRED )

if( NOT Vulkan_FOUND )
	message( FATAL_ERROR "Vulkan not found! Please install Vulkan SDK or specify Vulkan_ROOT" )
endif()

message( "Vulkan include dir: " ${Vulkan_INCLUDE_DIRS} )
message( "Vulkan library: " ${Vulkan_LIBRARIES} )

# Get glslc compiler path
if( NOT Vulkan_GLSLC_EXECUTABLE )
	find_program( Vulkan_GLSLC_EXECUTABLE
		NAMES glslc
		HINTS ${Vulkan_GLSLC_EXECUTABLE}
			ENV VULKAN_SDK
		PATH_SUFFIXES bin
	)
endif()

message( "glslc compiler: " ${Vulkan_GLSLC_EXECUTABLE} )

set(VULKAN_INCS  "${Vulkan_INCLUDE_DIRS}")	
set(VULKAN_LIB  "${Vulkan_LIBRARIES}")

if(WIN32)
	set( VULKAN_WSI "VK_USE_PLATFORM_WIN32_KHR" CACHE STRING "WSI type used by this app" )
else()
	if(APPLE)
		set( VULKAN_WSI "VK_USE_PLATFORM_MACOS_MVK" CACHE STRING "WSI type used by this app" )
	else()
		#Linux
		set( VULKAN_WSI "VK_USE_PLATFORM_WAYLAND_KHR" CACHE STRING "WSI type used by this app" )
	endif()

endif()

