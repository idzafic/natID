#SecuLib

if (WIN32)
    set(NATGUI_LIB_DEBUG "${MY_LIB}/natGUID.lib")
    set(NATGUI_LIB_RELEASE "${MY_LIB}/natGUI.lib")
elseif(APPLE)
    set(NATGUI_LIB_DEBUG "${MY_LIB}/natGUID.dylib")
    set(NATGUI_LIB_RELEASE "${MY_LIB}/natGUI.dylib")
else()
    set(NATGUI_LIB_DEBUG "${MY_LIB}/natGUID.so")
    set(NATGUI_LIB_RELEASE "${MY_LIB}/natGUI.so")
endif()
