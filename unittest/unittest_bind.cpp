#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <functional>
#include "itree/bind.hpp"

int Add(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

int main() {
    using namespace std::placeholders;
    itree::Function<int(int, int, int, int, int)> f1 = Add;
    int i = 2;
    auto f2 = itree::Bind(f1, _1, i, 2, 4, 4);
    i = 3;
    std::cout << f2(2, 3, 1,2,3) << std::endl;
}
