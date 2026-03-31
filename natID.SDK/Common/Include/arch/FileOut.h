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
#include <arch/ArchiveOut.h>

namespace arch
{
	class FileOut : public ArchiveOut
	{
	protected:
		FileSerializerOut _fOut;		
	public:

		template <size_t size>
		FileOut(const char(&strMajorVersion)[size])
			: ArchiveOut(strMajorVersion, _fOut) 			
		{ 		
		}

		FileOut(const Header* pExtHeader, bool calcPayloadCRC)
			: ArchiveOut(pExtHeader, _fOut, calcPayloadCRC)
		{
		}

		~FileOut()			
		{
			close();
		}


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

		std::ofstream& outFile()
		{
			return _fOut.outFile();
		}
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
