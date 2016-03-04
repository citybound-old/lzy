#include <iostream>
#include <list>
#include <vector>

#include "lzy.h"

using namespace lzy;

int main() {
    std::vector<int> aCoupleNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    auto iteration = filter([](int i) {return i % 2 == 0;}) | map([](int i) {return i * 1.1;});

    // iterate directly over a sequence
    for (auto d : from(aCoupleNumbers) >> iteration) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;
    // output: 2.2, 4.4, 6.6, 8.8, 11, 13.2,

    // compose iterations on the fly and create a container from a sequence
    auto list = from(aCoupleNumbers) >> (iteration | take(3)) >> to<std::list<double>>();
    // list contains 2.2, 4.4, 6.6

    // concatenate sequences
    std::vector<int> first = {1, 2, 3};
    std::vector<int> second = {4, 5, 6};

    for (auto d : from(first) + from(second) >> map([](int i) {return i * 2;})) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;
    // output: 2, 4, 6, 8, 10, 12,

    // reduce over a sequence
    std::cout << "sum: " << (from(aCoupleNumbers) >> reduce(std::plus<int>(), 0)) << std::endl;
    // output: sum: 78

    // sliding windows (of size 3 here) over a sequence
    for (auto w : from(aCoupleNumbers) >> (windows<3>() | slice(3, 6))) {
        for (auto i : w) {
            std::cout << i << ",";
        }
        std::cout << std::endl;
    }
    // output:
    // 4,5,6,
    // 5,6,7,
    // 6,7,8,


    // zip two sequences together
    for (auto pair : zip(from(first), from(second))) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }
    // output:
    // (1, 4)
    // (2, 5)
    // (3, 6)

    return 0;
}
