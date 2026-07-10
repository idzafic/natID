// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ToolBarLayout.h
    @brief Layout manager that arranges toolbar controls in a grid with optional labels. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/GridLayout.h>
#include <cnt/SafeFullVector.h>
//#include <cnt/PushBackVector.h>
#include <gui/Label.h>

namespace gui
{

class ToolBarView;
class ToolBarItem;

/// @brief Layout manager for toolbar controls arranged in a horizontal grid with optional text labels.
class NATGUI_API ToolBarLayout : public GridLayout
{
    friend class ToolBarView;

public:
    /// @brief Pairs a control pointer with its associated label and ownership flag.
    class Pair
    {
    public:
        Control* pControl = nullptr; ///< Pointer to the toolbar control.
        Label label = "";            ///< Text label displayed beneath the control.
        bool dynamicControl = false; ///< True if the control was allocated dynamically and must be deleted by the layout.
    };

    /// @brief Stores positional and visibility information for a single toolbar button.
    typedef struct _BH
    {
        td::INT2 pos;       ///< Column position of the button within the layout.
        td::WORD width;     ///< Width of the button in pixels.
        td::BYTE hidden;    ///< Non-zero when the button is currently hidden.
    } Buttons;
protected:
    cnt::SafeFullVector<Pair> _pairs;           ///< Collection of control/label pairs managed by this layout.
    cnt::SafeFullVector<Buttons> _buttonPositions; ///< Positional metadata for each button in the layout.
    ToolBarView* _pToolBarView;                 ///< Pointer to the owning ToolBarView.
    td::BYTE _menuIncluded = 0;                 ///< Non-zero if a menu button is included in the layout.
    td::BYTE _labelsVisible = 1;                ///< Non-zero when text labels are currently visible.
protected:
    /// @brief Measures the preferred size of the layout cell.
    /// @param cell Cell information structure to populate with measurement data.
    void measure(CellInfo& cell) override;
    //void reMeasure(CellInfo& cell) override;
    /// @brief Applies the given frame geometry to the layout and its children.
    /// @param cellFrame The bounding rectangle available for the layout.
    /// @param cell Cell descriptor containing alignment and sizing hints.
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    //bool initialMeasure() override;
    /// @brief Performs the initial measurement pass for the layout.
    /// @param cell Cell information structure to populate with initial measurement data.
    void initialMeasure(CellInfo& cell) override;
    /// @brief Returns the minimum width required by the layout.
    /// @return Minimum width in pixels.
    int getMinWidth();
    /// @brief Returns the pixel width of a specific column.
    /// @param colPos Zero-based column index.
    /// @return Width of the specified column in pixels.
    td::WORD getColWidth(td::BYTE colPos) const;
    /// @brief Returns the pixel height reserved for text labels.
    /// @return Label row height in pixels.
    td::BYTE getLabelHeight() const;
    /// @brief Returns whether text labels are currently shown.
    /// @return True if labels are visible, false otherwise.
    bool areLabelsVisible() const;
    /// @brief Updates the stored label width for a given column.
    /// @param colPos Zero-based column index whose label width is to be updated.
    /// @param newWidth New width value in pixels.
    void updateLabelWidth(td::BYTE colPos, td::UINT2 newWidth);

    /// @brief Default constructor (protected, for internal subclass use).
    ToolBarLayout();
    ToolBarLayout(const ToolBarLayout&) = delete;
    ToolBarLayout& operator =(const ToolBarLayout&) = delete;
public:
    /// @brief Constructs a ToolBarLayout associated with a given ToolBarView.
    /// @param pToolBarView Pointer to the ToolBarView that owns this layout.
    ToolBarLayout(ToolBarView*);
    /// @brief Destructor.
    ~ToolBarLayout();
    /// @brief Constructs a ToolBarLayout with a fixed number of columns.
    /// @param nCols Number of columns to pre-allocate.
    ToolBarLayout(td::BYTE nCols);
//    void reserve(td::BYTE nCols);
//    void populatePairs();
//    void insert(td::BYTE iCol, Pair& pair);
    /// @brief Inserts a control at a specific column position with an optional label.
    /// @param iCol Zero-based column index at which to insert the control.
    /// @param pCtrl Pointer to the control to insert.
    /// @param label Text label to display beneath the control.
    /// @param showLabel True to display the label; false to hide it.
    void insert(td::BYTE iCol, Control* pCtrl, const td::String& label, bool showLabel);
    /// @brief Inserts a horizontal spacer at the specified column.
    /// @param iCol Zero-based column index at which to insert the spacer.
    /// @param horSpace Width of the spacer in pixels (default 10).
    void insertSpace(td::BYTE iCol, td::UINT2 horSpace = 10);
    /// @brief Shows or hides text labels for all toolbar items.
    /// @param bShow True to show labels, false to hide them.
    /// @return True if the visibility state changed, false if it was already in the requested state.
    bool showLabels(bool bShow);
    /// @brief Updates the visible text label for a specific toolbar item.
    /// @param pTBI Pointer to the toolbar item whose label should be updated.
    /// @param strLbl New label text.
    /// @return True if the label was successfully updated.
    bool updateLabel(const ToolBarItem* pTBI, const td::String& strLbl);
    /// @brief Updates the tooltip text for a specific toolbar item.
    /// @param pTBI Pointer to the toolbar item whose tooltip should be updated.
    /// @param strToolTip New tooltip text.
    void updateToolTip(const ToolBarItem* pTBI, const td::String& strToolTip);
    /// @brief Marks that a menu button is included in this layout.
    void setMenuIncluded();
    /// @brief Returns whether a menu button is included in this layout.
    /// @return True if a menu button is present, false otherwise.
    bool isMenuIncluded() const;

};

} //namespace gui
