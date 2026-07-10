// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BLOB.h
    @brief Defines the BLOB class for handling binary large object data from files or memory buffers. */
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
	/// @brief Represents a Binary Large Object (BLOB) supporting file-based and in-memory data sources and destinations.
	class BLOB
	{
		template <class TSTAT> friend class db::DataMover;
		template <class TDB> friend	class db::ODBCStatement;
        template <class TSTAT> friend class dp::DataMover;
        template <class TDB> friend class dp::ODBCStatement;
        template <class TDB> friend class dp::SQLiteStatement;
	public:
		/// @brief Specifies the content type of the BLOB data.
		typedef enum {
            TYPE_TXT = 0,         ///< Plain text content.
            TYPE_PNG,             ///< PNG image content.
            TYPE_JPG,             ///< JPEG image content.
            TYPE_PDF,             ///< PDF document content.
            TYPE_DOC,             ///< Word document content.
            TYPE_BINARY_UNKNOWN   ///< Unknown binary content.
        } Type;

		/// @brief Specifies whether the BLOB operates on memory or file storage.
		typedef enum {
            SRC_MEMORY_IN = 0,  ///< BLOB reads from an in-memory buffer.
            SRC_MEMORY_OUT,     ///< BLOB writes to an in-memory buffer.
            SRC_FILE            ///< BLOB operates on file-based storage.
        } MemType;

	protected:
		std::ifstream _if;              ///< Input file stream used for reading BLOB data from a file.
		std::ofstream _of;              ///< Output file stream used for writing BLOB data to a file.
		td::String _inFileName;         ///< Path to the input file from which BLOB data is read.
		td::String _outFileName;        ///< Path to the output file to which BLOB data is written.
		td::LUINT8 _nLen;               ///< Total length of the BLOB data in bytes.
		td::LUINT8 _nInd;               ///< Indicator value used by database drivers for NULL detection.
		td::INT4 _paramID = 0;          ///< Parameter identifier used when binding BLOB to a SQL statement.
		char* _buf = nullptr;           ///< Internal transfer buffer for chunked read/write operations.
		char* _pMemData = nullptr;      ///< Pointer to in-memory data buffer for memory-based operations.
        const char* _pSQLiteData = nullptr; //For blob-out operation (no need to copy twice)
		td::LUINT8 _memLen = 0;         ///< Total length of the in-memory data buffer.
		Type _type;                     ///< Content type of the BLOB.
		MemType _memType;               ///< Storage type indicating whether BLOB uses memory or file I/O.
		td::UINT4 _bufferSize;          ///< Size of the internal transfer buffer in bytes.
		td::LUINT8 _memPos = 0;         ///< Current read/write position within the in-memory buffer.

	protected:
		BLOB(const BLOB& ){} //forbidden
		void operator = (const BLOB& ) {} //forbidden

		/// @brief Opens the input source (file or memory) for reading.
		/// @return True if the input source was successfully opened, false otherwise.
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

		/// @brief Creates or prepares the output destination (file or memory) for writing.
		/// @return True if the output destination was successfully prepared, false otherwise.
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


		/// @brief Constructs a BLOB object with the specified storage type, buffer size, and content type.
		/// @param memType The storage type (memory input, memory output, or file).
		/// @param bufferSize The size of the internal transfer buffer in bytes.
		/// @param type The content type of the BLOB data.
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

		/// @brief Destructor that releases all allocated buffers and closes open file streams.
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


		/// @brief Checks whether the BLOB content type is plain text.
		/// @return True if the content type is TYPE_TXT, false otherwise.
		inline bool isTxt() const
		{
			return _type == TYPE_TXT;
		}

		/// @brief Checks whether the BLOB is operating on in-memory data.
		/// @return True if the BLOB uses a memory-based source or destination.
		inline bool isMem() const
		{
			return _memType <= SRC_MEMORY_OUT;
		}

		/// @brief Returns the content type of the BLOB.
		/// @return The Type enum value indicating the BLOB content type.
		Type getType() const
		{
			return _type;
		}

		/// @brief Closes open file streams and resets memory data pointer.
		void close()
		{
			if (_memType == SRC_FILE)
			{
				_if.close();
				_of.close();
				_pMemData = nullptr;
			}
		}

		/// @brief Returns a pointer to the internal BLOB length field.
		/// @return Pointer to the length variable used by database drivers.
		td::LUINT8* getLenPtr()
		{
			return &_nLen;
		}

		/// @brief Returns a pointer to the internal indicator field.
		/// @return Pointer to the indicator variable used by database drivers for NULL detection.
		td::LUINT8* getIndicatorPtr()
		{
			return &_nInd;
		}

		/// @brief Returns a pointer to the parameter ID field.
		/// @return Pointer to the parameter identifier used for SQL binding.
		td::INT4* getParamIDPtr()
		{
			return &_paramID;
		}

		/// @brief Returns the total length of the BLOB data.
		/// @return The length of the BLOB in bytes.
		td::LUINT8 length() const
		{
			return _nLen;
		}

		/// @brief Returns the size of the internal transfer buffer.
		/// @return The buffer size in bytes.
		td::UINT4 getBufLen() const
		{
			return _bufferSize;
		}

		/// @brief Returns a pointer to the internal transfer buffer.
		/// @return Pointer to the internal character buffer used for chunked I/O.
		char* getBufPtr() const
		{
			return _buf;
		}

		/// @brief Reads the next chunk of data into the internal buffer from file or memory.
		/// @return The number of bytes actually read into the buffer.
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


		/// @brief Reserves a memory buffer of the given size for output operations.
		/// @param size The number of bytes to allocate in the output memory buffer.
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
        /// @brief Writes data directly to the output file, used exclusively for SQLite BLOB transfer.
        /// @param pData Pointer to the data buffer to write.
        /// @param nBytes Number of bytes to write from pData.
        /// @return The number of bytes successfully written, or 0 on failure.
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

		/// @brief Writes the current buffer contents to the output destination (file or memory).
		/// @param nBytes Number of bytes from the internal buffer to write.
		/// @return The number of bytes successfully written, or 0 if nBytes is non-positive.
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
		/// @brief Sets the input file name for file-based BLOB reading.
		/// @param fileName Path to the input file.
		/// @return True if the specified file exists, false otherwise.
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

        /// @brief Sets raw SQLite BLOB data directly from an external pointer, used exclusively for SQLite operations.
        /// @param pData Pointer to the SQLite BLOB data. If null, resets the internal state.
        /// @param nBytes Number of bytes in the provided data buffer.
        /// @return The number of bytes recorded, or 0 if data could not be stored.
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

		/// @brief Sets the in-memory data source for reading.
		/// @param pData Pointer to the data buffer to read from.
		/// @param len Length of the data buffer in bytes.
		/// @return True if the data source was successfully set.
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
		/// @brief Sets the output file name for file-based BLOB writing.
		/// @param fileName Path to the output file.
		/// @return True if the file does not already exist, false if it already exists.
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

		/// @brief Returns a pointer to the stored in-memory output data.
		/// @return Pointer to the memory buffer containing the BLOB data.
		const char* getData() const
		{
			assert(_memType == SRC_MEMORY_OUT);

			if (_pSQLiteData != nullptr)
                return _pSQLiteData;

			return _pMemData;
		}

		/// @brief Returns the input file name associated with this BLOB.
		/// @return A const reference to the input file path string.
		const td::String& getInFileName() const
		{
			return _inFileName;
		}

		/// @brief Returns the output file name associated with this BLOB.
		/// @return A const reference to the output file path string.
		const td::String& getOutFileName() const
		{
			return _outFileName;
		}
	};

	/// @brief Returns the DataType identifier for BLOB values.
	/// @return The td::binary DataType constant.
	inline DataType getType(const td::BLOB&){ return binary;}
};
