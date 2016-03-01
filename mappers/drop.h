#ifndef LZY_DROP_H
#define LZY_DROP_H

#include "../common.h"

namespace lzy {

    template<typename Sequence>
    class drop_sequence : public sequence<drop_sequence<Sequence>> {
        using item = SequenceItemType<Sequence>;
    public:
        drop_sequence(Sequence &&source, size_t nToDrop) : source(std::move(source)), nToDrop(nToDrop) { };

        drop_sequence(drop_sequence &&other) : source(std::move(other.source)), nToDrop(other.nToDrop) { };

        bool done() {
            while (nToDrop > 0 && !source.done()) {
                source.advance();
                nToDrop--;
            }
            return source.done();
        };

        void advance() {
            source.advance();
        }

        item current() {
            return source.current();
        }

    private:
        Sequence source;
        size_t nToDrop;
    };

    class drop : public composable<drop> {
    public:
        drop(size_t nToDrop) : nToDrop(nToDrop) { };

        template<typename InputSequence>
        drop_sequence<InputSequence> operator()(InputSequence &&input) {
            return drop_sequence<InputSequence>(std::move(input), nToDrop);
        }

    private:
        size_t nToDrop;
    };

}

#endif //LZY_DROP_H
