#ifndef LZY_TAKE_H
#define LZY_TAKE_H

#include "../common.h"

namespace lzy {

    template<typename Sequence>
    class take_sequence : public sequence<take_sequence<Sequence>> {
        using item = SequenceItemType<Sequence>;
    public:
        take_sequence(Sequence &&source, size_t nToTake) : source(std::move(source)), left(nToTake) { };

        take_sequence(take_sequence &&other) : source(std::move(other.source)), left(other.left) { };

        bool done() {
            return source.done() || left == 0;
        };

        void advance() {
            source.advance();
            left--;
        }

        item current() {
            return source.current();
        }

    private:
        Sequence source;
        size_t left;
    };

    class take : public composable<take> {
    public:
        take(size_t nToTake) : nToTake(nToTake) { };

        template<typename InputSequence>
        take_sequence<InputSequence> operator()(InputSequence &&input) {
            return take_sequence<InputSequence>(std::move(input), nToTake);
        }

    private:
        size_t nToTake;
    };

}

#endif //LZY_TAKE_H
