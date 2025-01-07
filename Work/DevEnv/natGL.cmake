#SecuLib

if (WIN32)
    set(NATGL_LIB_DEBUG "${MY_LIB}/natGLD.lib")
    set(NATGL_LIB_RELEASE "${MY_LIB}/natGL.lib")
elseif(APPLE)
    set(NATGL_LIB_DEBUG "${MY_LIB}/natGLD.dylib")
    set(NATGL_LIB_RELEASE "${MY_LIB}/natGL.dylib")
else()
    set(NATGL_LIB_DEBUG "${MY_LIB}/natGLD.so")
    set(NATGL_LIB_RELEASE "${MY_LIB}/natGL.so")
endif()
