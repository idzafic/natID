#SecuLib

if (WIN32)
    set(NATCODE_LIB_DEBUG "${NATID_SDK_LIB}/natCodeD.lib")
    set(NATCODE_LIB_RELEASE "${NATID_SDK_LIB}/natCode.lib")
elseif(APPLE)
    set(NATCODE_LIB_DEBUG "${NATID_SDK_LIB}/natCodeD.dylib")
    set(NATCODE_LIB_RELEASE "${NATID_SDK_LIB}/natCode.dylib")
else()
    set(NATCODE_LIB_DEBUG "${NATID_SDK_LIB}/natCodeD.so")
    set(NATCODE_LIB_RELEASE "${NATID_SDK_LIB}/natCode.so")
endif()
