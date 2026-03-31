#SecuLib

if (WIN32)
    set(NATGUI_LIB_DEBUG "${NATID_SDK_LIB}/natGUID.lib")
    set(NATGUI_LIB_RELEASE "${NATID_SDK_LIB}/natGUI.lib")
elseif(APPLE)
    set(NATGUI_LIB_DEBUG "${NATID_SDK_LIB}/natGUID.dylib")
    set(NATGUI_LIB_RELEASE "${NATID_SDK_LIB}/natGUI.dylib")
else()
    set(NATGUI_LIB_DEBUG "${NATID_SDK_LIB}/natGUID.so")
    set(NATGUI_LIB_RELEASE "${NATID_SDK_LIB}/natGUI.so")
endif()
