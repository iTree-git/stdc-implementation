/*
 * Copyright(c) 2020-present by Liang Yu
 */

#ifndef ITREE_BIND_HPP_
#define ITREE_BIND_HPP_

#include <type_traits>
#include <cassert>
#include <memory>

#include "itree/function.hpp"

namespace itree {

namespace placeholders {
constexpr auto _1 = std::integral_constant<int, 1>();
constexpr auto _2 = std::integral_constant<int, 2>();
constexpr auto _3 = std::integral_constant<int, 3>();
constexpr auto _4 = std::integral_constant<int, 4>();
constexpr auto _5 = std::integral_constant<int, 5>();
constexpr auto _6 = std::integral_constant<int, 6>();
constexpr auto _7 = std::integral_constant<int, 7>();
constexpr auto _8 = std::integral_constant<int, 8>();
constexpr auto _9 = std::integral_constant<int, 9>();
}

template <typename F>
struct FunctionType;
template <typename T>
struct FunctionType<Function<T>> {
    using type = Function<T>;
};
template <size_t I>
using size_constant = std::integral_constant<size_t, I>;

template <typename ...Args>
struct BinderList {
    template <typename ...TArgs>
    BinderList(TArgs&& ...args) : args_list_(std::forward<TArgs>(args)...) {}

    template <size_t N>
    constexpr decltype(auto) operator[](size_constant<N> ) {
        return std::get<N>(args_list_);
    }
    std::tuple<Args...> args_list_;
};

template <typename ...Args>
class ArgsList {
public:
    ArgsList(Args&&...args) : binder_list_(std::forward<Args>(args)...){}

    template <typename T,
              typename std::enable_if<std::is_placeholder<T>::value == 0,
                                      void*>::type = nullptr>
    constexpr decltype(auto) operator[] (T&& t) {
        return std::forward<T>(t);
    }

    template <typename T,
              typename std::enable_if<std::is_placeholder<T>::value != 0,
                                      void*>::type = nullptr>
    constexpr decltype(auto) operator[] (T) {
        return std::move(binder_list_[size_constant<std::is_placeholder<T>::value - 1>()]);
    }

private:
    BinderList<Args&&...> binder_list_;
};

template <typename F, typename ...Args>
class Binder {
public:
    Binder(F&& f, Args&& ...args)
        : binder_list_(std::forward<Args>(args)...)
          , f_(std::forward<F>(f)) {}

    template <typename ...Input>
    auto operator() (Input&&...input) {
        return Invoke(std::make_index_sequence<sizeof...(Args)>{},
                      std::forward<Input>(input)...);
    }
private:
    template <size_t ...I, typename ...Input>
    decltype (auto) Invoke(std::index_sequence<I...>, Input&& ...input) {
        return f_(ArgsList<Input...>(std::forward<Input>(input)...)[
            std::move(binder_list_[size_constant<I>()])]...);
    }

    typename FunctionType<typename std::remove_reference<F>::type>::type f_;
    BinderList<typename std::decay<Args>::type...> binder_list_;
};

template <typename F, typename ...Args>
constexpr auto Bind(F&& f, Args&&...args) {
    return Binder<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}
}

#endif  // ITREE_BIND_HPP_
