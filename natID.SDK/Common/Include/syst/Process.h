// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Process.h
    @brief Provides utilities to query and launch operating-system processes. */
#pragma once
#include <mu/muLib.h>
#include <td/Types.h>
#include <td/String.h>
#include <cnt/SafeFullVector.h>

namespace syst
{

/// @brief Provides static methods for process management and execution.
class MAINUTILS_API Process
{
public:
    /// @brief Returns the process identifier (PID) of the current process.
    /// @return The PID as a size_t value.
    static size_t getID();

    /// @brief Executes an external program with a raw argument array.
    /// @param pathToExecutable Null-terminated path to the executable file.
    /// @param argv Null-terminated array of argument strings passed to the process.
    /// @return Exit code of the executed process, or -1 on failure.
    static int execute(const char* pathToExecutable, char* const* argv);

    /// @brief Executes an external program with a vector of string arguments.
    /// @param pathToExecutable Path to the executable file.
    /// @param argv Vector of argument strings passed to the process.
    /// @param bWait If true, blocks until the process completes.
    /// @return Exit code of the executed process, or -1 on failure.
    static int execute(const td::String& pathToExecutable, const cnt::SafeFullVector<td::String>& argv, bool bWait = true);

    /// @brief Restarts the current process with the same command-line arguments.
    /// @param argc Number of command-line arguments.
    /// @param argv Array of command-line argument strings.
    /// @return Exit code; should not return on success.
    static int restartCurrent(int argc, const char* argv[]);
};

} //namespace syst
