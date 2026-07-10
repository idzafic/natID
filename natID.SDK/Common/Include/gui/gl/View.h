// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file View.h
    @brief OpenGL-backed drawable view that manages context creation, frame rendering, and animation. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGL.h"
#include "Context.h"
#include <gui/DrawableView.h>
#include <gui/gl/GL.h>
#include <tuple>
#include <gui/Cursor.h>
#include <gui/Texture.h>
#include <functional>

namespace gui
{
namespace gl
{

class ViewHelper;

/// @brief A drawable view that owns an OpenGL rendering context and provides the base for
///        all GPU-rendered scenes within the GUI framework.
class NATGL_API View : public gui::DrawableView
{
    friend class ViewHelper;

private:
    std::function<void()> _onTextureRead; ///< Callback invoked after an asynchronous framebuffer read completes.
protected:
    float _logicalToPhysicalPixelScale = 1.0f;  ///< Scale factor from logical pixels to physical (HiDPI) pixels.
    float _logicaToPhysicalSizeScale = 1.0f;    ///< Scale factor from logical size units to physical size units.
private:
    td::UINT4 _screenFB = 0xFFFFFFFF;   ///< OpenGL framebuffer object ID for the default screen framebuffer.
    td::BYTE _contRenderMode = 0;        ///< Non-zero when continuous (animation) render mode is active.
private:
    /// @brief Internal helper called before each new frame to prepare GL state.
    void _prepareNextFrame();

protected:
    /// @brief Computes the preferred size of the view for layout purposes.
    /// @param cell Layout cell info to populate with size constraints.
    void measure(CellInfo& cell) override;
    /// @brief Re-computes the view size after a layout change.
    /// @param cell Layout cell info to update.
    void reMeasure(CellInfo& cell) override;
    /// @brief Requests a redraw of the view without changing layout.
    void reRender() const;

    /// @brief Returns the current zoom factor. Should be overloaded by subclasses.
    /// @return The zoom level as a double.
    virtual double getZoom() const; //neeeds to overloaded in upper classes
    //link GLSL program using vertex and fragment shader from resource texts
    /// @brief Links a GLSL program from embedded resource shader texts.
    /// @param vertexResID Resource identifier string for the vertex shader source.
    /// @param fragmentResID Resource identifier string for the fragment shader source.
    /// @return The OpenGL program object ID, or 0 on failure.
    td::UINT4 linkProgram(const char* vertexResID, const char* fragmentResID);

    //create programs, allocate vbos, etc in onInit
    /// @brief Called once after the GL context is ready. Override to create shader programs and buffers.
    virtual void onInit();

    /// @brief Makes this view's GL context the current context on the calling thread.
    void makeCurrentContext() const;

    //delete programs, vbos, ...
    /// @brief Called when the view is being destroyed. Override to release GPU resources.
    virtual void cleanResources();

    /// @brief Queries the OpenGL version supported by the current context.
    /// @return A tuple of (major version, minor version).
    std::tuple<int, int> getOpenGLVersion() const;
//    void releaseGL();
    /// @brief Called when the zoom level has changed. Override to update zoom-dependent resources.
    virtual void onZoomChanged();
    /// @brief Called when the view origin (scroll position) has changed.
    virtual void onOriginChanged();

    /// @brief Creates an OpenGL context with the specified settings.
    /// @param context Context configuration parameters.
    void createContext(const gl::Context& context);
    /// @brief Creates an OpenGL context and registers input device event handlers.
    /// @param context Context configuration parameters.
    /// @param events List of input device events to handle.
    /// @param g Optional geometry constraint for the context surface.
    void createContext(const gl::Context& context, const std::initializer_list<InputDevice::Event>& events, const Geometry* g = nullptr);

    /// @brief Initiates an asynchronous read of the current framebuffer into a texture.
    /// @param texture Target texture to receive the framebuffer pixel data.
    /// @param fn Callback invoked on the main thread once the read is complete.
    void readFrameFrameBuffer(gui::Texture& texture, const std::function<void()>& fn);
    /// @brief Called before each frame to allow subclasses to decide whether to redraw.
    /// @return True if a new frame should be rendered, false to skip rendering.
    virtual bool prepareNextFrame();

    /// @brief Registers the view to receive focus gain/loss events.
    void registerForFocusEvents() const;

    /// @brief Activates the default screen framebuffer for rendering.
    void activateScreenFB();

    /// @brief Returns the scale factor from logical pixels to physical pixels.
    /// @return The logical-to-physical pixel scale as a float.
    float getLogicalToPhysicalPixelScale() const
    {
        return _logicalToPhysicalPixelScale;
    }

    /// @brief Returns the scale factor from logical size units to physical size units.
    /// @return The logical-to-physical size scale as a float.
    float getLogicalToPhysicalSizeScale() const
    {
        return _logicaToPhysicalSizeScale;
    }
#ifdef MU_DEBUG
    /// @brief Returns a human-readable description for the given OpenGL error code (debug builds only).
    /// @param error The OpenGL error code returned by glGetError().
    /// @return A null-terminated string describing the error.
    const char* const dbgGetErrorStringCode(GLenum error) const
    {
        switch (error) {
            case GL_NO_ERROR:
                return "GL_NO_ERROR: No error has been recorded.";
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE: A numeric argument is out of range.";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
#ifdef GL_STACK_OVERFLOW
            case GL_STACK_OVERFLOW:
                return "GL_STACK_OVERFLOW: This command would cause a stack overflow. (Deprecated in modern OpenGL)";
#endif
#ifdef GL_STACK_UNDERFLOW
            case GL_STACK_UNDERFLOW:
                return "GL_STACK_UNDERFLOW: This command would cause a stack underflow. (Deprecated in modern OpenGL)";
#endif
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    #ifdef GL_CONTEXT_LOST
            case GL_CONTEXT_LOST:
                return "GL_CONTEXT_LOST: The OpenGL context has been lost (typically due to a graphics reset).";
    #endif
            default:
                return "Unknown OpenGL error.";
        }
    }
#endif

    /// @brief Checks for a pending OpenGL error and logs/asserts in debug builds.
    inline void dbgCheckGLError() const
    {
#ifdef MU_DEBUG
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            const char* const pErr = dbgGetErrorStringCode(error);
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x, desc=%s", error, pErr);
            assert(false);
        }
#endif
    }
    View(const View&) = delete;
    View& operator =(const View&) = delete;
    /// @brief Default constructor. Initializes the view without creating a GL context.
    View();
public:
    /// @brief Destructor. Cleans up the GL context and associated resources.
    ~View();
    /// @brief Returns the GUI object type identifier for this view.
    /// @return ObjType::GLView.
    gui::ObjType getObjType() const override { return ObjType::GLView;}

    /// @brief Enables or disables continuous rendering (animation) mode.
    /// @param bContinuous True to enable continuous frame rendering, false for on-demand rendering.
    void setContRenderMode(bool bContinuous);
    /// @brief Returns true if continuous rendering mode is currently active.
    /// @return True if the view is in continuous render mode.
    bool isContRenderMode() const;
    /// @brief Computes the portion of the scene visible in the current viewport in model coordinates.
    /// @param r Output rectangle filled with the visible area in model (world) coordinates.
    void getVisiblePartInModelCoordinates(gui::Rect& r) const;

    /// @brief Changes the mouse cursor displayed over the view.
    /// @param ct The cursor type to display.
    /// @param showImmediately If true, the cursor change is applied immediately.
    void setCursor(Cursor::Type ct, bool showImmediately = false) const;

    /// @brief Starts the continuous rendering animation loop.
    void startAnimation();
    /// @brief Stops the continuous rendering animation loop.
    void stopAnimation();
    /// @brief Returns true if the animation loop is currently running.
    /// @return True if the view is animating.
    bool isAnimated() const;
};

} //namespace gl
} //namespace gui
