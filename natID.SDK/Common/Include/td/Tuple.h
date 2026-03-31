// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <tuple>

//check if its possible to call some class ctor with tuple<params...>
template<class T, class ... params>
struct can_construct_from_tuple;

template<class T>
struct can_construct_from_tuple<T>
{
    static constexpr bool value = std::is_constructible_v<T>;
};

template<class T>
struct can_construct_from_tuple<T, std::tuple<>>
{
    static constexpr bool value = std::is_constructible_v<T>;
};

template<class T, class ... params>
struct can_construct_from_tuple<T, std::tuple<params...>>
{
    static constexpr bool value = std::is_constructible_v<T, params...>;
};

template<class T, class ... params>
struct can_construct_from_tuple
{
    static constexpr bool value = std::is_constructible_v<T, params...>;
};
template<class T, class ... params>
inline constexpr bool can_construct_from_tuple_v = can_construct_from_tuple<T, params...>::value;



template<std::size_t N, class idx>
struct offset_index_sequence;

template<std::size_t N, std::size_t... indices>
struct offset_index_sequence<N, std::index_sequence<indices...>>
{
    using type = std::index_sequence<indices + N...>;
};
template<std::size_t N, class idx>
using offset_index_sequence_t = typename offset_index_sequence<N, idx>::type;


//add type to a tuple
template<bool end, class new_type, class... args>
struct tuple_concat;

template<bool end, class new_type>
struct tuple_concat<end, new_type, std::tuple<>>
{
    using type = std::tuple<new_type>;
};

template<bool end, class new_type, class... args>
struct tuple_concat<end, new_type, std::tuple<args...>>
{
    using type = std::conditional_t<end, std::tuple<args..., new_type>, std::tuple<new_type, args...>>;
};

template <class new_type, class... args>
using tuple_concat_end_t = typename tuple_concat<true, new_type, args...>::type;


template <class new_type, class... args>
using tuple_concat_front_t = typename tuple_concat<false, new_type, args...>::type;


//remove types from a tuple
template<bool end, size_t N, class... args>
struct tuple_remove_N;

template<bool end, size_t N>
struct tuple_remove_N<end, N, std::tuple<>>
{
    using type = std::tuple<>;
};

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
        decltype(fn(offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>()))>;
};

template<size_t N, class... args>
using tuple_remove_last_N_t = typename tuple_remove_N<true, N, args...>::type;
template<size_t N, class... args>
using tuple_remove_first_N_t = typename tuple_remove_N<false, N, args...>::type;



//check if it is possible to construct first and second from all_params
//tries to construct first from all_params, then removes one by one from the set from the end of all_params until it finds matching ctor
//second is constructed from the remaining all_params
template<size_t cnt_first, size_t cnt_second, class first, class second, class first_params_tuple, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl;

template<class first, class second>
struct construct_from_pack_end_impl<0, 0, first, second, std::tuple<>, std::tuple<>>
{
    static constexpr bool can_construct = (can_construct_from_tuple_v<first> && can_construct_from_tuple_v<second>);
    using first_params = std::tuple<>;
    using second_params = std::tuple<>;
};

template<size_t cnt_second, class first, class second, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl<0, cnt_second, first, second, std::tuple<>, second_params_tuple, all_params...>
{
    static constexpr bool is_constructible_at_this_first = can_construct_from_tuple_v<first>;
    static constexpr bool is_constructible_at_this_second = can_construct_from_tuple_v<second, second_params_tuple>;

    static constexpr bool can_construct = (can_construct_from_tuple_v<first> && can_construct_from_tuple_v<second, second_params_tuple>);
    using first_params = std::tuple<>;
    using second_params = second_params_tuple;
};

template<size_t cnt_first, size_t cnt_second, class first, class second, class first_params_tuple, class second_params_tuple, class ... all_params>
struct construct_from_pack_end_impl
{
    static constexpr bool is_constructible_at_this_first = can_construct_from_tuple_v<first, first_params_tuple>;
    static constexpr bool is_constructible_at_this_second = can_construct_from_tuple_v<second, second_params_tuple>;
    static constexpr bool is_constructible_at_this = (can_construct_from_tuple_v<first, first_params_tuple> && can_construct_from_tuple_v<second, second_params_tuple>);
    using removed_type = std::tuple_element_t<std::tuple_size_v<first_params_tuple>-1, first_params_tuple>;
    using next = construct_from_pack_end_impl //next is one less param for first, one more for second
        <cnt_first - 1, cnt_second + 1, first, second,
        tuple_remove_last_N_t<1, first_params_tuple>,
        tuple_concat_front_t<removed_type, second_params_tuple>,
        all_params...>;
    static constexpr bool can_construct = is_constructible_at_this ? true : next::can_construct;
    using first_params = std::conditional_t<is_constructible_at_this, first_params_tuple, typename next::first_params>;
    using second_params = std::conditional_t<is_constructible_at_this, second_params_tuple, typename next::second_params>;
};


template<class first, class second, class ... all_params>
struct construct_from_pack_end
{
    using impl = construct_from_pack_end_impl<sizeof...(all_params), 0, first, second, std::tuple<all_params...>, std::tuple<>, all_params...>;
    static constexpr bool can_construct = impl::can_construct;
    static_assert(can_construct, "Cannot construct first and second with given params.");
    using first_params_tuple_type = typename impl::first_params;
    using second_params_tuple_type = typename impl::second_params;
};

template<class first, class second, class ... all_params>
struct construct_from_pack_end_with_lambda
{
    using impl = construct_from_pack_end_impl<sizeof...(all_params) - 1, 0, first, second, tuple_remove_last_N_t<1, std::tuple<all_params...>>, std::tuple<>, all_params...>;
    static constexpr bool can_construct = impl::can_construct;
    static_assert(can_construct, "Cannot construct first and second with given params.");
    using first_params_tuple_type = typename impl::first_params;
    using second_params_tuple_type = typename impl::second_params;
};

//get type of N elem in param pack
template<size_t N, class... args>
using ppack_typeof_N_t = std::tuple_element_t<N, std::tuple<args...>>;


template<size_t N1, size_t N2> 
struct static_assert_equal
{
    static_assert(N1 == N2, "static_assert_equal : false");
    static constexpr bool value = (N1 == N2);
};
template<size_t N1, size_t N2>
struct static_assert_less
{
    static_assert(N1 < N2, "static_assert_less : false");
    static constexpr bool value = (N1 < N2);
};
template<size_t N1, size_t N2>
struct static_assert_lessequal
{
    static_assert(N1 <= N2, "static_assert_lessequal : false");
    static constexpr bool value = (N1 <= N2);
};
template<size_t N1, size_t N2>
struct static_assert_greater
{
    static_assert(N1 > N2, "static_assert_greater : false");
    static constexpr bool value = (N1 > N2);
};
template<size_t N1, size_t N2>
struct static_assert_greaterequal
{
    static_assert(N1 >= N2, "static_assert_greaterequal : false");
    static constexpr bool value = (N1 >= N2);
};

//tuple indices manipulation
template <size_t N, class... args>
constexpr auto tuple_indices_cut_last_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<sizeof...(args) - N>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_cut_first_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_keep_last_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<sizeof...(args) - N, std::make_index_sequence<N>>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_keep_first_N(std::tuple<args...>&&)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

template <size_t N1, size_t N2, class... args>
constexpr auto tuple_indices_keep_range(std::tuple<args...>&&)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}
//param pack indices
template <size_t N, class... args>
constexpr auto tuple_indices_cut_last_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<sizeof...(args) - N>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_cut_first_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_keep_last_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<sizeof...(args) - N, std::make_index_sequence<N>>();
}

template <size_t N, class... args>
constexpr auto tuple_indices_keep_first_N(args&&...)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

template <size_t N1, size_t N2, class... args>
constexpr auto tuple_indices_keep_range(args&&...)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}

template <size_t N, size_t SIZE>
constexpr auto tuple_indices_cut_last_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_cut_last_N : N must not be greater than tuple size");
    return std::make_index_sequence<SIZE - N>();
}

template <size_t N, size_t SIZE>
constexpr auto tuple_indices_cut_first_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_cut_first_N : N must not be greater than tuple size");
    return offset_index_sequence_t<N, std::make_index_sequence<SIZE - N>>();
}

template <size_t N, size_t SIZE>
constexpr auto tuple_indices_keep_last_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_keep_last_N : N must not be greater than tuple size");
    return offset_index_sequence_t<SIZE - N, std::make_index_sequence<N>>();
}

template <size_t N, size_t SIZE>
constexpr auto tuple_indices_keep_first_N()
{
    static_assert(static_assert_lessequal<N, SIZE>::value, "tuple_indices_keep_first_N : N must not be greater than tuple size");
    return std::make_index_sequence<N>();
}

template <size_t N1, size_t N2, size_t SIZE>
constexpr auto tuple_indices_keep_range()
{
    static_assert(static_assert_less<N1, SIZE>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, SIZE>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");
    return offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>();
}

//tuple subtuple, extract elements at indices...
template <class... args, std::size_t... indices>
constexpr auto tuple_slice_impl(std::tuple<args...>&& tuple, std::index_sequence<indices...>)
{
    return std::make_tuple(std::get<indices>(std::forward<std::tuple<args...>>(tuple))...);
}

template <size_t N, class... args>
constexpr auto tuple_cut_last_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_cut_last_N : N must not be greater than tuple size");
    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), std::make_index_sequence<sizeof...(args) - N>());
}

template <size_t N, class... args>
constexpr auto tuple_cut_first_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_cut_first_N : N must not be greater than tuple size");
    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), offset_index_sequence_t<N, std::make_index_sequence<sizeof...(args) - N>>());

}

template <size_t N, class... args>
constexpr auto tuple_keep_last_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_keep_last_N : N must not be greater than tuple size");
    return tuple_cut_first_N<sizeof...(args) - N>(std::forward<std::tuple<args...>>(tuple));
}

template <size_t N, class... args>
constexpr auto tuple_keep_first_N(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_lessequal<N, sizeof...(args)>::value, "tuple_keep_first_N : N must not be greater than tuple size");
    return tuple_cut_last_N<sizeof...(args) - N>(std::forward<std::tuple<args...>>(tuple));
}

template <size_t N1, size_t N2, class... args>
constexpr auto tuple_keep_range(std::tuple<args...>&& tuple)
{
    static_assert(static_assert_less<N1, sizeof...(args)>::value, "tuple_keep_range : N1 must not be greater than tuple size");
    static_assert(static_assert_less<N2, sizeof...(args)>::value, "tuple_keep_range : N2 must not be greater than tuple size");
    static_assert(static_assert_lessequal<N1, N2>::value, "tuple_keep_range : N1 must not be greater than N2");

    return tuple_slice_impl(std::forward<std::tuple<args...>>(tuple), offset_index_sequence_t<N1, std::make_index_sequence<(N2 - N1 + 1)>>());
}
