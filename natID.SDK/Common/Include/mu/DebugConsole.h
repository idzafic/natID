// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

// DebugConsole.h
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


 // @class DualStreamBuffer
 // @brief Stream buffer that outputs to multiple destinations

class DualStreamBuffer : public std::streambuf
{
private:
    std::streambuf* _originalBuffer;
    std::ofstream _logFile;
    std::string _logPath;
    std::mutex _mutex;
    bool _skipOriginalBuffer;
    
public:
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
    
    ~DualStreamBuffer()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_logFile.is_open())
            _logFile.close();
    }
    
    void attach()
    {
        // Set this as the new buffer for all standard streams
        std::cout.rdbuf(this);
        std::cerr.rdbuf(this);
        std::clog.rdbuf(this);
    }
    
    void detach()
    {
        // Restore original buffers
        std::cout.rdbuf(_originalBuffer);
        std::cerr.rdbuf(_originalBuffer);
        std::clog.rdbuf(_originalBuffer);
    }
    
protected:
    // Handle character output
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
    
    // Handle buffer sync
    int sync() override
    {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if (_originalBuffer && !_skipOriginalBuffer)
            _originalBuffer->pubsync();
        
        if (_logFile.is_open())
            _logFile.flush();
        
        return 0;
    }
    
    // Handle string output
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


// @class DebugConsole
// @brief Cross-platform console manager for GUI applications
 
class DebugConsole
{
    bool _initialized = false;
    bool _usingRAMDisk = false;
    bool _enableConsoleWindow = true;
    bool _runningFromIDE = false;
    std::string _appName;
    std::string _title;
    std::string _logPath;
    std::unique_ptr<DualStreamBuffer> _streamBuffer;
    
#ifdef MU_WINDOWS
    HWND _consoleWindow = nullptr;
    FILE* _stdoutFile = nullptr;
    FILE* _stderrFile = nullptr;
    FILE* _stdinFile = nullptr;
#else
    int _originalStdout = -1;
    int _originalStderr = -1;
    pid_t _terminalPid = -1;
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
    
    // @brief Constructor - creates console if MU_DEBUG is defined
    // @param appName Application name for console title/log file
    // @param enableConsoleWindow true to open a separate console window
     
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
    
    //
    // @brief Check if console was successfully initialized
    //
    bool isInitialized() const { return _initialized; }
    
    //
    // @brief Get log file path
    //
    const std::string& getLogPath() const { return _logPath; }
    
    //
    // @brief Check if using RAMDisk
    //
    bool isUsingRAMDisk() const { return _usingRAMDisk; }
    
    //
    // @brief Platform-specific console show/hide
    //
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
    
    //
    // @brief Clear console screen
    //
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
    
    //
    // @brief Write colored output (ANSI codes)
    //
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
    
    //
    // @brief Convenience methods for colored output
    //
    template<typename T> void writeInfo(const T& msg) { writeColored(msg, "36"); } // Cyan
    template<typename T> void writeSuccess(const T& msg) { writeColored(msg, "32"); } // Green
    template<typename T> void writeWarning(const T& msg) { writeColored(msg, "33"); } // Yellow
    template<typename T> void writeError(const T& msg) { writeColored(msg, "31"); } // Red
    
private:
    
    // ==================== Log Path Determination ====================
    
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

//
// @class GlobalConsoleManager
// @brief Singleton manager for global console access
//
class GlobalConsoleManager
{
public:
    static DebugConsole& initialize(const std::string& appName = "App", bool enableConsole = true)
    {
        static DebugConsole instance(appName, enableConsole);
        return instance;
    }
    
    static DebugConsole& getInstance()
    {
        return initialize();
    }
    
    static void log(const std::string& message)
    {
#ifdef MU_DEBUG
        std::cout << "[LOG] " << message << std::endl;
#endif
    }
    
    static void logError(const std::string& message)
    {
#ifdef MU_DEBUG
        getInstance().writeError("[ERROR] " + message + "\n");
#endif
    }
    
    static void logWarning(const std::string& message)
    {
#ifdef MU_DEBUG
        getInstance().writeWarning("[WARNING] " + message + "\n");
#endif
    }
    
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
