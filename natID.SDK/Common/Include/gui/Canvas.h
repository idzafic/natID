// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Canvas.h
    @brief A drawable view that supports scaling, animation, scrolling, and export to multiple formats. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DrawableView.h"
#include "Cursor.h"

namespace gui
{

class Image;
class ViewScroller;
class CanvasHelper;

#ifdef MU_WINDOWS
/// @brief Internal private data structure used by Canvas on Windows.
struct CanvasPriv
{
    double scale = 1.0f; ///< Current drawing scale factor
    td::Point<td::INT4> initialDragPoint; ///< Starting point of a drag operation
    td::LUINT8 animatingToken = 0; ///< Token used to track animation state
    td::LUINT8 drawToken = 0; ///< Token used to track draw operations
    void* zoomManager = nullptr; ///< Pointer to an optional zoom manager

    td::UINT2 inputEvents = 0; ///< Bitmask of registered input events

    gui::Cursor::Type cursorType = gui::Cursor::Type::Default;; ///< Current cursor type
    td::BYTE sendScrollEvents = 0; ///< Flag indicating whether scroll events are forwarded
    td::BYTE sendFocusEvents = 0; ///< Flag indicating whether focus events are forwarded
    td::BYTE enableResizeEvent = 0; ///< Flag indicating whether resize events are enabled
    td::BYTE isAnimating = 0; ///< Flag indicating whether the canvas is currently animating
    td::BYTE closing = 0; ///< Flag indicating whether the canvas is being closed
    td::BYTE dragInProgress = 0; ///< Flag indicating whether a drag gesture is active
};
#endif

/// @brief A view that provides drawing surface capabilities with support for scaling, animation, and export.
class NATGUI_API Canvas : public DrawableView
{
    friend class CanvasHelper;

public:
    /// @brief Rendering backend type for the canvas.
    enum class Backend : unsigned char {Display=0, Bitmap, EPS, PDF, SVG, Printer};
    /// @brief Placement mode that determines how the canvas is positioned and scaled within its parent.
    enum class Placement {SingleNonScalable=0, SingleScalable, InScroller};
protected:
    ViewScroller* _pScroller = nullptr; ///< Pointer to the associated view scroller, if any
private:

#ifdef MU_WINDOWS
    CanvasPriv _priv; ///< Windows-specific private canvas data
#endif

protected:

    td::UINT2 _fixedWidth = 0; ///< Fixed width of the canvas in pixels (0 means auto)
    td::UINT2 _fixedHeight = 0; ///< Fixed height of the canvas in pixels (0 means auto)
    Backend _backend = Backend::Display; ///< Current rendering backend
protected:
    /// @brief Internal constructor for subclass use.
    /// @param unused Placeholder parameter to differentiate from default constructor.
    Canvas(int);
    /// @brief Measures the canvas cell for layout purposes.
    /// @param cell Layout cell information to fill.
    void measure(CellInfo& cell) override;
    /// @brief Re-measures the canvas cell after layout changes.
    /// @param cell Layout cell information to update.
    void reMeasure(CellInfo& cell) override;
    /// @brief Applies a new drawing scale to the canvas.
    /// @param newScale The scale factor to apply.
    void setScale(double newScale) override;
//
    /// @brief Handles a key press event on the canvas.
    /// @param key The key that was pressed.
    /// @return True if the event was consumed, false otherwise.
    bool onKeyPressed(const gui::Key& key) override;

    /// @brief Enables or disables resize events for the canvas.
    /// @param bEnable True to enable, false to disable (default is disabled).
    void enableResizeEvent(bool bEnable); //false if not called

    /// @brief Associates a ViewScroller with this canvas.
    /// @param pScroller Pointer to the ViewScroller to attach.
    void setScroller(ViewScroller* pScroller) override;
    /// @brief Returns the currently associated ViewScroller.
    /// @return Pointer to the ViewScroller, or nullptr if none.
    ViewScroller* getScroller();

    /// @brief Notifies the canvas that the model size has changed.
    void handleModelSizeChanged();
    /// @brief Sets the preferred frame rate range for animation.
    /// @param minFPS Minimum frames per second.
    /// @param maxFPS Maximum frames per second.
    void setPreferredFrameRateRange(float minFPS, float maxFPS);

    //required on macOS to clip the drawing within the borders of the canvas
    /// @brief Sets the clipping mode to restrict drawing within canvas bounds.
    /// @param clipMode Clipping mode (defaults to ClipMode::All).
    void setClipsToBounds(ClipMode clipMode = ClipMode::All);

    Canvas(const Canvas&) = delete;
    Canvas& operator =(const Canvas&) = delete;
public:
    /// @brief Default constructor; creates a canvas with display backend.
    Canvas();
    /// @brief Constructs a canvas with specified input events and optional geometry.
    /// @param events List of input device events to register.
    /// @param g Optional pointer to initial geometry.
    Canvas(const std::initializer_list<InputDevice::Event>& events, const Geometry* g = nullptr);
    /// @brief Destructor.
    ~Canvas();
    /// @brief Returns the object type identifier for this canvas.
    /// @return ObjType::Canvas
    gui::ObjType getObjType() const override { return ObjType::Canvas;}

    /// @brief Returns the placement mode of this canvas within its container.
    /// @return The Placement enum value describing the layout behavior.
    virtual Placement getPlacement() const;

    /// @brief Retrieves the visible portion of the canvas in model coordinates.
    /// @param r Output rectangle in model coordinate space.
    void getVisiblePartInModelCoordinates(gui::Rect& r) const;

    /// @brief Retrieves the current logical size of the canvas.
    /// @param sz Output size structure to fill.
    void getSize(Size& sz) const;

    /// @brief Scales the canvas by the given factor.
    /// @param newScale The scale factor to apply.
    void scale(double newScale) override;
    /// @brief Scales the canvas to a specific point.
    /// @param newScale The scale factor to apply.
    /// @param toPoint The point in canvas coordinates to scale toward.
    void scaleToPoint(double newScale, const gui::Point& toPoint) override;

    /// @brief Returns the current scale factor of the canvas.
    /// @return The current scale factor.
    double getScale() const override;

    /// @brief Sets the mouse cursor type for this canvas.
    /// @param ct The cursor type to apply.
    /// @param showImmediately If true, the cursor changes immediately.
    void setCursor(Cursor::Type ct, bool showImmediately = false) const;

    //Do not use Tiners or threads to implement animation
    //These two methods should be used for animation
    /// @brief Starts the canvas animation loop.
    void startAnimation();
    /// @brief Stops the canvas animation loop.
    void stopAnimation();
    /// @brief Returns whether the canvas is currently animating.
    /// @return True if animating, false otherwise.
    bool isAnimating() const;

//    void showCursor(bool bShow); //Show/hide cursor
//    void moveCursorToCenter() const; //moves cursor to center of this view

    /// @brief Sets a fixed width for the canvas.
    /// @param w Fixed width in pixels.
    void setFixedWidth(td::WORD w);
    /// @brief Returns the fixed width of the canvas.
    /// @return Fixed width in pixels.
    td::WORD getFixedWidth() const;

    /// @brief Sets a fixed height for the canvas.
    /// @param h Fixed height in pixels.
    void setFixedHeight(td::WORD h);
    /// @brief Returns the fixed height of the canvas.
    /// @return Fixed height in pixels.
    td::WORD getFixedHeight() const;

    /// @brief Renders the canvas content to an image.
    /// @param imgToDrawOn The target image to draw on.
    /// @param resScale Resolution scale factor (default 1.0).
    /// @param onlyVisiblePart If true, only the visible portion is drawn.
    /// @return True on success, false on failure.
    bool drawToImage(gui::Image& imgToDrawOn, double resScale=1.0, bool onlyVisiblePart=true);
    /// @brief Renders a specific model region to an image.
    /// @param imgToDrawOn The target image to draw on.
    /// @param modelBound The rectangle in model coordinates to render.
    /// @param resScale Resolution scale factor (default 1.0).
    /// @return True on success, false on failure.
    bool drawToImage(gui::Image& imgToDrawOn, const gui::Rect& modelBound, double resScale = 1.0);

    /// @brief Exports the canvas content to an EPS file.
    /// @param fileName Output file name.
    /// @param onlyVisiblePart If true, only the visible portion is exported.
    /// @return True on success, false on failure.
    bool exportToEPS(const td::String& fileName, bool onlyVisiblePart=true);
    /// @brief Exports a specific region to an EPS file.
    /// @param modelBound The rectangle in model coordinates to export.
    /// @param fileName Output file name.
    /// @return True on success, false on failure.
    bool exportToEPS(const gui::Rect& modelBound, const td::String& fileName);

    /// @brief Exports the canvas content to a PDF file.
    /// @param fileName Output file name.
    /// @param onlyVisiblePart If true, only the visible portion is exported.
    /// @return True on success, false on failure.
    bool exportToPDF(const td::String& fileName, bool onlyVisiblePart=true);
    /// @brief Exports a specific region to a PDF file.
    /// @param modelBound The rectangle in model coordinates to export.
    /// @param fileName Output file name.
    /// @return True on success, false on failure.
    bool exportToPDF(const gui::Rect& modelBound, const td::String& fileName);

    /// @brief Exports the canvas content to an SVG file.
    /// @param fileName Output file name.
    /// @param onlyVisiblePart If true, only the visible portion is exported.
    /// @return True on success, false on failure.
    bool exportToSVG(const td::String& fileName, bool onlyVisiblePart=true);
    /// @brief Exports a specific region to an SVG file.
    /// @param modelBound The rectangle in model coordinates to export.
    /// @param fileName Output file name.
    /// @return True on success, false on failure.
    bool exportToSVG(const gui::Rect& modelBound, const td::String& fileName);

    /// @brief Begins a print operation using the given page info.
    /// @param pi Pointer to the page info structure.
    /// @return True if printing started successfully.
    bool startPrinting(const PageInfo* pi);
    /// @brief Begins printing to a PDF file using the given page info.
    /// @param pi Pointer to the page info structure.
    /// @param fileName Output PDF file name.
    /// @return True if printing started successfully.
    bool startPrintingToPDF(const PageInfo* pi, const td::String& fileName);

//    bool exportToArtwork(gui::Artwork artworkType, const td::String& fileName, bool onlyVisiblePart=true);
//    bool exportToSVG(gui::Artwork artworkType, const gui::Rect& modelBound, const td::String& fileName);

};

} //namespace gui
