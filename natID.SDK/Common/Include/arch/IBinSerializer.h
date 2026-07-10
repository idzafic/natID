// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IBinSerializer.h
    @brief Abstract interface for binary serialization operations. */
#pragma once
#include "td/Types.h"

namespace arch
{
	/// @brief Abstract interface that defines operations for reading from and writing to a binary stream.
	class IBinSerializer
	{
	public:
		/// @brief Opens the serializer with the given name or path.
		/// @param serializerName Name or path identifying the serializer target.
		/// @return True if opened successfully, false otherwise.
		virtual bool open(const char* serializerName) = 0;

		/// @brief Writes a block of bytes to the binary stream.
		/// @param bytes Pointer to the data buffer to write.
		/// @param nBytes Number of bytes to write.
		virtual void write(const char* bytes, td::UINT4 nBytes) = 0;

		/// @brief Reads a block of bytes from the binary stream.
		/// @param bytes Pointer to the buffer that receives the data.
		/// @param nBytes Number of bytes to read.
		virtual void read(char* bytes, td::UINT4 nBytes) = 0;

		/// @brief Closes the binary stream.
		virtual void close() = 0;

		/// @brief Releases resources associated with this serializer.
		virtual void release() = 0;

		/// @brief Moves the stream position to the specified byte offset.
		/// @param pos Target byte position in the stream.
		/// @return True if seeking succeeded, false otherwise.
		virtual bool goTo(td::LUINT8 pos) = 0;
		//virtual void setCompression(char compressionLevel) = 0;
		//virtual td::UINT4 getHeaderSize() const = 0;
		//virtual void setPayloadSize(td::UINT4 payloadSize) = 0;
		//virtual void markAsLastPayload() = 0;
		//virtual void setMask(td::UINT4 mask) = 0;

		/// @brief Virtual destructor.
		virtual ~IBinSerializer(){};
	};
};
