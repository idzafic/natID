#netOp - network operations

if (WIN32)
    set(NETOP_LIB_DEBUG "${NATID_SDK_LIB}/netOpD.lib")
    set(NETOP_LIB_RELEASE "${NATID_SDK_LIB}/netOp.lib")
elseif(APPLE)
    set(NETOP_LIB_DEBUG "${NATID_SDK_LIB}/netOpD.dylib")
    set(NETOP_LIB_RELEASE "${NATID_SDK_LIB}/netOp.dylib")
else()
    set(NETOP_LIB_DEBUG "${NATID_SDK_LIB}/netOpD.so")
    set(NETOP_LIB_RELEASE "${NATID_SDK_LIB}/netOp.so")
endif()
