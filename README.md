*This code is part of and developed for [Citybound](http://cityboundsim.com).
At some point in the future it might become stable and generally useful!*

# Lzy

## Lazy sequences and iterators for C++11


```c++
#include <iostream>
#include <list>
#include <vector>

#include "lzy.h"

using namespace std;
using namespace lzy;

int main() {
    vector<int> aCoupleNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    auto iteration = filter([](int i) {return i % 2 == 0;}) | map([](int i) {return i * 1.1;});

    // iterate directly over a sequence
    for (auto d : from(aCoupleNumbers) >> iteration) {
        cout << d << endl;
    }

    // compose iterations on the fly and create a container from a sequence
    auto l = from(aCoupleNumbers) >> (iteration | take(3)) >> to<list<double>>();

    return 0;
}
```

## License

MIT, see [LICENSE.md](LICENSE.md) for details.
