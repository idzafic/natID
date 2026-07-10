// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TabView.h
    @brief Tabbed view container that hosts multiple child views with a header scroller. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/_aux/TabHeaderScroller.h>
#include "VerticalLayout.h"
#include "ViewHeapSwitcher.h"
#include "ViewScroller.h"
#include "View.h"

namespace gui
{

/// @brief A view that presents child views using a tab-based navigation interface.
class NATGUI_API TabView : public View
{
    friend class TabHeader;
private:
    VerticalLayout _vl; ///< Internal vertical layout used to arrange header and content.
    TabHeaderScroller _headerScroller; ///< Scrollable tab header strip.
    ViewHeapSwitcher _views; ///< Switcher that manages heap-allocated child views.
    std::function<void(int)> _onChangedSelection; ///< Callback invoked when the selected tab changes.
    std::function<void(int pos)> _onClosedView; ///< Callback invoked when a tab is closed by the user.
protected:
    /// @brief Computes the preferred size of this tab view.
    /// @param[out] cell Cell information to fill with measurement data.
    void measure(CellInfo&) override;

    /// @brief Re-computes the size after a layout change.
    /// @param[out] cell Cell information to update.
    void reMeasure(CellInfo&) override;

    /// @brief Handles internal framework messages.
    /// @param msg The message to process.
    /// @return True if the message was consumed, false otherwise.
    bool _processFrameworkMessage(gui::Message& msg) override;

    /// @brief Removes a view at the given position and selects another.
    /// @param posToRemove Index of the view to remove.
    /// @param posToSelect Index of the view to select after removal.
    /// @param newSize New size to apply after removal.
    void remove(int posToRemove, int posToSelect, const Size& newSize);

    /// @brief Handles system context menu action items.
    /// @param aiDesc Descriptor of the triggered action item.
    /// @return True if the action was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override; //system context menus

    TabView(const TabView&) = delete;
    TabView& operator =(const TabView&) = delete;
    TabView() = delete;
public:
    /// @brief Constructs a TabView with the specified header style and font configuration.
    /// @param type Visual type of the tab header.
    /// @param minWidthInChars Minimum width of each tab in character units.
    /// @param maxWidthInChars Maximum width of each tab in character units.
    /// @param selFontID Font used for the selected tab label.
    /// @param fontID Font used for unselected tab labels.
    TabView(TabHeader::Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, gui::Font::ID selFontID = gui::Font::ID::SystemBold, gui::Font::ID fontID = gui::Font::ID::SystemNormal);

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::TabView.
    gui::ObjType getObjType() const override { return ObjType::TabView;}

    /// @brief Adds a view to the tab container with an image indicator.
    /// @param pView Pointer to the view to add.
    /// @param strTitle Display title for the tab.
    /// @param pImage Optional image shown on the tab header.
    /// @param selColor Background color used when this tab is selected.
    /// @return Zero-based index of the newly added view.
    int addView(BaseView* pView, const td::String& strTitle, const Image* pImage, td::ColorID selColor = td::ColorID::SysTransparentSelection);

    /// @brief Adds a view to the tab container with a symbol indicator.
    /// @param pView Pointer to the view to add.
    /// @param strTitle Display title for the tab.
    /// @param pSymbol Optional symbol shown on the tab header.
    /// @param selColor Background color used when this tab is selected.
    /// @return Zero-based index of the newly added view.
    int addView(BaseView* pView, const td::String& strTitle, const Symbol* pSymbol, td::ColorID selColor = td::ColorID::SysTransparentSelection);

    /// @brief Sets the ownership policy for a view at the given position.
    /// @param viewPos Zero-based index of the view.
    /// @param ownership Ownership mode (Intern means TabView will delete the view).
    void setViewOwnership(int viewPos, td::Ownership ownership); //Intern - TabView will delete the view (default)

    /// @brief Returns the current ownership policy for contained views.
    /// @return The ownership mode.
    td::Ownership getViewOwnership() const;

    /// @brief Detaches and returns a view without deleting it (requires Extern ownership).
    /// @param viewPos Zero-based index of the view to detach.
    /// @return Pointer to the detached view, or nullptr on failure.
    BaseView* detach(int viewPos); //if OwnerShip == Extern, otherwise fails

    /// @brief Removes the view at the specified position.
    /// @param viewPos Zero-based index of the view to remove.
    void removeView(int viewPos);

    /// @brief Removes the specified view by pointer.
    /// @param pView Pointer to the view to remove.
    void removeView(const BaseView* pView);

    /// @brief Removes all tabs to the left of the given position.
    /// @param viewPos Zero-based reference position.
    void removeAllLeftFrom(int viewPos);

    /// @brief Removes all tabs to the right of the given position.
    /// @param viewPos Zero-based reference position.
    void removeAllRightFrom(int viewPos);

    /// @brief Removes all tabs except the one at the specified position.
    /// @param viewPos Zero-based index of the tab to keep.
    void removeAllButOne(int viewPos);

    /// @brief Removes all views from the tab container.
    void removeAll();

    /// @brief Prevents or allows the user to close a specific tab.
    /// @param viewPos Zero-based index of the view.
    /// @param nonRemovable True to hide the close button and context menu entry.
    //view cannot be removed by user ineraction (there is no x button and no context menus for this view)
    void setNonRemovable(int viewPos, bool nonRemovable = true);

    /// @brief Returns a const pointer to the view at the given position.
    /// @param pos Zero-based index.
    /// @return Const pointer to the view.
    const BaseView* getView(int pos) const;

    /// @brief Returns a pointer to the view at the given position.
    /// @param pos Zero-based index.
    /// @return Pointer to the view.
    BaseView* getView(int pos);

    /// @brief Returns the currently displayed view.
    /// @return Pointer to the active view.
    BaseView* getCurrentView();

    /// @brief Returns the currently displayed view (const overload).
    /// @return Const pointer to the active view.
    const BaseView* getCurrentView() const;

    /// @brief Returns the zero-based index of the currently displayed view.
    /// @return Current view position.
    int getCurrentViewPos() const;

    /// @brief Returns the index of the given view within this container.
    /// @param pView Pointer to the view to locate.
    /// @return Zero-based index, or -1 if not found.
    int getViewPos(const BaseView* pView);

    /// @brief Makes the view at the given position visible.
    /// @param viewPos Zero-based index of the view to show.
    /// @param bSetFocus If true, the view also receives keyboard focus.
    /// @return True if the operation succeeded.
    bool showView(int viewPos, bool bSetFocus = true);

    /// @brief Sets a background view shown when no tab is active.
    /// @param pView Pointer to the background view.
    void setBackgroundView(gui::BaseView* pView);

    /// @brief Returns the total number of views in the container.
    /// @return Number of views.
    int getNumberOfViews() const;

    /// @brief Returns a reference to the tab header scroller.
    /// @return Reference to the internal TabHeaderScroller.
    TabHeaderScroller& getScroller();

    /// @brief Assigns a context menu to the tab header at the given position.
    /// @param pos Zero-based index of the tab.
    /// @param contextMenuID Identifier of the context menu to attach.
    /// @param contextMenuGroup Group identifier for the context menu.
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);

    /// @brief Forwards context-menu events for a tab to the specified frame.
    /// @param pos Zero-based index of the tab.
    /// @param pFrameToHandleCtxEvents Frame that will handle the context-menu events.
    void forwardContextMenuEvents(int pos, Frame* pFrameToHandleCtxEvents);

    ///Sets visible tab title
    /// @brief Sets the display title of the tab at the given position.
    /// @param pos Zero-based index of the tab.
    /// @param strTitle New title string to display.
    void setTitle(int pos, const td::String& strTitle);

    /// @brief Returns the display title of the tab at the given position.
    /// @param pos Zero-based index of the tab.
    /// @return The current title string.
    td::String getTitle(int pos) const;

    /// @brief Returns the selection-changed callback.
    /// @return Const reference to the stored callback function.
    const std::function<void(int)>& getChangedSelectionHandler() const;

    /// @brief Registers a callback invoked when the selected tab changes.
    /// @param fnToCall Function to call with the new tab index.
    void onChangedSelection(const std::function<void(int)>& fnToCall);

    /// @brief Returns the view-closed callback.
    /// @return Const reference to the stored callback function.
    const std::function<void(int)>& getClosedViewHandler() const;

    /// @brief Registers a callback invoked when a tab is closed by the user.
    /// @param fnToCall Function to call with the index of the closed tab.
    void onClosedView(const std::function<void(int)>& fnToCall);

    /// @brief Marks or unmarks a tab as modified (e.g., shows an unsaved-changes indicator).
    /// @param pos Zero-based index of the tab.
    /// @param bModified True to mark as modified.
    void setModified(int pos, bool bModified);

    /// @brief Returns whether the tab at the given position is marked as modified.
    /// @param pos Zero-based index of the tab.
    /// @return True if the tab is modified.
    bool isModified(int pos) const;

    /// @brief Notifies the tab view that the system color mode has changed.
    /// @param bDarkMode True if dark mode is now active.
    void systemColorModeChanged(bool bDarkMode) override;

    /// @brief Sets a numeric content-type identifier for the tab at the given position.
    /// @param pos Zero-based index of the tab.
    /// @param cntType Byte-sized content type value.
    void setContentTypeID(int pos, td::BYTE cntType);

    /// @brief Returns the numeric content-type identifier for the tab at the given position.
    /// @param pos Zero-based index of the tab.
    /// @return Byte-sized content type value.
    td::BYTE getContentTypeID(int pos) const;

    /// @brief Sets a typed content identifier for the tab at the given position.
    /// @tparam T Enum or integral type for the content identifier.
    /// @param pos Zero-based index of the tab.
    /// @param contentType Content type value to assign.
    template <typename T>
    void setContentType(int pos, T contentType)
    {
        setContentTypeID(pos, (td::BYTE) contentType);
    }

    /// @brief Retrieves the typed content identifier for the tab at the given position.
    /// @tparam T Enum or integral type for the content identifier.
    /// @param pos Zero-based index of the tab.
    /// @param[out] contentType Receives the stored content type value.
    template <typename T>
    void getContentType(int pos, T& contentType)
    {
        contentType = (T) getContentTypeID(pos);
    }

};

} //namespace gui
