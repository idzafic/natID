#SecuLib

if (WIN32)
    set(NATRPT_LIB_DEBUG "${MY_LIB}/natRptD.lib")
    set(NATRPT_LIB_RELEASE "${MY_LIB}/natRpt.lib")
elseif(APPLE)
    set(NATRPT_LIB_DEBUG "${MY_LIB}/natRptD.dylib")
    set(NATRPT_LIB_RELEASE "${MY_LIB}/natRpt.dylib")
else()
    set(NATRPT_LIB_DEBUG "${MY_LIB}/natRptD.so")
    set(NATRPT_LIB_RELEASE "${MY_LIB}/natRpt.so")
endif()
