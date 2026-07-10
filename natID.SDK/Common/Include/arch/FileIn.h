// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FileIn.h
 * @brief File-backed binary input archive for deserializing data from a file.
 */
#pragma once
#include <arch/FileSerializer.h>
#include <arch/ArchiveIn.h>

namespace arch
{
	/// @brief Archive that reads serialized binary data directly from a file.
	class FileIn : public ArchiveIn
	{
	protected:
		FileSerializerIn _fIn; ///< File serializer that wraps the input file stream.
	public:

		/// @brief Default constructor; initializes the archive with the internal file serializer.
		FileIn() : ArchiveIn(_fIn) { }

		/// @brief Opens the specified file and decodes the archive header.
		/// @param fileName Path to the file to open for reading.
		/// @return True if the file was opened and the header decoded successfully, false otherwise.
		bool open(const char* fileName)
		{

			try
			{
				if (!_fIn.open(fileName))
					return false;

				return decodeHeader();
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

		/// @brief Returns a reference to the underlying input file stream.
		/// @return Reference to the std::ifstream used for reading.
		std::ifstream& inFile()
		{
			return _fIn.inFile();
		}

	};
}
