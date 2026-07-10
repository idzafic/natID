// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file NavigatorView.h
    @brief Internal canvas view that renders and handles interaction for Navigator items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "ViewScroller.h"
#include "Canvas.h"
#include "Image.h"
#include <cnt/SafeFullVector.h>
//#include <gui/GraphicsShapeFactory.h>
#include <gui/DrawableString.h>
#include <gui/Symbol.h>

namespace gui
{

class Navigator;
class HorizontalNavigator;
class VerticalNavigator;

/// @brief Internal canvas that draws, measures, and handles mouse/keyboard input for a Navigator control.
class NATGUI_API NavigatorView : protected Canvas
{
    friend class Navigator;
    friend class HorizontalNavigator;
    friend class VerticalNavigator;

    /// @brief Describes a single selectable entry in the navigator.
    class Item
    {
    public:
        /// @brief Specifies whether the item uses an image or a vector symbol for its visual.
        enum class Type:unsigned char
        {
            NA=0,   ///< No visual assigned.
            Image,  ///< Item uses a raster Image.
            Symbol  ///< Item uses a vector Symbol.
        };
        union
        {
            Image* pImage;   ///< Pointer to the raster image (valid when type == Image).
            Symbol* pSymbol; ///< Pointer to the vector symbol (valid when type == Symbol).
        };
        DrawableString text;    ///< Pre-rendered text label for this item.
        float rotateRad = 0;    ///< Rotation angle in radians applied when drawing the symbol.
        Type type;              ///< Discriminator for the image/symbol union.
    };

    Navigator* _pNavigator;                        ///< Back-pointer to the owning Navigator control.
    cnt::SafeFullVector<NavigatorView::Item> _items; ///< Ordered collection of all navigator items.
    td::UINT2 _imageWidth;                         ///< Computed width in pixels allocated for each item's image area.
    td::UINT2 _imageHeight;                        ///< Height in pixels allocated for each item's image area.
    td::UINT2 _width;                              ///< Total width of the navigator view in pixels.
    td::UINT2 _height;                             ///< Total height of the navigator view in pixels.
    td::UINT2 _startPos = 0;                       ///< Scroll offset; index of the first visible item.
    td::ColorID symbolColor = td::ColorID::Transparent; ///< Override colour applied to symbols; Transparent means use default.
    td::UINT2 symbolLineWidth = 0;                 ///< Override stroke width for symbol rendering; 0 means use default.
    td::UINT2 _currentSelection=0;                 ///< Zero-based index of the currently selected item.
    td::BYTE _vertical = 1;                        ///< Non-zero if items are laid out vertically; 0 for horizontal.
    td::BYTE _alignment = 0;                       ///< Alignment flag for item content within each cell.

    NavigatorView() = delete;

    /// @brief Calculates the bounding rectangle for the text label of an item.
    /// @param itemPos Zero-based index of the item.
    /// @param r Output rectangle filled with the text area bounds.
    void getTxtRect(td::UINT2 itemPos, Rect& r) const;

    /// @brief Calculates the bounding rectangle for the image/symbol area of an item.
    /// @param itemPos Zero-based index of the item.
    /// @param r Output rectangle filled with the image area bounds.
    void getImgRect(td::UINT2 itemPos, Rect& r) const;

    /// @brief Calculates the full bounding rectangle (image + text) for an item.
    /// @param itemPos Zero-based index of the item.
    /// @param r Output rectangle filled with the full item bounds.
    void getBoundingRect(td::UINT2 itemPos, Rect& r) const;

    /// @brief Computes the total content size required to display all items.
    /// @param sz Output size filled with the required content dimensions.
    void getContentSize(gui::Size& sz) const;

    /// @brief Returns the scrollable model size for the ViewScroller integration.
    /// @param modelSize Output size filled with the full scrollable content dimensions.
    /// @return True if a valid model size was computed.
    bool getModelSize(gui::Size& modelSize) const override;

    /// @brief Dispatches a selection-changed message to the owning Navigator.
    void sendSelChangeMessage();

    /// @brief Computes the pixel length needed to display the given number of items.
    /// @param nItems Number of items to measure.
    /// @return The required length in coordinate units along the primary axis.
    gui::CoordType getLengthForNItems(size_t nItems) const;

    /// @brief Returns the current layout orientation of the navigator.
    /// @return Orientation::Vertical or Orientation::Horizontal.
    gui::Orientation getOrientation() const;

    /// @brief Returns the alignment flag for item content.
    /// @return The current alignment byte.
    td::BYTE getAlignment() const;

    /// @brief Sets the alignment flag for item content.
    /// @param alignment The new alignment value to apply.
    void setAlignment(td::BYTE alignment);

protected:
    /// @brief Selects the item at the given index and triggers a redraw.
    /// @param itemPos Zero-based index of the item to select.
    void selectItem(td::UINT2 itemPos);

    /// @brief Handles view resize events by updating internal dimensions and requesting a redraw.
    /// @param newSize The new size of the view after resizing.
    void onResize(const gui::Size& newSize) override;

    /// @brief Draws the visible items within the given dirty rectangle.
    /// @param rDraw The rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rDraw) override;

    /// @brief Handles primary mouse button press events for item selection.
    /// @param inputDevice Describes the current state of the input device at the time of the press.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Handles key press events to allow keyboard-based item navigation.
    /// @param key Describes the key that was pressed.
    /// @return True if the key event was consumed.
    bool onKeyPressed(const gui::Key& key) override;

    NavigatorView(const NavigatorView&) = delete;
    NavigatorView& operator =(const NavigatorView&) = delete;
public:
    /// @brief Constructs a NavigatorView for the given Navigator with the specified layout parameters.
    /// @param pNavigator Pointer to the owning Navigator control.
    /// @param nItems Number of items to allocate.
    /// @param orientation Layout orientation (Vertical or Horizontal).
    /// @param imageHeight Height in pixels for each item's image area.
    /// @param widthMultiplier Multiplier applied to imageHeight to determine item width.
    NavigatorView(Navigator* pNavigator, td::UINT2 nItems, gui::Orientation orientation, td::UINT2 imageHeight, float widthMultiplier);

    /// @brief Destroys the NavigatorView and releases associated resources.
    ~NavigatorView();
};

} //namespace gui
