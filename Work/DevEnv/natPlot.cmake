#SecuLib

if (WIN32)
    set(NATPLOT_LIB_DEBUG "${MY_LIB}/natPlotD.lib")
    set(NATPLOT_LIB_RELEASE "${MY_LIB}/natPlot.lib")
elseif(APPLE)
    set(NATPLOT_LIB_DEBUG "${MY_LIB}/natPlotD.dylib")
    set(NATPLOT_LIB_RELEASE "${MY_LIB}/natPlot.dylib")
else()
    set(NATPLOT_LIB_DEBUG "${MY_LIB}/natPlotD.so")
    set(NATPLOT_LIB_RELEASE "${MY_LIB}/natPlot.so")
endif()
