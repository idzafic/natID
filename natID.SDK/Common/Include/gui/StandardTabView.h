// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StandardTabView.h
 * @brief Provides the StandardTabView class for a native platform tab control.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "BaseView.h"
#include <gui/Types.h>
#include <td/String.h>

namespace gui
{

class Image;

/// @brief A tab view control using the native platform tab widget.
class NATGUI_API StandardTabView : public BaseView
{
private:
    std::function<void(int)> _onChangedSelection; ///< Callback invoked when the active tab changes.
public:
    /// @brief Position of the tab header bar.
    enum class TabLocation: unsigned char {Top=0, Bottom, Left, Right};
    /// @brief Visual style when no tabs exist.
    enum class NoTabsStyle : unsigned char {BezelBorder=0, LineBorder, NoBorder};
protected:
    /// @brief Measures the preferred size of the tab view.
    /// @param cell Cell info to populate.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the tab view after layout changes.
    /// @param cell Cell info to update.
    void reMeasure(CellInfo& cell) override;

    /// @brief Internal constructor used for subclassing.
    /// @param dummy Unused discriminator parameter.
    StandardTabView(int);
    StandardTabView(const StandardTabView&) = delete;
    StandardTabView& operator =(const StandardTabView&) = delete;
public:
    /// @brief Default constructor.
    StandardTabView();

    /// @brief Destructor.
    ~StandardTabView();

    /// @brief Sets the parent frame for this view.
    /// @param pFrame Pointer to the parent frame.
    void setParentFrame(Frame* pFrame) override;

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::StandardTabView.
    gui::ObjType getObjType() const override { return ObjType::StandardTabView;}

    /// @brief Adds a view as a new tab with a C string title.
    /// @param pView Pointer to the view to add.
    /// @param title Tab title.
    /// @param image Optional tab icon.
    /// @return Index of the newly added tab.
    int addView(BaseView* pView, const char* title, Image* image = nullptr);

    /// @brief Adds a view as a new tab with a td::String title.
    /// @param pView Pointer to the view to add.
    /// @param strTitle Tab title.
    /// @param image Optional tab icon.
    /// @return Index of the newly added tab.
    int addView(BaseView* pView, const td::String& strTitle, Image* image = nullptr);
   // void removeView(int pos);
    //void removeView(const BaseView* pView);

    /// @brief Returns a const pointer to the view at the given tab position.
    /// @param pos Tab index.
    /// @return Const pointer to the view.
    const BaseView* getView(int pos) const;

    /// @brief Returns a pointer to the view at the given tab position.
    /// @param pos Tab index.
    /// @return Pointer to the view.
    BaseView* getView(int pos);

    /// @brief Returns a pointer to the currently visible view.
    /// @return Pointer to the current view.
    BaseView* getCurrentView();

    /// @brief Returns a const pointer to the currently visible view.
    /// @return Const pointer to the current view.
    const BaseView* getCurrentView() const;

    /// @brief Returns the index of the currently selected tab.
    /// @return Zero-based tab index.
    int getCurrentViewPos() const;

    /// @brief Selects the tab at the given index.
    /// @param pos Zero-based tab index.
    void setCurrentViewPos(int pos);

    //int getViewPos(const BaseView* pView);

    /// @brief Returns the total number of tabs.
    /// @return Tab count.
    int getNumberOfViews() const;

    /// @brief Sets the position of the tab bar.
    /// @param tabLocation Desired tab location.
    void setTabLocation(StandardTabView::TabLocation tabLocation);

    /// @brief Sets the visual style used when there are no tabs.
    /// @param noTabsStyle Desired style.
    void setNoTabsStyle(StandardTabView::NoTabsStyle noTabsStyle);

    /// @brief Returns the current tab bar location.
    /// @return Current TabLocation value.
    StandardTabView::TabLocation getTabLocation() const;

    /// @brief Returns the registered selection-changed callback.
    /// @return Reference to the callback function.
    const std::function<void(int)>& getChangedSelectionHandler() const;

    /// @brief Registers a callback to invoke when the active tab changes.
    /// @param fnToCall Callback function receiving the new tab index.
    void onChangedSelection(const std::function<void(int)>& fnToCall);
};

} //namespace gui
