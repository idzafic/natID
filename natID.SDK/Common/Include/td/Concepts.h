// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Concepts.h
 * @brief Provides C++20 concept definitions used throughout the td namespace.
 */
#pragma once
#include <concepts>
#include <type_traits>

namespace td
{

/// @brief Nested namespace containing reusable C++20 concept constraints.
namespace conc
{

// Concept for classes with default constructor
/// @brief Concept satisfied by types that are default-initializable.
template <typename T>
concept DefaultConstructor = std::default_initializable<T>;

// Concept for copy-assignable
/// @brief Concept satisfied by types that are copy-assignable.
template <typename T>
concept CopyAssign =std::is_copy_assignable_v<T>;

// Concept for numeric types excluding bool
/// @brief Concept satisfied by arithmetic types excluding bool.
template<typename T>
concept NumericNotBool = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

// Concept for non-numeric types
/// @brief Concept satisfied by non-arithmetic types.
template<typename T>
concept NonNumeric = !std::is_arithmetic_v<T>;

} //namespace conc

} //namespace td
