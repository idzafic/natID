// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableView.h
 * @brief Base view class for custom-drawn, interactive canvas areas.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/BaseView.h>
#include <gui/Types.h>
#include <gui/InputDevice.h>
#include <gui/PrintInfo.h>
#include <gui/Cursor.h>

namespace gui
{

class DrawableViewHelper;

/// @brief A view that provides a full suite of drawing and input-event callbacks for custom content.
class NATGUI_API DrawableView : public BaseView
{
    friend class DrawableViewHelper;

protected:
    /// @brief Called when the view needs to be redrawn.
    /// @param rect The dirty rectangle that requires redrawing.
    virtual void onDraw(const gui::Rect& rect);

    /// @brief Provides the rectangle in model coordinates for a specific print page.
    /// @param pageNo                     One-based page number.
    /// @param pageRectInModelCoordinates Output rectangle in model coordinates.
    virtual void getRectForPage(td::UINT4 pageNo, gui::Rect& pageRectInModelCoordinates);

    /// @brief Called when the view should render itself to a printer.
    /// @param pi Print information describing the current print job.
    virtual void onPrint(const PrintInfo& pi);

    /// @brief Called when the view should render itself to a PDF document.
    /// @param pi Print information describing the current PDF export.
    virtual void onPDFPrint(const PrintInfo& pi);

    /// @brief Called when the primary mouse button is pressed inside the view.
    /// @param inputDevice Current state of the input device.
    virtual void onPrimaryButtonPressed(const gui::InputDevice& inputDevice);

    /// @brief Called when the primary mouse button is released inside the view.
    /// @param inputDevice Current state of the input device.
    virtual void onPrimaryButtonReleased(const gui::InputDevice& inputDevice);

    /// @brief Called when the secondary (right) mouse button is pressed inside the view.
    /// @param inputDevice Current state of the input device.
    virtual void onSecondaryButtonPressed(const gui::InputDevice& inputDevice);

    /// @brief Called when the secondary (right) mouse button is released inside the view.
    /// @param inputDevice Current state of the input device.
    virtual void onSecondaryButtonReleased(const gui::InputDevice& inputDevice);

    /// @brief Called when the primary mouse button is double-clicked inside the view.
    /// @param inputDevice Current state of the input device.
    virtual void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice);

    /// @brief Called when a zoom gesture is performed over the view.
    /// @param inputDevice Current state of the input device (includes zoom delta).
    /// @return True if the event was consumed.
    virtual bool onZoom(const gui::InputDevice& inputDevice);

    /// @brief Called when a scroll gesture is performed over the view.
    /// @param inputDevice Current state of the input device (includes scroll delta).
    /// @return True if the event was consumed.
    virtual bool onScroll(const gui::InputDevice& inputDevice);

    /// @brief Called when the cursor enters the view's bounds.
    /// @param inputDevice Current state of the input device.
    virtual void onCursorEntered(const gui::InputDevice& inputDevice);

    /// @brief Called when the cursor moves within the view.
    /// @param inputDevice Current state of the input device.
    virtual void onCursorMoved(const gui::InputDevice& inputDevice);

    /// @brief Called when the cursor is dragged (moved with a button held) within the view.
    /// @param inputDevice Current state of the input device.
    virtual void onCursorDragged(const gui::InputDevice& inputDevice);

    /// @brief Called when the cursor leaves the view's bounds.
    /// @param inputDevice Current state of the input device.
    virtual void onCursorExited(const gui::InputDevice& inputDevice);

//    virtual bool onKeyPressed(const gui::Key& key);
//    virtual bool onKeyReleased(const gui::Key& key);

//    void enableResizeEvent(bool bEnable); //false if not called
    /// @brief Called when the view is resized.
    /// @param newSize The new dimensions of the view.
    virtual void onResize(const gui::Size& newSize);

    /// @brief Called when the view gains keyboard focus.
    virtual void onFocusIn();

    /// @brief Called when the view loses keyboard focus.
    virtual void onFocusOut();

    /// @brief Protected default constructor; use derived classes to instantiate.
    DrawableView();
    DrawableView(const DrawableView&) = delete;
    DrawableView& operator =(const DrawableView&) = delete;
public:
    /// @brief Registers this view as the central content view of a window.
    /// @param pWnd      The window that will host this view.
    /// @param fixSizes  Size-fixing policy to apply.
    void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes) override;
//    void setCursor(Cursor::Type ct, bool showImmediately = false) const;

    /// @brief Subscribes this view to keyboard focus-change notifications.
    void registerForFocusEvents() const;

    /// @brief Shows or hides the mouse cursor while it is over this view.
    /// @param bShow True to show the cursor; false to hide it.
    void showCursor(bool bShow); //Show/hide cursor

    /// @brief Moves the mouse cursor to the center of this view.
    void moveCursorToCenter() const; //moves cursor to center of this view

    /// @brief Returns whether the view is focused, and the cursor position relative to the view.
    /// @param pt Output point in view-local coordinates.
    /// @return True if the view currently has focus.
    bool getCursorPosition(gui::Point& pt) const; //returns true if view is in focus, pt contains point relative to view
};

} //namespace gui
