// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IPlugin.h
 *  @brief Interface for solver plug-ins that expose GUI windows and serialised archives.
 */
#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <gui/Window.h>
#include <functional>
#include <arch/MemoryOut.h>
#include <cnt/Array.h>

namespace sc
{

/// @brief Abstract interface for a solver plug-in that can display a result window and provide memory archives.
class IPlugin
{
public:
    using CallBack = std::function<void(sc::IPlugin*)>; ///< Callback invoked when the plug-in completes its work.
    using Cleaner = std::function<void()>;              ///< Callback invoked to clean up plug-in resources.
    /// @brief Category of mathematical model handled by the plug-in.
    enum class ModelType : td::BYTE {NL=0, LSE, MLE, OPTIM, ODE, DAE};
    /// @brief Type of archive produced by the plug-in.
    enum class ArchType : td::BYTE {DigitalModel=0, VisualModel, NA}; //NA must be the last one (used as size for the container definition)
    using MemoryArchiveContainer = cnt::Array<arch::MemoryOut*, size_t(ArchType::NA)>; ///< Fixed-size array of memory archive pointers, indexed by ArchType.

    /// @brief Display the plug-in result window.
    /// @param parentWnd Parent GUI window.
    /// @param archives Container of memory archives to populate.
    /// @param wndID Identifier assigned to the created window.
    /// @param cleaner Callback invoked when the window resources must be released.
    /// @param onComplete Callback invoked when the plug-in finishes processing.
    virtual void show(gui::Window* parentWnd, MemoryArchiveContainer& archives, td::UINT4 wndID, const IPlugin::Cleaner& cleaner, const IPlugin::CallBack& onComplete) = 0;
    /// @brief Return the localised menu entry name for this plug-in.
    /// @return Display name string.
    virtual td::String getMenuName() const = 0;
    /// @brief Return a reference to the plug-in's archive container.
    /// @return Reference to the MemoryArchiveContainer.
    virtual MemoryArchiveContainer& getArchives() = 0;
    /// @brief Return the memory archive of a specific type.
    /// @param type The archive type to retrieve.
    /// @return Pointer to the corresponding MemoryOut archive.
    virtual arch::MemoryOut* getArchive(IPlugin::ArchType) = 0;
    /// @brief Return the suggested output file name for persisting the archive.
    /// @return File name string.
    virtual td::String getOutFileName() const = 0;
    /// @brief Return the maximum number of archive parts this plug-in can produce.
    /// @return Maximum supported archive part count.
    virtual size_t getMaxSupportedArchiveParts() const = 0;
    /// @brief Return the model type handled by this plug-in.
    /// @return ModelType enum value.
    virtual ModelType getModelType() const = 0;
};

} //sc

