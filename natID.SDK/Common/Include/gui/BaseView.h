// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BaseView.h
 * @brief Base class for all GUI views providing scaling, scrolling, and lifecycle hooks.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"
#include <gui/Alert.h>
#include <tuple>
#include <td/Variant.h>

namespace gui
{

/// @brief Namespace for popover positioning options.
namespace Popover
{
    /// @brief Describes the preferred location of a popover relative to its button.
    enum class Location : td::BYTE {AboveButton=0, BelowButon, LeftToButton, RightToButton};
}

class Application;
class Window;
class Image;
class Symbol;
class ToolBarItem;
class IPopoverButton;
class Layout;
class BaseViewHelper;
class ViewScroller;

/// @brief Base class for all view controls, providing geometry management, scaling, and event hooks.
class NATGUI_API BaseView : public Control
{
    friend class Application;
    friend class ToolBarItem;
    friend class BaseViewHelper;
    friend class ViewScroller;
    friend class Window;
protected:
    void* _menuActions = nullptr; ///< Pointer to registered menu action descriptors.
    td::Variant _varSource; ///< File name or other source indicator for the view.
    td::BYTE _contentType = 0; ///< User-defined content type identifier.

private:
    /// @brief Returns a handle to the native rendering surface.
    /// @return Platform-specific surface handle.
    virtual gui::Handle getNativeSurfaceHandle();
protected:
    /// @brief Default constructor.
    BaseView();
    BaseView(const BaseView&) = delete;
    BaseView& operator = (const BaseView&) = delete;

    /// @brief Adjusts the host window's initial size based on fixed-size constraints.
    /// @param fixSizes Fixed-size flags.
    /// @param sz Desired size.
    void adjustInitialWindowSize(gui::Frame::FixSizes fixSizes, const gui::Size& sz);

    /// @brief Re-adjusts the host window size after layout changes.
    /// @param fixSizes Fixed-size flags.
    /// @param sz Target size.
    /// @param minSize Minimum allowable size.
    void readjustWindowSize(gui::Frame::FixSizes fixSizes, const gui::Size& sz, const gui::Size& minSize);

    /// @brief Attaches a view scroller to this view.
    /// @param pScroller Pointer to the scroller to attach.
    virtual void setScroller(ViewScroller* pScroller);

    /// @brief Retrieves the logical model size if different from the view size.
    /// @param modelSize Output size of the model.
    /// @return True if a custom model size was set, false otherwise.
    virtual bool getModelSize(gui::Size& modelSize) const;

    /// @brief Estimates the desired initial size for the view.
    /// @param initialSize Proposed initial size.
    /// @param desiredSize Output desired size.
    /// @return True if the estimate was computed successfully.
    virtual bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const;

    /// @brief Called when the view's geometry changes.
    /// @param newGeometry The new geometry.
    virtual void onGeometryChange(const Geometry& newGeometry);

    /// @brief Called when the content size changes.
    /// @param newSize The new content size.
    virtual void onContentSizeChange(const gui::Size& newSize);

    /// @brief Sets the zoom scale factor for the view.
    /// @param newScale The new scale value.
    virtual void setScale(double newScale);

    /// @brief Called after all child controls have been measured.
    /// @param ci Cell info accumulating measurement data.
    virtual void finishedChildrenMeasuring(CellInfo& ci);

    /// @brief Called to determine whether the view should close.
    /// @return True to allow closing, false to prevent it.
    virtual bool shouldClose(); //tabview will not be closed if this method returns false

    /// @brief Called once when the view is about to close.
    virtual void onClose(); //will be called only once

    /// @brief Returns whether the view has fixed width and/or height.
    /// @return Tuple of (fixedWidth, fixedHeight) booleans.
    virtual std::tuple<bool, bool> isFixedSize() const;

    /// @brief Returns the layout associated with this view.
    /// @return Pointer to the layout, or nullptr if none.
    virtual Layout* getLayout();

    /// @brief Called when the system color mode (dark/light) changes.
    /// @param bDarkMode True if dark mode is now active.
    virtual void systemColorModeChanged(bool bDarkMode);

    /// @brief Adjusts the content area size of the host window.
    /// @param pWnd Pointer to the host window.
    /// @param fixSizes Fixed-size flags.
    void adjustContentSize(Window* pWnd, Frame::FixSizes fixSizes);

    /// @brief Called once when the view first appears as a central or main view.
    virtual void onInitialAppearance(); //will be called only once on central views and main dialog view
public:
    /// @brief Returns the fixed-size configuration info for this view.
    /// @return Fixed-size flags.
    virtual Frame::FixSizes getFixSizesInfo();

    /// @brief Places this view as the central view of a window.
    /// @param pWnd The host window.
    /// @param fixSizes Fixed-size flags.
    virtual void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes);

    /// @brief Scales the view by the given factor.
    /// @param newScale The scale factor to apply.
    virtual void scale(double newScale);

    /// @brief Scales the view around a specific point.
    /// @param newScale The scale factor to apply.
    /// @param toPoint The focal point for the zoom.
    virtual void scaleToPoint(double newScale, const gui::Point& toPoint);

    /// @brief Returns the current zoom scale factor.
    /// @return The current scale.
    virtual double getScale() const;

    /// @brief Sets the source indicator (e.g. file name) for this view.
    /// @param varFilenameOrOtherIndicator The source variant.
    void setSource(const td::Variant& varFilenameOrOtherIndicator);
    ///returnns fileName of other indicator used for the view
    const td::Variant& getSource() const;

    /// @brief Retrieves the source indicator, casting it to a specific type.
    /// @tparam TSOURCE Target type.
    /// @param src Output variable to fill with the source value.
    template <typename TSOURCE>
    void getSource(TSOURCE& src)
    {
        const td::Variant& var = getSource();
        var.getValue(src);
    }

    /// @brief Sets a user-defined content type byte.
    /// @param cntType The content type identifier.
    void setContentTypeID(td::BYTE cntType);

    /// @brief Returns the user-defined content type byte.
    /// @return The content type identifier.
    td::BYTE getContentTypeID() const;

    /// @brief Sets a typed content type value.
    /// @tparam T Enum or integer type.
    /// @param contentType The content type value.
    template <typename T>
    void setContentType(T contentType)
    {
        setContentTypeID((td::BYTE) contentType);
    }

    /// @brief Retrieves the content type as a specific type.
    /// @tparam T Enum or integer type.
    /// @param contentType Output variable to receive the value.
    template <typename T>
    void getContentType(T& contentType)
    {
        contentType = (T) getContentTypeID();
    }

    /// @brief Registers the view to receive scroll events.
    void registerForScrollEvents();

    //event handlers
    /// @brief Handles a menu or toolbar action item.
    /// @param aiDesc Descriptor of the triggered action item.
    /// @return True if the event was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;

    /// @brief Binds a callback to a menu action (no sub-menu).
    /// @param menuID Menu identifier.
    /// @param actionID Action identifier within the menu.
    /// @param fnToCall Callback to invoke.
    void onActionItem(td::BYTE menuID, td::BYTE actionID, const std::function<void()>& fnToCall); //first and last submenu = 0

    /// @brief Binds a callback to a menu action within a sub-menu range.
    /// @param menuID Menu identifier.
    /// @param firstSubMenuID First sub-menu identifier in the range.
    /// @param actionID Action identifier.
    /// @param lastMenuID Last sub-menu identifier in the range.
    /// @param fnToCall Callback to invoke.
    void onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE actionID, td::BYTE lastMenuID, const std::function<void()>& fnToCall);
};

} //namespace gui
