// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class MAINUTILS_API Buffer
{
    friend class IBufferAllocator;
    friend class BufferHelper;
public:
    class Desc
    {
    public:
        char* buffer = nullptr;
        td::UINT4 capacity = 0;
        td::UINT4 size = 0;
        td::UINT4 nRefs = 0;
        td::BYTE autoBuffRelease = 0;
    };
    typedef cnt::List<Buffer::Desc, true> TList;
#ifdef MU_VS_COMPILER
    class MAINUTILS_API TList;
#endif
    typedef typename TList::tEntry ListEntry;
    typedef typename TList::iterator ListIter;
    typedef typename TList::const_iterator ConstListIter;
    
protected:
    TList _chunks;
    mem::IBufferAllocator* _pBuffAllocator = nullptr;
private:
    void releaseRef();
    void addRef();
    size_t zlibCompress(fo::InFile& inFile, mem::Buffer::ListEntry*& currentEntry, int level);
    size_t zlibCompress(const mem::Buffer& toCompress, mem::Buffer::ListEntry*& currentEntry, int level);
    size_t zlibDecompress(const mem::Buffer::ListEntry* currentEntry, td::UINT4 fromPos, td::UINT4 nBytes, fo::OutFile& outFile) const;
    size_t zlibDecompress(const mem::Buffer::ListEntry* currentEntry, td::UINT4 fromPos, td::UINT4 nBytes, mem::Buffer& toBuffer) const;
    bool append(const char* pData, size_t size, ListEntry*& currentEntry);
    size_t appendFileContent(const td::String& fileName, ListEntry*& currentEntry, int compressionLevel);
    bool append(const td::String& str);
    bool append(const mem::Buffer& bufferToAppend, ListEntry*& currentEntry, int compressionLevel);
    std::tuple<char*, td::UINT4> getDataChunk(const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    void moveProcessedPos(td::UINT4 deltaToSkip, const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    void relaseChunksUntilCurrent(const ListEntry* currentEntry);
    bool moveBy(size_t nBytes, ListEntry*& pCurrentEntry);
    void moveToEnd(ListEntry*& currentEntry);
    //char* getDataChunk(td::UINT4& size);
    Buffer::ListEntry* getFirstEntry();
    const Buffer::ListEntry* getFirstEntry() const;
    const char* getFirstChunk() const;
    size_t getTotalProcessed(const ListEntry* currentEntry, td::UINT4 currProcessed) const;
    bool replaceBegin(const char* pDataIn, size_t size);
    bool getDataFromBegin(char* pBuffer, size_t nBytes) const;
    td::UINT4 getDataChunk(char* buffer, td::UINT4 nBytes, const ListEntry*& currentEntry, td::UINT4& currProcessed) const;
    char* getContinuousReadingDataChunk(td::UINT4 nBytes, const ListEntry* currentEntry, td::UINT4 currProcessed) const;
    char* getContinuousWritingDataChunk(td::UINT4 nToReceive, ListEntry* currentEntry);
public:
    Buffer(IBufferAllocator* pAllocator = mu::getMainThreadBufferAllocator());
    Buffer(const td::String* pStrings, size_t nStrings, IBufferAllocator* pAllocator = mu::getMainThreadBufferAllocator());
    Buffer(const Buffer& buffer);
    Buffer(const Buffer* pBuffer);
    ~Buffer();
    void operator = (const Buffer&);

    td::String toString() const;
    
    bool isOK() const;
    bool isEmpty() const;
    
    bool reserve(size_t size);
    void clean();
    
    //returns size of original file
    size_t loadFile(const td::String& fileName, int compressionLevel = 0);
    
    bool saveToFile(const char* fileName) const;
    
    size_t getTotalSize() const;
    size_t getTotalCapacity() const;
    
    bool getHeadingData(char* pBuff, size_t requestedSize, bool terminateWithZero = true) const;
    
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
    
    td::UINT4 getNumberOfChunks() const;
    
    int findFirst(const char* pStr, size_t strLen) const;
#ifdef MU_DEBUG
    td::UINT4 getNoOfRefs() const;
#endif
    
    class Writer
    {
    protected:
        mem::Buffer* _pBuffer;
        ListEntry* _pCurrentEntry;
        Writer() {}
        
    public:
        Writer(mem::Buffer& buffer)
            : _pBuffer(&buffer)
            , _pCurrentEntry(buffer.getFirstEntry())
        {}
        
        Writer(mem::Buffer* pBuffer)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(nullptr)
        {
            if (_pBuffer)
                _pCurrentEntry = _pBuffer->getFirstEntry();                
        }
        
        Writer(mem::Buffer::Writer& writer)
            : _pBuffer(writer._pBuffer)
            , _pCurrentEntry(writer._pCurrentEntry)
        {
        }
        
        bool append(const char* pDataIn, size_t size = 0)
        {
            assert(_pBuffer);
            return _pBuffer->append(pDataIn, size, _pCurrentEntry);
        }
        
        bool append(const mem::Buffer& bufferToAppend, int compressionLevel = 0)
        {
            assert(_pBuffer);
            return _pBuffer->append(bufferToAppend, _pCurrentEntry, compressionLevel);
        }
        
        bool moveBy(size_t nBytes)
        {
            assert(_pBuffer);
            return _pBuffer->moveBy(nBytes, _pCurrentEntry);
        }
        
        bool moveToEnd()
        {
            assert(_pBuffer);
            _pBuffer->moveToEnd(_pCurrentEntry);
            return true;
        }
        
        //returns size of original file
        size_t appendFileContent(const td::String& fileName, int compressionLevel = 0)
        {
            assert(_pBuffer);
            return _pBuffer->appendFileContent(fileName, _pCurrentEntry, compressionLevel);
        }
        
        bool replaceBegin(const char* pDataIn, size_t size)
        {
            assert(_pBuffer);
            return _pBuffer->replaceBegin(pDataIn, size);
        }

        bool append(const td::String& str)
        {
            assert(_pBuffer);
            return append(str.c_str(), str.length());
        }
        
        const char* getFirstChunk() const
        {
            assert(_pBuffer);
            return _pBuffer->getFirstChunk();
        }
        
        mem::Buffer* getBuffer()
        {
            return _pBuffer;
        }
        
        char* getContinuousDataChunk(td::UINT4 nToReceive)
        {
            assert(_pBuffer);
            return _pBuffer->getContinuousWritingDataChunk(nToReceive, _pCurrentEntry);
        }
    };
    
    class Reader
    {
    protected:
        const mem::Buffer* _pBuffer;
        const ListEntry* _pCurrentEntry;
        td::UINT4 _processed;
        
    public:
        Reader()
            : _pBuffer(nullptr)
            , _pCurrentEntry(nullptr)
            , _processed(0)
        {}
        
        Reader(const mem::Buffer* pBuffer)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(pBuffer->getFirstEntry())
            , _processed(0)
        {
        }
        
        Reader(const mem::Buffer& buffer)
            : _pBuffer(&buffer)
            , _pCurrentEntry(buffer.getFirstEntry())
            , _processed(0)
        {
        }

        Reader(const mem::Buffer* pBuffer, ListEntry* pEntry)
            : _pBuffer(pBuffer)
            , _pCurrentEntry(pEntry)
            , _processed(0)
        {
        }
        
        Reader(const Reader& it)
            : _pBuffer(it._pBuffer)
            , _pCurrentEntry(it._pCurrentEntry)
            , _processed(it._processed)
        {
        }
        
        ~Reader()
        {
        }
        
        const Buffer* getBuffer() const
        {
            return _pBuffer;
        }

        void operator ++ ()
        {
            //int g = 1;
            assert(_pCurrentEntry);
            if (_pCurrentEntry)
                _pCurrentEntry = _pCurrentEntry->next;
            _processed = 0;
        }
        
        void moveFirst()
        {
            if (_pBuffer)
                _pCurrentEntry = _pBuffer->getFirstEntry();
            else
                _pCurrentEntry = nullptr;
            _processed = 0;
        }
        
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

        const Buffer::Desc& operator* () const
        {
            assert(_pCurrentEntry);
            return _pCurrentEntry->data;
        }

        const Buffer::Desc* operator ->() const
        {
            return &(operator*());
        }

        void operator = (const Reader& it)
        {
            _pBuffer = it._pBuffer;
            _pCurrentEntry = it._pCurrentEntry;
            _processed = it._processed;
        }
        
        void operator = (mem::Buffer* pBuffer)
        {
            _pBuffer = pBuffer;
            _pCurrentEntry = _pBuffer->getFirstEntry();
            _processed = 0;
        }

        bool operator == (const Reader& it) const
        {
            return ( (_pCurrentEntry == it._pCurrentEntry) /*&& (_processed == it._processed) */);
        }

        bool operator != (const Reader& it) const
        {
            return !(operator == (it));
        }
        
        size_t getTotalSize() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalSize();
        }
        
        size_t getTotalCapacity() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalCapacity();
        }
        
        size_t getTotalProcessed() const
        {
            assert(_pBuffer);
            return _pBuffer->getTotalProcessed(_pCurrentEntry, _processed);
        }
        
        bool isOK() const
        {
            return (_pCurrentEntry != nullptr);
        }
        
        void assertRefsNumber( [[maybe_unused]] td::UINT4 nRefs)
        {
            assert(_pBuffer);
#ifdef MU_DEBUG
            auto firstEntry = _pBuffer->getFirstEntry();
            assert (firstEntry->data.nRefs == nRefs);
#endif
        }
        
        std::tuple<char*, td::UINT4> getDataChunk()
        {
            assert(_pBuffer);
            return _pBuffer->getDataChunk(_pCurrentEntry, _processed);
        }
        
        //current position will be unchanged
        //user needs to call moveProcessedPos after calling this method
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
        td::UINT4 relaseChunksUntilCurrent()
        {
            if (!_pCurrentEntry)
                return 0;
            assert(_pBuffer);
            mem::Buffer* pBuffer = const_cast<mem::Buffer*>(_pBuffer);
            pBuffer->relaseChunksUntilCurrent(_pCurrentEntry);
            return _processed;
        }
        
        char* getContinuousDataChunk(td::UINT4 nBytes) const
        {
            assert(_pBuffer);
            return _pBuffer->getContinuousReadingDataChunk(nBytes, _pCurrentEntry, _processed);
        }
        
        void moveProcessedPos(td::UINT4 deltaToSkip)
        {
            assert(_pBuffer);
            _pBuffer->moveProcessedPos(deltaToSkip, _pCurrentEntry, _processed);
            //_processed += deltaToSkip;
        }
        
        const char* getFirstChunk() const
        {
            assert(_pBuffer);
            return _pBuffer->getFirstChunk();
        }
    };
    
    Reader begin();
    
    Writer writer();
    
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
