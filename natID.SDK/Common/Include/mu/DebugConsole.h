// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DebugConsole.h
    @brief Cross-platform debug console manager that opens a console window,
           tees standard output to a log file, and provides coloured log helpers. */
#pragma once
#include <mu/mu.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <atomic>
#include <vector>

#ifdef MU_WINDOWS
    #include <windows.h>
    #include <fileapi.h>
    #include <io.h>
    #include <fcntl.h>
#else
    #include <cstdlib>
    #include <unistd.h>
    #include <fcntl.h>
    #include <csignal>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <pthread.h>
#endif

namespace mu
{


/// @brief Stream buffer that outputs to both the original standard output and a log file simultaneously.
class DualStreamBuffer : public std::streambuf
{
private:
    std::streambuf* _originalBuffer; ///< Saved pointer to the original std::cout rdbuf.
    std::ofstream _logFile;          ///< Output file stream for the log file.
    std::string _logPath;            ///< Filesystem path of the log file.
    std::mutex _mutex;               ///< Mutex serialising concurrent writes.
    bool _skipOriginalBuffer;        ///< When true, output is sent only to the log file and not to the original buffer.

public:
    /// @brief Constructs the buffer, opens the log file, and saves the original stdout buffer.
    /// @param logPath            Path of the log file to create or append to.
    /// @param skipOriginalBuffer When true, bytes are not forwarded to the original stdout buffer.
    DualStreamBuffer(const std::string& logPath, bool skipOriginalBuffer = false)
        : _logPath(logPath), _skipOriginalBuffer(skipOriginalBuffer)
    {
        // Open log file
        _logFile.open(logPath, std::ios::app | std::ios::out);
        if (!_logFile.is_open())
            std::cerr << "Failed to open log file: " << logPath << std::endl;
        
        // Save original stdout buffer
        _originalBuffer = std::cout.rdbuf();
    }
    
    /// @brief Destructor. Flushes and closes the log file.
    ~DualStreamBuffer()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_logFile.is_open())
            _logFile.close();
    }

    /// @brief Redirects std::cout, std::cerr and std::clog to this buffer.
    void attach()
    {
        // Set this as the new buffer for all standard streams
        std::cout.rdbuf(this);
        std::cerr.rdbuf(this);
        std::clog.rdbuf(this);
    }

    /// @brief Restores std::cout, std::cerr and std::clog to the original buffer.
    void detach()
    {
        // Restore original buffers
        std::cout.rdbuf(_originalBuffer);
        std::cerr.rdbuf(_originalBuffer);
        std::clog.rdbuf(_originalBuffer);
    }

protected:
    /// @brief Called by the stream when the put-area is full; writes one character to both sinks.
    /// @param c The character to write (may be EOF to flush).
    /// @return The character written, or EOF on failure.
    int overflow(int c) override
    {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (c != EOF)
        {
            // Write to original stdout (console) only if not skipping
            if (_originalBuffer && !_skipOriginalBuffer)
                _originalBuffer->sputc(c);
            
            // Write to log file
            if (_logFile.is_open())
                _logFile.put(static_cast<char>(c));
        }
        return c;
    }
    
    /// @brief Flushes both the original buffer and the log file.
    /// @return 0 on success, -1 on failure.
    int sync() override
    {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (_originalBuffer && !_skipOriginalBuffer)
            _originalBuffer->pubsync();
        
        if (_logFile.is_open())
            _logFile.flush();
        
        return 0;
    }
    
    /// @brief Writes a block of characters to both the original buffer and the log file.
    /// @param s Pointer to the character data.
    /// @param n Number of characters to write.
    /// @return Number of characters written.
    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        std::lock_guard<std::mutex> lock(_mutex);
        
        // Write to original stdout only if not skipping
        if (_originalBuffer && !_skipOriginalBuffer)
            _originalBuffer->sputn(s, n);
        
        // Write to log file
        if (_logFile.is_open()) {
            _logFile.write(s, n);
            _logFile.flush();
        }
        
        return n;
    }
};


/// @brief Cross-platform debug console manager that opens a console window and redirects standard output to a log file.
class DebugConsole
{
    bool _initialized = false;          ///< True once the console was successfully set up.
    bool _usingRAMDisk = false;         ///< True when the log file was placed on a RAM disk.
    bool _enableConsoleWindow = true;   ///< Whether to open a dedicated console window.
    bool _runningFromIDE = false;       ///< True when the process is detected to run inside an IDE.
    std::string _appName;               ///< Application name used for the window title and log file name.
    std::string _title;                 ///< Full console window title string.
    std::string _logPath;               ///< Resolved filesystem path of the log file.
    std::unique_ptr<DualStreamBuffer> _streamBuffer; ///< Dual-sink stream buffer that tees output to the log file.

#ifdef MU_WINDOWS
    HWND _consoleWindow = nullptr; ///< Handle to the allocated console window (Windows only).
    FILE* _stdoutFile = nullptr;   ///< Reopened stdout FILE* (Windows only).
    FILE* _stderrFile = nullptr;   ///< Reopened stderr FILE* (Windows only).
    FILE* _stdinFile  = nullptr;   ///< Reopened stdin FILE* (Windows only).
#else
    int    _originalStdout = -1;  ///< Saved file descriptor for stdout (POSIX only).
    int    _originalStderr = -1;  ///< Saved file descriptor for stderr (POSIX only).
    pid_t  _terminalPid    = -1;  ///< PID of the spawned terminal process (POSIX only).
#endif
    
private:
#ifdef MU_WINDOWS
    bool SetConsoleOutputUTF8()
    {
        // Set console output code page to UTF-8
        SetConsoleOutputCP(CP_UTF8);

        //Also set input code page to UTF-8
        //SetConsoleCP(CP_UTF8);

        // Enable ANSI escape codes
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        if (GetConsoleMode(hConsole, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, dwMode);
        }

        // Use _setmode for CRT functions
        _setmode(_fileno(stdout), _O_U8TEXT);
        _setmode(_fileno(stderr), _O_U8TEXT);
        //_setmode(_fileno(stdin), _O_U8TEXT);

        // Verify the code page was set
        UINT currentCP = GetConsoleOutputCP();
        if (currentCP != CP_UTF8)
        {
            std::cerr << "Warning: Failed to set console to UTF-8. Current code page: " << currentCP << std::endl;
            return false;
        }

        return true;
    }
#endif
public:
    
    /// @brief Constructs and (when MU_DEBUG is defined) initialises the debug console.
    /// @param appName             Application name used for the window title and log-file base name.
    /// @param enableConsoleWindow When true a dedicated console window is opened; false uses logging only.
    explicit DebugConsole(const std::string& appName = "App", bool enableConsoleWindow = true)
    {
#ifdef MU_DEBUG
        _appName = appName;
        _enableConsoleWindow = enableConsoleWindow;
        _title = appName + " - Debug Console";
        _logPath = determineLogPath();
        
        if (initializeConsole())
        {
            _initialized = true;
            mu::initDebugConsole();
            
            // Create and attach dual stream buffer
            // If running from IDE, skip writing to original buffer (IDE's console)
            // and only write to log file (which our Terminal is tailing)
            _streamBuffer = std::make_unique<DualStreamBuffer>(_logPath, _runningFromIDE);
            _streamBuffer->attach();
            
            // Welcome message
            std::cout << "\n═══════════════════════════════════════════════════\n";
            std::cout << "  " << _title << "\n";
            std::cout << "  Log file: " << _logPath << "\n";
            if (_usingRAMDisk) {
                std::cout << "  ✓ Using RAMDisk for optimal performance\n";
            }
            if (_runningFromIDE) {
                std::cout << "  ✓ Running from IDE - output redirected to Terminal\n";
            }
            std::cout << "═══════════════════════════════════════════════════\n\n";
            
            // Flush immediately
            std::cout.flush();
        }
#endif
    }
    
    /// @brief Destructor. Prints a farewell message, detaches the stream buffer, and releases the console.
    ~DebugConsole()
    {
#ifdef MU_DEBUG
        if (_initialized)
        {
            // Farewell message
            std::cout << "\n═══════════════════════════════════════════════════\n";
            std::cout << "  Console session ended\n";
            std::cout << "  Log saved to: " << _logPath << "\n";
            std::cout << "═══════════════════════════════════════════════════\n";
            std::cout.flush();
            
            // Detach stream buffer
            if (_streamBuffer) {
                _streamBuffer->detach();
                _streamBuffer.reset();
            }
            
            // Platform-specific cleanup
            cleanupConsole();
        }
#endif
    }
    
    // Delete copy constructor and assignment operator
    DebugConsole(const DebugConsole&) = delete;
    DebugConsole& operator=(const DebugConsole&) = delete;
    
    /// @brief Returns true when the console was successfully initialised.
    /// @return True if initialisation succeeded.
    bool isInitialized() const { return _initialized; }

    /// @brief Returns the filesystem path of the log file.
    /// @return Constant reference to the log-file path string.
    const std::string& getLogPath() const { return _logPath; }

    /// @brief Returns true when the log file resides on a RAM disk.
    /// @return True if a RAM disk location was found and used.
    bool isUsingRAMDisk() const { return _usingRAMDisk; }

    /// @brief Shows or hides the console window (Windows only; no-op on other platforms).
    /// @param show True to show the window, false to hide it.
    void showConsole(bool show) {
#ifdef MU_DEBUG
#ifdef MU_WINDOWS
        if (_consoleWindow) {
            ShowWindow(_consoleWindow, show ? SW_SHOW : SW_HIDE);
        }
#endif
        // macOS/Linux consoles can't be hidden programmatically
#endif
    }
    
    /// @brief Clears the console screen (MU_DEBUG builds only).
    void clearConsole()
    {
#ifdef MU_DEBUG
#ifdef MU_WINDOWS
        system("cls");
#else
        system("clear");
#endif
#endif
    }
    
    /// @brief Writes a message to std::cout with an optional ANSI colour code.
    /// @tparam T        Type of the message value (must be streamable).
    /// @param msg       The message to print.
    /// @param colorCode ANSI colour escape code string (e.g. "31" for red), or nullptr for default colour.
    template<typename T>
    void writeColored(const T& msg, const char* colorCode = nullptr) // Default txt color
    {
#ifdef MU_DEBUG
        if (colorCode)
            std::cout << "\033[" << colorCode << "m" << msg << "\033[0m";
        else
            std::cout << msg;
        std::cout.flush();
#endif
    }
    
    /// @brief Writes an informational message in cyan.
    /// @tparam T Type of the message value.
    /// @param msg The message to print.
    template<typename T> void writeInfo(const T& msg) { writeColored(msg, "36"); } // Cyan
    /// @brief Writes a success message in green.
    /// @tparam T Type of the message value.
    /// @param msg The message to print.
    template<typename T> void writeSuccess(const T& msg) { writeColored(msg, "32"); } // Green
    /// @brief Writes a warning message in yellow.
    /// @tparam T Type of the message value.
    /// @param msg The message to print.
    template<typename T> void writeWarning(const T& msg) { writeColored(msg, "33"); } // Yellow
    /// @brief Writes an error message in red.
    /// @tparam T Type of the message value.
    /// @param msg The message to print.
    template<typename T> void writeError(const T& msg) { writeColored(msg, "31"); } // Red
    
private:
    
    // ==================== Log Path Determination ====================

    /// @brief Determines the best available log-file path, preferring RAM-disk locations.
    /// @return Absolute or relative path string for the log file.
    std::string determineLogPath()
    {
        std::string fileName = sanitizeFileName(_appName) + ".log";
        std::string ramDiskPath;
        
        // Check RAMDisk locations first
#ifdef MU_WINDOWS
        const char* windowsRamDisks[] = {"R:\\", "X:\\", "Z:\\", "T:\\"};
        for (const char* drive : windowsRamDisks)
        {
            if (testWriteAccess(drive))
            {
                ramDiskPath = std::string(drive) + fileName;
                _usingRAMDisk = true;
                break;
            }
        }
        // Fallback to current directory
        return _usingRAMDisk ? ramDiskPath : fileName;
        
#elif defined(__APPLE__)
        const char* macRamDisks[] = {
            "/Volumes/RAMDisk/",
            "/Volumes/RAM/",
            "/Volumes/RamDisk/",
            "/tmp/ramdisk/"
        };
        for (const char* path : macRamDisks)
        {
            if (testWriteAccess(path))
            {
                ramDiskPath = std::string(path) + fileName;
                _usingRAMDisk = true;
                break;
            }
        }
        return _usingRAMDisk ? ramDiskPath : "/tmp/" + fileName;
        
#elif defined(__linux__)
        const char* linuxRamDisks[] = {
            "/media/RAMDisk/",
            "/media/RAM/",
            "/ramdisk/",
            "/dev/shm/",
            "/run/shm/",
            "/mnt/ramdisk/"
        };
        for (const char* path : linuxRamDisks)
        {
            if (testWriteAccess(path))
            {
                ramDiskPath = std::string(path) + fileName;
                _usingRAMDisk = true;
                break;
            }
        }
        return _usingRAMDisk ? ramDiskPath : "/tmp/" + fileName;
#endif
    }
    
    /// @brief Tests whether the given directory path is writable by creating and deleting a temporary file.
    /// @param path Directory path to test (must end with a path separator).
    /// @return True if the directory exists and a file could be created in it.
    bool testWriteAccess(const std::string& path)
    {
        // Check if directory exists and is writable
#ifdef MU_WINDOWS
        DWORD attrs = GetFileAttributesA(path.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) return false;
        if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) return false;
        
        // Try to create a test file
        std::string testFile = path + "test_access.tmp";
        HANDLE hFile = CreateFileA(testFile.c_str(), GENERIC_WRITE, 0, NULL,
                                   CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return false;
        
        CloseHandle(hFile);
        DeleteFileA(testFile.c_str());
        return true;
#else
        struct stat info;
        if (stat(path.c_str(), &info) != 0)
            return false;
        if (!S_ISDIR(info.st_mode))
            return false;
        if (access(path.c_str(), W_OK) != 0)
            return false;
        
        // Additional check for tmpfs (RAM disk) on Linux/macOS
#ifndef MU_WINDOWS
        std::string testFile = path + "test_access.tmp";
        FILE* test = fopen(testFile.c_str(), "w");
        if (!test)
            return false;
        fclose(test);
        std::remove(testFile.c_str());
        
        // On Linux, check if mounted as tmpfs
#ifdef __linux__
        std::string cmd = "mount | grep -q '\"" + path + "\".*tmpfs'";
        if (system(cmd.c_str()) == 0)
            return true;
#endif
        return true;
#endif
#endif
    }
    
    /// @brief Converts an arbitrary application name into a filesystem-safe filename stem.
    /// @param name Input name string (may contain spaces, dots, or other special characters).
    /// @return A sanitised string containing only alphanumerics, underscores, and hyphens.
    std::string sanitizeFileName(const std::string& name)
    {
        std::string sanitized;
        for (char c : name) {
            if (isalnum(c) || c == '_' || c == '-')
                sanitized += c;
            else if (c == ' ' || c == '.')
                sanitized += '_';
        }
        if (sanitized.empty())
            sanitized = "app_log";
        return sanitized;
    }
    
    // ==================== Platform-Specific Console Initialization ====================

    /// @brief Dispatches to the platform-specific console initialisation routine.
    /// @return True if initialisation succeeded (or MU_DEBUG is not defined).
    bool initializeConsole()
    {
#ifdef MU_DEBUG
#ifdef MU_WINDOWS
        return initializeWindowsConsole();
#elif MU_MACOS //defined(__APPLE__)
        return initializeMacConsole();
#elif defined(__linux__)
        return initializeLinuxConsole();
#endif
#else
        return false;
#endif
    }
    
    /// @brief Allocates a Windows console, sets UTF-8 encoding, and redirects standard I/O.
    /// @return True on success, false if the console could not be created or redirected.
    bool initializeWindowsConsole()
    {
#ifdef MU_WINDOWS
        if (!_enableConsoleWindow)
        {
            // Just set up logging without console window
            return true;
        }
        
        // Check if console already exists (e.g., running from command line)
        bool consoleExists = (GetConsoleWindow() != NULL);
        
        if (!consoleExists)
        {
            // Allocate new console for GUI application
            if (!AllocConsole()) {
                DWORD error = GetLastError();
                if (error == ERROR_ACCESS_DENIED)
                {
                    // Console already exists (unlikely but possible)
                    consoleExists = true;
                }
                else
                {
                    // Failed to create console
                    return false;
                }
            }
            
            // Set console title
            SetConsoleTitle(_title.c_str());
            _consoleWindow = GetConsoleWindow();
            
            // Make console window a reasonable size
            HWND console = GetConsoleWindow();
            RECT rect;
            GetWindowRect(console, &rect);
            MoveWindow(console, rect.left, rect.top, 800, 600, TRUE);
            
            // Enable ANSI escape codes (Windows 10+)
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            if (GetConsoleMode(hConsole, &dwMode))
            {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hConsole, dwMode);
            }
            
            // Enable mouse input (optional)
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            if (GetConsoleMode(hIn, &dwMode))
            {
                dwMode |= ENABLE_MOUSE_INPUT;
                SetConsoleMode(hIn, dwMode);
            }

            // Set UTF-8 encoding for the console
            SetConsoleOutputUTF8();
        }
        
        // Redirect standard I/O to console
        if (!consoleExists)
        {
            // Reopen stdin, stdout, stderr
            freopen_s(&_stdinFile, "CONIN$", "r", stdin);
            freopen_s(&_stdoutFile, "CONOUT$", "w", stdout);
            freopen_s(&_stderrFile, "CONOUT$", "w", stderr);
        }

        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE**)stdin, "CONIN$", "r", stdin);

        
        // Clear any error flags
        std::ios::sync_with_stdio(true);
        std::cin.clear();
        std::cout.clear();
        std::cerr.clear();
        std::clog.clear();
        
        return true;
#endif
        return false;
    }
    
    /// @brief Opens a macOS Terminal window that tails the log file and detects IDE execution.
    /// @return True on success or when running in a real terminal, false if the log file could not be created.
    bool initializeMacConsole()
    {
#ifdef MU_MACOS
        if (!_enableConsoleWindow) {
            // Just set up logging without new terminal window
            return true;
        }
        
        // Check if running from Xcode by looking at environment variables
        const char* xcode_colors = getenv("XcodeColors");
        const char* xcode_version = getenv("__XCODE_BUILT_PRODUCTS_DIR_PATHS");
        const char* ide_disabled = getenv("IDE_DISABLED_OS_ACTIVITY_DT_MODE");
        
        if (xcode_colors || xcode_version || ide_disabled)
            _runningFromIDE = true;
        
        // Only skip terminal creation if we're in a real terminal (not Xcode)
        if (isatty(STDOUT_FILENO) && !_runningFromIDE)
        {
            // Already running in a real terminal, no need to open new one
            return true;
        }
        
        // Create log file first (empty it if it exists)
        std::ofstream logFile(_logPath, std::ios::trunc);
        if (!logFile.is_open())
            return false;
        
        logFile.close();
        
        // Build AppleScript command to open Terminal and tail the log file
        std::string appleScript =
            "tell application \"Terminal\"\n"
            "    do script \"tail -f '" + _logPath + "'; exit\"\n"
            "    set custom title of front window to \"" + _title + "\"\n"
            "    activate\n"
            "end tell";
        
        // Launch terminal in background using fork
        pid_t pid = fork();
        if (pid == -1)
            return false;
        
        if (pid == 0)
        {
            // Child process - launch terminal
            setsid(); // Create new session
            
            // Execute AppleScript
            execlp("osascript", "osascript", "-e", appleScript.c_str(), (char*)NULL);
            
            // If exec fails
            exit(1);
        }
        else
        {
            // Parent process
            _terminalPid = pid;
            
            // Give terminal time to start
            usleep(500000); // 500ms
        }
        
        return true;
#endif
        return false;
    }
    
    /// @brief Opens a Linux terminal emulator that tails the log file and detects IDE execution.
    /// @return True on success, false if no terminal could be found or the log file could not be created.
    bool initializeLinuxConsole()
    {
#ifndef MU_WINDOWS
        if (!_enableConsoleWindow)
        {
            // Just set up logging without new terminal window
            return true;
        }
        
        // Check if running from an IDE
        const char* term_program = getenv("TERM_PROGRAM");
        const char* clion_vm = getenv("CLION_VM_OPTIONS");
        const char* idea_initial = getenv("IDEA_INITIAL_DIRECTORY");
        const char* qtcreator = getenv("QTCREATOR_PID");
        const char* vscode_ipc = getenv("VSCODE_IPC_HOOK_CLI");
        
        if (term_program || clion_vm || idea_initial || qtcreator || vscode_ipc)
            _runningFromIDE = true;
        
        // Only skip terminal creation if we're in a real terminal (not IDE)
        if (isatty(STDOUT_FILENO) && !_runningFromIDE)
            return true;
        
        // Check DISPLAY environment variable (X11) or WAYLAND_DISPLAY
        const char* display = getenv("DISPLAY");
        const char* wayland = getenv("WAYLAND_DISPLAY");
        if ((!display || display[0] == '\0') && (!wayland || wayland[0] == '\0'))
        {
            // No GUI environment, can't open terminal
            return true;
        }
        
        // Create log file first (empty it if it exists)
        std::ofstream logFile(_logPath, std::ios::trunc);
        if (!logFile.is_open())
            return false;
    
        logFile.close();
        
        // Try to find an available terminal emulator
        const char* terminals[] = {
            "gnome-terminal", "konsole", "xfce4-terminal",
            "mate-terminal", "xterm", "terminator", "alacritty"
        };
        
        std::string terminalCmd;
        std::string terminal;
        
        for (const char* term : terminals)
        {
            std::string checkCmd = "command -v " + std::string(term) + " > /dev/null 2>&1";
            if (system(checkCmd.c_str()) == 0)
            {
                terminal = term;
                
                // Build command based on terminal type
                if (terminal == "gnome-terminal")
                {
                    terminalCmd = terminal + " --title=\"" + _title +
                                 "\" -- bash -c \"tail -f '" + _logPath + "'; exec bash\"";
                }
                else if (terminal == "konsole")
                {
                    terminalCmd = terminal + " --caption \"" + _title +
                                 "\" -e bash -c \"tail -f '" + _logPath + "'; exec bash\"";
                }
                else if (terminal == "xfce4-terminal")
                {
                    terminalCmd = terminal + " --title=\"" + _title +
                                 "\" -e \"bash -c 'tail -f " + _logPath + "; exec bash'\"";
                }
                else if (terminal == "xterm")
                {
                    terminalCmd = terminal + " -title \"" + _title +
                                 "\" -e bash -c \"tail -f '" + _logPath + "'; exec bash\"";
                }
                else if (terminal == "alacritty")
                {
                    terminalCmd = terminal + " --title \"" + _title +
                                 "\" -e bash -c \"tail -f '" + _logPath + "'; exec bash\"";
                }
                else
                    terminalCmd = terminal + " -e \"bash -c 'tail -f " + _logPath + "; exec bash'\"";
                break;
            }
        }
        
        if (!terminalCmd.empty())
        {
            // Launch terminal in background
            pid_t pid = fork();
            if (pid == -1)
                return false;
            
            if (pid == 0)
            {
                // Child process
                setsid(); // Create new session
                
                // Execute terminal command
                execl("/bin/sh", "sh", "-c", terminalCmd.c_str(), (char*)NULL);
                
                // If exec fails
                exit(1);
            }
            else
            {
                // Parent process
                _terminalPid = pid;
                
                // Give terminal time to start
                usleep(500000); // 500ms
            }
        }
        
        return true;
#endif
        return false;
    }
    
    // ==================== Cleanup ====================
    /// @brief Releases platform-specific console resources and restores the original I/O streams.
    void cleanupConsole()
    {
#ifdef MU_DEBUG
#ifdef MU_WINDOWS
        // Close file handles
        if (_stdoutFile)
            fclose(_stdoutFile);
        if (_stderrFile)
            fclose(_stderrFile);
        if (_stdinFile)
            fclose(_stdinFile);
        
        // Only free console if we created it
        if (_consoleWindow && !GetConsoleWindow())
            FreeConsole();
        
#else
        // Restore original stdout/stderr if they were saved
        if (_originalStdout != -1)
        {
            dup2(_originalStdout, STDOUT_FILENO);
            close(_originalStdout);
        }
        if (_originalStderr != -1)
        {
            dup2(_originalStderr, STDERR_FILENO);
            close(_originalStderr);
        }
        
        // Note: We don't kill the terminal process because:
        // 1. The terminal is showing tail -f which will exit when the app closes
        // 2. Users may want to review the log after the app closes
        // If you want to force-close the terminal, uncomment:
        // if (_terminalPid > 0) {
        //     kill(_terminalPid, SIGTERM);
        // }
#endif
        mu::cleanDebugConsole();
#endif
    }
};

/// @brief Singleton manager providing global access to a shared DebugConsole instance and convenience logging methods.
class GlobalConsoleManager
{
public:
    /// @brief Creates (on first call) or returns the shared DebugConsole instance.
    /// @param appName       Application name forwarded to DebugConsole on first call.
    /// @param enableConsole Whether to open a console window on first call.
    /// @return Reference to the singleton DebugConsole.
    static DebugConsole& initialize(const std::string& appName = "App", bool enableConsole = true)
    {
        static DebugConsole instance(appName, enableConsole);
        return instance;
    }
    
    /// @brief Returns the singleton DebugConsole, initialising it with default parameters if needed.
    /// @return Reference to the singleton DebugConsole.
    static DebugConsole& getInstance()
    {
        return initialize();
    }

    /// @brief Prints a plain informational log message prefixed with "[LOG]".
    /// @param message The message text to log.
    static void log(const std::string& message)
    {
#ifdef MU_DEBUG
        std::cout << "[LOG] " << message << std::endl;
#endif
    }

    /// @brief Prints an error message in red, prefixed with "[ERROR]".
    /// @param message The error message text to log.
    static void logError(const std::string& message)
    {
#ifdef MU_DEBUG
        getInstance().writeError("[ERROR] " + message + "\n");
#endif
    }

    /// @brief Prints a warning message in yellow, prefixed with "[WARNING]".
    /// @param message The warning message text to log.
    static void logWarning(const std::string& message)
    {
#ifdef MU_DEBUG
        getInstance().writeWarning("[WARNING] " + message + "\n");
#endif
    }

    /// @brief Prints a success message in green, prefixed with "[OK]".
    /// @param message The success message text to log.
    static void logSuccess(const std::string& message)
    {
#ifdef MU_DEBUG
        getInstance().writeSuccess("[OK] " + message + "\n");
#endif
    }
    
private:
    GlobalConsoleManager() = delete;
    ~GlobalConsoleManager() = delete;
};

} //namespace mu
