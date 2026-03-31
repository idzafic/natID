// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <arch/Archive.h>
#include <fo/FileOperations.h>
#include <crpt/CRC32.h>

namespace arch
{
	template <class TSTREAM>
	class FileSerializer : public IBinSerializer
	{
	protected:
		TSTREAM _f;
	public:
		~FileSerializer()
		{
			close();
		}

		virtual void write(const char* bytes, td::UINT4 nBytes)
		{		
			assert(false);
		}

		//virtual void markAsLastPayload()
		//{
		//}

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
		virtual void close()
		{
			if (_f.is_open())
				_f.close();
		}
		//virtual void setCompression(char compressionLevel)
		//{			
		//}
		virtual void release()
		{
			if (_f.is_open())
				_f.close();
			//delete this;
		}

		virtual td::UINT4 getHeaderSize() const
		{
			return 16;
		}

		bool is_open()const
		{
			return _f.is_open();
		}

		virtual bool goTo(td::LUINT8 pos)
		{			
			return false;
		}
	};

	class FileSerializerOut : public FileSerializer<std::ofstream>
	{
	protected:
		Header* _pExtHeader;

	public:
		FileSerializerOut()
			: _pExtHeader(nullptr)
		{
			_f.exceptions( std::ifstream::failbit | std::ifstream::badbit);
		}

		bool open(const char* fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_TRUNCATE);			
		}

		bool open(const td::String& fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_TRUNCATE);
		}

		virtual void write(const char* bytes, td::UINT4 nBytes)
		{
			_f.write(bytes, nBytes);
		}		
		
		std::ofstream& outFile()
		{
			return _f;
		}
	};

	class FileSerializerIn : public FileSerializer<std::ifstream>
	{
	public:
		FileSerializerIn()
		{
			_f.exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
		}

		bool open(const char* fileName)
		{
			return fo::openFile(_f, fileName, FO_BINARY_OPEN_EXISTING);	
		}
        
        bool open(const td::String& fileName)
        {
            return fo::openFile(_f, fileName, FO_BINARY_OPEN_EXISTING);
        }

		virtual void read(char* bytes, td::UINT4 nBytes)
		{
			_f.read(bytes, nBytes);
		}

		std::ifstream& inFile()
		{
			return _f;
		}

		virtual bool goTo(td::LUINT8 pos)
		{
			_f.seekg(pos);
			return true;
		}
	};
}
