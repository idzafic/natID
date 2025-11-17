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

class Keyboard 
{
    int _sleepMs = 0;
#ifndef MU_WINDOWS
    unsigned char _ch = 0;
#endif
public:
    explicit Keyboard(int pollIntervalInMilliseconds = 50)
        : _sleepMs(pollIntervalInMilliseconds)
    {
#ifndef MU_WINDOWS
        configureTerminal(true);
#endif
    }

    ~Keyboard() 
    {
#ifndef MU_WINDOWS
        configureTerminal(false);
#endif
    }

    // Returns true if a key was pressed
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
    void wait() const 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_sleepMs));
    }

private:
#ifndef MU_WINDOWS
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
