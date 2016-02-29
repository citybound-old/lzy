#include <iostream>
#include <list>
#include <vector>
#include <list>

#include "lzy.h"

using namespace std;
using namespace lzy;

int main() {
    vector<int> shortVector = {1, 2, 3, 4, 5, 6, 7, 8};

    auto iteration = filter([](int i) {return i % 2 == 0;}) | map([](int i) {return i * 1.1;});
    auto l = from(shortVector) >> (iteration | take(3)) >> to<list<double>>();

    for (auto d : l) {
        cout << d << endl;
    }

    return 0;
}