#netOp - network operations

if (WIN32)
    set(NETOP_LIB_DEBUG "${MY_LIB}/netOpD.lib")
    set(NETOP_LIB_RELEASE "${MY_LIB}/netOp.lib")
elseif(APPLE)
    set(NETOP_LIB_DEBUG "${MY_LIB}/netOpD.dylib")
    set(NETOP_LIB_RELEASE "${MY_LIB}/netOp.dylib")
else()
    set(NETOP_LIB_DEBUG "${MY_LIB}/netOpD.so")
    set(NETOP_LIB_RELEASE "${MY_LIB}/netOp.so")
endif()
