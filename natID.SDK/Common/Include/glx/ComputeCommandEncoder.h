// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ComputeCommandEncoder.h
    @brief Provides the ComputeCommandEncoder class for encoding GPU compute dispatch commands. */
#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <glx/Buffer.h>
#include <td/String.h>

namespace MTL
{
class ComputeCommandEncoder;
} //namespace MTL

namespace glx {

	class CommandBuffer;
	class Texture;
	class SamplerState;
	class ComputePipelineState;
	class Device;

	/// @brief Encodes compute shader dispatch commands for execution on the GPU.
	class NATGLX_API ComputeCommandEncoder
	{
		friend class CommandBuffer;
	private:
		MTL::ComputeCommandEncoder* _pComputeCommandEncoder; ///< Pointer to the underlying Metal compute command encoder.
		/// @brief Constructs a ComputeCommandEncoder wrapping an existing Metal encoder.
		/// @param pComputeCommandEncoder Pointer to the native MTL::ComputeCommandEncoder.
		ComputeCommandEncoder(MTL::ComputeCommandEncoder* pComputeCommandEncoder);
	public:
		/// @brief Default constructor. Creates an empty ComputeCommandEncoder.
		ComputeCommandEncoder();
		/// @brief Copy-assigns another ComputeCommandEncoder to this one.
		/// @param computeCommandEncoder The source encoder to assign from.
		void operator = (const ComputeCommandEncoder& computeCommandEncoder);

		/// @brief Sets the compute pipeline state for subsequent dispatch calls.
		/// @param state The ComputePipelineState to use for dispatched compute kernels.
		void setComputePipelineState(const ComputePipelineState& state);

		/// @brief Binds a buffer to the specified index in the compute shader's buffer argument table.
		/// @param buffer The buffer to bind.
		/// @param offset Byte offset into the buffer.
		/// @param index The buffer argument table index to bind to.
		void setBuffer(const Buffer& buffer, size_t offset, size_t index);

		/// @brief Binds an array of buffers at a contiguous range of argument table indices.
		/// @param buffers Pointer to an array of Buffer objects to bind.
		/// @param offsets Pointer to an array of byte offsets, one per buffer.
		/// @param range The range of argument table indices to bind to.
		void setBuffers(const Buffer* buffers, const size_t* offsets, const Buffer::Range& range);

		/// @brief Binds a texture to the specified index in the compute shader's texture argument table.
		/// @param texture The texture to bind.
		/// @param index The texture argument table index to bind to.
		void setTexture(const Texture& texture, size_t index);

		/// @brief Binds an array of textures at a contiguous range of argument table indices.
		/// @param textures Pointer to an array of Texture objects to bind.
		/// @param range The range of argument table indices to bind to.
		void setTextures(const Texture* textures, const Buffer::Range& range);

		/// @brief Binds a sampler state to the specified index in the compute shader's sampler argument table.
		/// @param sampler Pointer to the SamplerState to bind.
		/// @param index The sampler argument table index to bind to.
		void setSamplerState(const SamplerState* sampler, size_t index);

		/// @brief Binds an array of sampler states at a contiguous range of argument table indices.
		/// @param samplers Pointer to an array of pointers to SamplerState objects.
		/// @param range The range of argument table indices to bind to.
		void setSamplerStates(const SamplerState* const* samplers, const Buffer::Range& range);

		/// @brief Sets raw bytes as inline data for the compute shader at the specified index.
		/// @param bytes Pointer to the data to set.
		/// @param length Number of bytes to set.
		/// @param index The argument table index to bind to.
		void setBytes(const void* bytes, size_t length, size_t index);

		/// @brief Dispatches threadgroups across a 3D grid of thread groups.
		/// @param threadgroupsPerGrid The number of threadgroups in each dimension of the grid.
		/// @param threadsPerThreadgroup The number of threads in each dimension of a single threadgroup.
		void dispatchThreadgroups(const Volume<td::UINT4>& threadgroupsPerGrid, const Volume<td::UINT4>& threadsPerThreadgroup);

		/// @brief Dispatches threads across a 3D grid, allowing the GPU to partition into threadgroups.
		/// @param threadsPerGrid The total number of threads in each dimension of the grid.
		/// @param threadsPerThreadgroup The number of threads in each dimension of a single threadgroup.
		void dispatchThreads(const Volume<td::UINT4>& threadsPerGrid, const Volume<td::UINT4>& threadsPerThreadgroup);

		/// @brief Inserts a memory barrier for the specified memory scope.
		/// @param scope Bitmask specifying the scope of the memory barrier.
		void memoryBarrier(uint32_t scope);
		/// @brief Inserts a memory barrier ensuring visibility of writes to the given buffer.
		/// @param buffer The buffer whose writes should be made visible.
		void memoryBarrier(const Buffer& buffer);
		/// @brief Inserts a memory barrier ensuring visibility of writes to the given texture.
		/// @param texture The texture whose writes should be made visible.
		void memoryBarrier(const Texture& texture);

		/// @brief Sets the debug label for this encoder.
		/// @param label The label string to assign.
		void setLabel(const td::String& label);
		/// @brief Returns the debug label of this encoder.
		/// @return The current label string.
		td::String label() const;

		/// @brief Pushes a named debug group onto the encoder's debug group stack.
		/// @param string The name of the debug group.
		void pushDebugGroup(const char* string);
		/// @brief Pops the current debug group from the encoder's debug group stack.
		void popDebugGroup();

		/// @brief Returns the device associated with this encoder.
		/// @return The Device used to create this encoder.
		Device device() const;

		/// @brief Finalizes encoding and signals that no more commands will be added.
		void endEncoding();

		/// @brief Releases the underlying native resource.
		void release();
	};

}// namespace glx
