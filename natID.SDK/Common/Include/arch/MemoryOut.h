// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "MemorySerializer.h"
#include "ArchiveOut.h"

namespace arch
{

class MemoryOut : public ArchiveOut
{
public:
    enum class PageSize : td::BYTE {Small=0, Normal, Large};
protected:
    MemorySerializer<MemoryOut, true> _fOut;
    bool _bUseGuardBytes;
    //bool _useAutoHeader;
public:
    template <size_t size>
    MemoryOut(const char(&magicWord)[size], IBufferProvider* pBufferProvider, bool useGuardBytes = false/*, bool autoHeaderCreation = true*/)
        : ArchiveOut(magicWord, _fOut)
        , _bUseGuardBytes(useGuardBytes)
        //, _useAutoHeader(autoHeaderCreation)
    {
        if (pBufferProvider)
            pBufferProvider->setReadMode(false);

        _fOut.setBufferProvider(pBufferProvider, this);
        _fOut.open(nullptr);
    }

    MemoryOut(const Header* pExtHeader, IBufferProvider* pBufferProvider, bool calcPayloadCRC, bool useGuardBytes = false)
        : ArchiveOut(pExtHeader, _fOut, calcPayloadCRC)
        , _bUseGuardBytes(useGuardBytes)
        //, _useAutoHeader(autoHeaderCreation)
    {
        if (pBufferProvider)
            pBufferProvider->setReadMode(false);

        _fOut.setBufferProvider(pBufferProvider, this);
        _fOut.open(nullptr);
    }

    //void showHeaderOnEveryPage(bool showOnEveryPage)
    //{
    //	_fOut.showHeaderOnEveryPage(showOnEveryPage);
    //}
    td::UINT4 getBufferSize()
    {
        return _fOut.getBufferSize();
    }

    const char* getCurrentBuffer() const
    {
        //getBuffers
        return _fOut.getCurrentBuffer();
    }

    Header* encodeHeader2()
    {
        //if (_useAutoHeader)
        {
            encodeHeader(0, _bUseGuardBytes);
            return &_header;
        }
        return nullptr;
    }

    Header* decodeHeader2(const char*)
    {
        //if (!_useAutoHeader)
        //{
        //	if (_header.decode(pBuff))
        //		return &_header;
        //	return nullptr;
        //}
        assert(false);
        return nullptr;
    }

    Header& getHeader()
    {
        return _header;
    }

    bool open(const char*)
    {
        //_fOut.open(nullptr);
        //return true;
        //try
        //{
        //	if (!)
        //		return false;
        //}
        //catch (...)
        //{
        //	return false;
        //}
        return true;
    }

    template <class T>
    inline size_t readFromFile(T& f, size_t nBytes = 0) //0 means whole content from the current location
    {
        return _fOut.readFromFile(f, nBytes);
    }

    template <class T>
    inline size_t writeToFile(T& f, bool withHeaders = false)
    {
        return _fOut.writeToFile(f, withHeaders);
    }
    
    void release()
    {
        //this method should be called only if the allocate method way used to create an object
        IBufferProvider* pProvider = _fOut.getBufferProvider();
        delete this;
        if (pProvider)
            pProvider->release();
    }
    
    MAINUTILS_API static MemoryOut* allocate(arch::MemoryOut::PageSize pageSize);
};

}
