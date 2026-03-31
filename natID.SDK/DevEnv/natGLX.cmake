#SecuLib

if (WIN32)
    set(NATGLX_LIB_DEBUG "${NATID_SDK_LIB}/natGLXD.lib")
    set(NATGLX_LIB_RELEASE "${NATID_SDK_LIB}/natGLX.lib")
elseif(APPLE)
    set(NATGLX_LIB_DEBUG "${NATID_SDK_LIB}/natGLXD.dylib")
    set(NATGLX_LIB_RELEASE "${NATID_SDK_LIB}/natGLX.dylib")
else()
    set(NATGLX_LIB_DEBUG "${NATID_SDK_LIB}/natGLXD.so")
    set(NATGLX_LIB_RELEASE "${NATID_SDK_LIB}/natGLX.so")
endif()
