#SecuLib

if (WIN32)
    set(NATGL_LIB_DEBUG "${NATID_SDK_LIB}/natGLD.lib")
    set(NATGL_LIB_RELEASE "${NATID_SDK_LIB}/natGL.lib")
elseif(APPLE)
    set(NATGL_LIB_DEBUG "${NATID_SDK_LIB}/natGLD.dylib")
    set(NATGL_LIB_RELEASE "${NATID_SDK_LIB}/natGL.dylib")
else()
    set(NATGL_LIB_DEBUG "${NATID_SDK_LIB}/natGLD.so")
    set(NATGL_LIB_RELEASE "${NATID_SDK_LIB}/natGL.so")
endif()
