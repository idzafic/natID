// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

	class NATGLX_API ComputeCommandEncoder
	{
		friend class CommandBuffer;
	private:
		MTL::ComputeCommandEncoder* _pComputeCommandEncoder;
		ComputeCommandEncoder(MTL::ComputeCommandEncoder* pComputeCommandEncoder);
	public:
		ComputeCommandEncoder();
		void operator = (const ComputeCommandEncoder& computeCommandEncoder);

		void setComputePipelineState(const ComputePipelineState& state);

		void setBuffer(const Buffer& buffer, size_t offset, size_t index);

		void setBuffers(const Buffer* buffers, const size_t* offsets, const Buffer::Range& range);

		void setTexture(const Texture& texture, size_t index);

		void setTextures(const Texture* textures, const Buffer::Range& range);

		void setSamplerState(const SamplerState* sampler, size_t index);

		void setSamplerStates(const SamplerState* const* samplers, const Buffer::Range& range);

		void setBytes(const void* bytes, size_t length, size_t index);

		void dispatchThreadgroups(const Volume<td::UINT4>& threadgroupsPerGrid, const Volume<td::UINT4>& threadsPerThreadgroup);

		void dispatchThreads(const Volume<td::UINT4>& threadsPerGrid, const Volume<td::UINT4>& threadsPerThreadgroup);

		void memoryBarrier(uint32_t scope);
		void memoryBarrier(const Buffer& buffer);
		void memoryBarrier(const Texture& texture);

		void setLabel(const td::String& label);
		td::String label() const;

		void pushDebugGroup(const char* string);
		void popDebugGroup();

		Device device() const;

		void endEncoding();

		void release();
	};

}// namespace glx