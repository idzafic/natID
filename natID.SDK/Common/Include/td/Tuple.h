// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Tuple.h
    @brief Compile-time tuple manipulation utilities: constructibility checks, index sequences, slicing, and two-object construction splitting. */
#pragma once
#include <tuple>

//check if its possible to call some class ctor with tuple<params...>
/// @brief Primary template; tests whether T can be constructed from a parameter pack.
/// @tparam T      The type to test construction for.
/// @tparam params The parameter pack (or tuple of types) to use for construction.
template<class T, class ... params>
struct can_construct_from_tuple;

/// @brief Specialisation for an empty parameter pack.
/// @tparam T The type to test.
template<class T>
struct can_construct_from_tuple<T>
{
    static constexpr bool value = std::is_constructible_v<T>; ///< true if T is default-constructible.
};

/// @brief Specialisation for an empty tuple<>.
/// @tparam T The type to test.
template<class T>
struct can_construct_from_tuple<T, std::tuple<>>
{
    static constexpr bool value = std::is_constructible_v<T>; ///< true if T is default-constructible.
};

/// @brief Specialisation for a non-empty tuple of types.
/// @tparam T      The type to test.
/// @tparam params The types inside the tuple.
template<class T, class ... params>
struct can_construct_from_tuple<T, std::tuple<params...>>
{
    static constexpr bool value = std::is_constructible_v<T, params...>; ///< true if T is constructible from the tuple's types.
};

/// @brief General fallback: tests whether T is constructible from a raw parameter pack.
/// @tparam T      The type to test.
/// @tparam params The parameter pack.
template<class T, class ... params>
struct can_construct_from_tuple
{
    static constexpr bool value = std::is_constructible_v<T, params...>; ///< true if T is constructible from params.
};

/// @brief Variable template helper for can_construct_from_tuple.
/// @tparam T      The type to test.
/// @tparam params The parameter pack or tuple.
template<class T, class ... params>
inline constexpr bool can_construct_from_tuple_v = can_construct_from_tuple<T, params...>::value;



/// @brief Produces an index sequence offset by N.
/// @tparam N       The offset to add to every index.
/// @tparam idx     The original index sequence.
template<std::size_t N, class idx>
struct offset_index_sequence;

/// @brief Specialisation that adds N to each index in a std::index_sequence.
/// @tparam N       The offset.
/// @tparam indices The original indices.
template<std::size_t N, std::size_t... indices>
struct offset_index_sequence<N, std::index_sequence<indices...>>
{
    using type = std::index_sequence<indices + N...>; ///< Resulting offset index sequence.
};
/// @brief Alias for offset_index_sequence::type.
/// @tparam N   Offset value.
/// @tparam idx Original index sequence.
template<std::size_t N, class idx>
using offset_index_sequence_t = typename offset_index_sequence<N, idx>::type;


//add type to a tuple
/// @brief Primary template for appending or prepending a type to a tuple.
/// @tparam end      true to append; false to prepend.
/// @tparam new_type The type to insert.
/// @tparam args     Parameter pack or tuple.
template<bool end, class new_type, class... args>
struct tuple_concat;

/// @brief Specialisation for inserting into an empty tuple.
/// @tparam end      Append or prepend direction.
/// @tparam new_type The type to insert.
template<bool end, class new_type>
struct tuple_concat<end, new_type, std::tuple<>>
{
    using type = std::tuple<new_type>; ///< Single-element tuple containing new_type.
};

/// @brief Specialisation for inserting into a non-empty tuple.
/// @tparam end      true to append at end; false to prepend at front.
/// @tparam new_type The type to insert.
/// @tparam args     Existing tuple element types.
template<bool end, class new_type, class... args>
struct tuple_concat<end, new_type, std::tuple<args...>>
{
    using type = std::conditional_t<end, std::tuple<args..., new_type>, std::tuple<new_type, args...>>; ///< Resulting tuple type.
};

/// @brief Alias for appending new_type to the end of a tuple.
template <class new_type, class... args>
using tuple_concat_end_t = typename tuple_concat<true, new_type, args...>::type;


/// @brief Alias for prepending new_type to the front of a tuple.
template <class new_type, class... args>
using tuple_concat_front_t = typename tuple_concat<false, new_type, args...>::type;


//remove types from a tuple
/// @brief Primary template for removing N types from either the front or the end of a tuple.
/// @tparam end  true to remove from the end; false to remove from the front.
/// @tparam N    Number of types to remove.
/// @tparam args Tuple element types or parameter pack.
template<bool end, size_t N, class... args>
struct tuple_remove_N;

/// @brief Specialisation for an empty tuple.
template<bool end, size_t N>
struct tuple_remove_N<end, N, std::tuple<>>
{
    using type = std::tuple<>; ///< Removing from an empty tuple yields an empty tuple.
};

/// @brief Specialisation for a non-empty tuple.
/// @tparam end  true to remove from the end; false to remove from the front.
/// @tparam N    Number of elements to remove.
/// @tparam args Existing tuple element types.
template<bool end, size_t N, class... args>
struct tuple_remove_N<end, N, std::tuple<args...>>
{
private:
    template<std::size_t... indices>
    static constexpr std::tuple<std::tuple_element_t<indices, std::tuple<args...>>...> fn(std::index_sequence<indices...>);

public:
    static_assert(N <= sizeof...(args), "tuple_remove_N : N must not be greater than the number of tuple args");
    using type = std::conditional_t<end,
        decltype(fn(std::make_index_sequence<sizeof...(args) - N>())),
        decltype(fn(offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>()))>; ///< Resulting tuple with N elements removed.
};

/// @brief Alias for removing the last N types from a tuple.
template<size_t N, class... args>
using tuple_remove_last_N_t = typename tuple_remove_N<true, N, args...>::type;
/// @brief Alias for removing the first N types from a tuple.
template<size_t N, class... args>
using tuple_remove_first_N_t = typename tuple_remove_N<false, N, args...>::type;



//check if it is possible to construct first and second from all_params
//tries to construct first from all_params, then removes one by one from the set from the end of all_params until it finds matching ctor
//second is constructed from the remaining all_params
/// @brief Implementation helper that splits a parameter pack into two constructor argument sets.
/// @tparam cnt_first       Number of parameters remaining for first.
/// @tparam cnt_second      Number of parameters assigned to second.
/// @tparam first           First type to construct.
/// @tparam second          Second type to construct.
/// @tparam first_params_tuple  Tuple of types tried for first's constructor.
/// @tparam second_params_tuple Tuple of types tried for second's constructor.
/// @tparam all_params      The complete parameter pack.
template<size_t cnt_first, size_t cnt_second, class first, class second, class first_params_tuple, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl;

/// @brief Base case: both parameter sets are empty.
template<class first, class second>
struct construct_from_pack_end_impl<0, 0, first, second, std::tuple<>, std::tuple<>>
{
    static constexpr bool can_construct = (can_construct_from_tuple_v<first> && can_construct_from_tuple_v<second>); ///< true if both types are default-constructible.
    using first_params = std::tuple<>;  ///< Empty parameter set for first.
    using second_params = std::tuple<>; ///< Empty parameter set for second.
};

/// @brief Partial base case: first has no parameters; second has remaining parameters.
template<size_t cnt_second, class first, class second, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl<0, cnt_second, first, second, std::tuple<>, second_params_tuple, all_params...>
{
    static constexpr bool is_constructible_at_this_first  = can_construct_from_tuple_v<first>; ///< true if first is default-constructible.
    static constexpr bool is_constructible_at_this_second = can_construct_from_tuple_v<second, second_params_tuple>; ///< true if second is constructible from second_params_tuple.

    static constexpr bool can_construct = (can_construct_from_tuple_v<first> && can_construct_from_tuple_v<second, second_params_tuple>); ///< true if both can be constructed at this split.
    using first_params  = std::tuple<>;          ///< Empty parameter set for first.
    using second_params = second_params_tuple;   ///< Parameter set for second.
};

/// @brief Recursive case: tries current split, or moves one parameter from first to second.
template<size_t cnt_first, size_t cnt_second, class first, class second, class first_params_tuple, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl
{
    static constexpr bool is_constructible_at_this_first  = can_construct_from_tuple_v<first, first_params_tuple>; ///< Constructibility at this split for first.
    static constexpr bool is_constructible_at_this_second = can_construct_from_tuple_v<second, second_params_tuple>; ///< Constructibility at this split for second.
    static constexpr bool is_constructible_at_this = (can_construct_from_tuple_v<first, first_params_tuple> && can_construct_from_tuple_v<second, second_params_tuple>); ///< true if both can be constructed at this split.
    using removed_type = std::tuple_element_t<std::tuple_size_v<first_params_tuple>-1, first_params_tuple>; ///< Type moved from first's set to second's set in the next step.
    using next = construct_from_pack_end_impl //next is one less param for first, one more for second
        <cnt_first - 1, cnt_second + 1, first, second,
        tuple_remove_last_N_t<1, first_params_tuple>,
        tuple_concat_front_t<removed_type, second_params_tuple>,
        all_params...>;
    static constexpr bool can_construct = is_constructible_at_this ? true : next::can_construct; ///< true if a valid split was found at this level or deeper.
    using first_params  = std::conditional_t<is_constructible_at_this, first_params_tuple, typename next::first_params>; ///< Resolved parameter set for first.
    using second_params = std::conditional_t<is_constructible_at_this, second_params_tuple, typename next::second_params>; ///< Resolved parameter set for second.
};


/// @brief Finds the optimal split of all_params so that first and second can each be constructed.
/// @tparam first      First type to construct.
/// @tparam second     Second type to construct.
/// @tparam all_params Complete parameter pack.
template<class first, class second, class ... all_params>
struct construct_from_pack_end
{
    using impl = construct_from_pack_end_impl<sizeof...(all_params), 0, first, second, std::tuple<all_params...>, std::tuple<>, all_params...>;
    static constexpr bool can_construct = impl::can_construct; ///< true if a valid split exists.
    static_assert(can_construct, "Cannot construct first and second with given params.");
    using first_params_tuple_type  = typename impl::first_params;  ///< Resolved tuple type for first's constructor arguments.
    using second_params_tuple_type = typename impl::second_params; ///< Resolved tuple type for second's constructor arguments.
};

/// @brief Variant of construct_from_pack_end that reserves the last parameter for a lambda.
/// @tparam first      First type to construct.
/// @tparam second     Second type to construct.
/// @tparam all_params Complete parameter pack (last element is the lambda).
template<class first, class second, class ... all_params>
struct construct_from_pack_end_with_lambda
{
    using impl = construct_from_pack_end_impl<sizeof...(all_params) - 1, 0, first, second, tuple_remove_last_N_t<1, std::tuple<all_params...>>, std::tuple<>, all_params...>;
    static constexpr bool can_construct = impl::can_construct; ///< true if a valid split exists (excluding the lambda).
    static_assert(can_construct, "Cannot construct first and second with given params.");
    using first_params_tuple_type  = typename impl::first_params;  ///< Resolved tuple type for first's constructor arguments.
    using second_params_tuple_type = typename impl::second_params; ///< Resolved tuple type for second's constructor arguments.
};

//get type of N elem in param pack
/// @brief Alias that retrieves the type at index N in a parameter pack.
/// @tparam N    Zero-based index.
/// @tparam args The parameter pack.
template<size_t N, class... args>
using ppack_typeof_N_t = std::tuple_element_t<N, std::tuple<args...>>;


/// @brief Compile-time static assertion that N1 == N2.
/// @tparam N1 First value.
/// @tparam N2 Second value.
template<size_t N1, size_t N2>
struct static_assert_equal
{
    static_assert(N1 == N2, "static_assert_equal : false");
    static constexpr bool value = (N1 == N2); ///< true if N1 equals N2.
};
/// @brief Compile-time static assertion that N1 < N2.
template<size_t N1, size_t N2>
struct static_assert_less
{
    static_assert(N1 < N2, "static_assert_less : false");
    static constexpr bool value = (N1 < N2); ///< true if N1 is strictly less than N2.
};
/// @brief Compile-time static assertion that N1 <= N2.
template<size_t N1, size_t N2>
struct static_assert_lessequal
{
    static_assert(N1 <= N2, "static_assert_lessequal : false");
    static constexpr bool value = (N1 <= N2); ///< true if N1 is less than or equal to N2.
};
/// @brief Compile-time static assertion that N1 > N2.
template<size_t N1, size_t N2>
struct static_assert_greater
{
    static_assert(N1 > N2, "static_assert_greater : false");
    static constexpr bool value = (N1 > N2); ///< true if N1 is strictly greater than N2.
};
/// @brief Compile-time static assertion that N1 >= N2.
template<size_t N1, size_t N2>
struct static_assert_greaterequal
{
    static_assert(N1 >= N2, "static_assert_greaterequal : false");
    static constexpr bool value = (N1 >= N2); ///< true if N1 is greater than or equal to N2.
};

//tuple indices manipulation
/// @brief Returns the index sequence for a tuple with the last N indices removed.
template <size_t N, class... args>
constexpr auto tuple_indices_cut_last_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<sizeof...(args) - N>();
}

/// @brief Returns the index sequence for a tuple with the first N indices removed.
template <size_t N, class... args>
constexpr auto tuple_indices_cut_first_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>();
}

/// @brief Returns the index sequence for the last N elements of a tuple.
template <size_t N, class... args>
constexpr auto tuple_indices_keep_last_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<sizeof...(args) - N, std::make_index_sequence<N>>();
}

/// @brief Returns the index sequence for the first N elements of a tuple.
template <size_t N, class... args>
constexpr auto tuple_indices_keep_first_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

/// @brief Returns the index sequence for a contiguous range [N1, N2] of tuple elements.
template <size_t N1, size_t N2, class... args>
constexpr auto tuple_indices_keep_range(std::tuple<args...>&&)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}
//param pack indices
/// @brief Returns the index sequence for a parameter pack with the last N indices removed.
template <size_t N, class... args>
constexpr auto tuple_indices_cut_last_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<sizeof...(args) - N>();
}

/// @brief Returns the index sequence for a parameter pack with the first N indices removed.
template <size_t N, class... args>
constexpr auto tuple_indices_cut_first_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>();
}

/// @brief Returns the index sequence for the last N elements of a parameter pack.
template <size_t N, class... args>
constexpr auto tuple_indices_keep_last_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<sizeof...(args) - N, std::make_index_sequence<N>>();
}

/// @brief Returns the index sequence for the first N elements of a parameter pack.
template <size_t N, class... args>
constexpr auto tuple_indices_keep_first_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

/// @brief Returns the index sequence for a contiguous range [N1, N2] of a parameter pack.
template <size_t N1, size_t N2, class... args>
constexpr auto tuple_indices_keep_range(args&&...)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}

/// @brief Returns the index sequence for a fixed-SIZE pack with the last N indices removed.
template <size_t N, size_t SIZE>
constexpr auto tuple_indices_cut_last_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<SIZE - N>();
}

/// @brief Returns the index sequence for a fixed-SIZE pack with the first N indices removed.
template <size_t N, size_t SIZE>
constexpr auto tuple_indices_cut_first_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<SIZE - N>>();
}

/// @brief Returns the index sequence for the last N elements of a fixed-SIZE pack.
template <size_t N, size_t SIZE>
constexpr auto tuple_indices_keep_last_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<SIZE - N, std::make_index_sequence<N>>();
}

/// @brief Returns the index sequence for the first N elements of a fixed-SIZE pack.
template <size_t N, size_t SIZE>
constexpr auto tuple_indices_keep_first_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

/// @brief Returns the index sequence for a contiguous range [N1, N2] in a fixed-SIZE pack.
template <size_t N1, size_t N2, size_t SIZE>
constexpr auto tuple_indices_keep_range()
{
    static_assert(static_assert_less<N1, SIZE>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, SIZE>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");
    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}

//tuple subtuple, extract elements at indices...
/// @brief Constructs a new tuple by selecting elements from a source tuple at the given indices.
/// @tparam args    Element types of the source tuple.
/// @tparam indices Index pack selecting elements to include.
/// @param tuple    The source tuple.
/// @return A new tuple containing only the selected elements.
template <class... args, std::size_t... indices>
constexpr auto tuple_slice_impl(std::tuple<args...>&& tuple, std::index_sequence<indices...>)
{
    return std::make_tuple(std::get<indices>(std::forward<std::tuple<args...>>(tuple))...);
}

/// @brief Returns a new tuple containing all elements except the last N.
/// @tparam N    Number of elements to remove from the end.
/// @tparam args Element types.
/// @param tuple The source tuple.
/// @return Tuple with the last N elements removed.
template <size_t N, class... args>
constexpr auto tuple_cut_last_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_cut_last_N : N must not be greater than tuple size");
    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), std::make_index_sequence<sizeof...(args) - N>());
}

/// @brief Returns a new tuple containing all elements except the first N.
/// @tparam N    Number of elements to remove from the front.
/// @tparam args Element types.
/// @param tuple The source tuple.
/// @return Tuple with the first N elements removed.
template <size_t N, class... args>
constexpr auto tuple_cut_first_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_cut_first_N : N must not be greater than tuple size");
    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>());

}

/// @brief Returns a new tuple containing only the last N elements.
/// @tparam N    Number of elements to keep from the end.
/// @tparam args Element types.
/// @param tuple The source tuple.
/// @return Tuple of the last N elements.
template <size_t N, class... args>
constexpr auto tuple_keep_last_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_keep_last_N : N must not be greater than tuple size");
    return tuple_cut_first_N<sizeof...(args) - N>(std::forward<std::tuple<args...>>(tuple));
}

/// @brief Returns a new tuple containing only the first N elements.
/// @tparam N    Number of elements to keep from the front.
/// @tparam args Element types.
/// @param tuple The source tuple.
/// @return Tuple of the first N elements.
template <size_t N, class... args>
constexpr auto tuple_keep_first_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_keep_first_N : N must not be greater than tuple size");
    return tuple_cut_last_N<sizeof...(args) - N>(std::forward<std::tuple<args...>>(tuple));
}

/// @brief Returns a new tuple containing elements at indices in the inclusive range [N1, N2].
/// @tparam N1   First index of the range.
/// @tparam N2   Last index of the range.
/// @tparam args Element types.
/// @param tuple The source tuple.
/// @return Tuple of elements at indices N1 through N2.
template <size_t N1, size_t N2, class... args>
constexpr auto tuple_keep_range(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>());
}
