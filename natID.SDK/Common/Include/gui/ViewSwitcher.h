// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ViewSwitcher.h
    @brief Container that displays one child view at a time, switching between them programmatically. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include "BaseView.h"
#include <gui/Types.h>
#include <cnt/PushBackVector.h>

namespace gui
{
/// @brief Manages a stack of child views and exposes exactly one at a time.
class NATGUI_API ViewSwitcher : public BaseView
{
protected:
    /// @brief Associates a view pointer with its ownership policy.
    using ViewOwnership= struct
    {
        BaseView* pView;       ///< Pointer to the child view.
        td::Ownership ownerShip; ///< Whether the switcher owns (and will delete) the view.
    };
    cnt::PushBackVector<ViewOwnership, 64> _views; ///< Ordered collection of managed views and their ownership flags.
    int _currPos = -1; ///< Zero-based index of the currently visible view, or -1 if none.
protected:
    /// @brief Measures the preferred size of the switcher (uses the largest child).
    /// @param cell Cell information structure to populate.
    void measure(CellInfo& cell) override;
    /// @brief Re-measures the switcher after a child size change.
    /// @param cell Cell information structure to update.
    void reMeasure(CellInfo& cell) override;
    /// @brief Applies the given frame geometry to the switcher and its visible child.
    /// @param cellFrame Available bounding rectangle.
    /// @param cell Cell descriptor with alignment hints.
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    /// @brief Returns the optional background view shown when no content view is selected.
    /// @return Pointer to the background BaseView, or nullptr by default.
    virtual BaseView* getBackgroundView();
    //void showBackgroundView();
    ViewSwitcher() = delete;
    ViewSwitcher(const ViewSwitcher&) = delete;
    ViewSwitcher& operator =(const ViewSwitcher&) = delete;
public:
    /// @brief Constructs a ViewSwitcher with an initial view capacity.
    /// @param nViews Initial capacity for managed child views.
    ViewSwitcher(size_t nViews);
    /// @brief Destructor. Deletes all internally owned views.
    ~ViewSwitcher();
    /// @brief Propagates the parent frame pointer to all managed child views.
    /// @param pFrame Pointer to the parent frame.
    void setParentFrame(Frame* pFrame) override;
    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::ViewSwitcher.
    gui::ObjType getObjType() const override { return ObjType::ViewSwitcher;}
    /// @brief Adds a view to the switcher.
    /// @param pView Pointer to the view to add.
    /// @param bMakeVisible True to immediately display the added view (default false).
    /// @return Zero-based index at which the view was inserted.
    int addView(BaseView* pView, bool bMakeVisible = false);
    /// @brief Inserts a view at a specific position.
    /// @param pView Pointer to the view to insert.
    /// @param atPos Zero-based index at which to insert the view.
    /// @return Actual zero-based index of the inserted view.
    int insertView(BaseView* pView, int atPos);

    /// @brief Returns the current position of a view in the switcher.
    /// @param pView Pointer to the view to locate.
    /// @return Zero-based index, or -1 if the view is not found.
    int getViewPos(const BaseView* pView) const;
    /// @brief Returns the view at the specified position.
    /// @param pos Zero-based index of the view to retrieve.
    /// @return Pointer to the view, or nullptr if the index is out of range.
    BaseView* getView(int pos) const;
    /// @brief Returns a const pointer to the currently visible view.
    /// @return Const pointer to the current view, or nullptr if none is visible.
    const BaseView* getCurrentView() const;
    /// @brief Returns a pointer to the currently visible view.
    /// @return Pointer to the current view, or nullptr if none is visible.
    BaseView* getCurrentView();

    /// @brief Makes the specified view visible.
    /// @param pView Pointer to the view to display.
    /// @param bSetFocus True to also transfer keyboard focus to the view (default true).
    void showView(BaseView* pView, bool bSetFocus = true);
    /// @brief Makes the view at the given position visible.
    /// @param pos Zero-based index of the view to display.
    /// @param bSetFocus True to also transfer keyboard focus to the view (default true).
    void showView(int pos, bool bSetFocus = true);

    /// @brief Returns the total number of managed views.
    /// @return Number of views currently in the switcher.
    int getNoOfViews() const;
    /// @brief Returns the zero-based index of the currently visible view.
    /// @return Current view index, or -1 if no view is selected.
    int getCurrentViewPos() const;
};

} //namespace gui
