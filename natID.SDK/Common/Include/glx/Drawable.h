// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Drawable.h
 * @brief Represents a displayable surface that can be rendered into and presented on screen.
 */
#pragma once
#include "natGLX.h"
#include <glx/Types.h>
#include <functional>

namespace MTL
{
class Drawable;
class MetalLayer;
} //namespace MTL

namespace glx
{

class Drawable;
class Texture;
class CommandBuffer;


/// @brief Wraps a Metal drawable surface for rendering and presentation.
class NATGLX_API Drawable
{
    friend class CommandBuffer;
    friend class MTL::MetalLayer;

private:
    MTL::Drawable* _pDrawable; ///< Pointer to the underlying Metal drawable.

    /// @brief Returns the mutable native drawable pointer.
    /// @return Pointer to MTL::Drawable.
    MTL::Drawable* getNatPtr()
    {
        return _pDrawable;
    }

    /// @brief Returns the const native drawable pointer.
    /// @return Const pointer to MTL::Drawable.
    const MTL::Drawable* getNatPtr() const
    {
        return _pDrawable;
    }
public:
    /// @brief Default constructor; creates an invalid drawable.
    Drawable();

    /// @brief Constructs a drawable from a native Metal drawable pointer.
    /// @param pDrawable Pointer to the MTL::Drawable to wrap.
    Drawable(MTL::Drawable* pDrawable);

    /// @brief Copy constructor.
    /// @param drawable The drawable to copy from.
    Drawable(const Drawable& drawable);

    /// @brief Assignment operator.
    /// @param drawable The drawable to assign from.
    void operator = (const Drawable& drawable);

    /// @brief Type alias for a callback invoked when the drawable has been presented.
    using PresentedHandlerFunction = std::function<void(Drawable*)>;

	friend class MTL::Drawable;
public:

    /// @brief Returns the texture associated with this drawable.
    /// @return The drawable's color Texture.
    Texture texture();

    /// @brief Registers a callback to be invoked after this drawable is presented.
    /// @param function The callback to invoke on presentation.
    void addPresentedHandler(const Drawable::PresentedHandlerFunction& function);

    /// @brief Returns whether this drawable wraps a valid native object.
    /// @return True if the drawable is valid.
    bool isOk() const;

    /// @brief Presents the drawable to the display as soon as possible.
    void present();

    /// @brief Presents the drawable at the specified time.
    /// @param presentationTime The absolute time at which to present.
    void presentAtTime(TimeInterval presentationTime);

    /// @brief Presents the drawable after a minimum duration has elapsed.
    /// @param duration The minimum time interval to wait before presenting.
    void presentAfterMinimumDuration(TimeInterval duration);

    //void           addPresentedHandler(const MTL::DrawablePresentedHandler block);

    /// @brief Returns the time at which this drawable was presented.
    /// @return The presentation timestamp.
    TimeInterval presentedTime() const;

    /// @brief Returns the unique identifier for this drawable.
    /// @return The drawable ID.
    size_t   drawableID() const;


    /// @brief Increments the retain count of the underlying drawable.
    void retain();

    /// @brief Releases the underlying drawable resource.
    void release();
};


} //namespace glx

