// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ComputePipelineState.h
    @brief Provides the ComputePipelineState class representing a compiled GPU compute pipeline. */
#pragma once
#include "natGLX.h"
#include <td/String.h>

namespace MTL
{
class ComputePipelineState;
} //namespace MTL

namespace glx
{
	class Device;

	/// @brief Represents a compiled and linked compute pipeline ready for GPU dispatch.
	class NATGLX_API ComputePipelineState
	{
	public:
		/// @brief Describes the maximum number of threads per threadgroup in each dimension.
		struct WorkgroupSize {
			uint32_t x = 0; ///< Maximum threads in the X dimension.
			uint32_t y = 0; ///< Maximum threads in the Y dimension.
			uint32_t z = 0; ///< Maximum threads in the Z dimension.
		};

	private:
		MTL::ComputePipelineState* _pComputePipelineState; ///< Pointer to the underlying Metal compute pipeline state.
		/// @brief Constructs a ComputePipelineState wrapping an existing Metal pipeline state.
		/// @param pComputePipelineState Pointer to the native MTL::ComputePipelineState.
		ComputePipelineState(MTL::ComputePipelineState* pComputePipelineState);
		friend class Device;

	public:
		/// @brief Default constructor. Creates an empty, uninitialized ComputePipelineState.
		ComputePipelineState();
		/// @brief Copy constructor. Shares the underlying Metal pipeline state.
		/// @param computePipelineState The ComputePipelineState to copy from.
		ComputePipelineState(const ComputePipelineState& computePipelineState);
		/// @brief Copy-assigns another ComputePipelineState to this one.
		/// @param computePipelineState The ComputePipelineState to assign from.
		void operator = (const ComputePipelineState& computePipelineState);
		/// @brief Destructor. Releases the reference to the Metal pipeline state.
		~ComputePipelineState();

		/// @brief Returns the debug label of this pipeline state.
		/// @return The label string.
		td::String label() const;

		/// @brief Returns the device that created this pipeline state.
		/// @return The associated Device.
		Device device() const;

		/// @brief Returns the maximum number of threads allowed per threadgroup.
		/// @return The maximum total thread count per threadgroup.
		size_t maxTotalThreadsPerThreadgroup() const;

		/// @brief Returns the number of threads that execute in a single SIMD group (warp).
		/// @return The thread execution width.
		size_t threadExecutionWidth() const;

		/// @brief Checks whether the pipeline state is valid and successfully compiled.
		/// @return True if the pipeline state is valid, false otherwise.
		bool isOk() const;

		/// @brief Returns the amount of statically allocated threadgroup memory in bytes.
		/// @return The static threadgroup memory length in bytes.
		size_t staticThreadgroupMemoryLength() const;

		/// @brief Returns the recommended workgroup size for this pipeline.
		/// @return A constant reference to the WorkgroupSize describing the optimal dispatch dimensions.
		const WorkgroupSize& getWorkgroupSize() const;

		/// @brief Releases the underlying native resource.
		void release();

		/// @brief Returns a mutable pointer to the underlying Metal pipeline state.
		/// @return Pointer to the native MTL::ComputePipelineState.
		MTL::ComputePipelineState* getNatPtr()
		{
			return _pComputePipelineState;
		}

		/// @brief Returns a const pointer to the underlying Metal pipeline state.
		/// @return Const pointer to the native MTL::ComputePipelineState.
		const MTL::ComputePipelineState* getNatPtr() const
		{
			return _pComputePipelineState;
		}
	};

}//namespace glx
