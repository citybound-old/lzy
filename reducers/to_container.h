#ifndef LZY_TO_CONTAINER_H
#define LZY_TO_CONTAINER_H

#include "../common.h"

namespace lzy {

    template<typename Container>
    struct to {
        template<typename Sequence>
        Container operator()(Sequence sequence) {
            return Container(sequence.begin(), sequence.end());
        };
    };

}

#endif //LZY_TO_CONTAINER_H
