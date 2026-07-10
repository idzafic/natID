// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <td/String.h>
#include <stdexcept>

#if defined(MU_WINDOWS)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace syst
{
class LibraryLoader
{
private:
    void* _handle = nullptr;
    bool _autoUnload = true;
public:
    LibraryLoader() = default;
    explicit LibraryLoader(bool autoUnload)
    : _autoUnload(autoUnload)
    {
        
    }

    explicit LibraryLoader(const td::String& path)
    {
        load(path);
    }

    ~LibraryLoader()
    {
        if (_autoUnload)
            unload();
    }

    // Non-copyable
    LibraryLoader(const LibraryLoader&) = delete;
    LibraryLoader& operator=(const LibraryLoader&) = delete;

    // Movable
    LibraryLoader(LibraryLoader&& other) noexcept
    {
        _handle = other._handle;
        _autoUnload = other._autoUnload;
        other._handle = nullptr;
        other._autoUnload = false;
    }

    LibraryLoader& operator=(LibraryLoader&& other) noexcept
    {
        if (this != &other)
        {
            unload();
            _handle = other._handle;
            _autoUnload = other._autoUnload;
            other._handle = nullptr;
            other._autoUnload = false;
        }
        return *this;
    }

    bool load(const td::String& path)
    {
        unload();

#if defined(MU_WINDOWS)
        _handle = (void*)LoadLibraryA(path.c_str());
#else
        _handle = dlopen(path.c_str(), RTLD_LAZY);
#endif

        if (!_handle)
			return false;
        return true;
    }

    void unload()
    {
        if (!_handle)
            return;

#if defined(MU_WINDOWS)
        FreeLibrary((HMODULE)_handle);
#else
        dlclose(_handle);
#endif
        _handle = nullptr;
    }

    template<typename T>
    T getFunctionPtr(const td::String& name) const
    {
        if (!_handle)
			return T(nullptr);
            //throw std::runtime_error("Library not loaded");

#if defined(MU_WINDOWS)
        void* sym = (void*)GetProcAddress((HMODULE)_handle, name.c_str());
#else
        void* sym = dlsym(_handle, name.c_str());
#endif

        if (!sym)
        {
			return T(sym);
            //throw std::runtime_error("Function not found: " + name);
        }

        return reinterpret_cast<T>(sym);
    }
    
    template<typename T>
    T getFunctionPtr(const char* name) const
    {
        if (!_handle)
            return T(nullptr);
            //throw std::runtime_error("Library not loaded");

#if defined(MU_WINDOWS)
        void* sym = (void*)GetProcAddress((HMODULE)_handle, name);
#else
        void* sym = dlsym(_handle, name);
#endif

        if (!sym)
        {
            return T(sym);
            //throw std::runtime_error("Function not found: " + name);
        }

        return reinterpret_cast<T>(sym);
    }

    bool isLoaded() const
    {
        return _handle != nullptr;
    }
};

}
