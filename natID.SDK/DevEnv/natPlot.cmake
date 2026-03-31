#SecuLib

if (WIN32)
    set(NATPLOT_LIB_DEBUG "${NATID_SDK_LIB}/natPlotD.lib")
    set(NATPLOT_LIB_RELEASE "${NATID_SDK_LIB}/natPlot.lib")
elseif(APPLE)
    set(NATPLOT_LIB_DEBUG "${NATID_SDK_LIB}/natPlotD.dylib")
    set(NATPLOT_LIB_RELEASE "${NATID_SDK_LIB}/natPlot.dylib")
else()
    set(NATPLOT_LIB_DEBUG "${NATID_SDK_LIB}/natPlotD.so")
    set(NATPLOT_LIB_RELEASE "${NATID_SDK_LIB}/natPlot.so")
endif()
