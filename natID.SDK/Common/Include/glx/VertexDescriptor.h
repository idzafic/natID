// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VertexDescriptor.h
    @brief Declares classes for describing vertex attribute layouts and buffer bindings. */
#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <td/String.h>
#include <glx/RenderPipeline.h>

namespace MTL
{
    class VertexDescriptor;
    class VertexAttributeDescriptor;
    class VertexBufferLayoutDescriptor;
    class VertexAttributeDescriptorArray;
    class VertexBufferLayoutDescriptorArray;
}

namespace glx
{
    /// @brief Describes a single vertex attribute (its format, buffer offset and buffer binding index).
    class NATGLX_API VertexAttribute
    {
        friend class VertexAttributes;

    private:
        MTL::VertexAttributeDescriptor* _pVertexAttributeDescriptor; ///< Pointer to the underlying native vertex attribute descriptor.

        /// @brief Constructs a VertexAttribute from a native descriptor pointer.
        /// @param pVertexAttributeDescriptor Pointer to an existing MTL::VertexAttributeDescriptor.
        VertexAttribute(MTL::VertexAttributeDescriptor* pVertexAttributeDescriptor);

    public:
        /// @brief Default constructor; creates an empty vertex attribute.
        VertexAttribute();

        /// @brief Copy constructor.
        /// @param other The source attribute to copy from.
        VertexAttribute(const VertexAttribute& other);

        /// @brief Copy-assignment operator.
        /// @param other The source attribute to copy from.
        /// @return Reference to this attribute.
        VertexAttribute& operator=(const VertexAttribute& other);

        /// @brief Trivial destructor.
        ~VertexAttribute() = default;

        /// @brief Returns the data format of the vertex attribute.
        /// @return The VertexFormat describing the attribute's data type and component count.
        VertexFormat format() const;

        /// @brief Sets the data format of the vertex attribute.
        /// @param format The VertexFormat to apply.
        void setFormat(VertexFormat format);

        /// @brief Returns the byte offset of the attribute within a vertex buffer element.
        /// @return The offset in bytes.
        uint32_t offset() const;

        /// @brief Sets the byte offset of the attribute within a vertex buffer element.
        /// @param offset The offset in bytes.
        void setOffset(uint32_t offset);

        /// @brief Returns the index of the vertex buffer that supplies this attribute's data.
        /// @return The buffer index.
        uint32_t bufferIndex() const;

        /// @brief Sets the index of the vertex buffer that supplies this attribute's data.
        /// @param index The buffer index.
        void setBufferIndex(uint32_t index);
    };

    /// @brief Describes the layout of a vertex buffer (stride and stepping behaviour).
    class NATGLX_API VertexBufferLayout
    {
        friend class VertexBufferLayouts;

    private:
        MTL::VertexBufferLayoutDescriptor* _pVertexBufferLayoutDescriptor; ///< Pointer to the underlying native vertex buffer layout descriptor.

        /// @brief Constructs a VertexBufferLayout from a native descriptor pointer.
        /// @param pVertexBufferLayoutDescriptor Pointer to an existing MTL::VertexBufferLayoutDescriptor.
        VertexBufferLayout(MTL::VertexBufferLayoutDescriptor* pVertexBufferLayoutDescriptor);

    public:
        /// @brief Default constructor; creates an empty buffer layout.
        VertexBufferLayout();

        /// @brief Copy constructor.
        /// @param other The source layout to copy from.
        VertexBufferLayout(const VertexBufferLayout& other);

        /// @brief Copy-assignment operator.
        /// @param other The source layout to copy from.
        /// @return Reference to this layout.
        VertexBufferLayout& operator=(const VertexBufferLayout& other);

        /// @brief Trivial destructor.
        ~VertexBufferLayout() = default;

        /// @brief Returns the stride (in bytes) between consecutive vertices in the buffer.
        /// @return The stride in bytes.
        uint32_t stride() const;

        /// @brief Sets the stride (in bytes) between consecutive vertices in the buffer.
        /// @param stride The stride in bytes.
        void setStride(uint32_t stride);

        /// @brief Returns the step function controlling how vertex data advances per vertex or instance.
        /// @return The VertexStepFunction setting.
        VertexStepFunction stepFunction() const;

        /// @brief Sets the step function controlling how vertex data advances.
        /// @param function The VertexStepFunction to apply.
        void setStepFunction(VertexStepFunction function);

        /// @brief Returns the step rate used together with the step function.
        /// @return The step rate value.
        uint32_t stepRate() const;

        /// @brief Sets the step rate used together with the step function.
        /// @param rate The step rate value.
        void setStepRate(uint32_t rate);
    };

    /// @brief Provides indexed access to a collection of VertexAttribute descriptors.
    class NATGLX_API VertexAttributes
    {
        friend class VertexDescriptor;

    private:
        MTL::VertexAttributeDescriptorArray* _pArray; ///< Pointer to the underlying native attribute descriptor array.

        /// @brief Constructs a VertexAttributes collection from a native array pointer.
        /// @param pArray Pointer to an existing MTL::VertexAttributeDescriptorArray.
        VertexAttributes(MTL::VertexAttributeDescriptorArray* pArray);

    public:
        /// @brief Default constructor; creates an empty attributes collection.
        VertexAttributes();

        /// @brief Copy constructor.
        /// @param other The source collection to copy from.
        VertexAttributes(const VertexAttributes& other);

        /// @brief Copy-assignment operator.
        /// @param other The source collection to copy from.
        /// @return Reference to this collection.
        VertexAttributes& operator=(const VertexAttributes& other);

        /// @brief Trivial destructor.
        ~VertexAttributes() = default;

        /// @brief Returns the vertex attribute at the given index.
        /// @param index Zero-based index into the attribute array.
        /// @return The VertexAttribute at the specified index.
        VertexAttribute object(size_t index);

        /// @brief Returns the vertex attribute at the given index using the subscript operator.
        /// @param index Zero-based index into the attribute array.
        /// @return The VertexAttribute at the specified index.
        VertexAttribute operator[](size_t index);
    };

    /// @brief Provides indexed access to a collection of VertexBufferLayout descriptors.
    class NATGLX_API VertexBufferLayouts
    {
        friend class VertexDescriptor;

    private:
        MTL::VertexBufferLayoutDescriptorArray* _pArray; ///< Pointer to the underlying native buffer layout descriptor array.

        /// @brief Constructs a VertexBufferLayouts collection from a native array pointer.
        /// @param pArray Pointer to an existing MTL::VertexBufferLayoutDescriptorArray.
        VertexBufferLayouts(MTL::VertexBufferLayoutDescriptorArray* pArray);

    public:
        /// @brief Default constructor; creates an empty layouts collection.
        VertexBufferLayouts();

        /// @brief Copy constructor.
        /// @param other The source collection to copy from.
        VertexBufferLayouts(const VertexBufferLayouts& other);

        /// @brief Copy-assignment operator.
        /// @param other The source collection to copy from.
        /// @return Reference to this collection.
        VertexBufferLayouts& operator=(const VertexBufferLayouts& other);

        /// @brief Trivial destructor.
        ~VertexBufferLayouts() = default;

        /// @brief Returns the buffer layout at the given index.
        /// @param index Zero-based index into the layout array.
        /// @return The VertexBufferLayout at the specified index.
        VertexBufferLayout object(size_t index);

        /// @brief Returns the buffer layout at the given index using the subscript operator.
        /// @param index Zero-based index into the layout array.
        /// @return The VertexBufferLayout at the specified index.
        VertexBufferLayout operator[](size_t index);
    };

    /// @brief Aggregates all vertex attribute and buffer layout descriptors needed by a render pipeline.
    class NATGLX_API VertexDescriptor
    {
        friend class RenderPipeline::Descriptor;

    private:
        MTL::VertexDescriptor* _pVertexDescriptor; ///< Pointer to the underlying native Metal vertex descriptor.

        /// @brief Constructs a VertexDescriptor from a native Metal descriptor pointer.
        /// @param pVertexDescriptor Pointer to an existing MTL::VertexDescriptor.
        VertexDescriptor(MTL::VertexDescriptor* pVertexDescriptor);

    public:
        /// @brief Default constructor; creates an empty vertex descriptor.
        VertexDescriptor();

        /// @brief Copy constructor.
        /// @param other The source descriptor to copy from.
        VertexDescriptor(const VertexDescriptor& other);

        /// @brief Copy-assignment operator.
        /// @param other The source descriptor to copy from.
        /// @return Reference to this descriptor.
        VertexDescriptor& operator=(const VertexDescriptor& other);

        /// @brief Destructor; releases the underlying native descriptor.
        ~VertexDescriptor();

        /// @brief Allocates a new, uninitialized vertex descriptor.
        /// @return A newly allocated VertexDescriptor.
        static VertexDescriptor alloc();

        /// @brief Initialises and returns this vertex descriptor.
        /// @return This descriptor after initialisation.
        VertexDescriptor init();

        /// @brief Returns the collection of vertex attribute descriptors.
        /// @return A VertexAttributes accessor for this descriptor.
        VertexAttributes attributes();

        /// @brief Returns the collection of vertex buffer layout descriptors.
        /// @return A VertexBufferLayouts accessor for this descriptor.
        VertexBufferLayouts layouts();

        /// @brief Resets all vertex attributes and buffer layouts to default values.
        void reset();

        /// @brief Checks whether the vertex descriptor holds a valid native pointer.
        /// @return True if the descriptor is valid.
        bool isOk() const;

        /// @brief Releases the underlying native vertex descriptor resource.
        void release();

        /// @brief Returns a mutable pointer to the underlying native vertex descriptor.
        /// @return Pointer to MTL::VertexDescriptor.
        MTL::VertexDescriptor* getNatPtr();

        /// @brief Returns a const pointer to the underlying native vertex descriptor.
        /// @return Const pointer to MTL::VertexDescriptor.
        const MTL::VertexDescriptor* getNatPtr() const;
    };

} // namespace glx
