#ifndef LZY_SLICE_H
#define LZY_SLICE_H

#include "../common.h"
#include "take.h"
#include "drop.h"

namespace lzy {
    owning_composition<drop, take> slice (size_t start, size_t end) {
        return owning_composition<drop, take>(drop(start), take(end - start));
    };
}

#endif //LZY_SLICE_H
