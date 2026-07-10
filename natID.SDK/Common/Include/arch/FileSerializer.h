// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FileSerializer.h
 *  @brief File-based binary serializer classes for reading and writing archive data. */
#pragma once
#include <arch/Archive.h>
#include <fo/FileOperations.h>
#include <crpt/CRC32.h>

namespace arch
{
	/// @brief Base template for file serializers that wrap a stream object.
	/// @tparam TSTREAM The underlying stream type (std::ifstream or std::ofstream).
	template <class TSTREAM>
	class FileSerializer : public IBinSerializer
	{
	protected:
		TSTREAM _f; ///< The underlying file stream.
	public:
		/// @brief Destructor; closes the stream if open.
		~FileSerializer()
		{
			close();
		}

		/// @brief Write bytes to the stream (not implemented in base class).
		/// @param bytes Pointer to data to write.
		/// @param nBytes Number of bytes to write.
		virtual void write(const char* bytes, td::UINT4 nBytes)
		{
			assert(false);
		}

		//virtual void markAsLastPayload()
		//{
		//}

		/// @brief Read bytes from the stream (not implemented in base class).
		/// @param bytes Buffer to read data into.
		/// @param nBytes Number of bytes to read.
		virtual void read(char* bytes, td::UINT4 nBytes)
		{
			assert(false);
		}

		//virtual void setPayloadSize(td::UINT4 payloadSize)
		//{
		//}
		//virtual void setMask(td::UINT4 mask)
		//{
		//}

		/// @brief Closes the file stream if it is currently open.
		virtual void close()
		{
			if (_f.is_open())
				_f.close();
		}
		//virtual void setCompression(char compressionLevel)
		//{
		//}

		/// @brief Releases resources; closes the file stream.
		virtual void release()
		{
			if (_f.is_open())
				_f.close();
			//delete this;
		}

		/// @brief Returns the fixed size of the archive header.
		/// @return Header size in bytes (always 16).
		virtual td::UINT4 getHeaderSize() const
		{
			return 16;
		}

		/// @brief Checks whether the underlying stream is open.
		/// @return true if the stream is open, false otherwise.
		bool is_open()const
		{
			return _f.is_open();
		}

		/// @brief Seeks to a given position in the stream.
		/// @param pos Byte offset to seek to.
		/// @return true on success, false if not supported.
		virtual bool goTo(td::LUINT8 pos)
		{
			return false;
		}
	};

	/// @brief File serializer specialised for writing (output) archive files.
	class FileSerializerOut : public FileSerializer<std::ofstream>
	{
	protected:
		Header* _pExtHeader; ///< Optional pointer to an external header structure.

	public:
		/// @brief Default constructor; configures the stream to throw on errors.
		FileSerializerOut()
			: _pExtHeader(nullptr)
		{
			_f.exceptions( std::ifstream::failbit | std::ifstream::badbit);
		}

		/// @brief Opens a file for writing by C-string path.
		/// @param fileName Null-terminated path to the file.
		/// @return true if the file was opened successfully.
		bool open(const char* fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_TRUNCATE);
		}

		/// @brief Opens a file for writing by td::String path.
		/// @param fileName Path to the file as a td::String.
		/// @return true if the file was opened successfully.
		bool open(const td::String& fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_TRUNCATE);
		}

		/// @brief Writes raw bytes to the output file.
		/// @param bytes Pointer to the data buffer.
		/// @param nBytes Number of bytes to write.
		virtual void write(const char* bytes, td::UINT4 nBytes)
		{
			_f.write(bytes, nBytes);
		}

		/// @brief Returns a reference to the underlying output file stream.
		/// @return Reference to the internal std::ofstream.
		std::ofstream& outFile()
		{
			return _f;
		}
	};

	/// @brief File serializer specialised for reading (input) archive files.
	class FileSerializerIn : public FileSerializer<std::ifstream>
	{
	public:
		/// @brief Default constructor; configures the stream to throw on errors including EOF.
		FileSerializerIn()
		{
			_f.exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
		}

		/// @brief Opens an existing file for reading by C-string path.
		/// @param fileName Null-terminated path to the file.
		/// @return true if the file was opened successfully.
		bool open(const char* fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_EXISTING);
		}

		/// @brief Opens an existing file for reading by td::String path.
		/// @param fileName Path to the file as a td::String.
		/// @return true if the file was opened successfully.
        bool open(const td::String& fileName)
        {
            return fo::openFile(_f, fileName, FO_BINARY_OPEN_EXISTING);
        }

		/// @brief Reads raw bytes from the input file.
		/// @param bytes Buffer to read data into.
		/// @param nBytes Number of bytes to read.
		virtual void read(char* bytes, td::UINT4 nBytes)
		{
			_f.read(bytes, nBytes);
		}

		/// @brief Returns a reference to the underlying input file stream.
		/// @return Reference to the internal std::ifstream.
		std::ifstream& inFile()
		{
			return _f;
		}

		/// @brief Seeks to an absolute byte position within the input file.
		/// @param pos The byte offset to seek to.
		/// @return true on success.
		virtual bool goTo(td::LUINT8 pos)
		{
			_f.seekg(pos);
			return true;
		}
	};
}
