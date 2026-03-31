#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

void writeData(const char* fileName)
{
    std::ofstream f;
    f.open(fileName);
    if (f.is_open())
    {
        std::string str("Howdy! How are you?");
        float flt = 15.1234f;
        int x = 1234567;
        double pi = std::asin(1.) * 2;
        f << x << " " << flt << " " << str << " " << pi << std::endl;
    }
}

void readData(const char* fileName)
{
    std::ifstream f;
    f.open(fileName);
    if (f.is_open())
    {
        std::string str;
        float flt;
        int x;
        double pi;
        f >> x >> flt >> str >> pi;
        std::cout << x << " " << flt << " " << str << " " << pi << std::endl;
    }
}

int main()
{
    const char* fileName = "/Volumes/Ramdisk/Test.txt";
    writeData(fileName);
    readData(fileName);
    return 0; 
}
