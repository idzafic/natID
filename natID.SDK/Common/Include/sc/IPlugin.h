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
#include <gui/Window.h>
#include <functional>
#include <arch/MemoryOut.h>
#include <cnt/Array.h>

namespace sc
{

class IPlugin
{
public:
    using CallBack = std::function<void(sc::IPlugin*)>;
    using Cleaner = std::function<void()>;
    enum class ModelType : td::BYTE {NL=0, LSE, MLE, OPTIM, ODE, DAE};
    enum class ArchType : td::BYTE {DigitalModel=0, VisualModel, NA}; //NA must be the last one (used as size for the container definition)
    using MemoryArchiveContainer = cnt::Array<arch::MemoryOut*, size_t(ArchType::NA)>;
    
    virtual void show(gui::Window* parentWnd, MemoryArchiveContainer& archives, td::UINT4 wndID, const IPlugin::Cleaner& cleaner, const IPlugin::CallBack& onComplete) = 0;
    virtual td::String getMenuName() const = 0;
    virtual MemoryArchiveContainer& getArchives() = 0;
    virtual arch::MemoryOut* getArchive(IPlugin::ArchType) = 0;
    virtual td::String getOutFileName() const = 0;
    virtual size_t getMaxSupportedArchiveParts() const = 0;
    virtual ModelType getModelType() const = 0;
};

} //sc

