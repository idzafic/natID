// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IViewer.h
    @brief Abstract interface for a report viewer that displays a report with optional icon and zoom controls. */
#pragma once

#include "rptLib.h"
#include <td/String.h>
#include <rpt/ReportData.h>
#include <gui/Image.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <rpt/IResources.h>

namespace rpt
{

/// @brief Abstract interface for displaying a report in a viewer window with zoom and audio feedback controls.
class IViewer
{
public:
    /// @brief Returns the configuration name associated with this viewer.
    /// @return Const reference to the configuration name string.
    virtual const td::String& getConfigName() const = 0;
    /// @brief Displays the report viewer with a title and an image icon.
    /// @param title Title text to show in the viewer window.
    /// @param pRepIcon Pointer to an image to use as the report icon.
    virtual void show(const td::String& title, const gui::Image* pRepIcon) = 0;
    /// @brief Displays the report viewer with a title and a symbol icon.
    /// @param title Title text to show in the viewer window.
    /// @param pRepIcon Pointer to a symbol to use as the report icon.
    virtual void show(const td::String& title, const gui::Symbol* pRepIcon) = 0;
    /// @brief Retrieves the current minimum and maximum zoom levels.
    /// @param minZoom Output parameter for the minimum zoom value.
    /// @param maxZoom Output parameter for the maximum zoom value.
    virtual void getMinMaxZoom(double& minZoom, double& maxZoom) = 0;
    /// @brief Sets the minimum and maximum allowed zoom levels.
    /// @param minZoom Minimum zoom value to set.
    /// @param maxZoom Maximum zoom value to set.
    virtual void setMinMaxZoom(double minZoom, double maxZoom) = 0;
    /// @brief Releases this viewer instance and frees associated resources.
    virtual void release() = 0;

    /// @brief Creates a new IViewer instance for the given configuration and report data.
    /// @param configName Name of the report configuration to load.
    /// @param repData Reference to the report data to display.
    /// @return Pointer to the newly created IViewer instance.
    NATRPT_API static IViewer* create(const td::String& configName, const ReportData& repData);
    /// @brief Enables or disables playing a sound when the selection changes in the viewer.
    /// @param bPlay True to enable sound playback, false to disable.
    NATRPT_API static void setPlaySoundOnSelectionChange(bool bPlay);
    /// @brief Returns whether sound playback on selection change is currently enabled.
    /// @return True if sound playback is enabled, false otherwise.
    NATRPT_API static bool shouldPlaySoundOnSelectionChange();

    /// @brief Sets the default zoom levels for portrait and landscape orientations.
    /// @param defaultPortraitZoom Default zoom for portrait orientation.
    /// @param defaultLandscapeZoom Default zoom for landscape orientation.
    NATRPT_API static void setDefaultZoom(float defaultPortraitZoom, float defaultLandscapeZoom);
    /// @brief Retrieves the default zoom levels for portrait and landscape orientations.
    /// @param defaultPortraitZoom Output parameter for the default portrait zoom.
    /// @param defaultLandscapeZoom Output parameter for the default landscape zoom.
    NATRPT_API static void getDefaultZoom(float& defaultPortraitZoom, float& defaultLandscapeZoom);
};


}
