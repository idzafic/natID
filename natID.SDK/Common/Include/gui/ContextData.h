// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ContextData.h
    @brief Declares ContextData, a structure that carries contextual information for context menus and actions. */
#pragma once
#include <td/String.h>
#include <dp/IDataSet.h>

namespace gui
{

class BaseView;
class Control;

/// @brief Carries contextual information passed to context menus and related handlers.
///
/// Identifies the originating view, control, data set, layout, and record position
/// so that handlers can tailor their responses to the exact location and object that
/// triggered the context action.
class ContextData
{
public:
    /// @brief Identifies the type of GUI element that initiated the context action.
    enum class Source : td::BYTE
    {
        Unknown = 0, ///< Source is not known.
        Report,      ///< Originating from a report viewer.
        View,        ///< Originating from a view.
        Control,     ///< Originating from an individual control.
        TabView,     ///< Originating from a tab view.
        Custom       ///< Originating from a custom source.
    };

    td::String configName;   ///< Report configuration name associated with this context.
    td::String layoutName;   ///< Layout name associated with this context.
    td::String elementName;  ///< Report element name or grid name within the layout.
//    rpt::IViewer* pViewer = nullptr;
    dp::IDataSet* pDP = nullptr; ///< Pointer to the data set relevant to this context.

    union
    {
        gui::BaseView* pView = nullptr; ///< Pointer to the originating view (when Source is View or TabView).
        gui::Control* pControl;         ///< Pointer to the originating control (when Source is Control).
    };

    union
    {
        td::UINT4 recNo = 0;  ///< Zero-based record number in the data set (when applicable).
        td::INT4 viewPos;     ///< Position within the view (when applicable).
    };

    Source type = Source::Unknown; ///< Identifies which member of the anonymous unions is active.
};

}
