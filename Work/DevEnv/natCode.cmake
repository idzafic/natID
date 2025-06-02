#SecuLib

if (WIN32)
    set(NATCODE_LIB_DEBUG "${MY_LIB}/NatCodeD.lib")
    set(NATCODE_LIB_RELEASE "${MY_LIB}/NatCode.lib")
elseif(APPLE)
    set(NATCODE_LIB_DEBUG "${MY_LIB}/NatCodeD.dylib")
    set(NATCODE_LIB_RELEASE "${MY_LIB}/NatCode.dylib")
else()
    set(NATCODE_LIB_DEBUG "${MY_LIB}/NatCodeD.so")
    set(NATCODE_LIB_RELEASE "${MY_LIB}/NatCode.so")
endif()
