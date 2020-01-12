#include <iostream>
#include <typeinfo>
#include <type_traits>
#include "itree/function.hpp"

#include <iostream>
 
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};

class Copy {
public:
    Copy() {
        std::cout << "Copy()" << std::endl;
    }
    Copy(const Copy &t) {
        std::cout << "Copy(&)" << std::endl;
    }
    Copy(Copy &&t) {
        std::cout << "Copy(&&)" << std::endl;
    }
};

void print_num(Copy i)
{
    std::cout << "print_num" << '\n';
}
struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};
void test(itree::Function<void(int)> &&func, int a) {
    func(a);
}

int main()
{


    // store a free function
    Copy i;
    std::cout << std::is_function<decltype(print_num)>::value << std::endl;
    std::cout << "normal call print_num function" << std::endl;
    print_num(i);
    std::cout << "function print_num function" << std::endl;
    itree::Function<void(Copy)> f_display = print_num;
    f_display(i);
    //test(std::move(f_display), 1);

    //// store a lambda
    //itree::Function<void(int, int, int)> f_display_42 = [](int a, int b, int c) {
    //    print_num(a); };
    //f_display_42(1, 2,3);


    /*
    // store the result of a call to std::bind
    itree::Function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // store a call to a member function
    itree::Function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // store a call to a data member accessor
    itree::Function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';

   // store a call to a member function and object
    using std::placeholders::_1;
    itree::Function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);

    // store a call to a member function and object ptr
    itree::Function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);

    // store a call to a function object
    itree::Function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);
    */
}
