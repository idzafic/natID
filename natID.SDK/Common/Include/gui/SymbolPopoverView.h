// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SymbolPopoverView.h
    @brief Popover canvas that displays a grid of symbol or image items for selection. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "gui/PopoverCanvas.h"
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <cnt/PushBackVector.h>
#include <gui/DrawableString.h>

namespace gui
{

//class used to draw custom Popover Views
class SymbolPopoverButton;

/// @brief A popover canvas that renders a grid of symbol or image items with optional labels and tooltips.
class NATGUI_API SymbolPopoverView : public PopoverCanvas
{
    friend class SymbolPopoverButton;

protected:
    /// @brief Describes a single entry in the popover grid.
    class Item
    {
    public:
        /// @brief Distinguishes between a vector symbol and a raster image item.
        enum class Type : td::BYTE {
            Symbol=0, ///< Item is a vector Symbol.
            Image     ///< Item is a raster Image.
        };

        union
        {
            gui::Symbol* symbol = nullptr; ///< Pointer to the vector symbol (when type == Symbol).
            gui::Image* image;             ///< Pointer to the raster image (when type == Image).
        };

        union
        {
            gui::Symbol* attachedSymbol = nullptr; ///< Optional secondary symbol overlaid on the primary (when type == Symbol).
            float hMargin;                         ///< Horizontal margin applied to the image (when type == Image).
        };

        td::String label;                    ///< Display label shown below or beside the item.
        DrawableString drawableLabel;        ///< Pre-rendered drawable form of the label.

        td::String toolTip;                  ///< Tooltip text shown when the cursor hovers over the item.
        td::Variant userData;                ///< Arbitrary user data associated with this item.
        //gui::NatString natLabel = nullptr;
        //gui::NatString natToolTip = nullptr;
        union
        {
            float rotationClockwiseDeg = 0; ///< Clockwise rotation angle in degrees applied to the symbol.
            float vMargin;                  ///< Vertical margin applied to the image (when type == Image).
        };

        td::BYTE useSymbolStyling = 0; ///< Non-zero to apply system colour styling to the symbol.
        td::BYTE checked = 0;          ///< Non-zero if the item is in a checked/selected state.
        td::BYTE disabled = 0;         ///< Non-zero if the item is disabled (greyed out).
        Type type = Type::Symbol;      ///< Whether this item holds a Symbol or an Image.

        /// @brief Default constructor. Leaves all fields at their default-initialised values.
        Item() {}

        /// @brief Copy constructor.
        /// @param item Source item to copy from.
        Item(const Item& item)
        : symbol(item.symbol)
        , attachedSymbol(item.attachedSymbol)
        , label(item.label)
        , toolTip(item.toolTip)
        , userData(item.userData)
        , rotationClockwiseDeg(item.rotationClockwiseDeg)
        , useSymbolStyling(item.useSymbolStyling)
        , type(item.type)
        {
        }

        /// @brief Copy assignment operator.
        /// @param item Source item to copy from.
        /// @return Reference to this item.
        const Item& operator = (const Item& item)
        {
            symbol = item.symbol;
            attachedSymbol = item.attachedSymbol;
            label = item.label;
            toolTip = item.toolTip;
            userData = item.userData;
            rotationClockwiseDeg = item.rotationClockwiseDeg;
            useSymbolStyling = item.useSymbolStyling;
            type = item.type;
            return *this;
        }
    };

    cnt::PushBackVector<SymbolPopoverView::Item> _items; ///< All items displayed in the popover grid.
    union
    {
        gui::Symbol* _pPOSymbol = nullptr; ///< Fixed symbol shown on the popover button face (when _typePOButton == Symbol).
        gui::Image* _pPOImage;             ///< Fixed image shown on the popover button face (when _typePOButton == Image).
    };
    float _maxHWRatio = 1.0f;            ///< Maximum height-to-width ratio of any symbol in the grid.
    Item::Type _typePOButton = Item::Type::Image; ///< Whether the popover button face shows a symbol or an image.
private:
    /// @brief Draws the currently selected item onto the popover button face.
    /// @param sz Size of the button face area.
    void drawCurrentItemOnPOButton(const Size& sz) override;
protected:
    //void prepareImage();

    /// @brief Renders a single symbol item into the given rectangle.
    /// @param simbPos Index of the symbol item to draw.
    /// @param rec Target rectangle.
    /// @param doStyling Non-zero to apply colour styling.
    /// @param forParrentButton True if drawing for the parent button face rather than the grid.
    void drawSymbol(td::UINT2 simbPos, const gui::Rect& rec, td::BYTE doStyling, bool forParrentButton);

    /// @brief Returns a reference to the item at the given position.
    /// @param iPos Zero-based item index.
    /// @return Reference to the item.
    SymbolPopoverView::Item& getItem(size_t iPos);

    /// @brief Recalculates the cell size based on the current set of items.
    void calcCellSize() override;

    /// @brief Returns the label string for a given item position.
    /// @param itemPos Zero-based item index.
    /// @return Const reference to the label string.
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;

    /// @brief Returns the tooltip string for a given item position.
    /// @param itemPos Zero-based item index.
    /// @return Const reference to the tooltip string.
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;

    /// @brief Draws the item at the given position into the specified rectangle.
    /// @param itemPos Zero-based item index.
    /// @param r Target rectangle within the popover canvas.
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;

    /// @brief Draws the tooltip text for the item at the given position.
    /// @param itemPos Zero-based item index.
    /// @param r Rectangle in which to render the tooltip text.
    void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r) override;

    //IPopoverUpdater should call these method
    /// @brief Sets the checked state of an item.
    /// @param iPos Zero-based item index.
    /// @param bChecked True to check; false to uncheck.
    void setChecked(size_t iPos, bool bChecked) override;

    /// @brief Sets the disabled state of an item.
    /// @param iPos Zero-based item index.
    /// @param bDisabled True to disable; false to enable.
    void setDisabled(size_t iPos, bool bDisabled) override;
private:
    SymbolPopoverView() {};
    SymbolPopoverView(const SymbolPopoverView&) = delete;
    SymbolPopoverView& operator =(const SymbolPopoverView&) = delete;
public:

    /// @brief Constructs a symbol popover view with explicit cell dimensions.
    /// @param type Canvas type determining layout and styling behaviour.
    /// @param nItems Initial item capacity.
    /// @param nCols Number of columns in the grid.
    /// @param cellWidth Width of each cell in pixels.
    /// @param cellHeight Height of each cell in pixels.
    /// @param numberOfToolTipTextLines Number of text lines allocated for the tooltip.
    SymbolPopoverView(Type type, size_t nItems, td::BYTE nCols, td::BYTE cellWidth=48, td::BYTE cellHeight = 48, td::BYTE numberOfToolTipTextLines = 1);

    /// @brief Constructs a symbol popover view where cell height matches text height.
    /// @param type Canvas type.
    /// @param nItems Initial item capacity.
    /// @param symbolWidth Symbol width policy (more suitable for SystemAndText type).
    SymbolPopoverView(Type type, size_t nItems, PopoverCanvas::SymbolWidth symbolWidth= PopoverCanvas::SymbolWidth::UserDefined); //more suitable with SystemAndText type, nRows = nItems, nCols = 1, symbolHeight = textHeight

    //allocate capacity
    /// @brief Reserves storage for the given number of items without changing the current count.
    /// @param nItems Number of items to reserve.
    void reserve(size_t nItems);

    //remove existing items while keeping allocated capacity
    /// @brief Removes all items while keeping the reserved capacity.
    void reset();

    /// @brief Sets a fixed raster image to display on the popover button face.
    /// @param pFixedPOImage Pointer to the image.
    void setButtonImage(Image* pFixedPOImage);

    /// @brief Sets a fixed vector symbol to display on the popover button face.
    /// @param pFixedPOSymbol Pointer to the symbol.
    void setButtonSymbol(Symbol* pFixedPOSymbol);

    /// @brief Appends a pre-constructed item to the grid.
    /// @param item Item to add.
    void addItem(SymbolPopoverView::Item& item);

    /// @brief Appends a symbol-based item with all properties specified individually.
    /// @param symbol Primary symbol to display.
    /// @param attachedSymbol Optional overlay symbol (may be nullptr).
    /// @param label Display label.
    /// @param toolTip Tooltip text.
    /// @param styleWithColors True to apply colour styling.
    /// @param userData Arbitrary user data associated with this item.
    /// @param rotation Clockwise rotation angle in degrees.
    /// @param deltaX Horizontal rendering offset.
    /// @param deltaY Vertical rendering offset.
    void addItem(gui::Symbol* symbol, gui::Symbol* attachedSymbol, const td::String& label, const td::String& toolTip, bool styleWithColors, td::Variant userData = (td::UINT4)0, float rotation = 0, char deltaX = 0, char deltaY = 0);

    /// @brief Appends an image-based item with all properties specified individually.
    /// @param image Raster image to display.
    /// @param label Display label.
    /// @param toolTip Tooltip text.
    /// @param hMargin Horizontal margin inside the cell.
    /// @param vMargin Vertical margin inside the cell.
    /// @param userData Arbitrary user data associated with this item.
    void addItem(gui::Image* image, const td::String& label, const td::String& toolTip, float hMargin, float vMargin, td::Variant userData = (td::UINT4)0);
};

} //namespace gui
