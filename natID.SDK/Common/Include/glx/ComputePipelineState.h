// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

	class NATGLX_API ComputePipelineState
	{
	public:
		struct WorkgroupSize {
			uint32_t x = 0;
			uint32_t y = 0;
			uint32_t z = 0;
		};

	private:
		MTL::ComputePipelineState* _pComputePipelineState;
		ComputePipelineState(MTL::ComputePipelineState* pComputePipelineState);
		friend class Device;

	public:
		ComputePipelineState();
		ComputePipelineState(const ComputePipelineState& computePipelineState);
		void operator = (const ComputePipelineState& computePipelineState);
		~ComputePipelineState();

		td::String label() const;

		Device device() const;

		size_t maxTotalThreadsPerThreadgroup() const;

		size_t threadExecutionWidth() const;

		bool isOk() const;

		size_t staticThreadgroupMemoryLength() const;

		const WorkgroupSize& getWorkgroupSize() const;

		void release();

		MTL::ComputePipelineState* getNatPtr()
		{
			return _pComputePipelineState;
		}

		const MTL::ComputePipelineState* getNatPtr() const
		{
			return _pComputePipelineState;
		}
	};

}//namespace glx