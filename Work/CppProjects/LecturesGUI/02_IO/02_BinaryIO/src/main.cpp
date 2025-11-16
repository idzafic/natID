#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

void writeData(const char* fileName)
{
    std::ofstream f;
    f.open(fileName, std::ios::binary | std::ios::out);
    if (f.is_open())
    {
        std::string str("Howdy! How are you?");
        float flt = 15.1234f;
        int x = 1234567;
        double pi = std::asin(1.) * 2;
        f.write((const char*) &x, sizeof(int));
        f.write((const char*) &flt, sizeof(float));
        size_t strLen = str.length();
        //upisivanje duzine stringa
        f.write((const char*) &strLen, sizeof(size_t));
        //upis sadrzaja stringa
        f.write(str.c_str(), strLen);
        
        f.write((const char*) &pi, sizeof(double));
        f.flush();
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
        f.read((char*) &x, sizeof(int));
        f.read((char*) &flt, sizeof(float));
        //ucitanje duzine stringa
        size_t strLen;
        f.read((char*) &strLen, sizeof(size_t));
        char* tmp = new char[strLen + 1];
        
        f.read(tmp, strLen);
        tmp[strLen] = '\0';
        str = tmp;
        delete [] tmp;
        
        f.read((char*) &pi, sizeof(double));
        
        std::cout << x << " " << flt << " " << str << " " << pi << std::endl;
    }
}

int main()
{
    const char* fileName = "/Volumes/Ramdisk/Test.bin";
    writeData(fileName);
    readData(fileName);
    return 0; 
}
