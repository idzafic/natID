// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Keyboard.h
    @brief Provides cross-platform non-blocking keyboard input polling. */
#pragma once

#include <chrono>
#include <thread>

#ifdef MU_WINDOWS
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace syst
{

/// @brief Cross-platform utility class for non-blocking keyboard input detection.
class Keyboard
{
    int _sleepMs = 0; ///< Poll interval in milliseconds used by wait().
#ifndef MU_WINDOWS
    unsigned char _ch = 0; ///< Last character read on POSIX platforms.
#endif
public:
    /// @brief Constructs a Keyboard with a specified polling interval.
    /// @param pollIntervalInMilliseconds Duration in milliseconds to sleep between polls.
    explicit Keyboard(int pollIntervalInMilliseconds = 50)
        : _sleepMs(pollIntervalInMilliseconds)
    {
#ifndef MU_WINDOWS
        configureTerminal(true);
#endif
    }

    /// @brief Destructor; restores original terminal settings on POSIX platforms.
    ~Keyboard()
    {
#ifndef MU_WINDOWS
        configureTerminal(false);
#endif
    }

    // Returns true if a key was pressed
    /// @brief Checks whether a key has been pressed without blocking.
    /// @return true if a key is available; false otherwise.
    bool keyPressed()
    {
#ifdef MU_WINDOWS
        return _kbhit();
#else
        if (read(STDIN_FILENO, &_ch, 1) > 0)
        {
            return true;
        }
        return false;
#endif
    }

    // Reads and returns a key if pressed
    /// @brief Reads the most recently pressed key character.
    /// @return The character code of the pressed key, or 0 if none is available.
    char readKey()
    {
#ifdef MU_WINDOWS
        return _getch();
#else
        return _ch;
//        unsigned char ch;
//        if (read(STDIN_FILENO, &ch, 1) > 0)
//            return ch;
//        return 0;
#endif
    }

    // Sleep for the configured duration
    /// @brief Sleeps for the poll interval specified at construction.
    void wait() const
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_sleepMs));
    }

private:
#ifndef MU_WINDOWS
    /// @brief Configures or restores the terminal for raw, non-blocking input.
    /// @param enable true to enable raw non-blocking mode; false to restore saved settings.
    void configureTerminal(bool enable)
    {
        static struct termios oldt;
        static bool configured = false;

        if (enable && !configured)
        {
            struct termios newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

            configured = true;
        }
        else if (!enable && configured)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            configured = false;
        }
    }
#endif
};

} //namespace syst
