// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file OrthoView.h
 *  @brief OpenGL view with an orthographic projection that supports pan, zoom, and scroll. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <cnt/PushBackVector.h>

namespace gui
{
namespace gl
{

/// @brief gui::gl::View subclass that uses an orthographic (parallel) projection with interactive pan and zoom.
class NATGL_API OrthoView : public gui::gl::View
{
    glm::mat4 _orthoProjection; ///< Orthographic projection matrix passed to shaders.
    gui::Point _modelRefPt;     ///< Model-space origin of the visible region (bottom-left for normal Y, top-left for inverted Y).
    float _width;               ///< Visible width in logical pixels.
    float _height;              ///< Visible height in logical pixels.
    double _zoom = 1.0;         ///< Current zoom level (1.0 = 100%).
    double _minZoom = 0.01;     ///< Minimum allowed zoom level.
    double _maxZoom = 100;      ///< Maximum allowed zoom level.

    td::BYTE _dragging = 0;     ///< Non-zero while the user is panning the view by dragging.
    td::BYTE _invertY = 0;      ///< Non-zero if the Y axis is inverted (origin at top-left).

    /// @brief Recomputes the orthographic projection matrix from current width, height, zoom, and origin.
    void adjustOrthoProjectionMatrix();

    /// @brief Deleted copy constructor — OrthoView is not copyable.
    OrthoView(const OrthoView&) = delete;
    /// @brief Deleted copy assignment operator — OrthoView is not copyable.
    OrthoView& operator =(const OrthoView&) = delete;
protected:
    /// @brief Default constructor (used by subclasses).
    OrthoView();

    /// @brief Sets the current zoom level and updates the projection matrix.
    /// @param zoom The new zoom level.
    void setZoom(double zoom);

    /// @brief Returns the current zoom level.
    /// @return The zoom level (1.0 = 100%).
    double getZoom() const override;

    /// @brief Sets the minimum and maximum allowed zoom levels.
    /// @param minZoom Minimum zoom (e.g. 0.01 for 1%).
    /// @param maxZoom Maximum zoom (e.g. 100 for 10000%).
    void setZoomLimits(double minZoom, double maxZoom);

    /// @brief Returns the current model-space origin of the visible region.
    /// @return Reference to the origin point.
    const gui::Point& getOrigin() const;

    /// @brief Returns the current orthographic projection matrix.
    /// @return Reference to the 4x4 projection matrix.
    const glm::mat4& getProjection() const;

    /// @brief Converts a point from view (frame) coordinates to model coordinates.
    /// @param framePoint The point in view/frame coordinates.
    /// @return The corresponding point in model coordinates.
    gui::Point getModelPoint(const gui::Point& framePoint) const;

    /// @brief Handles view resize events and updates the projection matrix.
    /// @param newSize The new size of the view.
    void onResize(const gui::Size& newSize) override;

    /// @brief Handles keyboard events for zooming and panning.
    /// @param key The key event descriptor.
    /// @return True if the key event was consumed.
    bool onKeyPressed(const Key& key) override;
//    bool onKeyReleased(const Key& key) override;

    /// @brief Handles pinch-to-zoom or scroll-wheel zoom gestures.
    /// @param inputDevice The input device that generated the zoom event.
    /// @return True if the zoom event was consumed.
    bool onZoom(const gui::InputDevice& inputDevice) override;

    /// @brief Handles scroll events to pan the view.
    /// @param inputDevice The input device that generated the scroll event.
    /// @return True if the scroll event was consumed.
    bool onScroll(const gui::InputDevice& inputDevice) override;

    /// @brief Handles primary button press to begin a drag-pan operation.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Handles primary button release to end a drag-pan operation.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;

    /// @brief Handles primary button double-click to reset zoom to 100%.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;

    /// @brief Handles cursor drag events to pan the view.
    /// @param inputDevice The input device that generated the event.
    void onCursorDragged(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor exits the view area; ends any ongoing drag.
    /// @param inputDevice The input device that generated the event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;

    /// @brief Creates the OpenGL context and sets up the initial projection.
    /// @param invertYAxis If true the Y axis is flipped so that Y increases downward.
    void createContext(bool invertYAxis = false);

    /// @brief Sets the model-space reference point used as the view origin.
    /// @param refPt The new reference point in model coordinates.
    void setModelRefPoint(const gui::Point& refPt);
};

} //namespace gl
} //namespace gui
