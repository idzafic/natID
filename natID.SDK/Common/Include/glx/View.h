// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file View.h
    @brief Declares the glx::View class, a GPU-backed drawable view for Metal rendering. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#ifdef MU_NATGLXS
#include <glx/sa/DrawableView.h>
using DrawableView = glx::DrawableView;
#else
#include <gui/DrawableView.h>
using DrawableView = gui::DrawableView;
#endif

#include <gui/Cursor.h>
#include <glx/IRenderer.h>
#include <glx/Drawable.h>
#include <glx/Texture.h>
#include <glx/Types.h>
#include <glx/RenderPassDescriptor.h>
#include <glx/CommandQueue.h>
#include <mutex>

namespace glx
{

/// @brief Enumerates image file formats supported when saving the drawable contents.
enum class ImageFormat : td::BYTE {
    PNG = 0, ///< Portable Network Graphics format.
    BMP,     ///< Bitmap image format.
    TGA,     ///< Truevision TGA format.
    JPG      ///< JPEG compressed format.
};
class ViewHelper;

/// @brief Holds the textures used during a fake (off-screen) render pass.
struct FakeRenderPassData
{
    Texture colorTexture; ///< Off-screen colour attachment texture.
    Texture depthTexture; ///< Off-screen depth attachment texture.
};


/// @brief A GPU-backed drawable view that drives Metal rendering through an IRenderer.
class NATGLX_API View : public DrawableView
{
    friend class ViewHelper;

protected:
    glx::IRenderer* _pRenderer = nullptr;         ///< Pointer to the renderer that performs GPU drawing.
private:
    glx::Device _device;                           ///< The Metal device used for GPU operations.
	std::mutex _currentDrawableMutex;              ///< Mutex protecting access to the current drawable.
    FakeRenderPassData _fakeRenderPassData;        ///< Data for the optional off-screen render pass.

    glx::Size _pendingSize;      ///< Requested new size, stored until the swapchain can be safely recreated.
    bool _resizePending = false; ///< True when a swapchain resize has been deferred.
    bool _useFakeRenderPass = false; ///< True when rendering to an off-screen texture instead of the swapchain.
private:
    /// @brief Returns the native surface handle used to create the Metal layer.
    /// @return Platform-specific surface handle.
    gui::Handle getNativeSurfaceHandle() override final;

    /// @brief Creates an off-screen (non-framebuffer) render pass.
    void createNonFramebufferRenderPass();
//    void _prepareNextFrame();
protected:
    /// @brief Calculates the preferred size of this view.
    /// @param cell Layout cell information to be filled in.
    void measure(gui::CellInfo& cell) override final;

    /// @brief Recalculates the preferred size of this view after a layout change.
    /// @param cell Layout cell information to be filled in.
    void reMeasure(gui::CellInfo& cell) override final;

    /// @brief Factory method; creates and returns the renderer for this view.
    /// @return Pointer to the newly created IRenderer, or nullptr to use no renderer.
    virtual glx::IRenderer* createRenderer();

    /// @brief Handles a view resize event by deferring the swapchain recreation.
    /// @param newSize The new view size in points.
    void onResize(const gui::Size& newSize) override final;

    /// @brief Drives a single frame render via the attached renderer.
    /// @param rect The dirty rectangle that triggered the redraw.
    void onDraw(const gui::Rect& rect) override final;

    /// @brief Deleted copy constructor; views are non-copyable.
    View(const View&) = delete;

    /// @brief Deleted copy-assignment operator; views are non-copyable.
    View& operator =(const View&) = delete;
public:
    /// @brief Constructs a View and optionally subscribes to a set of input device events.
    /// @param eventList List of input events the view should receive.
    View(const std::initializer_list<gui::InputDevice::Event>& eventList = {});

    /// @brief Constructs a View on a specific Metal device and optionally subscribes to input events.
    /// @param device    The Metal device to use for rendering.
    /// @param eventList List of input events the view should receive.
    View(const Device& device, const std::initializer_list<gui::InputDevice::Event>& eventList = {});

    /// @brief Destructor; releases all Metal resources held by the view.
    ~View();

    /// @brief Returns the GUI object type identifier for this view.
    /// @return gui::ObjType::GLXView.
    gui::ObjType getObjType() const override { return gui::ObjType::GLXView;}
//
    /// @brief Sets the pixel format used for the drawable's colour attachment.
    /// @param format The desired glx::PixelFormat.
    void setPixelFormat(glx::PixelFormat format);

    /// @brief Returns the pixel format of the drawable's colour attachment.
    /// @return The current glx::PixelFormat.
    glx::PixelFormat pixelFormat() const;

    /// @brief Assigns a Metal device to the view.
    /// @param device The glx::Device to use.
    void setDevice(Device& device);

    /// @brief Returns the Metal device associated with this view.
    /// @return Const reference to the current glx::Device.
    const Device& device() const;

//    CA::MetalDrawable* currentDrawable() const;
    /// @brief Returns the drawable that is currently being rendered into.
    /// @return The current Drawable.
    Drawable currentDrawable() const;

    /// @brief Controls whether the drawable texture is restricted to framebuffer use only.
    /// @param framebufferOnly True to restrict the drawable to framebuffer operations.
    void setFramebufferOnly( bool framebufferOnly );

    /// @brief Returns whether the drawable texture is restricted to framebuffer use only.
    /// @return True if framebuffer-only mode is active.
    bool framebufferOnly() const;

    /// @brief Sets the texture usage flags for the depth-stencil attachment.
    /// @param textureUsage Texture::Usage flags to apply.
    void setDepthStencilAttachmentTextureUsage(Texture::Usage textureUsage);

    /// @brief Returns the texture usage flags for the depth-stencil attachment.
    /// @return The current Texture::Usage flags for the depth-stencil attachment.
    Texture::Usage depthStencilAttachmentTextureUsage() const;

    /// @brief Sets the texture usage flags for the multisample colour attachment.
    /// @param textureUsage Texture::Usage flags to apply.
    void setMultisampleColorAttachmentTextureUsage(Texture::Usage textureUsage);

    /// @brief Returns the texture usage flags for the multisample colour attachment.
    /// @return The current Texture::Usage flags for the multisample attachment.
    Texture::Usage multisampleColorAttachmentTextureUsage() const;

    /// @brief Controls whether the view presents its drawable in a separate transaction.
    /// @param presentsWithTransaction True to present using a Core Animation transaction.
    void setPresentsWithTransaction( bool presentsWithTransaction );

    /// @brief Returns whether the view uses a transaction to present its drawable.
    /// @return True if transaction-based presentation is enabled.
    bool presentsWithTransaction() const;

    /// @brief Sets the pixel format for the colour attachment.
    /// @param colorPixelFormat The desired colour PixelFormat.
    void setColorPixelFormat(PixelFormat colorPixelFormat);

    /// @brief Returns the pixel format of the colour attachment.
    /// @return The current colour PixelFormat.
    PixelFormat colorPixelFormat() const;

    /// @brief Sets the pixel format for the depth-stencil attachment.
    /// @param colorPixelFormat The desired depth-stencil PixelFormat.
    void setDepthStencilPixelFormat(PixelFormat colorPixelFormat);

    /// @brief Returns the pixel format of the depth-stencil attachment.
    /// @return The current depth-stencil PixelFormat.
    PixelFormat  depthStencilPixelFormat() const;

    /// @brief Sets the number of samples per pixel for multisampled rendering.
    /// @param sampleCount The desired sample count.
    void setSampleCount( size_t sampleCount );

    /// @brief Returns the number of samples per pixel for multisampled rendering.
    /// @return The current sample count.
    size_t sampleCount() const;

    /// @brief Sets the RGBA colour used to clear the colour attachment at the start of each frame.
    /// @param clearColor The clear colour value.
    void setClearColor( td::Color clearColor );

    /// @brief Returns the RGBA colour used to clear the colour attachment.
    /// @return The current clear colour.
    td::Color clearColor() const;

    /// @brief Sets the depth value used to clear the depth attachment at the start of each frame.
    /// @param clearDepth The clear depth value (typically 1.0).
    void setClearDepth( double clearDepth );

    /// @brief Returns the depth value used to clear the depth attachment.
    /// @return The current clear depth value.
    double clearDepth() const;

    /// @brief Sets the stencil value used to clear the stencil attachment at the start of each frame.
    /// @param clearStencil The clear stencil value.
    void setClearStencil( uint32_t clearStencil );

    /// @brief Returns the stencil value used to clear the stencil attachment.
    /// @return The current clear stencil value.
    uint32_t clearStencil() const;

    /// @brief Returns the depth-stencil texture associated with the current render pass.
    /// @return The depth-stencil Texture.
    Texture depthStencilTexture() const;

    /// @brief Returns the multisample colour texture associated with the current render pass.
    /// @return The multisample colour Texture.
    Texture multisampleColorTexture() const;

    /// @brief Releases all cached drawable objects.
    void releaseDrawables() const;

    /// @brief Returns the render pass descriptor for the current frame.
    /// @return The current RenderPassDescriptor.
    RenderPassDescriptor currentRenderPassDescriptor() const;

    /// @brief Sets the preferred number of frames rendered per second.
    /// @param preferredFramesPerSecond The desired frame rate.
    void setPreferredFramesPerSecond(int preferredFramesPerSecond);

    /// @brief Returns the preferred number of frames rendered per second.
    /// @return The current preferred frame rate.
    int preferredFramesPerSecond() const;

    /// @brief Controls whether the view uses setNeedsDisplay to trigger redraws.
    /// @param enableSetNeedsDisplay True to use setNeedsDisplay-based rendering.
    void setEnableSetNeedsDisplay( bool enableSetNeedsDisplay );

    /// @brief Returns whether setNeedsDisplay-based rendering is enabled.
    /// @return True if setNeedsDisplay triggering is active.
    bool enableSetNeedsDisplay() const;

    /// @brief Controls whether the drawable is automatically resized with the view.
    /// @param autoresizeDrawable True to enable automatic drawable resizing.
    void setAutoresizeDrawable( bool autoresizeDrawable );

    /// @brief Returns whether the drawable is automatically resized with the view.
    /// @return True if automatic drawable resizing is active.
    bool autoresizeDrawable();

    /// @brief Sets the drawable size in pixels, overriding automatic sizing.
    /// @param drawableSize The desired drawable size.
    void setDrawableSize( const glx::Size& drawableSize );

    /// @brief Returns the current drawable size in pixels.
    /// @param drawableSize Output parameter populated with the drawable size.
    void getDrawableSize(glx::Size& drawableSize) const;

    /// @brief Returns the preferred drawable size as suggested by the display.
    /// @param sz Output parameter populated with the preferred size.
    void getPreferredDrawableSize(glx::Size& sz) const;

    /// @brief Returns the preferred Metal device for this view as suggested by the system.
    /// @return The preferred glx::Device.
    Device preferredDevice() const;

    /// @brief Pauses or resumes the rendering loop.
    /// @param paused True to pause rendering.
    void setPaused( bool paused );

    /// @brief Returns whether the rendering loop is currently paused.
    /// @return True if rendering is paused.
    bool isPaused() const;

//    void                        setColorSpace( CGColorSpaceRef colorSpace );
//    CGColorSpaceRef                colorSpace() const;

    /// @brief Triggers an immediate redraw of the view.
    void draw();

    /// @brief Saves the current drawable contents to a file.
    /// @param filepath     Destination file path including extension.
    /// @param commandQueue The command queue used for the GPU readback.
    /// @param format       Image file format (default: PNG).
    /// @return True if the file was saved successfully.
    bool saveDrawable(const td::String& filepath, CommandQueue commandQueue, ImageFormat format = ImageFormat::PNG);

    /// @brief Returns whether the view is in continuous (animation) render mode.
    /// @return True if continuous rendering is active.
    bool isContinousRenderMode() const;

    /// @brief Enables or disables continuous (animation) render mode.
    /// @param bContinous True to enable continuous rendering.
    void setContinousRenderMode(bool bContinous);

    /// @brief Locks and hides the cursor, directing all cursor movement to this view.
    void enableCursorCapture();

    /// @brief Toggles cursor capture on or off.
    void toggleCursorCapture();

//    void setContRenderMode(bool bContinuous);
//    bool isContRenderMode() const;
//    void getVisiblePartInModelCoordinates(gui::Rect& r) const;
//
//    void setCursor(gui::Cursor::Type ct, bool showImmediately = false) const;
//
//    void startAnimation();
//    void stopAnimation();
//    bool isAnimated() const;
};

} //namespace glx
