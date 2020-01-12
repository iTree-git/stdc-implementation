/*
 * Copyright(c) 2019-present by Liang Yu
 */

#ifndef ITREE_FUNCTION_HPP_
#define ITREE_FUNCTION_HPP_

#include <type_traits>
#include <cassert>
#include <memory>

namespace itree {

template <typename T>
class Function;

template <typename R, typename ...Args>
class Function<R(Args...)> {
public:
    class CallBackBase {
    public:
        virtual ~CallBackBase() {};
        virtual R Invoke(Args&&...) = 0;
    };
    template <typename T>
    class CallBack : public CallBackBase {
    public:
        CallBack(T t) : t_(t) {}
        virtual ~CallBack() {}
        virtual R Invoke(Args&&... args) { return t_(std::forward<Args>(args)...); }
        T t_;
    };
    template <typename T>
    Function<R(Args...)>(T t) {
        call_back_ = std::make_shared<CallBack<T>>(t);
    }

    Function<R(Args...)>(const Function<R(Args...)> &t) {
        call_back_ = t.call_back_;
    }

    Function<R(Args...)>(Function<R(Args...)> &&t) {
        call_back_ = std::move(t.call_back_);
    }

    R operator()(Args...args) {
        assert(call_back_);
        call_back_->Invoke(std::forward<Args>(args)...);
    }

private:
    std::shared_ptr<CallBackBase> call_back_;
};
};  // itree

#endif  // ITREE_FUNCTION_HPP_

