// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Frame.h
    @brief Base class for all visible GUI frame elements including views and windows. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Consumer.h"
#include "Display.h"
#include "Alert.h"
#include <cnt/PushBackVector.h>
#include <gui/_aux/ContextMenuNat.h>
#include <gui/InputDevice.h>

namespace gui
{
class FrameHelper;
class Window;
class Button;

/// @brief Base class representing a rectangular UI region that can receive input and display content.
class NATGUI_API Frame : public Consumer
{
    friend class FrameHelper;
public:
    /// @brief Controls which dimensions of a frame are fixed and cannot be resized.
    enum class FixSizes : td::BYTE {
        No=0,              ///< No dimensions are fixed; frame is freely resizable.
        FixMin=1,          ///< Minimum size is fixed.
        FixHorMax=2,       ///< Maximum horizontal size is fixed.
        FixMin_andHorMax=3,///< Minimum size and maximum horizontal size are fixed.
        FixVerMax=4,       ///< Maximum vertical size is fixed.
        FixMin_andVerMax=5,///< Minimum size and maximum vertical size are fixed.
        FixHorMinAndMax=6, ///< Both minimum and maximum horizontal sizes are fixed.
        FixAll=7,          ///< Both horizontal and vertical sizes are fixed (non-resizable).
        FixAuto=8          ///< Size fixing is determined automatically.
    };

    /// @brief Controls whether resize/show animations are enabled.
    enum class Animation : td::BYTE {
        Yes,           ///< Animation is always enabled.
        No,            ///< Animation is always disabled.
        SystemDefault  ///< Use the system preference.
    };
protected:
    Frame* _parentFrame = nullptr; ///< Parent frame in which this frame is embedded, or nullptr for top-level.
    td::LUINT8 _tagID;             ///< Application-assigned tag identifier.
    cnt::PushBackVector<ContextMenuNat> _contextMenus; ///< Registered context menus.
private:
    /// @brief Returns the native handle of a context menu by its menu ID.
    /// @param menuID Identifier of the context menu.
    /// @return Native handle for the requested menu.
    gui::Handle getContextMenuNatHandle(td::BYTE menuID) const;
    Frame();
    Frame(const Frame&) = delete;
    void operator = (const Frame&) = delete;
protected:
    /// @brief Called when the frame receives keyboard focus.
    virtual void onSetFocus();

    /// @brief Called when the frame loses keyboard focus.
    virtual void onLostFocus();

    /// @brief Requests a redraw of the specified rectangular region.
    /// @param rToRedraw Rectangle to invalidate (in frame-local coordinates).
    void reDraw(const gui::Rect& rToRedraw);

    /// @brief Shows an asynchronous Yes/No question dialog and notifies a consumer with the result.
    /// @param questionID Identifier sent with the message to the consumer.
    /// @param pConsumer Consumer that will receive the response message.
    /// @param msgText Primary message text.
    /// @param informativeText Secondary informative text.
    /// @param strYes Label for the Yes button.
    /// @param strNo Label for the No button.
    void showYesNoQuestionAsyncWithConsumer(td::UINT4 questionID, IMessageConsumer* pConsumer, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const;

    //key events
    /// @brief Called when a key is pressed while this frame has focus.
    /// @param key Key descriptor.
    /// @return True if the event was consumed.
    virtual bool onKeyPressed(const gui::Key& key);

    /// @brief Called when a key is released while this frame has focus.
    /// @param key Key descriptor.
    /// @return True if the event was consumed.
    virtual bool onKeyReleased(const gui::Key& key);
public:
//    Frame();
    /// @brief Constructs a frame with the given parent.
    /// @param parentFrame Pointer to the parent frame (may be nullptr for top-level frames).
    Frame(Frame* parentFrame);
//    ~Frame();

    /// @brief Requests a full redraw of the entire frame area.
    void reDraw();

    /// @brief Sets the frame geometry (position and size).
    /// @param g New geometry to apply.
    void setGeometry(const Geometry& g);

    /// @brief Returns the current frame geometry.
    /// @param g Geometry structure to fill.
    void getGeometry(Geometry& g) const;

    /// @brief Returns display information for the screen on which this frame resides.
    /// @param di Display::Info structure to fill.
    void getDisplayInfo(gui::Display::Info& di) const;

    /// @brief Sets the frame height.
    /// @param h New height in logical pixels.
    void setHeight(gui::CoordType h);

    /// @brief Sets the frame width.
    /// @param w New width in logical pixels.
    void setWidth(gui::CoordType w);

    /// @brief Requests a new frame size.
    /// @param desiredSize Desired width and height.
    void setSize(const Size& desiredSize);

    /// @brief Returns the current frame size.
    /// @param sz Size structure to fill.
    void getSize(Size& sz) const;

    /// @brief Returns the top-left corner of the frame in screen coordinates.
    /// @param tl Point structure to fill.
    void getTLInScreenCoordinates(gui::Point& tl) const;

    /// @brief Returns the centre of the frame in screen coordinates.
    /// @param center Point structure to fill.
    void getCenterInScreenCoordinates(gui::Point& center) const;

    /// @brief Suggests a preferred content size to the layout system.
    /// @param sz Preferred content size.
    void setPreferedContentSize(const gui::Size& sz);

    /// @brief Hides or shows the frame.
    /// @param bHide True to hide; false to show.
    void hide(bool bHide = true);

    /// @brief Un-hides the frame.
    /// @param bUnhide True to make the frame visible.
    void unHide(bool bUnhide = true);

    /// @brief Returns whether the frame is currently hidden.
    /// @return True if hidden.
    virtual bool isHidden() const;

    /// @brief Returns whether the frame is currently visible.
    /// @return True if visible (not hidden).
    bool isVisible() const;

    /// @brief Returns whether the frame is in a state where drawing is permitted.
    /// @return True if drawable.
    bool isDrawable() const;

    /// @brief Sets the title text of the frame.
    /// @param title New title string.
    virtual void setTitle(const td::String& title);

    /// @brief Sets the title text of the frame from a C-string.
    /// @param title Null-terminated title string.
    virtual void setTitle(const char* title);

    /// @brief Returns the current title of the frame.
    /// @return Title string.
    td::String getTitle() const;

    /// @brief Moves keyboard focus to this frame.
    /// @param selectAll If true, all content is selected when focus is obtained (e.g. for text controls).
    void setFocus(bool selectAll = true);

    /// @brief Enables or disables the ability to receive keyboard focus.
    /// @param bCanTakeFocus True to allow focus; false to prevent it.
    void setFocusable(bool bCanTakeFocus);

    /// @brief Sets the background colour of the frame.
    /// @param colorID Colour identifier to use.
    void setBackgroundColor(td::ColorID colorID);

    /// @brief Sets the parent frame reference.
    /// @param pFrame Pointer to the new parent frame.
    virtual void setParentFrame(Frame* pFrame);

    //Returns Frame in which this frame resides
    //If this is Window it returns nullptr
    /// @brief Returns the parent frame in which this frame is embedded.
    /// @return Pointer to the parent frame, or nullptr if this is a top-level window.
    Frame* getParentFrame();

    /// @brief Returns a const pointer to the parent frame.
    /// @return Const pointer to the parent frame.
    const Frame* getParentFrame() const;

    //Returns the parent Window in which this frame is located.
    //If this is Window, it returns the parent window (on which is kept on top)
    /// @brief Returns the Window that directly contains this frame.
    /// @return Pointer to the parent Window, or nullptr if this is a top-level window without a parent.
    Window* getParentWindow();

    /// @brief Returns a const pointer to the parent Window.
    /// @return Const pointer to the parent Window.
    const Window* getParentWindow() const;

    /// @brief Converts a point from this frame's coordinate space to the parent window's coordinate space.
    /// @param ptIn Point in frame-local coordinates.
    /// @param ptOut Point in parent-window coordinates.
    void getPointInParentWindow(const gui::Point& ptIn, gui::Point& ptOut) const;

    //Returns Window in which this frame resides
    //If this is Window it returns itself
    /// @brief Returns the Window that ultimately contains this frame.
    /// @return Const pointer to the containing Window (self if this frame is a Window).
    const Window* getContainingWindow() const;

    /// @brief Returns a non-const pointer to the Window that ultimately contains this frame.
    /// @return Pointer to the containing Window.
    Window* getContainingWindow();

    /// @brief Assigns an arbitrary tag identifier to this frame.
    /// @param tagID Tag value to store.
    void setTagID(const td::LUINT8 tagID);

    /// @brief Returns the tag identifier assigned to this frame.
    /// @return Tag identifier.
    td::LUINT8 getTagID() const;

    /// @brief Returns the tag identifier cast to a user-defined type.
    /// @tparam T Desired return type.
    /// @param tagID Variable to receive the tag cast to T.
    template <typename T>
    void getTagID(T& tagID)
    {
        tagID = T(getTagID());
    }

    /// @brief Displays a non-blocking informational alert with a message and supplementary text.
    /// @param msgText Primary alert text.
    /// @param informativeText Secondary informative text.
    void showAlert(const td::String& msgText, const td::String& informativeText) const;

    /// @brief Shows an asynchronous Yes/No question dialog with a callback.
    /// @param pCallBackFn Callback invoked with the user's answer.
    /// @param msgText Primary question text.
    /// @param informativeText Secondary informative text.
    /// @param strYes Label for the Yes button.
    /// @param strNo Label for the No button.
    void showYesNoQuestionAsync(Alert::CallBack* pCallBackFn, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const;

    /// @brief Shows an asynchronous Yes/No question and routes the answer to a message consumer.
    /// @tparam TINT Integral type of the question identifier.
    /// @param questionID Identifier sent with the response message.
    /// @param pConsumer Consumer that will receive the response.
    /// @param msgText Primary question text.
    /// @param informativeText Secondary informative text.
    /// @param strYes Label for the Yes button.
    /// @param strNo Label for the No button.
    template <typename TINT>
    void showYesNoQuestionAsync(TINT questionID, IMessageConsumer* pConsumer, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const
    {
        return showYesNoQuestionAsyncWithConsumer((td::UINT4)questionID, pConsumer, msgText, informativeText, strYes, strNo);
    }

    /// @brief Shows an asynchronous Yes/No question with a lambda callback.
    /// @param msgText Primary question text.
    /// @param informativeText Secondary informative text.
    /// @param strYes Label for the Yes button.
    /// @param strNo Label for the No button.
    /// @param callBackLambda Lambda invoked with the Alert::Answer when the user responds.
    void showYesNoQuestionAsync(const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo, Alert::CallBack callBackLambda) const;

    /// @brief Opens a context menu at the specified location.
    /// @param menuID Identifier of the menu to open.
    /// @param framePoint Position in frame-local coordinates where the menu should appear.
    /// @param pConsumer Optional consumer to receive menu action messages.
    /// @param pContextData Optional context data passed with action messages.
    /// @param groupID Optional group identifier for the menu.
    void openContextMenu(td::BYTE menuID, const gui::Point& framePoint, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 groupID = 0);

    /// @brief Returns whether this frame is backed by a native OS widget.
    /// @return True if native.
    bool isNative() const;

    //messages
    /// @brief Displays an informational message panel attached to this frame.
    /// @param txtHeader Heading text.
    /// @param txtMsg Body message text.
    void showInfo(const td::String& txtHeader, const td::String& txtMsg) const;

    /// @brief Displays a warning message panel attached to this frame.
    /// @param txtHeader Heading text.
    /// @param txtMsg Body message text.
    void showWarning(const td::String& txtHeader, const td::String& txtMsg) const;

    /// @brief Displays an error message panel attached to this frame.
    /// @param txtHeader Heading text.
    /// @param txtMsg Body message text.
    void showError(const td::String& txtHeader, const td::String& txtMsg) const;
};

} //namespace gui
