// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <fstream>
#include <fo/FileOperations.h>
#include <cnt/StringBuilder.h>

namespace db
{
template <class TSTAT>
class DataMover;

template <class TDB>
class ODBCStatement;

}

namespace dp
{
template <class TSTAT>
class DataMover;

template <class TDB>
class ODBCStatement;

template <class TDB>
class SQLiteStatement;
}

namespace td
{
	//template <int BUFSIZE = 4096>
	class BLOB
	{
		template <class TSTAT> friend class db::DataMover;
		template <class TDB> friend	class db::ODBCStatement;
        template <class TSTAT> friend class dp::DataMover;
        template <class TDB> friend class dp::ODBCStatement;
        template <class TDB> friend class dp::SQLiteStatement;
	public:
		typedef enum { TYPE_TXT = 0, TYPE_PNG, TYPE_JPG, TYPE_PDF, TYPE_DOC, TYPE_BINARY_UNKNOWN } Type;
		typedef enum { SRC_MEMORY_IN = 0, SRC_MEMORY_OUT, SRC_FILE} MemType;
	protected:
		std::ifstream _if;
		std::ofstream _of;
		td::String _inFileName;
		td::String _outFileName;
		td::LUINT8 _nLen;
		td::LUINT8 _nInd;
		td::INT4 _paramID = 0;
		char* _buf = nullptr;
		char* _pMemData = nullptr;
        const char* _pSQLiteData = nullptr; //For blob-out operation (no need to copy twice)
		td::LUINT8 _memLen = 0;
		Type _type;
		MemType _memType;
		td::UINT4 _bufferSize;
		td::LUINT8 _memPos = 0;		

	protected:
		BLOB(const BLOB& ){} //forbidden
		void operator = (const BLOB& ) {} //forbidden

		bool openInFile()
		{
			if (_memType == SRC_MEMORY_IN)
			{
				_memPos = 0;
				_nLen = _memLen;
				return true;
			}			

			if (_inFileName.length() == 0)
				return false;

			if (!fo::openExistingBinaryFile(_if, _inFileName)) 
				return false;

			_if.seekg (0, std::ios::end);
			std::streampos  length = _if.tellg();
			if (length <= 0)
			{
				_if.close();
				return false;
			}
			_nLen = (td::LUINT8) length;
			_if.seekg (0, std::ios::beg);
			return true;
		}

		bool createOutFile()
		{
			if (_memType == SRC_MEMORY_OUT)
			{
				_memPos = 0;
				return true;
			}
				
			return fo::createBinaryFile(_of, _outFileName); 
		}
		//	
	public:	
		

		BLOB(MemType memType = SRC_FILE, td::UINT4 bufferSize = 1024 * 16, Type type = TYPE_BINARY_UNKNOWN)
			: _nLen(0)
			, _buf(nullptr)
			, _nInd(0)
			, _type(type)
			, _bufferSize(bufferSize)
			, _memType(memType)
		{
			if (_bufferSize > 0)
				_buf = new char[_bufferSize];			
		}
		
		
		~BLOB()
		{
			if (_buf)
				delete _buf;
			_if.close();
			_of.close();

			if (_memType == SRC_MEMORY_OUT)
			{
				if (_pMemData)
					delete[] _pMemData;
			}
		}	
		

		inline bool isTxt() const
		{
			return _type == TYPE_TXT;
		}

		inline bool isMem() const
		{
			return _memType <= SRC_MEMORY_OUT;
		}

		Type getType() const
		{
			return _type;
		}

		void close()
		{
			if (_memType == SRC_FILE)			
			{
				_if.close();
				_of.close();
				_pMemData = nullptr;				
			}
		}
		td::LUINT8* getLenPtr()
		{
			return &_nLen;
		}

		td::LUINT8* getIndicatorPtr()
		{
			return &_nInd;
		}

		td::INT4* getParamIDPtr()
		{
			return &_paramID;
		}

		td::LUINT8 length() const
		{
			return _nLen;
		}

		td::UINT4 getBufLen() const
		{
			return _bufferSize;
		}

		char* getBufPtr() const
		{
			return _buf;
		}

		td::UINT4 getChunk()
		{
			if (_memType == SRC_MEMORY_IN)
			{
				assert(_pMemData);

				if (_memPos >= _memLen)
					return 0;
				td::UINT4 toRead = (td::UINT4) (_memLen - _memPos);
				if (toRead > _bufferSize)
					toRead = _bufferSize;
				mem::fastCopy(_buf, _pMemData + _memPos, toRead);
				_memPos += toRead;
				return toRead;
			}
			return (td::UINT4) fo::read(_if, _buf, _bufferSize);
		}


		void reserve(size_t size)
		{		
			assert(_memType == SRC_MEMORY_OUT);

			if (_pMemData)
				delete[] _pMemData;
			_pMemData = new char[size];
			_memLen = size;
			_nLen = _memLen;
		}

        //for sqlite only
        int transferToFile(const void* pData, int nBytes)
        {
            
            fo::createBinaryFile(_of, _outFileName);
            
            if (nBytes <= 0)
            {
                _of.close();
                return 0;
            }

//            if (_memType == SRC_MEMORY_OUT)
//            {
//                assert(_pMemData);
//
//                if (_memPos + nBytes > _memLen)
//                    return 0;
//
//                mem::fastCopy(_pMemData + _memPos, _buf, nBytes);
//                _memPos += nBytes;
//                return nBytes;
//            }
            int toRet = (int) fo::write(_of, (const char*) pData, nBytes);
            _of.close();
            return toRet;
        }
        
		int putChunk(int nBytes)
		{
			if (nBytes <= 0)
				return 0;

			if (_memType == SRC_MEMORY_OUT)
			{
				assert(_pMemData);

				if (_memPos + nBytes > _memLen)
					return 0;
				
				mem::fastCopy(_pMemData + _memPos, _buf, nBytes);
				_memPos += nBytes;
				return nBytes;
			}

			return (int) fo::write(_of, _buf, nBytes);
		}		

		//returns true if file exists
		bool setInFileName(const td::String& fileName)
		{
			assert(_memType == SRC_FILE);
			_of.close();
			_if.close();
			_nLen = 0;
			_inFileName = fileName;
			_outFileName.clean();

			return fo::fileExists(fileName);			
		}

        int setSQLiteData(const char* pData, int nBytes) //for sqlite only
        {
            assert(_memType == SRC_MEMORY_OUT);
            if (pData == nullptr)
            {
                _pSQLiteData = nullptr;
                _memPos = 0;
                _nLen = 0;
                return 0;
            }
                
            if (_memType == SRC_MEMORY_OUT)
            {
                if (_pMemData)
                {
                    if (_memPos + nBytes > _memLen)
                        return 0;
                    
                    mem::fastCopy(_pMemData + _memPos, _buf, nBytes);
                    _memPos += nBytes;
                }
                else
                {
                    _pSQLiteData = pData;
                    _memPos = nBytes;
                    _nLen = nBytes;
                }
                
                return nBytes;
            }
            return 0;
        }
        
		bool setData(const char* pData, size_t len)
		{
			assert(_memType == SRC_MEMORY_IN);
			_of.close();
			_if.close();
			_nLen = 0;
			_inFileName.clean();
			_outFileName.clean();

			_pMemData = const_cast<char*>(pData);
			_memLen = len;
			return true;
		}

		//returns true if file does not exist
		bool setOutFileName(const td::String& fileName)
		{
			assert(_memType == SRC_FILE);

			_of.close();
			_if.close();
			_inFileName.clean();
			_outFileName = fileName;
			_nLen = 0;
			return !(fo::fileExists(fileName));					
		}

		const char* getData() const
		{
			assert(_memType == SRC_MEMORY_OUT);
            
			if (_pSQLiteData != nullptr)
                return _pSQLiteData;
            
			return _pMemData;			
		}

		const td::String& getInFileName() const
		{
			return _inFileName;
		}

		const td::String& getOutFileName() const
		{
			return _outFileName;
		}
	};

	inline DataType getType(const td::BLOB&){ return binary;}	
};
