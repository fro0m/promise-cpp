#pragma once
#ifndef INC_COMPATIBILITY_HPP_
#define INC_COMPATIBILITY_HPP_

#include <functional>
#include <iterator>
#include <type_traits>

#include <utility>

#ifdef __cpp_rtti
#include <typeindex>
namespace promise
{
using type_index = std::type_index;

template <typename T>
inline type_index type_id()
{
    return typeid(T);
}
} // namespace promise
#else
namespace promise
{
using type_index = ptrdiff_t;

template <typename T>
inline type_index type_id()
{
    static char idHolder;
    return (type_index)&idHolder;
}
} // namespace promise
#endif

#ifndef INC_STD_ADD_ONS_HPP_
#define INC_STD_ADD_ONS_HPP_

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201402L) || (!defined(_MSVC_LANG) && __cplusplus < 201402L)
namespace std_comp
{

template <size_t... Ints>
struct index_sequence
{
    using type = index_sequence;
    using value_type = size_t;
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Ints);
    }
};

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <size_t... I1, size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...>
{
};

template <size_t N>
struct make_index_sequence : _merge_and_renumber<typename make_index_sequence<N / 2>::type,
                                                 typename make_index_sequence<N - N / 2>::type>
{
};

template <>
struct make_index_sequence<0> : index_sequence<>
{
};
template <>
struct make_index_sequence<1> : index_sequence<0>
{
};
} // namespace std_comp
#else
namespace std_comp
{
/// Alias template index_sequence
template <size_t... _Idx>
using index_sequence = std::integer_sequence<size_t, _Idx...>;

/// Alias template make_index_sequence
template <size_t _Num>
using make_index_sequence = std::make_integer_sequence<size_t, _Num>;

/// Alias template index_sequence_for
template <typename... _Types>
using index_sequence_for = std::make_index_sequence<sizeof...(_Types)>;
} // namespace std_comp
#endif //__cplusplus < 201402L

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) || (!defined(_MSVC_LANG) && __cplusplus < 201703L)
namespace std_comp
{

template <class...>
using void_t = void;
}
#endif //__cplusplus < 201703L

#if (defined(_MSVC_LANG) && _MSVC_LANG < 202002L) || (!defined(_MSVC_LANG) && __cplusplus < 202002L)
namespace std_comp
{

template <class T>
struct remove_cvref
{
    typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
};
} // namespace std_comp
#else
namespace std_comp
{
template <typename T>
using remove_cvref = std::remove_cvref<T>;
} // namespace std_comp
#endif //__cplusplus < 202002L

#endif // #ifndef INC_STD_ADD_ONS_HPP_

namespace promise
{

template <typename T>
struct tuple_remove_cvref
{
    using type = typename std_comp::remove_cvref<T>::type;
};
template <typename... T>
struct tuple_remove_cvref<std::tuple<T...>>
{
    using type = std::tuple<typename std_comp::remove_cvref<T>::type...>;
};

template <typename T, typename = void>
struct is_iterable : std::false_type
{
};

// this gets used only when we can call std::begin() and std::end() on that type
template <typename T>
struct is_iterable<
    T,
    std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
    : std::true_type
{
};

template <typename T>
struct is_std_function : std::false_type
{
};
template <typename T>
struct is_std_function<std::function<T>> : std::true_type
{
};

} // namespace promise

#endif
