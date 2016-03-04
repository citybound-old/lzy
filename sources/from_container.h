#ifndef LZY_FROM_CONTAINER_H
#define LZY_FROM_CONTAINER_H

#include "../common.h"

namespace lzy {

    template<typename Container>
    class container_sequence : public sequence<container_sequence<Container>> {
        using iterator = ContainerIteratorType<Container>;
        using item = ContainerItemType<Container>;
    public:
        container_sequence(Container &container)
                : iteratorCurrent(std::begin(container)), iteratorEnd(std::end(container)) { }

        container_sequence(container_sequence &&other)
                : iteratorCurrent(std::move(other.iteratorCurrent)), iteratorEnd(std::move(other.iteratorEnd)) { };

        bool done() {
            return iteratorCurrent == iteratorEnd;
        }

        void advance() {
            iteratorCurrent++;
        }

        const item& current() {
            return *iteratorCurrent;
        }

    private:
        iterator iteratorCurrent;
        iterator iteratorEnd;
    };

    template<typename Container>
    container_sequence<Container> from(Container &container) {
        return container_sequence<Container>(container);
    };

}

#endif //LZY_FROM_CONTAINER_H
