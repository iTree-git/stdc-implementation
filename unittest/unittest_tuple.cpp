#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <functional>
#include "itree/tuple.hpp"

int main() {
    int n = 1;
    auto a = itree::MakeTuple(std::ref(n), n, "char");
    n = 2;
    std::cout << itree::Get<0>(a) << std::endl;
    std::cout << itree::Get<1>(a) << std::endl;
    std::cout << itree::Get<2>(a) << std::endl;
    std::cout << itree::TupleSize<decltype(a)>::value << std::endl;
}
