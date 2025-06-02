// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#ifdef MU_USE_MEM_LEAK
#include <mu/muLib.h>

#ifdef MU_DEBUG
#if defined(MAINUTILS_EXPORTS) || defined(MU_SHOW_MEM_LEAKS)
#ifndef MU_WINDOWS
#include <cstdlib>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include <functional>
#include <tuple>
#include <optional>
#include <new>
#include <stack>
#include <queue>
#include <regex>

// #include <iostream>   // std::cout
//    // std::malloc, std::free
// #include <new>        // Overloading operator new
// #include <thread>     // std::this_thread::get_id
// #include <chrono>     // std::chrono::system_clock
// #include <ctime>      // std::ctime
// #include <memory>  // Ensure this is included when using std::addressof

// #include <map>
// #include <unordered_map>
// #include <unordered_set>

//#ifdef __cplusplus
//extern "C" {
//#endif

MAINUTILS_API void* dbgLeakMalloc(size_t size, const char* file, int line);
MAINUTILS_API void* dbgLeakCalloc(size_t num, size_t size, const char* file, int line);
MAINUTILS_API void* dbgLeakRealloc(void* ptr, size_t size, const char* file, int line);
MAINUTILS_API void dbgLeakFree2(void* ptr, const char* file, int line);
MAINUTILS_API void dbgLeakFree(void* ptr);


//#ifdef __cplusplus
//}
//#endif

#define malloc(size) dbgLeakMalloc(size, __FILE__, __LINE__)
#define calloc(num, size)  dbgLeakCalloc(num, size, __FILE__, __LINE__)
#define realloc(ptr, size) dbgLeakRrealloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) dbgLeakFree(ptr)

// Overload the new and delete operators
MAINUTILS_API void* operator new(size_t size, const char* file, int line);
MAINUTILS_API void* operator new[](size_t size, const char* file, int line);

MAINUTILS_API void operator delete(void* ptr) noexcept;
MAINUTILS_API void operator delete[](void* ptr) noexcept;

// Custom global delete overloads (c++ 20)
//MAINUTILS_API void operator delete(void* ptr, std::size_t size) noexcept;
//MAINUTILS_API void operator delete[](void* ptr, std::size_t size) noexcept;

MAINUTILS_API void operator delete(void* ptr, const char* file, int line) noexcept;
MAINUTILS_API void operator delete[](void* ptr, const char* file, int line) noexcept;

// Overload new macro to capture file and line
#define new new(__FILE__, __LINE__)

namespace mem
{
//     MAINUTILS_API void initLeakDetector();
    MAINUTILS_API void showLeakReport();
}
#endif //MU_WINDOWS
#endif //defined(MAINUTILS_EXPORTS) || defined(MU_SHOW_MEM_LEAKS)

#endif //MU_DEBUG

#endif // MU_USE_MEM_LEAK


