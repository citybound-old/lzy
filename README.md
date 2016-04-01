*Dieser Code ist Teil von und entwickelt für [Citybound](http://cityboundsim.com).
Irgendwann könnte er stabil und allgemein benutzbar werden!!*

# Lzy

## Faule Sequenzen und Iterationen für C++11


```c++
#include <iostream>
#include <list>
#include <vector>

#include "lzy.h"

using namespace lzy;

int main() {
    std::vector<int> aCoupleNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    auto iteration = filter([](int i) {return i % 2 == 0;}) | map([](int i) {return i * 1.1;});

    // iteriere direkt über eine Sequenz
    for (auto d : from(aCoupleNumbers) >> iteration) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;
    // Ausgabe: 2.2, 4.4, 6.6, 8.8, 11, 13.2,

    // Iterationen werden mittendrin komponiert und die Sequenz in einen Behälter ausgegeben
    auto list = from(aCoupleNumbers) >> (iteration | take(3)) >> to<std::list<double>>();
    // list enthält 2.2, 4.4, 6.6

    // Sequenzen aneinanderhängen
    std::vector<int> first = {1, 2, 3};
    std::vector<int> second = {4, 5, 6};

    for (auto d : from(first) + from(second) >> map([](int i) {return i * 2;})) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;
    // Ausgabe: 2, 4, 6, 8, 10, 12,

    // Über eine Sequenz reduzieren
    std::cout << "sum: " << (from(aCoupleNumbers) >> reduce(std::plus<int>(), 0)) << std::endl;
    // output: sum: 78

    // Sich über eine Sequenz schiebende Fenster (hier mit Größe 3)
    for (auto w : from(aCoupleNumbers) >> (windows<3>() | slice(3, 6))) {
        for (auto i : w) {
            std::cout << i << ",";
        }
        std::cout << std::endl;
    }
    // Ausgabe:
    // 4,5,6,
    // 5,6,7,
    // 6,7,8,


    // Zwei Sequenzen zusammenreißverschließen
    for (auto pair : zip(from(first), from(second))) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }
    // Ausgabe:
    // (1, 4)
    // (2, 5)
    // (3, 6)

    return 0;
}

```

## Lizenz

MIT, siehe [LICENSE.md](LICENSE.md) für Details.
Abhängigkeiten sind auch unter MIT lizensiert, siehe [static-queue/README.md](deps/static-queue/README.md)
