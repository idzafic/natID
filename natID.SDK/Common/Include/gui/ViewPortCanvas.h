// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ViewportCanvas.h
    @brief Canvas with built-in pan, zoom, and coordinate-system transformation for model-space rendering. */
#pragma once

#include "Canvas.h"

namespace gui
{

class ViewportCanvasHelper;

/// @brief Canvas that maintains a zoomable, pannable viewport mapping model coordinates to screen coordinates.
class NATGUI_API ViewportCanvas : public Canvas
{
    friend class ViewportCanvasHelper;

protected:
    gui::Size _size;         ///< Current pixel size of the canvas view.
    gui::Point _modelRefPt;  ///< Reference point in model space used as the zoom/pan anchor.
    double _minZoom = 0.001; ///< Minimum allowed zoom factor.
    double _maxZoom = 1000.; ///< Maximum allowed zoom factor.
    td::BYTE _invertY = 0;   ///< Non-zero to flip the y-axis so that y increases upward.
    td::BYTE _dragging = 0;  ///< Non-zero while the user is actively panning by dragging.
public:
    /// @brief Default constructor.
    ViewportCanvas();
    /// @brief Destructor.
    ~ViewportCanvas();

    /// @brief Called after the zoom level changes; override to react to zoom events.
    virtual void onZoomChanged();
    /// @brief Called after the viewport origin changes; override to react to pan events.
    virtual void onOriginChanged();

    /// @brief Sets the current zoom factor.
    /// @param zoom New zoom factor; clamped to [_minZoom, _maxZoom].
    void setZoom(double zoom);
    /// @brief Returns the current zoom factor.
    /// @return Current zoom factor.
    double getZoom() const;
    /// @brief Sets the allowable zoom range.
    /// @param minZoom Minimum zoom factor.
    /// @param maxZoom Maximum zoom factor.
    void setZoomLimits(double minZoom, double maxZoom);

    /// @brief Returns the current viewport origin in model coordinates.
    /// @return Const reference to the model-space origin point.
    const gui::Point& getOrigin() const; //in model coordinates
    /// @brief Sets the viewport origin in model coordinates.
    /// @param tl New top-left origin point in model coordinates.
    void setOrigin(const gui::Point& tl);   //in model coordinates

    /// @brief Returns the canvas placement policy (always Viewport for this class).
    /// @return Canvas::Placement::Viewport.
    Canvas::Placement getPlacement() const override final;
    /// @brief Converts a point from frame (screen) coordinates to model coordinates.
    /// @param framePoint Point in screen/frame coordinates.
    /// @return Corresponding point in model coordinates.
    gui::Point getModelPoint(const gui::Point& framePoint) const;
    /// @brief Handles resize events by updating the internal size and adjusting the viewport.
    /// @param newSize New pixel size of the canvas.
    void onResize(const gui::Size& newSize) override;
    /// @brief Handles key-press events for zoom/pan keyboard shortcuts.
    /// @param key Descriptor of the pressed key.
    /// @return True if the event was consumed, false to propagate.
    bool onKeyPressed(const Key& key) override;
    //bool onKeyReleased(const Key& key) override;
    /// @brief Handles pinch-to-zoom and scroll-wheel zoom gestures.
    /// @param inputDevice Descriptor of the input device event.
    /// @return True if the event was consumed, false to propagate.
    bool onZoom(const gui::InputDevice& inputDevice) override;
    /// @brief Handles scroll (pan) input events.
    /// @param inputDevice Descriptor of the input device event.
    /// @return True if the event was consumed, false to propagate.
    bool onScroll(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the primary mouse/touch button is pressed to begin a drag.
    /// @param inputDevice Descriptor of the input device event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the primary mouse/touch button is released to end a drag.
    /// @param inputDevice Descriptor of the input device event.
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
    /// @brief Called on a double-click of the primary button; may reset zoom/origin.
    /// @param inputDevice Descriptor of the input device event.
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
    /// @brief Called while the cursor is dragged to pan the viewport.
    /// @param inputDevice Descriptor of the input device event including current position.
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the cursor leaves the canvas boundary during a drag.
    /// @param inputDevice Descriptor of the input device event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    /// @brief Sets the model-space reference point used as the anchor for zoom operations.
    /// @param refPt New reference point in model coordinates.
    void setModelRefPoint(const gui::Point& refPt);
};

}
