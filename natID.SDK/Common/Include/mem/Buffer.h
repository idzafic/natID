// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Buffer.h
    @brief Chunked memory buffer with reference counting, compression support, and nested Writer/Reader helpers. */
//
//  Buffer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <mu/muLib.h>
#include <cnt/List.h>
#include <td/String.h>
#include <tuple>
#include <fo/FileOperations.h>

namespace mem
{

class IBufferAllocator;
class BufferHelper;

/// @brief A chunked, reference-counted memory buffer that supports sequential append, file loading, zlib compression, and chunk-based reading.
class MAINUTILS_API Buffer
{
    friend class IBufferAllocator;
    friend class BufferHelper;
public:
    /// @brief Describes a single memory chunk within the buffer.
    class Desc
    {
    public:
        char* buffer = nullptr;     ///< Pointer to the raw memory block of this chunk.
        td::UINT4 capacity = 0;     ///< Total allocated capacity of this chunk in bytes.
        td::UINT4 size = 0;         ///< Number of bytes currently used in this chunk.
        td::UINT4 nRefs = 0;        ///< Reference count for this chunk.
        td::BYTE autoBuffRelease = 0; ///< Non-zero if the chunk memory should be released automatically.
    };
    typedef cnt::List<Buffer::Desc, true> TList; ///< Doubly-linked list type used to chain buffer chunks.
#ifdef MU_VS_COMPILER
    class MAINUTILS_API TList;
#endif
    typedef typename TList::tEntry ListEntry;           ///< Type of a single list node holding a Desc.
    typedef typename TList::iterator ListIter;          ///< Mutable iterator over the chunk list.
    typedef typename TList::const_iterator ConstListIter; ///< Const iterator over the chunk list.

protected:
    TList _chunks;                              ///< Linked list of memory chunks that form the buffer.
    mem::IBufferAllocator* _pBuffAllocator = nullptr; ///< Allocator used to manage chunk memory.
private:
    /// @brief Decrements the reference count and frees resources when it reaches zero.
    void releaseRef();
    /// @brief Increments the reference count of the first chunk.
    void addRef();
    /// @brief Compresses data from an input file into this buffer using zlib.
    /// @param inFile Source file to compress.
    /// @param currentEntry Current write position entry (updated on return).
    /// @param level zlib compression level (0-9).
    /// @return Number of bytes from the original (uncompressed) file.
    size_t zlibCompress(fo::InFile& inFile, mem::Buffer::ListEntry*& currentEntry, int level);
    /// @brief Compresses data from another buffer into this buffer using zlib.
    /// @param toCompress Source buffer to compress.
    /// @param currentEntry Current write position entry (updated on return).
    /// @param level zlib compression level (0-9).
    /// @return Number of bytes from the original (uncompressed) source.
    size_t zlibCompress(const mem::Buffer& toCompress, mem::Buffer::ListEntry*& currentEntry, int level);
    /// @brief Decompresses a range of zlib-compressed data from this buffer into a file.
    /// @param currentEntry Starting chunk entry.
    /// @param fromPos Byte offset within the starting entry.
    /// @param nBytes Number of compressed bytes to process.
    /// @param outFile Destination file for decompressed data.
    /// @return Number of decompressed bytes written.
    size_t zlibDecompress(const mem::Buffer::ListEntry* currentEntry, td::UINT4 fromPos, td::UINT4 nBytes, fo::OutFile& outFile) const;
    /// @brief Decompresses a range of zlib-compressed data from this buffer into another buffer.
    /// @param currentEntry Starting chunk entry.
    /// @param fromPos Byte offset within the starting entry.
    /// @param nBytes Number of compressed bytes to process.
    /// @param toBuffer Destination buffer for decompressed data.
    /// @return Number of decompressed bytes written.
    size_t zlibDecompress(const mem::Buffer::ListEntry* currentEntry, td::UINT4 fromPos, td::UINT4 nBytes, mem::Buffer& toBuffer) const;
    /// @brief Appends raw bytes to the buffer, allocating new chunks as needed.
    /// @param pData Pointer to the source data.
    /// @param size Number of bytes to append.
    /// @param currentEntry Current write position entry (updated on return).
    /// @return true on success, false on allocation failure.
    bool append(const char* pData, size_t size, ListEntry*& currentEntry);
    /// @brief Appends the contents of a file (optionally compressed) to the buffer.
    /// @param fileName Path of the file to append.
    /// @param currentEntry Current write position entry (updated on return).
    /// @param compressionLevel zlib compression level; 0 means no compression.
    /// @return Number of bytes from the original file.
    size_t appendFileContent(const td::String& fileName, ListEntry*& currentEntry, int compressionLevel);
    /// @brief Appends a string to the buffer.
    /// @param str String to append.
    /// @return true on success, false on allocation failure.
    bool append(const td::String& str);
    /// @brief Appends another buffer (optionally compressed) to this buffer.
    /// @param bufferToAppend Source buffer.
    /// @param currentEntry Current write position entry (updated on return).
    /// @param compressionLevel zlib compression level; 0 means no compression.
    /// @return true on success.
    bool append(const mem::Buffer& bufferToAppend, ListEntry*& currentEntry, int compressionLevel);
    /// @brief Retrieves the next available data chunk for reading.
    /// @param currentEntry Current chunk entry (updated on return).
    /// @param currProcessed Byte offset within the current entry (updated on return).
    /// @return Tuple of (pointer to chunk data, number of available bytes).
    std::tuple<char*, td::UINT4> getDataChunk(const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    /// @brief Advances the read position by deltaToSkip bytes across chunk boundaries.
    /// @param deltaToSkip Number of bytes to skip.
    /// @param currentEntry Current chunk entry (updated on return).
    /// @param currProcessed Byte offset within the current entry (updated on return).
    void moveProcessedPos(td::UINT4 deltaToSkip, const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    /// @brief Releases all chunks before the specified entry.
    /// @param currentEntry Boundary entry; all entries before it are released.
    void relaseChunksUntilCurrent(const ListEntry* currentEntry);
    /// @brief Advances the write position by nBytes, allocating new chunks as needed.
    /// @param nBytes Number of bytes to advance.
    /// @param pCurrentEntry Current write entry (updated on return).
    /// @return true on success, false on allocation failure.
    bool moveBy(size_t nBytes, ListEntry*& pCurrentEntry);
    /// @brief Moves the write position to the end of all existing chunks.
    /// @param currentEntry Updated to point to the last chunk entry.
    void moveToEnd(ListEntry*& currentEntry);
    //char* getDataChunk(td::UINT4& size);
    /// @brief Returns a pointer to the first list entry.
    /// @return Pointer to the first ListEntry, or nullptr if empty.
    Buffer::ListEntry* getFirstEntry();
    /// @brief Returns a const pointer to the first list entry.
    /// @return Const pointer to the first ListEntry, or nullptr if empty.
    const Buffer::ListEntry* getFirstEntry() const;
    /// @brief Returns a pointer to the raw data of the first chunk.
    /// @return Const pointer to the first chunk's data, or nullptr if empty.
    const char* getFirstChunk() const;
    /// @brief Computes total bytes processed up to the current reading position.
    /// @param currentEntry Current chunk entry.
    /// @param currProcessed Byte offset within the current entry.
    /// @return Total number of bytes processed so far.
    size_t getTotalProcessed(const ListEntry* currentEntry, td::UINT4 currProcessed) const;
    /// @brief Overwrites the beginning of the buffer with new data.
    /// @param pDataIn Pointer to the replacement data.
    /// @param size Number of bytes to overwrite.
    /// @return true on success, false if size exceeds buffer content.
    bool replaceBegin(const char* pDataIn, size_t size);
    /// @brief Copies bytes from the beginning of the buffer into the provided buffer.
    /// @param pBuffer Destination buffer.
    /// @param nBytes Number of bytes to read.
    /// @return true on success, false if fewer bytes are available.
    bool getDataFromBegin(char* pBuffer, size_t nBytes) const;
    /// @brief Reads up to nBytes from the current reading position into a flat buffer.
    /// @param buffer Destination flat buffer.
    /// @param nBytes Maximum number of bytes to read.
    /// @param currentEntry Current chunk entry (updated on return).
    /// @param currProcessed Byte offset within the current entry (updated on return).
    /// @return Actual number of bytes placed into buffer.
    td::UINT4 getDataChunk(char* buffer, td::UINT4 nBytes, const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    /// @brief Returns a pointer to a contiguous block of nBytes for reading, if the current chunk can satisfy it.
    /// @param nBytes Number of contiguous bytes required.
    /// @param currentEntry Current chunk entry.
    /// @param currProcessed Byte offset within the current entry.
    /// @return Pointer to the contiguous data, or nullptr if not available in one chunk.
    char* getContinuousReadingDataChunk(td::UINT4 nBytes, const ListEntry* currentEntry, td::UINT4 currProcessed) const;
    /// @brief Returns a pointer to a contiguous block of nToReceive bytes for writing within the current chunk.
    /// @param nToReceive Number of bytes requested.
    /// @param currentEntry Current write entry.
    /// @return Pointer to the contiguous writable region, or nullptr if not available.
    char* getContinuousWritingDataChunk(td::UINT4 nToReceive, ListEntry* currentEntry);
public:
    /// @brief Constructs a Buffer using the specified allocator.
    /// @param pAllocator Allocator to use; defaults to the main-thread buffer allocator.
    Buffer(IBufferAllocator* pAllocator = mu::getMainThreadBufferAllocator());
    /// @brief Constructs a Buffer pre-populated from an array of strings.
    /// @param pStrings Pointer to the first element of a string array.
    /// @param nStrings Number of strings in the array.
    /// @param pAllocator Allocator to use; defaults to the main-thread buffer allocator.
    Buffer(const td::String* pStrings, size_t nStrings, IBufferAllocator* pAllocator = mu::getMainThreadBufferAllocator());
    /// @brief Copy-constructs a Buffer from another Buffer (increments reference count).
    /// @param buffer Source buffer to copy.
    Buffer(const Buffer& buffer);
    /// @brief Constructs a Buffer from a pointer to another Buffer (increments reference count).
    /// @param pBuffer Pointer to the source buffer.
    Buffer(const Buffer* pBuffer);
    /// @brief Destructor; releases all chunks via the allocator.
    ~Buffer();
    /// @brief Copy-assignment operator; releases current chunks and shares ownership with the source.
    /// @param src Source buffer.
    void operator = (const Buffer&);

    /// @brief Converts the entire buffer contents to a single td::String.
    /// @return String containing all buffer bytes.
    td::String toString() const;

    /// @brief Returns true if the buffer is in a valid, usable state.
    /// @return true if the buffer has an allocator and at least one chunk.
    bool isOK() const;
    /// @brief Returns true if the buffer contains no data.
    /// @return true if total size is zero.
    bool isEmpty() const;

    /// @brief Pre-allocates a chunk of at least size bytes.
    /// @param size Number of bytes to reserve.
    /// @return true on success, false on allocation failure.
    bool reserve(size_t size);
    /// @brief Releases all chunks and resets the buffer to an empty state.
    void clean();

    //returns size of original file
    /// @brief Loads a file into the buffer, optionally compressing it.
    /// @param fileName Path to the file to load.
    /// @param compressionLevel zlib compression level; 0 means no compression.
    /// @return Size of the original (uncompressed) file in bytes.
    size_t loadFile(const td::String& fileName, int compressionLevel = 0);

    /// @brief Saves the buffer contents to a file on disk.
    /// @param fileName Null-terminated path of the output file.
    /// @return true on success, false on I/O error.
    bool saveToFile(const char* fileName) const;

    /// @brief Returns the total number of data bytes stored across all chunks.
    /// @return Total used size in bytes.
    size_t getTotalSize() const;
    /// @brief Returns the total allocated capacity across all chunks.
    /// @return Total capacity in bytes.
    size_t getTotalCapacity() const;

    /// @brief Copies bytes from the beginning of the buffer into a caller-supplied buffer.
    /// @param pBuff Destination memory block.
    /// @param requestedSize Number of bytes to copy.
    /// @param terminateWithZero If true, a null terminator is appended after the data.
    /// @return true on success, false if insufficient data is available.
    bool getHeadingData(char* pBuff, size_t requestedSize, bool terminateWithZero = true) const;

    /// @brief Copies up to size bytes from the beginning of the buffer into a compile-time-sized static array.
    /// @tparam size Size of the static destination array.
    /// @param staticBuffer Destination array.
    /// @param requestedSize Number of bytes to copy; if 0, the full array size is used.
    /// @return true on success, false if requestedSize exceeds the array size.
    template <size_t size>
    bool getHeadingData(char (&staticBuffer)[size], size_t requestedSize = 0) const
    {
        if (requestedSize > size)
            return false;

        if (requestedSize == 0)
            return getDataFromBegin(&staticBuffer[0], size);

        if (requestedSize < size)
            staticBuffer[requestedSize] = 0;

        return getDataFromBegin(&staticBuffer[0], requestedSize);
    }

    /// @brief Returns the number of memory chunks currently held by this buffer.
    /// @return Number of chunks.
    td::UINT4 getNumberOfChunks() const;

    /// @brief Searches the buffer for the first occurrence of a byte sequence.
    /// @param pStr Pointer to the byte sequence to find.
    /// @param strLen Length of the byte sequence.
    /// @return Byte offset of the first match, or -1 if not found.
    int findFirst(const char* pStr, size_t strLen) const;
#ifdef MU_DEBUG
    /// @brief Returns the current reference count of the buffer (debug builds only).
    /// @return Reference count of the first chunk.
    td::UINT4 getNoOfRefs() const;
#endif

    /// @brief Provides sequential write access to a Buffer through a cursor-based interface.
    class Writer
    {
    protected:
        mem::Buffer* _pBuffer;      ///< Pointer to the buffer being written.
        ListEntry* _pCurrentEntry;  ///< Current chunk entry where data will be appended.
        /// @brief Default constructor for derived classes; leaves members uninitialized.
        Writer() {}

    public:
        /// @brief Constructs a Writer positioned at the start of the given buffer.
        /// @param buffer Buffer to write into.
        Writer(mem::Buffer& buffer)
            : _pBuffer(&buffer)
            , _pCurrentEntry(buffer.getFirstEntry())
        {}

        /// @brief Constructs a Writer from a buffer pointer.
        /// @param pBuffer Pointer to the buffer to write into; may be nullptr.
        Writer(mem::Buffer* pBuffer)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(nullptr)
        {
            if (_pBuffer)
                _pCurrentEntry = _pBuffer->getFirstEntry();
        }

        /// @brief Copy-constructs a Writer sharing the position of another Writer.
        /// @param writer Source writer to copy position from.
        Writer(mem::Buffer::Writer& writer)
            : _pBuffer(writer._pBuffer)
            , _pCurrentEntry(writer._pCurrentEntry)
        {
        }

        /// @brief Appends raw bytes to the buffer.
        /// @param pDataIn Pointer to the source data.
        /// @param size Number of bytes to append (0 means null-terminated string length).
        /// @return true on success, false on allocation failure.
        bool append(const char* pDataIn, size_t size = 0)
        {
            assert(_pBuffer);
            return _pBuffer->append(pDataIn, size, _pCurrentEntry);
        }

        /// @brief Appends another buffer (optionally compressed) to this buffer.
        /// @param bufferToAppend Source buffer to append.
        /// @param compressionLevel zlib compression level; 0 means no compression.
        /// @return true on success.
        bool append(const mem::Buffer& bufferToAppend, int compressionLevel = 0)
        {
            assert(_pBuffer);
            return _pBuffer->append(bufferToAppend, _pCurrentEntry, compressionLevel);
        }

        /// @brief Advances the write position by nBytes without writing data.
        /// @param nBytes Number of bytes to skip forward.
        /// @return true on success, false on allocation failure.
        bool moveBy(size_t nBytes)
        {
            assert(_pBuffer);
            return _pBuffer->moveBy(nBytes, _pCurrentEntry);
        }

        /// @brief Moves the write position to the end of all existing data.
        /// @return Always returns true.
        bool moveToEnd()
        {
            assert(_pBuffer);
            _pBuffer->moveToEnd(_pCurrentEntry);
            return true;
        }

        //returns size of original file
        /// @brief Appends file contents (optionally compressed) to the buffer.
        /// @param fileName Path to the file to append.
        /// @param compressionLevel zlib compression level; 0 means no compression.
        /// @return Size of the original (uncompressed) file in bytes.
        size_t appendFileContent(const td::String& fileName, int compressionLevel = 0)
        {
            assert(_pBuffer);
            return _pBuffer->appendFileContent(fileName, _pCurrentEntry, compressionLevel);
        }

        /// @brief Overwrites the beginning of the buffer with new data.
        /// @param pDataIn Pointer to the replacement data.
        /// @param size Number of bytes to overwrite.
        /// @return true on success.
        bool replaceBegin(const char* pDataIn, size_t size)
        {
            assert(_pBuffer);
            return _pBuffer->replaceBegin(pDataIn, size);
        }

        /// @brief Appends a td::String to the buffer.
        /// @param str String to append.
        /// @return true on success.
        bool append(const td::String& str)
        {
            assert(_pBuffer);
            return append(str.c_str(), str.length());
        }

        /// @brief Returns a pointer to the raw data of the first chunk.
        /// @return Const pointer to the first chunk data, or nullptr if empty.
        const char* getFirstChunk() const
        {
            assert(_pBuffer);
            return _pBuffer->getFirstChunk();
        }

        /// @brief Returns the underlying buffer pointer.
        /// @return Pointer to the managed Buffer.
        mem::Buffer* getBuffer()
        {
            return _pBuffer;
        }

        /// @brief Returns a pointer to a contiguous writable region of nToReceive bytes in the current chunk.
        /// @param nToReceive Number of bytes required.
        /// @return Pointer to the contiguous writable region, or nullptr if not available.
        char* getContinuousDataChunk(td::UINT4 nToReceive)
        {
            assert(_pBuffer);
            return _pBuffer->getContinuousWritingDataChunk(nToReceive, _pCurrentEntry);
        }
    };

    /// @brief Provides sequential read access to a Buffer through a cursor-based interface.
    class Reader
    {
    protected:
        const mem::Buffer* _pBuffer;    ///< Pointer to the buffer being read.
        const ListEntry* _pCurrentEntry; ///< Current chunk entry being read.
        td::UINT4 _processed;           ///< Number of bytes consumed within the current chunk.

    public:
        /// @brief Default constructor; creates an unattached reader.
        Reader()
            : _pBuffer(nullptr)
            , _pCurrentEntry(nullptr)
            , _processed(0)
        {}

        /// @brief Constructs a Reader positioned at the start of the given buffer pointer.
        /// @param pBuffer Pointer to the buffer to read.
        Reader(const mem::Buffer* pBuffer)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(pBuffer->getFirstEntry())
            , _processed(0)
        {
        }

        /// @brief Constructs a Reader positioned at the start of the given buffer reference.
        /// @param buffer Buffer to read.
        Reader(const mem::Buffer& buffer)
            : _pBuffer(&buffer)
            , _pCurrentEntry(buffer.getFirstEntry())
            , _processed(0)
        {
        }

        /// @brief Constructs a Reader starting at a specific chunk entry.
        /// @param pBuffer Pointer to the buffer to read.
        /// @param pEntry Starting chunk entry.
        Reader(const mem::Buffer* pBuffer, ListEntry* pEntry)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(pEntry)
            , _processed(0)
        {
        }

        /// @brief Copy-constructs a Reader from another Reader.
        /// @param it Source reader to copy state from.
        Reader(const Reader& it)
            : _pBuffer(it._pBuffer)
            , _pCurrentEntry(it._pCurrentEntry)
            , _processed(it._processed)
        {
        }

        /// @brief Destructor.
        ~Reader()
        {
        }

        /// @brief Returns the underlying buffer pointer.
        /// @return Const pointer to the managed Buffer.
        const Buffer* getBuffer() const
        {
            return _pBuffer;
        }

        /// @brief Advances to the next chunk entry.
        void operator ++ ()
        {
            //int g = 1;
            assert(_pCurrentEntry);
            if (_pCurrentEntry)
                _pCurrentEntry = _pCurrentEntry->next;
            _processed = 0;
        }

        /// @brief Resets the reader back to the first chunk of the buffer.
        void moveFirst()
        {
            if (_pBuffer)
                _pCurrentEntry = _pBuffer->getFirstEntry();
            else
                _pCurrentEntry = nullptr;
            _processed = 0;
        }

        /// @brief Cleans the underlying buffer and resets the reader state.
        void clean()
        {
            if (_pBuffer)
            {
                mem::Buffer* pBuffer = const_cast<mem::Buffer*>(_pBuffer);
                pBuffer->clean();
            }
            _pBuffer = nullptr;
            _pCurrentEntry = nullptr;
            _processed = 0;
        }

        /// @brief Dereferences the current chunk entry, returning its Desc.
        /// @return Const reference to the current Desc.
        const Buffer::Desc& operator* () const
        {
            assert(_pCurrentEntry);
            return _pCurrentEntry->data;
        }

        /// @brief Arrow operator returning a pointer to the current Desc.
        /// @return Const pointer to the current Desc.
        const Buffer::Desc* operator ->() const
        {
            return &(operator*());
        }

        /// @brief Assignment from another Reader.
        /// @param it Source reader.
        void operator = (const Reader& it)
        {
            _pBuffer = it._pBuffer;
            _pCurrentEntry = it._pCurrentEntry;
            _processed = it._processed;
        }

        /// @brief Reassigns the reader to a different buffer pointer.
        /// @param pBuffer Pointer to the new buffer.
        void operator = (mem::Buffer* pBuffer)
        {
            _pBuffer = pBuffer;
            _pCurrentEntry = _pBuffer->getFirstEntry();
            _processed = 0;
        }

        /// @brief Equality comparison with another Reader.
        /// @param it Reader to compare with.
        /// @return true if both readers point to the same chunk entry.
        bool operator == (const Reader& it) const
        {
            return ( (_pCurrentEntry == it._pCurrentEntry) /*&& (_processed == it._processed) */);
        }

        /// @brief Inequality comparison with another Reader.
        /// @param it Reader to compare with.
        /// @return true if the readers point to different chunk entries.
        bool operator != (const Reader& it) const
        {
            return !(operator == (it));
        }

        /// @brief Returns the total data size of the underlying buffer.
        /// @return Total size in bytes.
        size_t getTotalSize() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalSize();
        }

        /// @brief Returns the total allocated capacity of the underlying buffer.
        /// @return Total capacity in bytes.
        size_t getTotalCapacity() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalCapacity();
        }

        /// @brief Returns the total number of bytes consumed up to the current position.
        /// @return Total processed bytes.
        size_t getTotalProcessed() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalProcessed(_pCurrentEntry, _processed);
        }

        /// @brief Checks whether the reader is positioned on a valid chunk.
        /// @return true if the current entry is non-null.
        bool isOK() const
        {
            return (_pCurrentEntry != nullptr);
        }

        /// @brief Asserts that the first chunk's reference count equals nRefs (debug builds).
        /// @param nRefs Expected reference count.
        void assertRefsNumber( [[maybe_unused]] td::UINT4 nRefs)
        {
            assert(_pBuffer);
#ifdef MU_DEBUG
            auto firstEntry = _pBuffer->getFirstEntry();
            assert (firstEntry->data.nRefs == nRefs);
#endif
        }

        /// @brief Retrieves the next data chunk and advances to the following chunk.
        /// @return Tuple of (pointer to data, number of bytes available).
        std::tuple<char*, td::UINT4> getDataChunk()
        {
            assert(_pBuffer);
            return _pBuffer->getDataChunk(_pCurrentEntry, _processed);
        }

        //current position will be unchanged
        //user needs to call moveProcessedPos after calling this method
        /// @brief Reads up to nBytes from the current position into a flat buffer.
        /// @param buffer Destination flat buffer.
        /// @param nBytes Maximum number of bytes to read.
        /// @param updatePosition If true, advances the internal position after reading.
        /// @return Actual number of bytes placed into buffer.
        td::UINT4 getDataChunk(char* buffer, td::UINT4 nBytes, bool updatePosition = false)
        {
            assert(_pBuffer);
            auto pOldEntry = _pCurrentEntry;
            auto pOldProcessed = _processed;
            td::UINT4 bytesPopulated = _pBuffer->getDataChunk(buffer, nBytes, _pCurrentEntry, _processed);
            if (!updatePosition)
            {
                _pCurrentEntry = pOldEntry;
                _processed = pOldProcessed;
            }
            return bytesPopulated;
        }

        //removes all leading chunks until current
        //returns currents processed position
        /// @brief Releases all chunks preceding the current one and returns the current byte offset.
        /// @return Current processed byte offset within the active chunk.
        td::UINT4 relaseChunksUntilCurrent()
        {
            if (!_pCurrentEntry)
                return 0;
            assert(_pBuffer);
            mem::Buffer* pBuffer = const_cast<mem::Buffer*>(_pBuffer);
            pBuffer->relaseChunksUntilCurrent(_pCurrentEntry);
            return _processed;
        }

        /// @brief Returns a pointer to a contiguous block of nBytes starting at the current read position.
        /// @param nBytes Number of contiguous bytes required.
        /// @return Pointer to the data, or nullptr if not available in one chunk.
        char* getContinuousDataChunk(td::UINT4 nBytes) const
        {
            assert(_pBuffer);
            return _pBuffer->getContinuousReadingDataChunk(nBytes, _pCurrentEntry, _processed);
        }

        /// @brief Advances the read position by deltaToSkip bytes.
        /// @param deltaToSkip Number of bytes to skip.
        void moveProcessedPos(td::UINT4 deltaToSkip)
        {
            assert(_pBuffer);
            _pBuffer->moveProcessedPos(deltaToSkip, _pCurrentEntry, _processed);
            //_processed += deltaToSkip;
        }

        /// @brief Returns a pointer to the raw data of the first chunk.
        /// @return Const pointer to the first chunk data.
        const char* getFirstChunk() const
        {
            assert(_pBuffer);
            return _pBuffer->getFirstChunk();
        }
    };

    /// @brief Creates a Reader positioned at the beginning of this buffer.
    /// @return A Reader initialised to the first chunk.
    Reader begin();

    /// @brief Creates a Writer positioned at the beginning of this buffer.
    /// @return A Writer initialised to the first chunk.
    Writer writer();

    /// @brief Copies data from another buffer starting at a given byte offset into this buffer.
    /// @param pBuff Source buffer to copy from.
    /// @param startingPos Byte offset within the source buffer at which copying begins.
    void copyFrom(const mem::Buffer* pBuff, td::UINT4 startingPos)
    {
        mem::Buffer::Reader r(pBuff);
        r.moveProcessedPos(startingPos);
        mem::Buffer::Writer w(this);
        char tmp[4096];
        do
        {
            td::UINT4 nObtained = r.getDataChunk(tmp, 4096, true);
            if (nObtained > 0)
                w.append(tmp, nObtained);
            if (nObtained != 4096)
                break;
        }while (true);
    }
};

} //namespace no
