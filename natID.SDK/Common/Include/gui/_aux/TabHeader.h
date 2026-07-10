// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TabHeader.h
 *  @brief Canvas-based tab header bar that displays and manages clickable tab items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/BaseView.h>
#include <gui/Types.h>
#include <td/String.h>
#include <gui/Canvas.h>
#include <cnt/PushBackVector.h>
#include <gui/Shape.h>
#include <gui/Font.h>

namespace gui
{

class Image;
class Symbol;
class TabView;
class TabHeaderScroller;

/// @brief Renders a horizontal strip of tabs, handling selection, hover, close buttons, and context menus.
class NATGUI_API TabHeader : public Canvas
{
    friend class TabHeaderScroller;
public:
    /// @brief Controls how the width of each tab item is determined.
    enum class Type : unsigned char {Fix=0,       ///< All tabs have a fixed, equal width.
                                     FitToText,   ///< Each tab is wide enough to fit its label.
                                     Dynamic      ///< Tab widths are adjusted dynamically as tabs are added/removed.
                                    };
protected:
    /// @brief Internal descriptor for a single tab item.
    class Item
    {
    public:
        /// @brief Type of icon displayed alongside the tab label.
        enum class Type: unsigned char {Image=0,  ///< Item uses a raster Image icon.
                                        Symbol,   ///< Item uses a vector Symbol icon.
                                        NA        ///< Item has no icon.
                                       };
        union
        {
            Image* pImage = nullptr; ///< Pointer to the raster image icon (active when type == Image).
            Symbol* pSymbol;         ///< Pointer to the vector symbol icon (active when type == Symbol).
        };
        gui::Frame* pCtxFrameHandler; ///< Frame that receives forwarded context-menu action events.
        gui::CoordType x;             ///< Horizontal origin of this tab item within the header.
        gui::CoordType width;         ///< Width of this tab item.
        td::String lbl;               ///< Label text of the tab.
        NatString natStr;             ///< Native (platform) representation of the label used for drawing.
        //td::UINT2 posInSwitcher;
        td::UINT2 contextMenuGroup;   ///< System context menu group identifier for this tab.
        td::ColorID selLineColor;     ///< Color of the selection underline for this tab.
        Type type;                    ///< Icon type (Image, Symbol, or NA).
        td::BYTE contextMenuID;       ///< Context menu identifier used when right-clicking this tab.
        td::BYTE nonRemovable = 0;    ///< Non-zero if the tab cannot be closed by the user.
        td::BYTE modified = 0;        ///< Non-zero if the tab's content has unsaved changes.
//        td::BYTE contentType = 0;
    };
private:
    TabView* _pTabView;                          ///< Pointer to the owning TabView.
    cnt::PushBackVector<Item, 32> _items;        ///< List of tab items.
    Shape _lineCurrSelection;                    ///< Shape used to draw the selection underline.
    Shape _closeTabSymbol;                       ///< Shape used to draw the close-tab (X) button.
    Size _size;                                  ///< Current size of the header bar.
    double _closeTabSymbolScale;                 ///< Scale factor applied to the close-tab symbol.
    CoordType _lineLenVisible;                   ///< Visible length of the selection underline.
    int _currentSelection;                       ///< Index of the currently selected tab.
    int _currentHover;                           ///< Index of the tab currently under the cursor (-1 if none).
    td::UINT2 _minWidth;                         ///< Minimum tab item width in pixels.
    td::UINT2 _maxWidth;                         ///< Maximum tab item width in pixels.
    Font::ID _selFontID;                         ///< Font used for the selected tab label.
    Font::ID _nonSelFontID;                      ///< Font used for non-selected tab labels.
    Type _type;                                  ///< Tab sizing type.
    td::BYTE _selLineWidth;                      ///< Pixel thickness of the selection underline.
    static Shape selBorderLine;
protected:
    /// @brief Sends a selection-changed message to the owning TabView.
    void sendSelChangeMessage();

    /// @brief Calculates and sets the bounding rectangle of a tab item.
    /// @param item The item whose rectangle should be computed.
    void calcItemRect(Item& item) const;

    /// @brief Returns the width reserved for tab item icons.
    /// @return Width in pixels.
    CoordType getImageWidth() const;

    /// @brief Redraws the selection underline shape for the current tab.
    void updateSelectionLine();

    /// @brief Draws the close-tab (X) symbol at the correct position.
    void drawCloseTabSymbol() const;

    /// @brief Recalculates the geometry of all items starting at the given index.
    /// @param fromItem Index of the first item to recalculate.
    void updateGeometryOfItems(int fromItem);

    /// @brief Determines which tab the cursor is hovering over.
    /// @param viewPoint Current cursor position in view coordinates.
    void calcHoveringItem(const gui::Point& viewPoint);

    /// @brief Handles view resize events to reflow tab items.
    /// @param newSize The new size of the header view.
    void onResize(const gui::Size& newSize) override;

    /// @brief Removes the tab at the given position (called internally).
    /// @param pos Zero-based index of the tab to remove.
    void removeTab(int pos); //from inside

    /// @brief Removes a tab from an external caller with full control over selection and redraw.
    /// @param pos Zero-based index of the tab to remove.
    /// @param newSelection Index of the tab that should become selected after removal.
    /// @param sendMessage If true a selection-changed message is sent.
    /// @param bRedraw If true the header is redrawn after removal.
    void removeTabExt(int pos, int newSelection, bool sendMessage, bool bRedraw); //from outside

    /// @brief Scrolls the header so that the specified tab is visible and optionally focuses it.
    /// @param viewPos Zero-based index of the tab to show.
    /// @param bSetFocus If true keyboard focus is transferred to the tab content.
    void showTab(int viewPos, bool bSetFocus = true);

    /// @brief Returns the current size of the header bar.
    /// @return Reference to the size structure.
    const Size& getSize() const;

    /// @brief Returns the bounding rectangle of the specified tab item.
    /// @param posItem Zero-based item index.
    /// @param r Output rectangle.
    /// @return True if the item exists and the rectangle was filled.
    bool getItemRect(size_t posItem, Rect& r) const;

    /// @brief Sets the title text of an existing tab.
    /// @param pos Zero-based index of the tab.
    /// @param strTitle The new title string.
    void setTitle(int pos, const td::String& strTitle);

    /// @brief Returns the title text of the specified tab.
    /// @param pos Zero-based index of the tab.
    /// @return The title string.
    td::String getTitle(int pos) const;

    /// @brief Marks or unmarks a tab as having unsaved modifications.
    /// @param pos Zero-based index of the tab.
    /// @param bModified True to mark as modified.
    void setModified(int pos, bool bModified);

    /// @brief Returns whether the specified tab is marked as modified.
    /// @param Pos Zero-based index of the tab.
    /// @return True if the tab is marked as modified.
    bool isModified(int Pos) const;

//    void setContentTypeID(int pos, td::BYTE cntType);
//    td::BYTE getContentTypeID(int pos) const;

    /// @brief Fills common Item fields from a label and selection line color.
    /// @param item The item to populate.
    /// @param lbl The tab label string.
    /// @param selLineColor Color used for the selection underline.
    void populate(Item& item, const td::String& lbl, td::ColorID selLineColor);

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

    /// @brief Sets the visible length of the selection underline.
    /// @param newLineLen The new line length in logical units.
    void setLineLen(CoordType newLineLen);

    //virtual void onResize(const gui::Rect& newBounds);

    /// @brief Draws all tab items within the given dirty rectangle.
    /// @param rect The rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Handles a primary button press to select the clicked tab or close a tab.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Handles a secondary button press to show a context menu for the clicked tab.
    /// @param inputDevice The input device that generated the event.
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    //virtual void onMouseDrag(const gui::Point& viewPoint, td::UINT4 keyModifiers);

    /// @brief Called when the cursor enters the header area.
    /// @param inputDevice The input device that generated the event.
    void onCursorEntered(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor moves within the header area.
    /// @param inputDevice The input device that generated the event.
    void onCursorMoved(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor exits the header area.
    /// @param inputDevice The input device that generated the event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;

    /// @brief Returns the total model size required to display all tabs.
    /// @param modelSize Output size.
    /// @return True if the size was computed successfully.
    bool getModelSize(gui::Size& modelSize) const override;

    /// @brief Assigns a context menu to the specified tab.
    /// @param pos Zero-based index of the tab.
    /// @param contextMenuID Menu identifier.
    /// @param contextMenuGroup Optional menu group identifier.
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);

    /// @brief Routes context menu action events for the specified tab to an external frame.
    /// @param pos Zero-based index of the tab.
    /// @param pFrame The frame that should receive the context menu events.
    void forwardContextMenuEventsTo(int pos, Frame* pFrame);

    /// @brief Sets whether the user can close the specified tab.
    /// @param viewPos Zero-based index of the tab.
    /// @param nonRemovable True to prevent the user from closing the tab.
    void setNonRemovable(int viewPos, bool nonRemovable);

    /// @brief Called when the system color mode (dark/light) changes.
    /// @param bDarkMode True if the system switched to dark mode.
    void systemColorModeChanged(bool bDarkMode) override;
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    /// @brief Constructs a TabHeader associated with a TabView.
    /// @param pTabView Pointer to the owning TabView.
    /// @param type Tab sizing type (Fix, FitToText, or Dynamic).
    /// @param minWidthInChars Minimum tab width expressed in character units.
    /// @param maxWidthInChars Maximum tab width expressed in character units.
    /// @param selFontID Font identifier used for the selected tab label.
    /// @param fontID Font identifier used for non-selected tab labels.
    TabHeader(TabView* pTabView, Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, Font::ID selFontID, Font::ID fontID);

    /// @brief Destructor.
    ~TabHeader();
};

} //namespace gui
