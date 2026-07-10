// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Utils.h
 *  @brief Random-generation utility functions: byte buffer filling, alphanumeric string generation, shuffling, and unique-random population.
 */
#pragma once
#include <td/String.h>
#include <rnd/MinMax.h>
#include <random>
#include <algorithm>    // std::random_shuffle

namespace rnd
{
/// @brief Fills a fixed-size byte array with random values in the range [min, max].
/// @tparam size            Compile-time size of the output buffer.
/// @param min              Minimum byte value (inclusive).
/// @param max              Maximum byte value (inclusive).
/// @param buff             Output array to fill with random bytes.
/// @param putTrailingZero  If true, the last byte is set to zero (useful for C-string termination).
template <size_t size>
inline void generate(td::BYTE min, td::BYTE max, td::BYTE(&buff)[size], bool putTrailingZero = false)
{
    rnd::MinMax<int> rndGen(min, max);
    for (size_t i = 0; i < size; ++i)
        buff[i] = (td::BYTE) rndGen.next();
    if (putTrailingZero)
        buff[size-1] = 0;
}

/// @brief Generates a td::String of the requested size filled with random byte values in [min, max].
/// @param min   Minimum byte value (inclusive).
/// @param max   Maximum byte value (inclusive).
/// @param size  Number of random bytes to generate.
/// @return A td::String whose internal buffer contains size random bytes.
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

/// @brief Fills an existing byte buffer with random values in the range [min, max].
/// @param min    Minimum byte value (inclusive).
/// @param max    Maximum byte value (inclusive).
/// @param pBuff  Pointer to the output buffer (must be at least size bytes).
/// @param size   Number of random bytes to write.
inline void generate(td::BYTE min, td::BYTE max, td::BYTE* pBuff, size_t size)
{
    rnd::MinMax<int> rndGen(min, max);
    for (size_t i = 0; i < size; ++i)
        pBuff[i] = (td::BYTE) rndGen.next();
}

/// @brief Fills a character buffer with randomly selected alphanumeric characters (0-9, A-Z, a-z).
/// @param pBuff  Pointer to the output character buffer (must hold at least nLen bytes).
/// @param nLen   Number of alphanumeric characters to write; no null terminator is appended.
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

/// @brief Performs a Fisher-Yates shuffle on the range [first, last) using std::rand().
/// @tparam RandomIt Random-access iterator type.
/// @param first     Iterator to the first element.
/// @param last      Iterator one past the last element.
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

/// @brief Populates a vector with the integers [0, nToGen) in a randomly shuffled order (all values unique).
/// @tparam T       Integral type for the generated values and the count parameter.
/// @param rnds     Output vector that receives the shuffled unique values.
/// @param nToGen   Number of unique values to generate; the vector will have exactly nToGen elements.
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

/// @brief Fills a character buffer with random printable characters drawn from a predefined set of 87 symbols.
/// @param pBuff            Pointer to the output character buffer (must hold at least strLen bytes, plus one if terminateWithZero is true).
/// @param strLen           Number of random characters to write.
/// @param terminateWithZero If true, a null terminator is appended at position strLen.
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
