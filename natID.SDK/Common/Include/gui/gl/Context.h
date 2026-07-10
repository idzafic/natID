// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Context.h
 * @brief Describes an OpenGL rendering context and provides static pipeline-state helpers.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGL.h"
//#include <gui/NatObject.h>
#include <td/Types.h>
#include <tuple>
#include <td/ColorID.h>
#include "Textures.h"

namespace gui
{
namespace gl
{

class ContextHelper;

/// @brief Describes OpenGL context creation parameters and exposes static render-state control methods.
class NATGL_API Context //: public NatObject
{
    friend class ContextHelper;
public:
    /// @brief Controls whether the view renders continuously (animated) or on demand.
    enum class Animation : td::BYTE {Yes, No};
    /// @brief Selects the OpenGL Core Profile version to request.
    enum class Profile : td::BYTE {Core3_2=0, Core4_1};
    /// @brief Identifies which framebuffer attachment to clear.
    enum class Clear : td::BYTE {Color=1, Depth, Stencil};
    /// @brief Per-fragment test flags that can be enabled or disabled.
    enum class Flag : td::BYTE {DepthTest=0, CullFace, StencilTest, Blend};
    /// @brief Polygon rasterization mode.
    enum class PolygonMode : td::BYTE{Fill=0, Line, Point};
private:
    Profile _profile; ///< Minimum required OpenGL core profile version.
    DepthBuffer::Size _depthBufferSize = DepthBuffer::Size::B0; //0-bytes (no depth buffer)
    StencilBuffer::Size _stencilBufferSize = StencilBuffer::Size::B0; //0-bytes (no stencil buffer)
    float _minFPS = 0; ///< Minimum preferred frame rate.
    float _maxFPS = 0; ///< Maximum preferred frame rate.
    td::BYTE _animation = 0;  ///< Non-zero when continuous animation is requested.
    td::BYTE _rgbSize = 24;   ///< Total number of bits allocated for the RGB color channels.
    td::BYTE _alphaSize = 8;  ///< Number of bits allocated for the alpha channel.

protected:
    Context(const Context&) = delete;
public:
    /// @brief Creates a context with explicit animation control and optional buffer sizes.
    /// @param animation Whether to run in continuous animation mode.
    /// @param depthBufferSize Depth buffer bit depth.
    /// @param stencilBufferSize Stencil buffer bit depth.
    /// @param profile Minimum OpenGL Core Profile version.
    Context(Animation animation, DepthBuffer::Size depthBufferSize = DepthBuffer::Size::B0, StencilBuffer::Size stencilBufferSize = StencilBuffer::Size::B0, Context::Profile profile = Profile::Core4_1);

    /// @brief Creates a context with optional buffer sizes and no continuous animation.
    /// @param depthBufferSize Depth buffer bit depth.
    /// @param stencilBufferSize Stencil buffer bit depth.
    /// @param profile Minimum OpenGL Core Profile version.
    Context(DepthBuffer::Size depthBufferSize = DepthBuffer::Size::B0, StencilBuffer::Size stencilBufferSize = StencilBuffer::Size::B0, Context::Profile profile = Profile::Core4_1);

    /// @brief Returns the requested OpenGL core profile.
    /// @return The Profile enum value for this context.
    Context::Profile getProfile() const;

    /// @brief Sets the number of bits to request for the color and alpha framebuffer channels.
    /// @param numberOfBitsForFGB Total RGB channel bit depth.
    /// @param numberOfBitsForAlpha Alpha channel bit depth.
    void setRGBSize(td::BYTE numberOfBitsForFGB, td::BYTE numberOfBitsForAlpha);
//    void setAnimation(bool bAnimation);

    /// @brief Returns whether continuous animation rendering is enabled.
    /// @return true if animation mode is active.
    bool isAnimation() const;

    /// @brief Returns the RGB and alpha channel bit depths as a tuple.
    /// @return Tuple of (rgbBits, alphaBits).
    std::tuple<td::BYTE, td::BYTE> getRGBSize() const;

    /// @brief Returns the depth buffer size in bits.
    /// @return Depth buffer bit depth.
    td::BYTE getDepthBufferSize() const; //in bits

    /// @brief Returns the stencil buffer size in bits.
    /// @return Stencil buffer bit depth.
    td::BYTE getStencilBufferSize() const; //in bits

    /// @brief Sets the preferred minimum and maximum frame-rate range.
    /// @param minFPS Minimum frames per second.
    /// @param maxFPS Maximum frames per second.
    void setPreferredFrameRateRange(float minFPS, float maxFPS);

    /// @brief Returns the preferred frame-rate range as a tuple.
    /// @return Tuple of (minFPS, maxFPS).
    std::tuple<float, float> getPreferredFrameRateRange() const;

    /// @brief Clears the specified framebuffer attachment.
    /// @param clearBit Attachment to clear (Color, Depth, or Stencil).
    static void clear(Clear clearBit);

    /// @brief Clears multiple framebuffer attachments at once.
    /// @param clearList Initializer list of attachments to clear.
    static void clear(const std::initializer_list<Clear>& clearList);

    /// @brief Clears the color buffer to the specified color.
    /// @param clr Color identifier to use as the clear color.
    static void clear(td::ColorID clr);

    /// @brief Enables the specified per-fragment test or blending flag.
    /// @param flag The OpenGL capability flag to enable.
    static void enable(Flag flag);

    /// @brief Disables the specified per-fragment test or blending flag.
    /// @param flag The OpenGL capability flag to disable.
    static void disable(Flag flag);

    /// @brief Controls whether depth values are written to the depth buffer.
    /// @param bMask true to enable depth writes; false to disable them.
    static void maskDepthBuffer(bool bMask);

    /// @brief Sets the comparison function used by the depth test.
    /// @param fn Depth comparison function to use.
    static void setDepthBufferFunction(gui::gl::DepthBuffer::Function fn);

    /// @brief Sets the write mask applied to stencil buffer values.
    /// @param mask Bit mask controlling which stencil bits are written.
    static void setStencilBufferMask(td::UINT4 mask);

    /// @brief Sets the stencil test comparison function, reference value, and mask.
    /// @param fn Stencil comparison function.
    /// @param ref Reference value for the stencil test.
    /// @param mask Bit mask applied to both the reference and stencil value before comparison.
    static void setStencilBufferFunction(gui::gl::StencilBuffer::Function fn, td::INT4 ref, td::UINT4 mask);

    /// @brief Sets the stencil buffer operations for the three possible test outcomes.
    /// @param opStencilFail Operation when the stencil test fails.
    /// @param opStencilOkDepthFail Operation when the stencil test passes but depth test fails.
    /// @param opStencilAndDepthOK Operation when both the stencil and depth tests pass.
    static void setStencilBufferOperation(gui::gl::StencilBuffer::Operation opStencilFail, gui::gl::StencilBuffer::Operation opStencilOkDepthFail,gui::gl::StencilBuffer::Operation opStencilAndDepthOK);

    /// @brief Sets the source and destination blending factors.
    /// @param sourceFactor Blending factor applied to the source fragment color.
    /// @param destFactor Blending factor applied to the destination fragment color.
    static void setBlendFunction(gui::gl::Blend::Factor sourceFactor, gui::gl::Blend::Factor destFactor);

    /// @brief Sets the GL global point primitive size.
    /// @param pointSize Point diameter in pixels.
    static void setPointSize(float pointSize);

    /// @brief Sets the polygon rasterization mode.
    /// @param mode Fill, Line (wireframe), or Point rasterization.
    static void setPolygonMode(gui::gl::Context::PolygonMode mode);
//    static void setStencilBufferMask(td::UINT4 mask);
//        static void setStencilBufferFunction(gui::gl::StencilBuffer::Fn fn, td::INT4 ref, td::UINT4 mask);
//        static void setStencilBufferOperation(gui::gl::StencilBuffer::Op fnStencilFail, gui::gl::StencilBuffer::Op fnStencilOkDepthFail, gui::gl::StencilBuffer::Op fnStencilAndDepthOK);

    /// @brief Activates the specified texture unit.
    /// @param unit Texture unit to make active.
    static void setActiveTextureUnit(gui::gl::Textures::Unit unit);

    /// @brief Binds a texture object to the currently active texture unit.
    /// @param textureID OpenGL texture object identifier.
    /// @param textureType Type of texture (e.g., Regular2D, CubeMap).
    static void bindTextureToCurrentlyActiveTextureUnit(td::UINT4 textureID, gui::gl::Texture::Type textureType);

    /// @brief Destructor; releases the OpenGL context.
    ~Context();
};

} //namespace gl
} //namespace gui
