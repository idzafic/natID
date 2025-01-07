// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <rnd/MinMax.h>
#include <random>
#include <algorithm>    // std::random_shuffle

namespace rnd
{	
template <size_t size>
inline void generate(td::BYTE min, td::BYTE max, td::BYTE(&buff)[size], bool putTrailingZero = false)
{
    rnd::MinMax<int> rndGen(min, max);
    for (size_t i = 0; i < size; ++i)
        buff[i] = (td::BYTE) rndGen.next();
    if (putTrailingZero)
        buff[size-1] = 0;
}
    
inline td::String generate(td::BYTE min, td::BYTE max, size_t size)
{
    td::String str;
    str.reserve(size);
    td::BYTE* pBuff = (td::BYTE*) str.begin();
    rnd::MinMax<int> rndGen(min, max);
    for (size_t i = 0; i < size; ++i)
        pBuff[i] = (td::BYTE) rndGen.next();
    return str;
}

inline void generate(td::BYTE min, td::BYTE max, td::BYTE* pBuff, size_t size)
{
    rnd::MinMax<int> rndGen(min, max);
    for (size_t i = 0; i < size; ++i)
        pBuff[i] = (td::BYTE) rndGen.next();
}

inline void genAlphaNumerics(char* pBuff, size_t nLen)
{
    size_t cMin = '0';
    size_t cMax = 'z';
    rnd::MinMax<size_t> gen(cMin, cMax);
    size_t i = 0;
    while (i < nLen)
    {
        char ch = (char) gen.next();
        do
        {
            if (((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')))
                break;
            ch = (char) gen.next();
        } while (true);
        pBuff[i++] = ch;
    }
}

template< class RandomIt >
inline void random_shuffle( RandomIt first, RandomIt last )
{
    typename std::iterator_traits<RandomIt>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i) {
        using std::swap;
        swap(first[i], first[std::rand() % (i+1)]);
        // rand() % (i+1) isn't actually correct, because the generated number
        // is not uniformly distributed for most values of i. A correct implementation
        // will need to essentially reimplement C++11 std::uniform_int_distribution,
        // which is beyond the scope of this example.
    }
}
template <typename T>
inline void populateWithUniqueRnds(std::vector<T>& rnds, T nToGen)
{
    if (nToGen <= 0)
        return;
    rnds.reserve(nToGen);
    for (T i = 0; i < nToGen; ++i)
    {
        rnds.push_back(i);
    }

    // using built-in random generator (BAD)
    //std::random_shuffle(rnds.begin(), rnds.end());
    rnd::random_shuffle(rnds.begin(), rnds.end());

    ////check that all are unique
    //for (int i = 0; i < nToGen; ++i)
    //{
    //	int iFound = 0;
    //	for (int j = 0; j < nToGen; ++j)
    //	{
    //		if (rnds[j] == i)
    //			++iFound;
    //	}
    //	assert(iFound == 1);
    //}
    //int g = 5;
}

inline void genString(char* pBuff, size_t strLen, bool terminateWithZero = false)
{
    const size_t CHARACTERSLEN = 87;
    const char CHARACTERS[CHARACTERSLEN] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-.,:;*|/@!_&%$#<>(){}[]";
    //const CHARACTERS
    //auto chLen = strlen(CHARACTERS);
    std::random_device random_device;
    std::mt19937 generator(random_device());
    //std::uniform_int_distribution<> distribution(0, chLen - 1);
    std::uniform_int_distribution<> distribution(0, CHARACTERSLEN-1);

    for (size_t i = 0; i < strLen; ++i)
    {
        pBuff[i] = CHARACTERS[distribution(generator)];
    }
    if (terminateWithZero)
        pBuff[strLen] = 0;
}

}
