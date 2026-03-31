#include <iostream>
#include <thread>
#include <atomic>
#include <td/Types.h>
#include <syst/Keyboard.h>

using namespace std;

//few Global variables
namespace Global
{
bool Completed = false;
bool CancelFlag = false;
} //namespace Global


// Recursive Fibonacci with cancellation (using Global::CancelFlag)
td::LUINT8 fibRecursive(int n)
{
    if (Global::CancelFlag)
        return 0;
    if (n <= 1)
        return n;

    td::LUINT8 a = fibRecursive(n - 1);
    if (Global::CancelFlag)
        return 0;

    td::LUINT8 b = fibRecursive(n - 2);
    if (Global::CancelFlag)
        return 0;

    return a + b;
}



int main() 
{
    int n;
    cout << "Enter Fibonacci number to compute: ";
    cin >> n;

    atomic<bool> cancelFlag(false);
    long long result = 0;

    syst::Keyboard keyboard(100); // sleep 100 ms between checks

    thread worker([&]() {
        result = fibRecursive(n);
        Global::Completed = true;   //Computation done (with or without final result)
    });

    cout << "Press 'q' or 'Q' to quit computation..." << endl;

    int iSleeps = 0;
    while (!Global::Completed)
    {
        if (keyboard.keyPressed())
        {
            char ch = keyboard.readKey();
            if (ch == 'q' || ch == 'Q') 
            {
                Global::CancelFlag = true; //inform worker thread to stop computing
                cout << "\nCancelling computation..." << endl;
                break;
            }
        }
        keyboard.wait();
        ++iSleeps;
        if (iSleeps == 10) //print out dot each second
        {
            cout << ".";
            iSleeps = 0;
        }
    }

    worker.join();

    if (Global::CancelFlag)
        cout << "Computation stopped. Result = 0" << endl;
    else
        cout << "Fibonacci(" << n << ") = " << result << endl;

    return 0;
}
