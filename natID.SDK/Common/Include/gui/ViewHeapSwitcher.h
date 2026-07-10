// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ViewHeapSwitcher.h
    @brief ViewSwitcher variant that manages heap-allocated views with configurable ownership. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include "ViewSwitcher.h"


namespace gui
{

/// @brief Extends ViewSwitcher to manage dynamically allocated views with optional background view support.
class NATGUI_API ViewHeapSwitcher : public ViewSwitcher
{
protected:
    BaseView* _pBackgroundView = nullptr; ///< Optional view displayed when no managed view is active.
protected:
    /// @brief Returns the background view shown when no content view is selected.
    /// @return Pointer to the background BaseView, or nullptr if none is set.
    BaseView* getBackgroundView() override final;

    ViewHeapSwitcher(const ViewHeapSwitcher&) = delete;
    ViewHeapSwitcher& operator =(const ViewHeapSwitcher&) = delete;
public:
    /// @brief Constructs a ViewHeapSwitcher with an optional initial view capacity.
    /// @param nViews Initial capacity for managed views (default 0).
    ViewHeapSwitcher(size_t nViews = 0);
    /// @brief Destructor. Deletes all internally owned views.
    ~ViewHeapSwitcher();

    /// @brief Sets the view displayed when no content view is active.
    /// @param pBackgroundView Pointer to the background view (not owned by the switcher).
    void setBackgroundView(gui::BaseView* pBackgroundView);

    /// @brief Removes the view at the specified position and deletes it.
    /// @param pos Zero-based index of the view to remove.
    void removeView(int pos);
    /// @brief Removes the given view, deletes it, and returns its former position.
    /// @param pView Pointer to the view to remove; the pointer becomes invalid after this call.
    /// @return Former zero-based index of the removed view, or -1 if not found.
    int removeView(const BaseView* pView); //pView will be deleted (unusable)
    /// @brief Removes the view at posToRemove and makes the view at posToSelect visible.
    /// @param posToRemove Zero-based index of the view to remove and delete.
    /// @param posToSelect Zero-based index of the view to display after removal.
    void removeView(int posToRemove, int posToSelect);
    /// @brief Removes and deletes all managed views.
    /// @param bShowBackground True to display the background view after clearing (default true).
    void removeAll(bool bShowBackground = true);

    /// @brief Sets the ownership policy for a specific view.
    /// @param viewPos Zero-based index of the view whose ownership is to be changed.
    /// @param ownership Ownership policy (Intern = switcher deletes, Extern = caller retains).
    void setViewOwnership(int viewPos, td::Ownership ownership); //Intern - TabView will delete the view (default)
    /// @brief Returns the current ownership policy for managed views.
    /// @return Current td::Ownership value.
    td::Ownership getViewOwnership() const;
    /// @brief Detaches and returns the view at the specified position without deleting it.
    /// @param viewPos Zero-based index of the view to detach.
    /// @return Pointer to the detached view; caller takes ownership.
    BaseView* detach(int viewPos);
};

} //namespace gui
