// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Alg.h
    @brief Generic algorithm utilities for iterating over containers. */
#pragma once

namespace cnt
{
	/// @brief Applies a function to each element in the range [first, last).
	/// @tparam TITER Iterator type.
	/// @tparam TFN Callable type accepting a dereferenced iterator value.
	/// @param first Iterator to the first element; advanced to last on return.
	/// @param last Iterator one past the last element.
	/// @param f Function object invoked for each element.
	template<class TITER, class TFN>
	inline void forEach(TITER& first, const TITER& last, TFN& f)
	{
		while (first != last)
		{
			f(*first);
			++first;
		}

	}

	/// @brief Applies a function to every element in the given container.
	/// @tparam TCNT Container type providing begin() and cend() iterators.
	/// @tparam TFN Callable type accepting a dereferenced iterator value.
	/// @param container The container whose elements will be visited.
	/// @param f Function object invoked for each element.
	template<class TCNT, class TFN>
	inline void forEach(TCNT& container, TFN& f)
	{
		auto it = container.begin();
		auto itEnd = container.cend();

		while (it != itEnd)
		{
			f(*it);
			++it;
		}
	}
}
