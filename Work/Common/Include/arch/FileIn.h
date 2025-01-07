// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <arch/FileSerializer.h>
#include <arch/ArchiveIn.h>

namespace arch
{
	class FileIn : public ArchiveIn
	{
	protected:
		FileSerializerIn _fIn;
	public:
		
		FileIn() : ArchiveIn(_fIn) { }

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

		std::ifstream& inFile()
		{
			return _fIn.inFile();
		}

	};
}
