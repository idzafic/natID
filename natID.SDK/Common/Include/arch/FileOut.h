// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FileOut.h
 *  @brief Archive output class that writes serialized data to a file on disk. */
#pragma once
#include <arch/FileSerializer.h>
#include <arch/ArchiveOut.h>

namespace arch
{
	/// @brief Output archive that serializes data directly to a binary file.
	class FileOut : public ArchiveOut
	{
	protected:
		FileSerializerOut _fOut;		///< Underlying file serializer for output operations.
	public:

		/// @brief Constructs a FileOut with a major version string.
		/// @param strMajorVersion Null-terminated string literal identifying the archive version.
		template <size_t size>
		FileOut(const char(&strMajorVersion)[size])
			: ArchiveOut(strMajorVersion, _fOut)
		{
		}

		/// @brief Constructs a FileOut using an external header descriptor.
		/// @param pExtHeader Pointer to an existing header structure.
		/// @param calcPayloadCRC Whether to compute a CRC over the payload.
		FileOut(const Header* pExtHeader, bool calcPayloadCRC)
			: ArchiveOut(pExtHeader, _fOut, calcPayloadCRC)
		{
		}

		/// @brief Destructor; closes the file if still open.
		~FileOut()
		{
			close();
		}


		/// @brief Opens a file by name for writing and encodes the archive header.
		/// @param fileName Path to the file to create or truncate.
		/// @param bGuarded Whether to write a guarded (protected) header.
		/// @return true if the file was opened and the header written successfully, false otherwise.
		bool open(const char* fileName, bool bGuarded = false)
		{

			try
			{
				if (!_fOut.open(fileName))
					return false;

				return encodeHeader(0, bGuarded);
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

		/// @brief Opens a file by td::String name for writing and encodes the archive header.
		/// @param fileName Path to the file as a td::String.
		/// @param bGuarded Whether to write a guarded (protected) header.
		/// @return true if the file was opened and the header written successfully, false otherwise.
		bool open(const td::String& fileName, bool bGuarded = false)
		{

			try
			{
				if (!_fOut.open(fileName))
					return false;

				return encodeHeader(0, bGuarded);
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

		/// @brief Returns a reference to the underlying output file stream.
		/// @return Reference to the std::ofstream used internally.
		std::ofstream& outFile()
		{
			return _fOut.outFile();
		}

		/// @brief Closes the archive and the underlying file stream.
		void close()
		{
			std::ofstream& f(_fOut.outFile());
			if (f.is_open())
			{
				onClose(f);
				_fOut.close();
			}
		}
	};
}
