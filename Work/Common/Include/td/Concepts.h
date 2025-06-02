// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <concepts>
#include <type_traits>

namespace td
{

namespace conc
{

// Concept for classes with default constructor
template <typename T>
concept DefaultConstructor = std::default_initializable<T>;

// Concept for copy-assignable
template <typename T>
concept CopyAssign =std::is_copy_assignable_v<T>;

// Concept for numeric types excluding bool
template<typename T>
concept NumericNotBool = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

// Concept for non-numeric types
template<typename T>
concept NonNumeric = !std::is_arithmetic_v<T>;

} //namespace conc

} //namespace td
