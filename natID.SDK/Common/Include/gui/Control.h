// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Control.h
    @brief Declares the Control base class for all interactive GUI controls within layouts. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Frame.h"
#include <gui/_aux/Cell.h>
#include <td/Variant.h>
#include <gui/Font.h>
#include <functional>
#include <tuple>
#include <math/Limits.h>

namespace gui
{

class Layout;
class FileDialog;
class Dialog;
class View;
class StackedLayout;
class GridLayout;
class SplitterLayout;

/// @brief Base class for all GUI controls that participate in layout management.
///
/// Provides size negotiation, font management, tooltip support, context menu
/// attachment, and visual-ID tracking.  Derived controls implement the pure
/// virtual measure() and reMeasure() methods to report their preferred size to
/// the owning layout.
class NATGUI_API Control : public Frame
{
public:
    /// @brief Determines how a control's preferred size limit is applied by the layout.
    enum class Limit : td::BYTE
    {
        None = 0,   ///< No size limit is applied.
        UseAsMin,   ///< The preferred size is treated as a minimum.
        Fixed       ///< The preferred size is fixed and cannot be changed by the layout.
    };
private:
    friend class Layout;
    friend class StackedLayout;
    friend class GridLayout;
    friend class SplitterLayout;
    friend class ControlHelper;
    friend class View;

    td::UINT4 _visualID = 0;          ///< Optional visual identifier for this control.
//    td::UINT4 _maxWidth = math::maxVal<td::UINT4>(); //not used yet
    td::UINT2 _preferableWidth = 0;   ///< Preferred width hint supplied by the application.
    td::UINT2 _preferableHeight = 0;  ///< Preferred height hint supplied by the application.
protected:
    td::UINT2 _contextMenuGroup = 0;  ///< Group identifier of the associated context menu.
    td::BYTE _contextMenuID = 0;      ///< Identifier of the context menu to show on right-click.
    td::BYTE _preferableWidthUsage : 2;  ///< Encodes how the preferred width hint is used (maps to Limit).
    td::BYTE _preferableHeightUsage : 2; ///< Encodes how the preferred height hint is used (maps to Limit).
    td::BYTE _onToolbar : 1;             ///< Non-zero when this control is placed on a toolbar.
    td::BYTE _reMeasure : 1;             ///< Non-zero when a re-measure pass has been requested.
private:
    /// @brief Opens the context menu at the position reported by the input device.
    /// @param inputDevice  Input device event that triggered the context menu.
    void openContextMenu(const gui::InputDevice& inputDevice);

protected:
    Control(const Control&) = delete;
    Control& operator =(const Control&) = delete;

    /// @brief Default constructor; initialises all size and flag fields to their defaults.
    Control();

    /// @brief Sends a re-measure request message to the owning layout.
    void sendReMeasureMsg();

    /// @brief Returns the context menu identifier associated with this control.
    /// @return Byte identifier of the assigned context menu.
    td::BYTE getContextMenuID() const;

    /// @brief Adjusts this control's size to its preferred size constraints.
    /// @param ci  Cell info structure to update with the adjusted dimensions.
    void adjustToPreferableSize(CellInfo& ci);

    /// @brief Performs the initial measurement pass for this control within a layout.
    /// @return true if the initial measurement produced a valid result.
    virtual bool initialMeasure(); //for layouts to measure all cells

    /// @brief Re-measures this control after a layout change.
    /// @param ci  Cell info structure to fill with the updated size.
    virtual void reMeasure(CellInfo&) = 0;

    /// @brief Measures this control and fills in the required cell info.
    /// @param ci  Cell info structure to fill.
    virtual void measure(CellInfo&) = 0;

    /// @brief Freezes the control, preventing further layout updates.
    /// @return true if the freeze was successful.
    virtual bool freeze();

    /// @brief Unfreezes the control, allowing layout updates to resume.
    virtual void unFreeze();

public:
    /// @brief Returns the minimum size this control requires to display correctly.
    /// @param minSize  Receives the minimum width and height.
    virtual void getMinSize(Size& minSize) const;

    /// @brief Returns the total horizontal margin (left + right) consumed by this control.
    /// @return Total horizontal margin in logical pixels.
    virtual td::WORD getTotalHMargin() const;

    /// @brief Returns the total vertical margin (top + bottom) consumed by this control.
    /// @return Total vertical margin in logical pixels.
    virtual td::WORD getTotalVMargin() const;

    /// @brief Positions and sizes this control within the given cell geometry.
    /// @param cellFrame  Geometry of the layout cell assigned to this control.
    /// @param cell       Additional cell descriptor (span, alignment, etc.).
    virtual void setGeometry(const Geometry& cellFrame, const Cell& cell);

    /// @brief Measures the pixel dimensions required to render the given string using this control's font.
    /// @param pString  Null-terminated string to measure.
    /// @param sz       Receives the resulting width and height.
    void measure(const char* pString, Size& sz) const; //return size of the text in ptring using control's font

    /// @brief Measures the pixel dimensions for a string of nChars copies of ch using this control's font.
    /// @param nChars  Number of characters to measure.
    /// @param sz      Receives the resulting width and height.
    /// @param ch      Representative character (default 'H').
    void measure(td::BYTE nChars, Size& sz, char ch='H') const; //return size of nChars using control's font

    /// @brief Measures the size needed to display this control's current content without clipping.
    /// @param sizeToFit  Receives the content-fitting size.
    void measureToFitContent(Size& sizeToFit) const;

    /// @brief Sets preferred size limits for both width and height.
    /// @param width            Preferred width value.
    /// @param widthHandling    How the width limit is interpreted.
    /// @param height           Preferred height value (0 = no height limit).
    /// @param heightHandling   How the height limit is interpreted.
    void setSizeLimits(td::UINT2 width, Control::Limit widthHandling, td::UINT2 height = 0, Control::Limit heightHandling = Limit::None);

    /// @brief Sets the preferred width limit based on the pixel width of a pattern string.
    /// @param strWidthPattern  String whose rendered width is used as the limit.
    /// @param widthHandling    How the width limit is interpreted.
    void setSizeLimit(const char* strWidthPattern, Control::Limit widthHandling);

    /// @brief Sets the preferred width limit based on the width of nChars characters.
    /// @param nChars         Number of characters to measure.
    /// @param widthHandling  How the width limit is interpreted.
    /// @param ch             Representative character used for measurement (default 'H').
    void setSizeLimitForNChars(td::BYTE nChars, Control::Limit widthHandling, char ch='H');

    /// @brief Returns the current preferred-width limit and its handling mode.
    /// @return Tuple of (preferred width, Limit handling mode).
    std::tuple<td::UINT2, Control::Limit> getWidthLimit() const;

    /// @brief Returns the current preferred-height limit and its handling mode.
    /// @return Tuple of (preferred height, Limit handling mode).
    std::tuple<td::UINT2, Control::Limit> getHeightLimit() const;

    /// @brief Returns the preferred width for a given available height and minimum width.
    /// @param forHeight  Available height offered by the layout.
    /// @param minWidth   Minimum width the layout guarantees.
    /// @return Preferred width in logical pixels.
    virtual td::UINT2 getPreferableWidth(td::UINT2 forHeight, td::UINT2 minWidth) const;

    /// @brief Disables automatic re-measuring for this control.
    void disableRemeasuring();

    /// @brief Enables or disables this control.
    /// @param bEnable  true to enable (default), false to disable.
    void enable(bool bEnable = true);

    /// @brief Disables this control.
    /// @param bDisable  true to disable (default), false to enable.
    void disable(bool bDisable = true);

    /// @brief Returns whether this control is currently disabled.
    /// @return true if the control is disabled.
    bool isDisabled() const;

    /// @brief Returns whether this control allows user editing.
    /// @return true if the control is editable.
    virtual bool isEditable() const;

    /// @brief Applies the flat (borderless) visual style to this control.
    void setFlat() const;

    /// @brief Returns whether this control is a text-type control.
    /// @return true if the control displays text content.
    bool isText() const;

    /// @brief Returns whether this control is actually a layout container.
    /// @return true if the control acts as a layout.
    bool isLayout() const;

    /// @brief Shows or hides this control and optionally triggers a layout recalculation.
    /// @param bShow         true to show, false to hide.
    /// @param recalcLayout  If true, the parent layout is recalculated after visibility change.
    void setVisible(bool bShow, bool recalcLayout);

    /// @brief Resizes this control to fit its content exactly.
    virtual void sizeToFit();

    /// @brief Sets the tooltip text displayed when the user hovers over this control.
    /// @param toolTip  Tooltip string to display.
    void setToolTip(const td::String& toolTip);

    /// @brief Applies bold weight to this control's text.
    void setBold();

    /// @brief Sets the font used by this control from the predefined font ID table.
    /// @param fnt  Font identifier to apply.
    void setFont(Font::ID fnt);

    /// @brief Sets the text colour of this control using an accent colour identifier.
    /// @param accClrID  Accent colour identifier.
    void setTextColor(td::Accent accClrID);

    /// @brief Retrieves the font descriptor currently applied to this control.
    /// @param font  Receives the font description.
    void getFontDescription(gui::Font& font) const;

    /// @brief Returns the file dialog attached to this control with the given ID.
    /// @param fileDlgID  Identifier of the attached file dialog.
    /// @return Pointer to the attached FileDialog, or nullptr if not found.
    gui::FileDialog* getAttachedFileDialog(td::UINT4 fileDlgID);

    /// @brief Returns the window attached to this control with the given ID.
    /// @param wndID  Identifier of the attached window.
    /// @return Pointer to the attached Window, or nullptr if not found.
    gui::Window* getAttachedWindow(td::UINT4 wndID);

    /// @brief Returns the dialog attached to this control, cast to the specified type.
    /// @tparam TID   Type of the dialog identifier (typically an enum).
    /// @param wndID  Identifier of the attached dialog.
    /// @return Pointer to the Dialog cast to the appropriate type, or nullptr.
    template <typename TID>
    gui::Dialog* getAttachedDialog(TID wndID)
    {
        gui::Window* pWnd = getAttachedWindow((td::UINT4) wndID);
        if (!pWnd)
            return nullptr;
        gui::Dialog* pDlg = reinterpret_cast<gui::Dialog*>(pWnd);
        return pDlg;
    }

    /// @brief Adjusts the supplied size to match toolbar height conventions.
    /// @param sz  Size value to adjust in place.
    void adjustToToolBarSize(gui::Size& sz) const;

    /// @brief Assigns a visual identifier to this control.
    /// @param visualID  Identifier to assign.
    void setVisualID(td::UINT4 visualID);

    /// @brief Returns the visual identifier of this control.
    /// @return The visual identifier, or 0 if none has been assigned.
    td::UINT4 getVisualID() const override;

    /// @brief Returns whether this control is placed on a toolbar.
    /// @return true if the control resides on a toolbar.
    bool isOnToolBar() const;

    //contextMenuID has to be prepared; the context menu will be shown upon a right mouse click (on buttons)
    /// @brief Assigns a context menu to this control that opens on right-click.
    /// @param contextMenuID  Identifier of the prepared context menu.
    /// @param contextGroup   Optional group identifier (default 0).
    void setContextMenuID(td::BYTE contextMenuID, td::UINT2 contextGroup = 0);
};
}
