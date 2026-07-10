// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TabHeaderScroller.h
 *  @brief Scrollable container for a TabHeader, enabling navigation when tabs overflow the visible area. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Font.h>
#include "TabHeader.h"
#include <gui/ViewScroller.h>

namespace gui
{
/// @brief Wraps a TabHeader inside a ViewScroller so that tabs can be scrolled horizontally.
class TabView;

/// @brief ViewScroller subclass that hosts a TabHeader and exposes its tab management interface.
class NATGUI_API TabHeaderScroller : public ViewScroller
{
    friend class TabView;

private:
    TabHeader _header; ///< The embedded tab header rendered inside the scroller.
protected:
    /// @brief Measures the preferred size of the scroller and its header.
    /// @param cell Output cell layout information.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the scroller after content or layout changes.
    /// @param cell Output cell layout information.
    void reMeasure(CellInfo& cell) override;

    /// @brief Positions the scroller and header within the assigned cell frame.
    /// @param cellFrame The geometry of the cell allocated to this scroller.
    /// @param cell The cell layout descriptor.
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

    /// @brief Returns the current size of the embedded header.
    /// @return Reference to the header size structure.
    const Size& getHeaderSize() const;

    /// @brief Appends a new tab with a raster image icon.
    /// @param pImg Pointer to the icon image (may be nullptr).
    /// @param lbl Tab label string.
    /// @param selLineColor Color of the selection underline.
    void append(const Image* pImg, const td::String& lbl, td::ColorID selLineColor);

    /// @brief Appends a new tab with a vector symbol icon.
    /// @param pSymb Pointer to the symbol (may be nullptr).
    /// @param lbl Tab label string.
    /// @param selLineColor Color of the selection underline.
    void append(const Symbol* pSymb, const td::String& lbl, td::ColorID selLineColor);

    /// @brief Forwards a tab selection-changed message to the owning TabView.
    void sendSelChangeMessage();

    /// @brief Called when the scroller geometry changes; updates internal layout.
    /// @param newGeometry The new geometry of the scroller.
    void onGeometryChange(const Geometry& newGeometry) override;

    /// @brief Called when the scrollable content size changes.
    /// @param newSize The new content size.
    void onContentSizeChange(const gui::Size& newSize) override;

    /// @brief Sets the total scrollable content size.
    /// @param size The new content size.
    void setContentSize(const gui::Size& size);

    /// @brief Removes the tab at the specified position (called internally).
    /// @param pos Zero-based index of the tab to remove.
    void removeTab(int pos);

    /// @brief Removes a tab with full control over selection and redraw.
    /// @param pos Zero-based index of the tab to remove.
    /// @param newSelection Index of the tab that should become selected after removal.
    /// @param sendMessage If true a selection-changed message is sent.
    /// @param bRedraw If true the header is redrawn after removal.
    void removeTabExt(int pos, int newSelection, bool sendMessage, bool bRedraw);

    /// @brief Ensures the specified tab is visible by scrolling the header if needed.
    /// @param pos Zero-based index of the tab to show.
    /// @param bSetFocus If true keyboard focus is transferred to the tab content.
    void showTab(int pos, bool bSetFocus = true);

    /// @brief Sets the title of the specified tab.
    /// @param pos Zero-based index of the tab.
    /// @param strTitle The new title string.
    void setTitle(int pos, const td::String& strTitle);

    /// @brief Returns the title of the specified tab.
    /// @param pos Zero-based index of the tab.
    /// @return The title string.
    td::String getTitle(int pos) const;

    /// @brief Marks or unmarks the specified tab as having unsaved modifications.
    /// @param pos Zero-based index of the tab.
    /// @param bModified True to mark as modified.
    void setModified(int pos, bool bModified);

    /// @brief Returns whether the specified tab is marked as modified.
    /// @param Pos Zero-based index of the tab.
    /// @return True if the tab is marked as modified.
    bool isModified(int Pos) const;
//    void setContentTypeID(int pos, td::BYTE cntType);
//    td::BYTE getContentTypeID(int pos) const;

    /// @brief Assigns a context menu to the specified tab.
    /// @param pos Zero-based index of the tab.
    /// @param contextMenuID Menu identifier.
    /// @param contextMenuGroup Optional menu group identifier.
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);

    /// @brief Routes context menu events for the specified tab to an external frame.
    /// @param pos Zero-based index of the tab.
    /// @param pConsumer The frame that should receive the context menu events.
    void forwardContextMenuEventsTo(int pos, Frame* pConsumer);

    /// @brief Sets whether the user can close the specified tab.
    /// @param viewPos Zero-based index of the tab.
    /// @param nonRemovable True to prevent the user from closing the tab.
    void setNonRemovable(int viewPos, bool nonRemovable);

    /// @brief Called when the system color mode (dark/light) changes.
    /// @param bDarkMode True if the system switched to dark mode.
    void systemColorModeChanged(bool bDarkMode) override;
public:
    /// @brief Constructs a TabHeaderScroller associated with a TabView.
    /// @param pTabView Pointer to the owning TabView.
    /// @param type Tab sizing type (Fix, FitToText, or Dynamic).
    /// @param minWidthInChars Minimum tab width expressed in character units.
    /// @param maxWidthInChars Maximum tab width expressed in character units.
    /// @param selFontID Font identifier used for the selected tab label.
    /// @param fontID Font identifier used for non-selected tab labels.
    TabHeaderScroller(TabView* pTabView, TabHeader::Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, Font::ID selFontID, Font::ID fontID);

    /// @brief Destructor.
    ~TabHeaderScroller();
};

} //namespace gui
