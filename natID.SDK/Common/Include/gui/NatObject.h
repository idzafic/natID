// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file NatObject.h
    @brief Base class for all native GUI objects providing handle management and cross-thread utilities. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGUI.h"
#include <td/String.h>
#include "Types.h"
#include <tuple>
#include <td/Color.h>
#include <td/ColorID.h>
#include <gui/Font.h>
#include <functional>
#include "Lang.h"
#include <cnt/SafeFullVector.h>
#include <mu/IApplication.h>

namespace gui
{
class NatObjHelper;
class Application;
class Window;

/// @brief Type alias for a callable with no arguments used in async GUI calls.
typedef std::function<void()> AsyncFn;
/// @brief Function pointer type for a plain C async callback receiving a user-data pointer.
typedef void (*AsyncFn1)(void*);

/// @brief Base class for all native platform GUI objects; owns a platform handle and provides common services.
class NATGUI_API NatObject
{
    friend class NatObjHelper;

protected:
    gui::Handle _handle = nullptr; ///< Opaque native platform handle for this object.
private:
    NatObject(const NatObject&) = delete;
    NatObject& operator = (const NatObject&) = delete;
protected:
    /// @brief Default constructor; initialises the handle to nullptr.
    NatObject();

    /// @brief Move constructor; transfers ownership of the native handle.
    /// @param other The source object to move from.
    NatObject(NatObject&& other);

    /// @brief Move assignment operator; transfers ownership of the native handle.
    /// @param other The source object to move from.
    /// @return Reference to this object after the move.
    NatObject& operator=(NatObject&& other);

    /// @brief Virtual destructor; releases the native platform object if still owned.
    virtual ~NatObject();

    /// @brief Creates the underlying platform window object using geometry information.
    /// @param pg Pointer to the Geometry describing the initial position and size.
    void createObject(const gui::Geometry* pg); //Window

    /// @brief Creates the underlying platform object identified by a string resource ID.
    /// @param pStrID C-string resource identifier.
    void createObject(const char* pStrID); //Image

    /// @brief Creates the underlying platform object with the given size.
    /// @param sz Initial size for the object.
    void createObject(const gui::Size& sz);

    /// @brief Creates the underlying platform object from a string value.
    /// @param str String used to identify or initialise the platform object.
    void createObject(const td::String& str);

    /// @brief Creates the underlying platform object from a string and a font identifier.
    /// @param str String used to identify or initialise the platform object.
    /// @param fntID Font identifier associated with this object.
    void createObject(const td::String& str, gui::Font::ID fntID);

    /// @brief Releases the native platform object and resets the handle.
    void release();

    /// @brief Override point for platform-specific object destruction logic.
    virtual void destroy();

    /// @brief Resets the handle to nullptr without releasing resources (used when the window is closed externally).
    void resetHandle(); //just for window when is closed from outside

    /// @brief Returns a pointer to the application properties interface.
    /// @return Non-const pointer to the IAppProperties implementation.
    mu::IAppProperties* getAppProperties();

    /// @brief Returns a const pointer to the application properties interface.
    /// @return Const pointer to the IAppProperties implementation.
    const mu::IAppProperties* getAppProperties() const;

    /// @brief Copies a C-string to the system clipboard.
    /// @param pStr Null-terminated C-string to copy.
    void copyToClipboard(const char* pStr) const;

    /// @brief Copies a td::String to the system clipboard.
    /// @param str The string to copy.
    void copyToClipboard(const td::String& str) const;

public:
    /// @brief Returns the native platform handle for this object.
    /// @return The opaque gui::Handle value.
    gui::Handle getHandle() const;

    /// @brief Returns the object type and handle as a tuple for identification purposes.
    /// @return A tuple containing the ObjType and the native Handle.
    std::tuple<gui::ObjType, gui::Handle> getNatInfo() const;

    /// @brief Displays a modal alert dialog with the given message and informative text.
    /// @param msgText Primary message text shown in the alert.
    /// @param informativeText Secondary informative text providing additional context.
    void showAlert(const td::String& msgText, const td::String& informativeText) const;

    /// @brief Returns the GUI object type identifier for this object.
    /// @return ObjType::NA by default; overridden in derived classes.
    virtual gui::ObjType getObjType() const { return ObjType::NA;}

    //virtual gui::CtrlType getCtrlType() const { return CtrlType::NA;}

    /// @brief Returns whether this object is a control.
    /// @return True if the object is a control-type GUI element.
    bool isControl() const;

    /// @brief Returns whether this object is a view.
    /// @return True if the object is a view-type GUI element.
    bool isView() const;

    //call function from another thread and execute it in main (GUI) thread

    /// @brief Schedules a callable to be executed on the main GUI thread.
    /// @param pFn Pointer to the AsyncFn to call on the GUI thread.
    /// @param bDeleteFnPtrAfterCall If true, the AsyncFn pointer is deleted after the call completes.
    static void asyncCall(AsyncFn* pFn, bool bDeleteFnPtrAfterCall = false);

    /// @brief Schedules a plain C function to be executed on the main GUI thread with user data.
    /// @param pFn The function pointer to invoke on the GUI thread.
    /// @param userData Pointer passed as the argument to the function.
    static void asyncCall1(AsyncFn1 pFn, void* userData);

    ///translation

    /// @brief Translates a string key using the application's active language.
    /// @param strKey The translation key to look up.
    /// @return The translated td::String.
    td::String tr(const char* strKey) const;

    /// @brief Translates a string key and returns a C-string pointer.
    /// @param strKey The translation key to look up.
    /// @return A const char pointer to the translated string (valid until next translation call).
    const char* trCStr(const char* strKey) const;

    /// @brief Retrieves a text resource string by key.
    /// @param strKey The resource key to look up.
    /// @return The resolved resource string.
    td::String getResText(const char* strKey) const;

    /// @brief Retrieves a file-name resource string by key.
    /// @param strKey The resource key to look up.
    /// @return The resolved file-name string.
    td::String getResFileName(const char* strKey) const;

    /// @brief Retrieves a menu resource file-name string by key.
    /// @param strKey The resource key to look up.
    /// @return The resolved menu resource file-name string.
    td::String getMenuResFileName(const char* strKey) const;

    /// @brief Returns the list of languages supported by the application.
    /// @return Const reference to the vector of supported gui::Lang entries.
    const cnt::SafeFullVector<gui::Lang>& getSupportedLanguages() const;

    /// @brief Returns the index of the currently active translation language.
    /// @return Zero-based index into the supported-languages list.
    int getTranslationLanguageIndex() const;

    /// @brief Returns the currently active translation language descriptor.
    /// @return Const reference to the active gui::Lang.
    const gui::Lang& getTranslationLang() const;

    /// @brief Returns the visual identifier used for theming or rendering this object.
    /// @return A UINT4 visual ID; 0 if not overridden.
    virtual td::UINT4 getVisualID() const;

    /// @brief Returns a pointer to the owning Application object.
    /// @return Non-const pointer to the gui::Application.
    gui::Application* getApplication();

    /// @brief Returns a const pointer to the owning Application object.
    /// @return Const pointer to the gui::Application.
    const gui::Application* getApplication() const;

    /// @brief Returns a pointer to the application's initial (main) window.
    /// @return Non-const pointer to the main gui::Window.
    gui::Window* getInitialWindow();

    /// @brief Returns a const pointer to the application's initial (main) window.
    /// @return Const pointer to the main gui::Window.
    const gui::Window* getInitialWindow() const;
};

} //namespace gui
