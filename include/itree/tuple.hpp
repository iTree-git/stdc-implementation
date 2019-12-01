#ifndef __TREE_TUPLE_HPP__
#define __TREE_TUPLE_HPP__

#include <type_traits>

namespace itree {

/**
 * Tuple
 */
template <typename ...Types> class Tuple {};

template <typename T, typename... Types>
class Tuple<T, Types...> : public Tuple<Types...> {
 public:
    Tuple<T, Types...>(T t, Types...ts) : Tuple<Types...>(ts...), tail(t) {}
    T tail;
};

/**
 * TupleElement
 */
template <size_t I, typename T> class TupleElement;

template <typename T, typename ...Types>
class TupleElement<0, Tuple<T, Types...>> {
public:
    typedef T type;
};

template <size_t I, typename T, typename ...Types>
class TupleElement<I, Tuple<T, Types...>> {
public:
    typedef typename TupleElement<I-1, Tuple<Types...>>::type type;
};

template <size_t I, class T>
class TupleElement<I, const T> {
public:
    typedef typename
        std::add_const<typename TupleElement<I, T>::type>::type type;
};

template <size_t I, class T>
class TupleElement<I, volatile T> {
public:
    typedef typename
        std::add_volatile<typename TupleElement<I, T>::type>::type type;
};

template <size_t I, class T>
class TupleElement<I, const volatile T> {
public:
    typedef typename
        std::add_cv<typename TupleElement<I, T>::type>::type type;
};

/**
 * Get
 */
template <size_t I, typename ...Types>
typename std::enable_if<(I==0),
                        typename TupleElement<I, Tuple<Types...>>::type
                        >::type&
Get(Tuple<Types...> &tuple) {
    return tuple.tail;
}

template <size_t I, typename T, typename ...Types>
typename std::enable_if<(I!=0),
                        typename TupleElement<I, Tuple<T, Types...>>::type
                        >::type&
Get(Tuple<T, Types...>& tuple) {
    Tuple<Types...> &base = tuple;
    return Get<I-1>(base);
}

/**
 * MakeTuple
 */
template <typename T>
class TraitReferenceWrapper {
public:
    using type = T;
};

template <typename T>
class TraitReferenceWrapper<std::reference_wrapper<T>> {
public:
    using type = T&;
};

template <typename T>
using special_decay_t = typename TraitReferenceWrapper<typename std::decay<T>::type>::type;

template <typename... Types>
Tuple<special_decay_t<Types>...> MakeTuple(Types&&... args) {
    return Tuple<special_decay_t<Types>...>(std::forward<Types>(args)...);
}

/**
 * TupleSize
 */

template <typename T>
struct TupleSize;

template <typename... Types>
struct TupleSize<Tuple<Types...>>
    : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename... Types>
struct TupleSize<const Tuple<Types...>>
    : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename... Types>
struct TupleSize<volatile Tuple<Types...>>
    : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename... Types>
struct TupleSize<const volatile Tuple<Types...>>
    : std::integral_constant<std::size_t, sizeof...(Types)> {};


}  // itree

#endif  // __TREE_TUPLE_HPP__

