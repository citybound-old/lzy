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
                : curr(std::begin(container)), end(std::end(container)) { }

        container_sequence(container_sequence &&other)
                : curr(std::move(other.curr)), end(std::move(other.end)) { };

        bool done() {
            return curr == end;
        }

        void advance() {
            curr++;
        }

        const item &current() {
            return *curr;
        }

    private:
        iterator curr;
        iterator end;
    };

    template<typename Container>
    container_sequence<Container> from(Container &container) {
        return container_sequence<Container>(container);
    };

}

#endif //LZY_FROM_CONTAINER_H
