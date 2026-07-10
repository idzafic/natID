// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IBufferProvider.h
    @brief Abstract interface for buffer management used during binary serialization. */
#pragma once
#include <td/Types.h>
#include <cnt/List.h>

namespace arch
{
	/// @brief Abstract interface for providing and managing memory buffers used in archive streaming.
	class IBufferProvider
	{
	public:
		/// @brief Retrieves a buffer for use by the archive.
		/// @param buffSize Output parameter that receives the size of the returned buffer.
		/// @return Pointer to the allocated buffer.
		virtual char* get(td::UINT4& buffSize) = 0;

		/// @brief Called when the archive has finished writing to the buffer.
		/// @param buff Pointer to the buffer that was used.
		/// @param headerSize Size of the header portion in bytes.
		/// @param payloadSize Size of the payload portion in bytes.
		/// @param bError True if an error occurred during processing.
		virtual void completed(const char* buff, td::UINT4 headerSize, td::UINT4 payloadSize, bool bError) = 0;

		/// @brief Resets the buffer provider to its initial state.
		virtual void reset() = 0;

		/// @brief Sets whether the provider should operate in read mode.
		/// @param bRead True to enable read mode, false for write mode.
		virtual void setReadMode(bool bRead = true) = 0;

		/// @brief Releases all resources held by this buffer provider.
        virtual void release() = 0;

		/// @brief Returns the list of all managed buffers.
		/// @return Reference to the singly-linked list of buffer pointers.
		virtual cnt::ListSL< char* >& getBuffers() = 0;
	};
};
