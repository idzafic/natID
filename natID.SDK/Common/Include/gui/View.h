// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file View.h
    @brief Base container view that hosts a layout and manages margins and size negotiation. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "BaseView.h"
#include <gui/_aux/Cell.h>
#include <tuple>

namespace gui
{

class Control;
class ViewHelper;

/// @brief Generic container view that owns a layout and participates in the measurement/geometry protocol.
class NATGUI_API View : public BaseView
{
    friend class Layout;
    friend class Control;
    friend class ViewHelper;

protected:
    Cell _cell;                    ///< Cell descriptor that records the view's position and span within its parent layout.
    td::BYTE _marginLeft=0;        ///< Left margin in pixels applied around the layout content.
    td::BYTE _marginTop=0;         ///< Top margin in pixels applied around the layout content.
    td::BYTE _marginRight=0;       ///< Right margin in pixels applied around the layout content.
    td::BYTE _marginBottom=0;      ///< Bottom margin in pixels applied around the layout content.
    td::BYTE _initiallyMeasured = 0; ///< Non-zero after the first measurement pass has been completed.
    //td::BYTE _isClosing = 0;

private:
    /// @brief Detaches and releases all child controls.
    void removeAllCtrls();
    /// @brief Registers a child control with this view.
    /// @param ctrl Pointer to the control to add.
    void addCtrl(Control* ctrl);

protected:
    /// @brief Queries which dimensions of this view are fixed (non-resizable).
    /// @return FixSizes flags describing horizontal and vertical fixedness.
    Frame::FixSizes getFixSizesInfo() override;
    //bool initialMeasure() override;
    /// @brief Re-measures the view after a content or size change.
    /// @return True if the re-measurement caused a layout change.
    bool reMeasure() override;
    /// @brief Measures the preferred size of this view.
    /// @param cell Cell information structure to populate.
    void measure(CellInfo& cell) override;
    /// @brief Re-measures the view and updates the provided cell information.
    /// @param cell Cell information structure to update.
    void reMeasure(CellInfo& cell) override;
    /// @brief Called on every resize to allow the view to adjust its layout.
    /// @param newSize Updated size of the view.
    virtual void adjustLayout(const Size& newSize); //called on every resize
    /// @brief Sets the margins around the layout content.
    /// @param left Left margin in pixels.
    /// @param top Top margin in pixels.
    /// @param right Right margin in pixels.
    /// @param bottom Bottom margin in pixels.
    void setMargins(td::BYTE left, td::BYTE top, td::BYTE right, td::BYTE bottom);
    /// @brief Returns whether the view has fixed width and/or height.
    /// @return Tuple of (fixedWidth, fixedHeight) booleans.
    std::tuple<bool, bool> isFixedSize() const override;
    /// @brief Estimates the desired initial size based on a provided hint.
    /// @param initialSize Hint size provided by the framework.
    /// @param desiredSize Output parameter filled with the view's desired size.
    /// @return True if a meaningful estimate could be produced.
    bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const override;
    /// @brief Freezes the view, preventing layout updates.
    /// @return True if the view was successfully frozen.
    bool freeze() override;
    /// @brief Unfreezes the view, re-enabling layout updates.
    void unFreeze() override;
    /// @brief Returns the layout object managed by this view.
    /// @return Pointer to the current Layout, or nullptr if none is set.
    Layout* getLayout() override;

    /// @brief Internal constructor used by subclasses to bypass normal initialisation.
    /// @param k Unused discriminator key.
    View (int k);
    View(const View&) = delete;
    View& operator =(const View&) = delete;
public:

    /// @brief Default constructor creating a view with zero margins.
    View();
    /// @brief Constructs a view with explicit margins.
    /// @param marginLeft Left margin in pixels.
    /// @param marginTop Top margin in pixels.
    /// @param marginRight Right margin in pixels.
    /// @param marginBottom Bottom margin in pixels.
    View(td::BYTE marginLeft, td::BYTE marginTop, td::BYTE marginRight, td::BYTE marginBottom);
    /// @brief Destructor.
    ~View();
    /// @brief Returns the minimum size required to display this view.
    /// @param minSize Output parameter filled with the minimum size.
    void getMinSize(Size& minSize) const override;
    /// @brief Returns the four margin values of this view.
    /// @return Tuple of (left, top, right, bottom) margin values in pixels.
    std::tuple<td::BYTE, td::BYTE, td::BYTE, td::BYTE> getMargins() const; //left, top, right, bottom
    /// @brief Returns the sum of the left and right margins.
    /// @return Total horizontal margin in pixels.
    td::WORD getTotalHMargin() const override;
    /// @brief Returns the sum of the top and bottom margins.
    /// @return Total vertical margin in pixels.
    td::WORD getTotalVMargin() const override;
    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::View.
    gui::ObjType getObjType() const override { return ObjType::View;}
    /// @brief Registers this view as the central content view of a window.
    /// @param pWnd Pointer to the owning Window.
    /// @param fixSizes Specifies which dimensions should be fixed.
    void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes) override;

    /// @brief Returns the cell descriptor recording this view's position in its parent layout.
    /// @return Const reference to the internal Cell.
    const Cell& getCell() const;
    /// @brief Sets the layout that manages this view's children.
    /// @param pLayout Pointer to the layout to adopt.
    void setLayout(Layout* pLayout);
    //void setBackgroundColor(td::ColorID);
};

} //namespace gui
